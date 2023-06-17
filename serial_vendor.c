#include "serial_vendor.h"
#include <hal.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pico/platform.h"

#define SERIAL_USART_TIMEOUT 20
#define SERIAL_USART_SPEED 57600

static inline void pio_serve_interrupt(void);

#define MSG_PIO_ERROR ((msg_t)(-3))

#if defined(SERIAL_PIO_USE_PIO1)
static const PIO pio = pio1;

OSAL_IRQ_HANDLER(RP_PIO1_IRQ_0_HANDLER) {
    OSAL_IRQ_PROLOGUE();
    pio_serve_interrupt();
    OSAL_IRQ_EPILOGUE();
}
#else
static const PIO pio = pio0;

OSAL_IRQ_HANDLER(RP_PIO0_IRQ_0_HANDLER) {
    OSAL_IRQ_PROLOGUE();
    pio_serve_interrupt();
    OSAL_IRQ_EPILOGUE();
}
#endif

#define UART_TX_WRAP_TARGET 0
#define UART_TX_WRAP 3

static const uint16_t uart_tx_program_instructions[] = {
            //     .wrap_target
    0x9fa0, //  0: pull   block           side 1 [7]
    0xf727, //  1: set    x, 7            side 0 [7]
    0x6001, //  2: out    pins, 1
    0x0642, //  3: jmp    x--, 2                 [6]
            //     .wrap
};

static const pio_program_t uart_tx_program = {
    .instructions = uart_tx_program_instructions,
    .length       = 4,
    .origin       = -1,
};

#define UART_RX_WRAP_TARGET 0
#define UART_RX_WRAP 8

// clang-format off
static const uint16_t uart_rx_program_instructions[] = {
            //     .wrap_target
    0x2020, //  0: wait   0 pin, 0
    0xea27, //  1: set    x, 7                   [10]
    0x4001, //  2: in     pins, 1
    0x0642, //  3: jmp    x--, 2                 [6]
    0x00c8, //  4: jmp    pin, 8
    // 0xc020, //  5: irq    wait 0
    0xc014, //  5: irq    nowait 4 rel
    0x20a0, //  6: wait   1 pin, 0
    0x0000, //  7: jmp    0
    0x8020, //  8: push   block
            //     .wrap
};
// clang-format on

static const pio_program_t uart_rx_program = {
    .instructions = uart_rx_program_instructions,
    .length       = 9,
    .origin       = -1,
};

thread_reference_t rx_thread        = NULL;
static int         rx_state_machine = -1;

thread_reference_t tx_thread        = NULL;
static int         tx_state_machine = -1;


void pio_serve_interrupt(void) {
    uint32_t irqs = pio->ints0;

    // The RX FIFO is not empty any more, therefore wake any sleeping rx thread
    if (irqs & (PIO_IRQ0_INTF_SM0_RXNEMPTY_BITS << rx_state_machine)) {
        // Disable rx not empty interrupt
        pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + rx_state_machine, false);

        osalSysLockFromISR();
        osalThreadResumeI(&rx_thread, MSG_OK);
        osalSysUnlockFromISR();
    }

    // The TX FIFO is not full any more, therefore wake any sleeping tx thread
    if (irqs & (PIO_IRQ0_INTF_SM0_TXNFULL_BITS << tx_state_machine)) {
        // Disable tx not full interrupt
        pio_set_irq0_source_enabled(pio, pis_sm0_tx_fifo_not_full + tx_state_machine, false);
        osalSysLockFromISR();
        osalThreadResumeI(&tx_thread, MSG_OK);
        osalSysUnlockFromISR();
    }

    // IRQ 0 is set on framing or break errors by the rx state machine
    if (pio_interrupt_get(pio, 0UL)) {
        pio_interrupt_clear(pio, 0UL);

        osalSysLockFromISR();
        osalThreadResumeI(&rx_thread, MSG_PIO_ERROR);
        osalSysUnlockFromISR();
    }
}
static inline msg_t sync_tx(sysinterval_t timeout) {
    msg_t msg = MSG_OK;
    osalSysLock();
    while (pio_sm_is_tx_fifo_full(pio, tx_state_machine)) {
        pio_set_irq0_source_enabled(pio, pis_sm0_tx_fifo_not_full + tx_state_machine, true);
        msg = osalThreadSuspendTimeoutS(&tx_thread, timeout);
        if (msg < MSG_OK) {
            pio_set_irq0_source_enabled(pio, pis_sm0_tx_fifo_not_full + tx_state_machine, false);
            break;
        }
    }
    osalSysUnlock();
    return msg;
}

msg_t sync_rx(sysinterval_t timeout) {
    msg_t msg = MSG_OK;
    osalSysLock();
    while (pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
        pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + rx_state_machine, true);
        msg = osalThreadSuspendTimeoutS(&rx_thread, timeout);
        if (msg < MSG_OK) {
            pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + rx_state_machine, false);
            break;
        }
    }
    osalSysUnlock();
    return msg;
}

