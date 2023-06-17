#include "keyboard.h"
#include "reform_ortho.h"
#include QMK_KEYBOARD_H

enum reformortho_layers {
    _COLEMAK,
    _SHIFT,
    _LOWER,
    _RAISE,
    _NAVIG,
    _CIRCL
};

enum reformortho_keycodes {
    CIRCL = SAFE_RANGE
};

#define SHIFT LM(_SHIFT, MOD_LSFT)
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define NAVIG MO(_NAVIG)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_COLEMAK] = LAYOUT_ortho_6x15(
        KC_ESC,     KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,      XXXXXXX,    KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,     CIRCL,
        XXXXXXX,    KC_GRV,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       XXXXXXX,    KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_DEL,     XXXXXXX, 
        XXXXXXX,    KC_TAB,     KC_Q,       KC_W,       KC_F,       KC_P,       KC_G,       XXXXXXX,    KC_J,       KC_L,       KC_U,       KC_Y,       KC_SCLN,    KC_BSPC,    XXXXXXX, 
        XXXXXXX,    KC_MINS,    KC_A,       KC_R,       KC_S,       KC_T,       KC_D,       XXXXXXX,    KC_H,       KC_N,       KC_E,       KC_I,       KC_O,       KC_QUOT,    XXXXXXX, 
        XXXXXXX,    KC_ESC,     KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       XXXXXXX,    KC_K,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    KC_ENT,     XXXXXXX,   
        KC_MS_BTN1, KC_RSFT,    KC_LGUI,    KC_LALT,    KC_LCTL,    LOWER,      SHIFT,      XXXXXXX,    KC_SPC,     RAISE,      NAVIG,      KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT
    ),
    [_SHIFT] = LAYOUT_ortho_6x15(
        S(KC_ESC),  S(KC_F1),   S(KC_F2),   S(KC_F3),   S(KC_F4),   S(KC_F5),   S(KC_F6),   KC_NO,      S(KC_F7),   S(KC_F8),   S(KC_F9),   S(KC_F10),  S(KC_F11),  S(KC_F12),  _______,
        XXXXXXX,    S(KC_GRV),  S(KC_1),    S(KC_2),    S(KC_3),    S(KC_4),    S(KC_5),    KC_NO,      S(KC_6),    S(KC_7),    S(KC_8),    S(KC_9),    S(KC_0),    S(KC_DEL),  XXXXXXX,       
        XXXXXXX,    S(KC_TAB),  S(KC_Q),    S(KC_W),    S(KC_F),    S(KC_P),    S(KC_G),    KC_NO,      S(KC_J),    S(KC_L),    S(KC_U),    S(KC_Y),    KC_COLN,    S(KC_BSPC), XXXXXXX,    
        XXXXXXX,    S(KC_MINS), S(KC_A),    S(KC_R),    S(KC_S),    S(KC_T),    S(KC_D),    KC_NO,      S(KC_H),    S(KC_N),    S(KC_E),    S(KC_I),    S(KC_O),    S(KC_QUOT), XXXXXXX,    
        XXXXXXX,    S(KC_ESC),  S(KC_Z),    S(KC_X),    S(KC_C),    S(KC_V),    S(KC_B),    KC_NO,      S(KC_K),    S(KC_M),    KC_EXLM,    KC_AT,      KC_QUES,    S(KC_ENT),  XXXXXXX,    
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    S(KC_SPC),  _______,    _______,    _______,    _______,    _______,    XXXXXXX  
    ),
    [_LOWER] = LAYOUT_ortho_6x15(
        KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_NO,      KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    _______,
        XXXXXXX,    A(KC_GRV),  KC_EXLM,    KC_AT,      KC_HASH,    KC_DLR,     KC_PERC,    KC_TRNS,    KC_CIRC,    KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_BSPC,    XXXXXXX,    
        XXXXXXX,    A(KC_TAB),  XXXXXXX,    KC_7,       KC_8,       KC_9,       XXXXXXX,    KC_TRNS,    KC_DEL,     KC_F9,      KC_F10,     KC_F11,     KC_F12,     _______,    XXXXXXX,    
        XXXXXXX,    KC_DEL,     XXXXXXX,    KC_4,       KC_5,       KC_6,       XXXXXXX,    KC_TRNS,    XXXXXXX,    KC_F5,      KC_F6,      KC_F7,      KC_F8,      _______,    XXXXXXX,    
        XXXXXXX,    _______,    KC_0,       KC_1,       KC_2,       KC_3,       XXXXXXX,    KC_TRNS,    XXXXXXX,    KC_F1,      KC_F2,      KC_F3,      KC_F4,      _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_NO,      _______,    _______,    KC_VOLD,    KC_VOLU,    KC_MPLY,    XXXXXXX   
    ),
    [_RAISE] = LAYOUT_ortho_6x15(
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      _______,
        XXXXXXX,    KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      XXXXXXX,    
        XXXXXXX,    _______,    XXXXXXX,    KC_LT,      KC_DLR,     KC_GT,      XXXXXXX,    KC_NO,      XXXXXXX,    KC_LBRC,    KC_UNDS,    KC_RBRC,    XXXXXXX,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    KC_BSLS,    KC_LPRN,    KC_DQUO,    KC_RPRN,    KC_HASH,    KC_NO,      KC_PERC,    KC_LCBR,    KC_EQL,     KC_RCBR,    KC_PIPE,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    XXXXXXX,    KC_COLN,    KC_ASTR,    KC_PLUS,    XXXXXXX,    KC_NO,      XXXXXXX,    KC_AMPR,    KC_CIRC,    KC_TILD,    XXXXXXX,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_NO,      _______,    _______,    KC_NO,      KC_NO,      KC_NO,      XXXXXXX
    ),
    [_NAVIG] = LAYOUT_ortho_6x15(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_HOME,    KC_PGDN,    KC_PGUP,    KC_END,     _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_LEFT,    KC_DOWN,    KC_UP,      KC_RGHT,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    A(KC_LEFT), A(KC_DOWN), A(KC_UP),   A(KC_RIGHT)
    ),
    [_CIRCL] = LAYOUT_ortho_6x15(
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_MOD,    RGB_TOG,    _______,    KC_BRID,    KC_BRIU,    _______,
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RGB_VAD,    RGB_VAI,    RGB_HUD,    RGB_HUI,    RGB_SAD,    RGB_SAI,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    EE_CLR,     _______,    DB_TOGG,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX,    
        XXXXXXX,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    XXXXXXX
    )
};

