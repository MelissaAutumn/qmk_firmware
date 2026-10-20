# Wildcard to allow APM32 MCU
DFU_SUFFIX_ARGS = -v FFFF -p FFFF

VPATH += keyboards/cannonkeys/lib/satisfaction75
#SRC += satisfaction_encoder.c \ 
#      satisfaction_oled.c \
#      satisfaction_core.c 
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += sh1106_i2c