void serial_clear(void) {
    msg_t msg = MSG_OK;
    sync_tx(TIME_MS2I(SERIAL_USART_TIMEOUT));
    while (pio_sm_get_rx_fifo_level(pio, rx_state_machine)) {
        msg = sync_tx(TIME_MS2I(SERIAL_USART_TIMEOUT));
        if (msg < MSG_OK) {
            break;
        }
        osalSysLock();
        while (!pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
            pio_sm_clear_fifos(pio, rx_state_machine);
        } 
        osalSysUnlock();
    }
}

bool serial_has_rx(void) {
    return pio_sm_get_rx_fifo_level(pio, rx_state_machine);
}

bool serial_send(const uint8_t* source, const size_t size) {
   size_t send = 0;
    msg_t  msg;
    while (send < size) {
        msg = sync_tx(TIME_MS2I(SERIAL_USART_TIMEOUT));
        if (msg < MSG_OK) {
            return false;
        }

        osalSysLock();
        while (send < size) {
            if (pio_sm_is_tx_fifo_full(pio, tx_state_machine)) {
                break;
            }
            if (send >= size) {
                break;
            }
            pio_sm_put(pio, tx_state_machine, (uint32_t)(*source));
            source++;
            send++;
        }
        osalSysUnlock();
    }

    return send == size;
}

char serial_getc(void) {           
    // sync_rx(TIME_MS2I(SERIAL_USART_TIMEOUT));
     
    osalSysLock();

    // 8-bit read from the uppermost byte of the FIFO, as data is left-justifieds
    uint8_t *rxfifo_shift = ((uint8_t*)&pio->rxf[rx_state_machine] + 3U);
    while (pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
        tight_loop_contents();
    }        
    osalSysUnlock();

    return (char)*rxfifo_shift;
}

bool serial_receive(uint8_t* destination, const size_t size) {
  size_t read = 0U;

    while (read < size) {
        msg_t msg = sync_rx(TIME_MS2I(SERIAL_USART_TIMEOUT));
        if (msg < MSG_OK) {
            return false;
        }
        osalSysLock();
        while (true) {
            if (pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
                break;
            }
            if (read >= size) {
                break;
            }
            *destination++ = *((uint8_t*)&pio->rxf[rx_state_machine] + 3U);
            read++;
        }
        osalSysUnlock();
    }

    return read == size;
}

bool serial_receive_timeout(uint8_t* destination, const size_t size, uint32_t timeout) {
  size_t read = 0U;

    while (read < size) {
        msg_t msg = sync_rx(TIME_MS2I(timeout));
        if (msg < MSG_OK) {
            return false;
        }
        osalSysLock();
        while (true) {
            if (pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
                break;
            }
            if (read >= size) {
                break;
            }
            *destination++ = *((uint8_t*)&pio->rxf[rx_state_machine] + 3U);
            read++;
        }
        osalSysUnlock();
    }

    return read == size;
}

bool serial_receive_blocking(uint8_t* destination, const size_t size) {

  size_t read = 0U;

    while (read < size) {
        msg_t msg = sync_rx(TIME_INFINITE);
        if (msg < MSG_OK) {
            return false;
        }
        osalSysLock();
        while (true) {
            if (pio_sm_is_rx_fifo_empty(pio, rx_state_machine)) {
                break;
            }
            if (read >= size) {
                break;
            }
            *destination++ = *((uint8_t*)&pio->rxf[rx_state_machine] + 3U);
            read++;
        }
        osalSysUnlock();
    }

    return read == size;    
}


