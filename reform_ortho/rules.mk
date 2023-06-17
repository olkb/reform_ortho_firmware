# MCU name
MCU = RP2040
# Bootloader selection
BOOTLOADER = rp2040
# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = yes      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes        # Mouse keys
EXTRAKEY_ENABLE = yes       # Audio control and System control
CONSOLE_ENABLE = yes         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output
ENCODER_ENABLE = no

RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812
# RGB_MATRIX_CUSTOM_KB = yes
WS2812_DRIVER = vendor
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
SERIAL_DRIVER = vendor
ALLOW_WARNINGS = yes
DEFERRED_EXEC_ENABLE = yes
# VIRTSER_ENABLE = yes
RAW_ENABLE = yes
