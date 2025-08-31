# Wildcard to allow APM32 MCU
DFU_SUFFIX_ARGS = -v FFFF -p FFFF

VPATH += keyboards/cannonkeys/lib/satisfaction75
SRC += satisfaction_encoder.c \
      satisfaction_oled.c \
      satisfaction_core.c

# Extra option
COMMAND_ENABLE = yes    # Commands for debug and configuration
VIA_ENABLE = yes
WPM_ENABLE = yes


# From https://github.com/BlankSourceCode/qmk_firmware/commit/3ae097783d65e71062606906f7b4be639d9d321d#diff-8be459421b650fb05ccbcdde3bee40aa3be652af20b1b63e82dd98ffdc7fdcab
RAW_ENABLE = yes            # Enable RAW HID communication to PC
Link_Time_Optimization = yes # Reduce size of firmware by optimizing at link time
EXTRAFLAGS += -flto