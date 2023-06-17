#pragma once

// #include "config_common.h"

//#define MNT_REFORM_BATTERY_UPDATE

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 15

/* Keyboard Matrix Assignments */
#define MATRIX_ROW_PINS { GP8, GP9, GP10, GP11, GP12, GP13 }
#define MATRIX_COL_PINS { GP27, GP26, GP4, GP5, GP6, GP7, GP14, GP15, GP18, GP20, GP21, GP22, GP23, GP24, GP25 }
#define DIODE_DIRECTION COL2ROW

#define NO_USB_STARTUP_CHECK

#define DEBOUNCE 5

// #define SERIAL_PIO_USE_PIO1
// #define SERIAL_USART_DRIVER SIOD0
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_RX_PIN GP0
#define SERIAL_USART_TX_PIN GP1

/* Double tap the side button to enter bootloader */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

#ifdef RGB_MATRIX_ENABLE
    #define WS2812_DI_PIN GP19
    #define RGB_MATRIX_LED_COUNT 90
    #define RGBLED_NUM  90
    #define WS2812_PIO_USE_PIO1
    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 64
//    #define RGB_MATRIX_MAXIMUM_BRIGHTNESS 20
    #define RGB_MATRIX_DEFAULT_VAL RGB_MATRIX_MAXIMUM_BRIGHTNESS
    #define RGB_MATRIX_TIMEOUT 60000 // number of milliseconds to wait until rgb automatically turns off

    /* Enable Framebuffer and keypress effects */
    #define RGB_MATRIX_FRAMEBUFFER_EFFECTS
    #define RGB_MATRIX_KEYPRESSES

    #define ENABLE_RGB_MATRIX_ALPHAS_MODS
    #define ENABLE_RGB_MATRIX_GRADIENT_UP_DOWN
    #define ENABLE_RGB_MATRIX_GRADIENT_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_BREATHING
    #define ENABLE_RGB_MATRIX_BAND_SAT
    #define ENABLE_RGB_MATRIX_BAND_VAL
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_SAT
    #define ENABLE_RGB_MATRIX_BAND_PINWHEEL_VAL
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_SAT
    #define ENABLE_RGB_MATRIX_BAND_SPIRAL_VAL
    #define ENABLE_RGB_MATRIX_CYCLE_ALL
    #define ENABLE_RGB_MATRIX_CYCLE_LEFT_RIGHT
    #define ENABLE_RGB_MATRIX_CYCLE_UP_DOWN
    #define ENABLE_RGB_MATRIX_RAINBOW_MOVING_CHEVRON
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN
    #define ENABLE_RGB_MATRIX_CYCLE_OUT_IN_DUAL
    #define ENABLE_RGB_MATRIX_CYCLE_PINWHEEL
    #define ENABLE_RGB_MATRIX_CYCLE_SPIRAL
    #define ENABLE_RGB_MATRIX_DUAL_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_BEACON
    #define ENABLE_RGB_MATRIX_RAINBOW_PINWHEELS
    #define ENABLE_RGB_MATRIX_RAINDROPS
    #define ENABLE_RGB_MATRIX_JELLYBEAN_RAINDROPS
    #define ENABLE_RGB_MATRIX_HUE_BREATHING
    #define ENABLE_RGB_MATRIX_HUE_PENDULUM
    #define ENABLE_RGB_MATRIX_HUE_WAVE
    #define ENABLE_RGB_MATRIX_PIXEL_RAIN
    #define ENABLE_RGB_MATRIX_PIXEL_FLOW
    #define ENABLE_RGB_MATRIX_PIXEL_FRACTAL
    #define ENABLE_RGB_MATRIX_TYPING_HEATMAP
    #define ENABLE_RGB_MATRIX_DIGITAL_RAIN
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS
    #define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS
    #define ENABLE_RGB_MATRIX_SPLASH
    #define ENABLE_RGB_MATRIX_MULTISPLASH
    #define ENABLE_RGB_MATRIX_SOLID_SPLASH
    #define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
#endif

#ifdef OLED_ENABLE
    #define OLED_DISPLAY_128X32
    #define I2C1_SCL_PIN        GP16
    #define I2C1_SDA_PIN        GP17
    #define I2C_DRIVER I2CD0
    #define OLED_BRIGHTNESS 128
    //#define OLED_TIMEOUT 15000
    //#define OLED_FADE_OUT
    //#define OLED_FADE_OUT_INTERVAL 5
#endif
