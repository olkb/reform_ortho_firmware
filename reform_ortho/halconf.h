#pragma once

#include_next <halconf.h>

#undef HAL_USE_I2C
#define HAL_USE_I2C TRUE

#undef HAL_USE_SIO
#define HAL_USE_SIO TRUE
// #undef HAL_USE_PIO
// #define HAL_USE_PIO TRUE