// layer_state_t layer_state_set_user(layer_state_t state) {
    // return update_tri_layer_state(state, _LOWER, _RAISE, _CIRCL);
// }

uint32_t mnt_dfu(uint32_t trigger_time, void *cb_arg) {
    bootloader_jump();
    return 0;
}

bool is_circle = false;

uint8_t battery_status_length = 0;
bool is_charging = false;

bool oled_task_user(void) {
    bool charging_changed = is_charging ^ (get_charge_status() < 0);
    is_charging = get_charge_status() < 0;

    if ((!is_oled_on() && !charging_changed) || get_battery_status() == -1 || is_circle)
        return false;
        
    char batteryPerc[5] = {0};
    sprintf(batteryPerc, "%d%%", (uint8_t)get_battery_status());
    uint8_t length = strlen(batteryPerc);
    oled_set_cursor(oled_max_chars() - length - 1, 0);
    if (length < battery_status_length || !is_charging) {    
        oled_write_char(' ', false);
    } else if (is_charging) {
        oled_write_char('+', get_battery_status() % 2);
    }
    battery_status_length = length;
    oled_set_cursor(oled_max_chars() - length, 0);
    oled_write(batteryPerc, get_battery_status() % 2);
    oled_set_cursor(0, 0);
    return false;
}


