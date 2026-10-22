# Wildcard to allow APM32 MCU
DFU_SUFFIX_ARGS = -v FFFF -p FFFF

VPATH += keyboards/cannonkeys/lib/satisfaction75
#SRC += satisfaction_encoder.c \ 
#      satisfaction_oled.c \
#      satisfaction_core.c 
SRC += oopsallnaps_ssd1306.c \
	oopsallnaps_encoder.c \
	oopsallnaps_qp.c \
	display.c \
	scenes/fixed_info.c \
	scenes/clock.c \
	scenes/set_clock.c \
	scenes/trails.c \
	scenes/bongo.c \
	fonts/pixeloid_mono_9px.qff.c \
	images/bongo_minimal_idle.qgf.c \
	images/prep_minimal_0.qgf.c \
	images/tap_minimal_0.qgf.c \
	images/tap_minimal_1.qgf.c
	#fonts/pixeloid_mono_18px.qff.c

QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += sh1106_i2c