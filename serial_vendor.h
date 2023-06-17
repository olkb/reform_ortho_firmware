#pragma once

#include "quantum.h"

void pio_init(pin_t tx_pin, pin_t rx_pin);  
bool serial_send(const uint8_t* source, const size_t size);
void serial_clear(void);
bool serial_receive_blocking(uint8_t* destination, const size_t size);
bool serial_receive(uint8_t* destination, const size_t size);
bool serial_receive_timeout(uint8_t* destination, const size_t size, uint32_t timeout);
char serial_getc(void);
msg_t sync_rx(sysinterval_t timeout);
bool serial_has_rx(void);