#pragma once

#include_next <mcuconf.h>

#undef RP_I2C_USE_I2C0
#define RP_I2C_USE_I2C0 TRUE

#undef RP_SIO_USE_UART0
#define RP_SIO_USE_UART0 TRUE

// #define PLATFORM_SIO_USE_SIO0