bool process_native_menu(uint16_t keycode, bool act) {
    switch (keycode) {
        // keyboard dfu
        case KC_F7:
            if (act) {
            rgb_matrix_disable_noeeprom();
            oled_clear();
            //         "                     " 
            oled_write("Bootloader Mode      ", false);
            oled_write("Flash RPI-RP2 or     ", false);
            oled_write("press DFU to reset   ", false);
            defer_exec(300, mnt_dfu, NULL);
            }
            break;
        // wake
        case KC_F2:
            if (act) {
            // remote_try_command("0x", true);
            remote_try_command_size("1w", true, 0);
            remote_try_command_size("0w", true, 0);
            }
            break;
        // sleep
        // case KC_S:
            // if (act) remote_try_command("1x", true);
            // break;
        // power off
        case KC_F4:
            if (act) remote_try_command("0p", true);
            break;
        // power on
        case KC_F1:
            if (act) remote_try_command("1p", true);
            break;
        // reset
        case KC_F5:
            if (act) remote_try_command("2p", true);
            break;
        // aux power off
        // case KC_9:
            // if (act) remote_try_command("3p", true);
            // break;
        // aux power on
        // case KC_2:
            // if (act) remote_try_command("4p", true);
            // break;
        // som uart on
        // case KC_U:
            // if (act) remote_try_command("1u", true);
            // break;
        // som uart off
        // case KC_Y:
            // if (act) remote_try_command("0u", true);
            // break;
        // remote status
        // case KC_SLASH:
            // if (act) remote_try_command("s", true);
            // break;
        // get system voltage
        // case KC_V:
            // if (act) remote_try_command_size("V", true, 6);
            // break;
        // get system current
        // case KC_A:
            // if (act) remote_try_command_size("a", true, 4);
            // break;
        // get charge status
        // case KC_C:
            // if (act) remote_try_command("c", true);
            // break;
        // get battery capacity
        case KC_F10:
            if (act) remote_try_command("C", true);
            break;
        // turn on echo
        // case KC_E:
        //     if (act) remote_try_command("1e", true);
        //     break;
        // turn off echo
        // case KC_I:
        //     if (act) remote_try_command("0e", true);
        //     break;
        default:
            return true;
            break;
    }
    if (act) cancel_clear_screen();
    return false;
}

#define NUM_CENTER_LEDS 6

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t layer = get_highest_layer(layer_state);
    uint8_t base_layer = get_highest_layer(layer_state & ~(1<<_CIRCL));
    if (layer > 0) {
        for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
            for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
                uint8_t index = g_led_config.matrix_co[row][col];
                uint16_t keycode = keymap_key_to_keycode(layer, (keypos_t){col,row});
                uint16_t base_keycode = keymap_key_to_keycode(base_layer, (keypos_t){col,row});
                if (index >= led_min && index < led_max && index != NO_LED &&
                        (keycode > KC_TRNS || !process_native_menu(base_keycode, false))) {
                    HSV hsv = {35, 255, 255};    
                    if (is_circle) {
                        hsv.h = 130;
                    }

                    if (hsv.v > rgb_matrix_get_val()) {
                        hsv.v = rgb_matrix_get_val();
                    }
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(index, rgb.r, rgb.g, rgb.b);                       
                }
            }
        }
    }     
    for (int i = led_min; i < led_max; i++) {
                if (g_led_config.flags[i] & LED_FLAG_INDICATOR) {
                    HSV hsv = {130, 255, 255};
                    if (!is_circle) {
                        uint8_t index = (NUM_CENTER_LEDS-1) - (i / 15);
                        uint8_t color = 35;
                        if (get_charge_status() < 0) {
                            color = 75;
                        } else if (get_charge_status() == 0) {
                            color = 90;
                        }
                        hsv.h = MAX(MIN(((int32_t)get_battery_status() - ((100 / NUM_CENTER_LEDS) * index)) * NUM_CENTER_LEDS * color / 100, color), 0);
                    }
                    if (hsv.v > rgb_matrix_get_val()) {
                        hsv.v = rgb_matrix_get_val();
                    }
                    RGB rgb = hsv_to_rgb(hsv);
                    rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);  
                }               
    }
    return false;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (is_circle && record->event.pressed) {
        if (!process_native_menu(keycode, true)) {
            return false;
        }
    }
    switch (keycode) {
        case CIRCL:
            if (record->event.pressed && !is_circle) {
                is_circle = true;
                layer_on(_CIRCL);
                cancel_clear_screen();
                oled_clear();
                //         "                '    " 
                oled_write("F1:On  F4:Off  F5:Rst", false);
                oled_write("F7:DFU F2:Wake F9:RGB", false);
                oled_write("                     ", false);
            } else if (record->event.pressed) {
                is_circle = false;
                layer_off(_CIRCL);
                render_mnt_logo();
                delayed_clear_screen(2000);
            }
            return true;
            break;
    }
    return true;
}
