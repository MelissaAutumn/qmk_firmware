#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <qp.h>
#include "fonts/pixeloid_mono_9px.qff.h"
#include "fonts/pixeloid_mono_18px.qff.h"

#include <hal.h>
#include "satisfaction_keycodes.h"

/*
 * CORE
 */

#define EEPROM_ENABLED_ENCODER_MODES_OFFSET 0
#define EEPROM_DEFAULT_OLED_OFFSET 1
#define EEPROM_CUSTOM_ENCODER_OFFSET 2

// Keyboard Information
extern volatile uint8_t led_numlock;
extern volatile uint8_t led_capslock;
extern volatile uint8_t led_scrolllock;
extern uint8_t          layer;

// OLED Behavior
extern uint8_t  dm_mode;
extern bool     oled_repaint_requested;
extern bool     oled_wakeup_requested;
extern uint32_t oled_sleep_timer;

// Encoder Behavior
extern uint8_t encoder_value;
extern uint8_t encoder_mode;
extern uint8_t enabled_encoder_modes;

// RTC
extern RTCDateTime last_timespec;
extern uint16_t    last_minute;

// RTC Configuration
extern bool    clock_set_mode;
extern uint8_t time_config_idx;
extern int8_t  hour_config;
extern int16_t minute_config;
extern int8_t  year_config;
extern int8_t  month_config;
extern int8_t  day_config;
extern uint8_t previous_encoder_mode;

uint32_t get_scan_rate(void);

/*
 * DISPLAY
 */

typedef struct {
    uint8_t x;
    uint8_t y;
} vec2;

typedef struct {
    vec2 pos;
    uint8_t width;
    uint8_t height;
} rect;

extern painter_font_handle_t main_font;

#define DISPLAY_WIDTH_ACTUAL 128
#define DISPLAY_HEIGHT_ACTUAL 32
#define DISPLAY_I2C_ADDRESS 0x3C

#define DISPLAY_WIDTH (DISPLAY_WIDTH_ACTUAL - 1)
#define DISPLAY_HEIGHT (DISPLAY_HEIGHT_ACTUAL - 1)

enum DISPLAY_MODES { DM_CLOCK, DM_TRAIL, DM_BONGO, DM_OFF, _NUM_DM_MODES };

void change_dm_mode(uint8_t new_mode);

void display_post_init(void);
void display_powerdown(void);
void display_wakeup(void);
void display_housekeeping(void);

void request_wakeup(void);
void request_repaint(void);

/*
 * Custom QP commands
 */

bool set_display_contrast(painter_device_t display, uint8_t val);
bool batch_setpixel(painter_device_t device, const vec2 *sequence, size_t sequence_len, uint8_t offset_x, uint8_t offset_y);

/*
 * SCENES
 */


void setup_clock(void);
void teardown_clock(void);
void scene_clock(painter_device_t display);
void bounds_changed_clock(painter_device_t display);

void setup_wave_trail(painter_device_t display);
void teardown_wave_trail(void);
void scene_wave_trail(painter_device_t display);
void bounds_changed_trail(painter_device_t display);

void setup_bongo(void);
void teardown_bongo(void);
void scene_bongo(painter_device_t display, bool minimal);

// This one is special
void setup_set_clock(void);
void teardown_set_clock(void);
void scene_set_clock(painter_device_t display);

// This is the fixed ui
uint32_t get_fixed_info_width(void);
void setup_fixed_info(void);
void teardown_fixed_info(painter_device_t display);
void scene_fixed_info(painter_device_t display);
void bounds_fixed_info(void);

uint32_t scene_get_usable_width(void);

/*
 * ENCODER
 */

enum encoder_modes {
    ENC_MODE_VOLUME,
    ENC_MODE_MEDIA,
    ENC_MODE_SCROLL,
    ENC_MODE_BRIGHTNESS,
    ENC_MODE_BACKLIGHT,
    ENC_MODE_CUSTOM0,
    ENC_MODE_CUSTOM1,
    ENC_MODE_CUSTOM2,
    _NUM_ENCODER_MODES,
    ENC_MODE_CLOCK_SET // This shouldn't be included in the default modes, so we put it after NUM_ENCODER_MODES
};

enum custom_encoder_behavior { ENC_CUSTOM_CW, ENC_CUSTOM_CCW, ENC_CUSTOM_PRESS };

void     pre_encoder_mode_change(void);
void     post_encoder_mode_change(void);
void     change_encoder_mode(bool negative);
uint16_t handle_encoder_clockwise(void);
uint16_t handle_encoder_ccw(void);
uint16_t handle_encoder_press(void);
uint16_t retrieve_custom_encoder_config(uint8_t encoder_idx, uint8_t behavior);
void     set_custom_encoder_config(uint8_t encoder_idx, uint8_t behavior, uint16_t new_code);

void update_time_config(int8_t increment);

void custom_config_load(void);

uint32_t read_custom_config(void *data, uint32_t offset, uint32_t length);
uint32_t write_custom_config(const void *data, uint32_t offset, uint32_t length);
