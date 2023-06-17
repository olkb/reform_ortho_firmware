#pragma once

#include "quantum.h"
#include "reform_sysctl.h"

#define MNT_REFORM_BATTERY_UPDATE

int8_t get_battery_status(void);
int32_t get_charge_status(void);

static inline uint32_t clear_screen(uint32_t trigger_time, void *cb_arg) {
    oled_clear();
    return 0;
}

static deferred_token clear_screen_token = 0;

static inline void delayed_clear_screen(uint32_t delay) {
    cancel_deferred_exec(clear_screen_token);
    clear_screen_token = defer_exec(delay, clear_screen, NULL);
}

static inline void cancel_clear_screen(void) {
    cancel_deferred_exec(clear_screen_token);
}

#define LAYOUT_ortho_6x15( \
    K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K014, K015, \
    K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, K114, K115, \
    K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K213, K214, K215, \
    K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K312, K313, K314, K315, \
    K401, K402, K403, K404, K405, K406, K407, K408, K409, K410, K411, K412, K413, K414, K415, \
    K501, K502, K503, K504, K505, K506, K507, K508, K509, K510, K511, K512, K513, K514, K515  \
) { \
    { K001, K002, K003, K004, K005, K006, K007, K008, K009, K010, K011, K012, K013, K014, K015 } , \
    { K101, K102, K103, K104, K105, K106, K107, K108, K109, K110, K111, K112, K113, K114, K115 } , \
    { K201, K202, K203, K204, K205, K206, K207, K208, K209, K210, K211, K212, K213, K214, K215 } , \
    { K301, K302, K303, K304, K305, K306, K307, K308, K309, K310, K311, K312, K313, K314, K315 } , \
    { K401, K402, K403, K404, K405, K406, K407, K408, K409, K410, K411, K412, K413, K414, K415 } , \
    { K501, K502, K503, K504, K505, K506, K507, K508, K509, K510, K511, K512, K513, K514, K515 }   \
}
