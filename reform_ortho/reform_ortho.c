#include "reform_ortho.h"
#include "serial_vendor.h"
#include "reform_sysctl.h"
#include "hardware/watchdog.h"
#include "raw_hid.h"

void suspend_power_down_kb(void) {
    //oled_clear();
    //oled_write("Powering down", false);
}

void suspend_wakeup_init_kb(void) {
    //oled_clear();
    //oled_write("Waking up", false);
}

uint32_t batteryCheck = 0;
bool flip = false;

#define BATTERY_CHECK_FREQUENCY 15000
int8_t battery_status = -1;
int32_t charge_status = 0;

int8_t get_battery_status(void) {
    return battery_status;
}

int32_t get_charge_status(void) {
    return charge_status;
}

uint32_t update_battery_status(uint32_t trigger_time, void *cb_arg) {
    char * batteryStats = remote_try_command_size("c", true, 47);
    // char * batteryStats = remote_try_command("c", false);
    // if wrong, reschedule to run later
    if (strlen(batteryStats) < 47) {
      return 1000;
    }

    if (*batteryStats) {
        char * battery = &batteryStats[39];
        // probably won't be above 100
        if (*battery == '1') {
            battery_status = 100;
        } else {
            battery++;
            battery_status = (battery[0] - '0') * 10 + (battery[1] - '0');
        }
        char * charging = &batteryStats[26];
        charge_status = (charging[1] - '0') * 1000 + 
                        (charging[2] - '0') * 100 + 
                        (charging[3] - '0') * 10 + 
                        (charging[4] - '0');
        if (*charging == '-')
            charge_status *= -1;
    }
    return BATTERY_CHECK_FREQUENCY;
}

// hid commands are all 4-letter, so they fit in a 32 bit integer
#define cmd(_s) (*(uint32_t *)(_s))
#define CMD_TEXT_FRAME      cmd("OLED")     // fill the screen with a single wall of text
#define CMD_ROW_INVERT      cmd("OINV")     // invert a line of text
#define CMD_REPORT_POWER    cmd("RPRT")     // ask for power stats report over UART
#define CMD_OLED_CLEAR      cmd("WCLR")     // clear the oled display
#define CMD_OLED_BITMAP     cmd("WBIT")     // (u16 offset, u8 bytes...) write raw bytes into the oled framebuffer
#define CMD_POWER_OFF       cmd("PWR0")     // turn off power rails
#define CMD_BACKLIGHT       cmd("LITE")     // keyboard backlight level
#define CMD_UART_ON         cmd("UAR1")     // uart reporting on
#define CMD_UART_OFF        cmd("UAR0")     // uart reporting off
#if 0
void raw_hid_receive(uint8_t *data, uint8_t length) {
    oled_write((char*)data, false);
    if (length < 5)
        return;
    uint32_t command = cmd(data + 1);

// char buf[6];
// sprintf(buf, "%d", command);
// raw_hid_send((uint8_t*)buf, 6);
// return;

  if (command == CMD_TEXT_FRAME) {
    /*gfx_on();
    for (int y = 0; y < 4; y++) {
      for (int x = 0; x < 21; x++) {
        gfx_poke(x, y, data[4 + y * 21 + x]);
      }
    }
    gfx_flush();*/
  }
  else if (command == CMD_ROW_INVERT) {
    //gfx_clear_invert();
    //gfx_invert_row(data[4] - '0');
  }
  else if (command == CMD_BACKLIGHT) {
    char brite = data[4] - '0';
    brite++;
    if (brite <= 1) brite = 0;
    if (brite > 9) brite = 9;
   // kbd_brightness_set(brite);
  }
  else if (command == CMD_POWER_OFF) {
    //reset_menu();
    //anim_goodbye();
    remote_try_command("0p", true);
    //keyboard_power_off();
    //reset_keyboard_state();
  }
  else if (command == CMD_UART_ON) {
    remote_try_command("1u", true);
  }
  else if (command == CMD_UART_OFF) {
    remote_try_command("0u", true);
  }
  else if (command == CMD_REPORT_POWER) {
    remote_try_command("V", true);
  }
  else if (command == CMD_OLED_BITMAP) {
    oled_write_P(data + 4, false);
  }
  else if (command == CMD_OLED_CLEAR) {
    oled_clear();
  }
}
#endif

void keyboard_post_init_kb(void) {
    pio_init(SERIAL_USART_TX_PIN, SERIAL_USART_RX_PIN);
    serial_clear();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    #ifdef MNT_REFORM_BATTERY_UPDATE
      defer_exec(50, update_battery_status, NULL);
    #endif
    if (watchdog_enable_caused_reboot()) {
        oled_write("Watchdog reboot", false);
    } else {
        render_mnt_logo();
    }
    delayed_clear_screen(2000);
    watchdog_enable(3000, 1);
}

void matrix_scan_kb(void) {
    watchdog_update();
}

led_config_t g_led_config = { {
    // Key Matrix to LED Index
    {0,              1,              2,              3,              4,              5,              6,              7,              8,              9,             10,             11,             12,             13,             14},
    {29,            28,             27,             26,             25,             24,             23,             22,             21,             20,             19,             18,             17,             16,             15},
    {30,            31,             32,             33,             34,             35,             36,             37,             38,             39,             40,             41,             42,             43,             44},
    {59,            58,             57,             56,             55,             54,             53,             52,             51,             50,             49,             48,             47,             46,             45},
    {60,            61,             62,             63,             64,             65,             66,             67,             68,             69,             70,             71,             72,             73,             74},
    {89,            88,             87,             86,             85,             84,             83,             82,             81,             80,             79,             78,             77,             76,             75}
}, {
    // LED Index Physical location
    {0,0},          {17,0},         {34,0},         {51,0},         {68,0},         {85,0},         {102,0},        {112,0},        {122,0},        {139,0},        {156,0},        {173,0},        {190,0},        {207,0},        {224,0},
    {224,13},       {207,13},       {190,13},       {173,13},       {156,13},       {139,13},       {122,13},       {112,13},       {102,13},       {85,13},        {68,13},        {51,13},        {34,13},        {17,13},        {0,13},
    {0,26},         {17,26},        {34,26},        {51,26},        {68,26},        {85,26},        {102,26},       {112,26},       {122,26},       {139,26},       {156,26},       {173,26},       {190,26},       {207,26},       {224,26},
    {224,38},       {207,38},       {190,38},       {173,38},       {156,38},       {139,38},       {122,38},       {112,38},       {102,38},       {85,38},        {68,38},        {51,38},        {34,38},        {17,38},        {0,38},
    {0,51},         {17,51},        {34,51},        {51,51},        {68,51},        {85,51},        {102,51},       {112,51},       {122,51},       {139,51},       {156,51},       {173,51},       {190,51},       {207,51},       {224,51},
    {224,64},       {207,64},       {190,64},       {173,64},       {156,64},       {139,64},       {122,64},       {112,64},       {102,64},       {85,64},        {68,64},        {51,64},        {34,64},        {17,64},        {0,64}
}, {
    // LED Index to Flag
    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,
    LED_FLAG_NONE,  LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_NONE,
    LED_FLAG_NONE,  LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_NONE,
    LED_FLAG_NONE,  LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_NONE,
    LED_FLAG_NONE,  LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_NONE,
    LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_INDICATOR,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT,   LED_FLAG_KEYLIGHT
} };