static inline void pio_tx_init(pin_t tx_pin) {
    uint pio_idx = pio_get_index(pio);
    uint offset  = pio_add_program(pio, &uart_tx_program);

#if defined(SERIAL_USART_FULL_DUPLEX)
    // clang-format off
    iomode_t tx_pin_mode = PAL_RP_GPIO_OE |
                           PAL_RP_PAD_SLEWFAST |
                           PAL_RP_PAD_DRIVE4 |
                           (pio_idx == 0 ? PAL_MODE_ALTERNATE_PIO0 : PAL_MODE_ALTERNATE_PIO1);
    // clang-format on
    pio_sm_set_pins_with_mask(pio, tx_state_machine, 1U << tx_pin, 1U << tx_pin);
    pio_sm_set_consecutive_pindirs(pio, tx_state_machine, tx_pin, 1U, true);
#else
    // clang-format off
    iomode_t tx_pin_mode = PAL_RP_PAD_IE |
                           PAL_RP_GPIO_OE |
                           PAL_RP_PAD_SCHMITT |
                           PAL_RP_PAD_PUE |
                           PAL_RP_PAD_SLEWFAST |
                           PAL_RP_PAD_DRIVE12 |
                           PAL_RP_IOCTRL_OEOVER_DRVINVPERI |
                           (pio_idx == 0 ? PAL_MODE_ALTERNATE_PIO0 : PAL_MODE_ALTERNATE_PIO1);
    // clang-format on
    pio_sm_set_pins_with_mask(pio, tx_state_machine, 0U << tx_pin, 1U << tx_pin);
    pio_sm_set_consecutive_pindirs(pio, tx_state_machine, tx_pin, 1U, true);
#endif

    palSetLineMode(tx_pin, tx_pin_mode);

    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_wrap(&config, offset + UART_TX_WRAP_TARGET, offset + UART_TX_WRAP);
#if defined(SERIAL_USART_FULL_DUPLEX)
    sm_config_set_sideset(&config, 2, true, false);
#else
    sm_config_set_sideset(&config, 2, true, true);
#endif
    // OUT shifts to right, no autopull
    sm_config_set_out_shift(&config, true, false, 32);
    // We are mapping both OUT and side-set to the same pin, because sometimes
    // we need to assert user data onto the pin (with OUT) and sometimes
    // assert constant values (start/stop bit)
    sm_config_set_out_pins(&config, tx_pin, 1);
    sm_config_set_sideset_pins(&config, tx_pin);
    // We only need TX, so get an 8-deep FIFO!
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_TX);
    // SM transmits 1 bit per 8 execution cycles.
    float div = (float)clock_get_hz(clk_sys) / (8 * SERIAL_USART_SPEED);
    sm_config_set_clkdiv(&config, div);
    pio_sm_init(pio, tx_state_machine, offset, &config);
    pio_sm_set_enabled(pio, tx_state_machine, true);
}

static inline void pio_rx_init(pin_t rx_pin) {
    uint offset = pio_add_program(pio, &uart_rx_program);

#if defined(SERIAL_USART_FULL_DUPLEX)
    uint pio_idx = pio_get_index(pio);
    pio_sm_set_consecutive_pindirs(pio, rx_state_machine, rx_pin, 1, false);
    // clang-format off
    iomode_t rx_pin_mode = PAL_RP_PAD_IE |
                           PAL_RP_PAD_SCHMITT |
                           PAL_RP_PAD_PUE |
                           (pio_idx == 0 ? PAL_MODE_ALTERNATE_PIO0 : PAL_MODE_ALTERNATE_PIO1);
    // clang-format on
    palSetLineMode(rx_pin, rx_pin_mode);
#endif

    pio_sm_config config = pio_get_default_sm_config();
    sm_config_set_wrap(&config, offset + UART_RX_WRAP_TARGET, offset + UART_RX_WRAP);
    sm_config_set_in_pins(&config, rx_pin); // for WAIT, IN
    sm_config_set_jmp_pin(&config, rx_pin); // for JMP
    // Shift to right, autopush disabled
    sm_config_set_in_shift(&config, true, false, 32);
    // Deeper FIFO as we're not doing any TX
    sm_config_set_fifo_join(&config, PIO_FIFO_JOIN_RX);
    // SM transmits 1 bit per 8 execution cycles.
    float div = (float)clock_get_hz(clk_sys) / (8 * SERIAL_USART_SPEED);
    sm_config_set_clkdiv(&config, div);
    pio_sm_init(pio, rx_state_machine, offset, &config);
    pio_sm_set_enabled(pio, rx_state_machine, true);
}

void pio_init(pin_t tx_pin, pin_t rx_pin) {
    uint pio_idx = pio_get_index(pio);

    /* Get PIOx peripheral out of reset state. */
    hal_lld_peripheral_unreset(pio_idx == 0 ? RESETS_ALLREG_PIO0 : RESETS_ALLREG_PIO1);

    tx_state_machine = pio_claim_unused_sm(pio, true);
    if (tx_state_machine < 0) {
        dprintln("ERROR: Failed to acquire state machine for serial transmission!");
        return;
    }
    pio_tx_init(tx_pin);

    rx_state_machine = pio_claim_unused_sm(pio, true);
    if (rx_state_machine < 0) {
        dprintln("ERROR: Failed to acquire state machine for serial reception!");
        return;
    }
    pio_rx_init(rx_pin);

    // Enable error flag IRQ source for rx state machine
    pio_set_irq0_source_enabled(pio, pis_sm0_rx_fifo_not_empty + rx_state_machine, true);
    pio_set_irq0_source_enabled(pio, pis_sm0_tx_fifo_not_full + tx_state_machine, true);
    pio_set_irq0_source_enabled(pio, pis_interrupt0, true);

    // Enable PIO specific interrupt vector, as the pio implementation is timing
    // critical we use the highest possible priority.
#if defined(SERIAL_PIO_USE_PIO1)
    nvicEnableVector(RP_PIO1_IRQ_0_NUMBER, CORTEX_MAX_KERNEL_PRIORITY);
#else
    nvicEnableVector(RP_PIO0_IRQ_0_NUMBER, CORTEX_MAX_KERNEL_PRIORITY);
#endif
}
