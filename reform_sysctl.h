#include "quantum.h"

// \n and space anywhere in command are ignored
// commands are ended with \r
// numbered arguments are passed before command
// <n>p execute command
//  0     system power off
//  1     system power on (if not already on)
//  2     system reset
//  3     aux power off
//  4     aux power on 
// <n>x test sleep, force_sleep = n
//    a get system current
// <n>v get cell n(0-7) voltage
//    V get system voltage
//    s get charger system state (prepended by firmware rev)
// <n>u toggle reporting to i.MX
//  0     on
//  1     off 
// <n>w wake i.MX
//  0     pulse wake GPIO
//  *     send a string via UART
//    c get status of cells, current, voltage, fuel gauge (som power is last, 'P<n>')
//    S get chargers system cycles in current state
//    C get battery capacity (mAh)
// <n>e toggle serial echo
//  1     on (sends chars as they're read, then a \n)
//  0     off
//    * syntax error

char * remote_try_command(char* cmd, bool print);
char * remote_try_command_size(char* cmd, bool print, uint8_t expected_size);
bool try_wake(bool print);
void render_mnt_logo(void);