// Copyright 2023 Andrew Kannan (@awkannan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "oopsallnaps_core.h"
#include "printf.h"

#include <ch.h>
#include <hal.h>
#include "matrix.h"
#include "quantum.h"
#include "encoder.h"
#include "eeprom.h"
#include "version.h" // for QMK_BUILDDATE used in EEPROM magic

// From satisfaction_core

/* Artificial delay added to get media keys to work in the encoder*/
#define MEDIA_KEY_DELAY 10

volatile uint8_t led_numlock    = false;
volatile uint8_t led_capslock   = false;
volatile uint8_t led_scrolllock = false;

uint8_t layer;

bool     clock_set_mode         = false;
uint8_t  dm_mode                = DM_CLOCK;
bool     oled_repaint_requested = false;
bool     oled_wakeup_requested  = false;
uint32_t oled_sleep_timer;

uint8_t encoder_value         = 32;
uint8_t encoder_mode          = ENC_MODE_VOLUME;
uint8_t enabled_encoder_modes = 0x1F;

RTCDateTime last_timespec;
uint16_t    last_minute = 0;

uint8_t time_config_idx       = 0;
int8_t  hour_config           = 0;
int16_t minute_config         = 0;
int8_t  year_config           = 0;
int8_t  month_config          = 0;
int8_t  day_config            = 0;
uint8_t previous_encoder_mode = 0;

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {[0] = LAYOUT_all(KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, ENC_PRESS, KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_DEL, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENTER, KC_PGUP, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_UP, KC_PGDN, KC_LCTL, KC_LGUI, KC_LALT, KC_SPC, KC_RCTL, MO(1), KC_RALT, KC_LEFT, KC_DOWN, KC_RGHT),
                                                              [1] = LAYOUT_all(QK_REBOOT, DB_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, OLED_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MS_BTN1, MS_UP, MS_BTN2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CLOCK_SET, _______, MS_LEFT, MS_DOWN, MS_RGHT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______)};

static uint32_t matrix_timer           = 0;
static uint32_t matrix_scan_count      = 0;
static uint32_t last_matrix_scan_count = 0;

void track_scan_rate(void) {
    matrix_scan_count++;

    uint32_t timer_now = timer_read32();
    if (TIMER_DIFF_32(timer_now, matrix_timer) >= 1000) {
        last_matrix_scan_count = matrix_scan_count;
        matrix_timer           = timer_now;
        matrix_scan_count      = 0;
    }
}

uint32_t get_scan_rate(void) {
    return last_matrix_scan_count;
}

void board_init(void) {
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_DMA_RMP;
    SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_SPI2_DMA_RMP);
}

void keyboard_post_init_kb(void) {
    debug_enable = true;
    printf("[oopsallnaps] keyboard_post_init_kb ran !\n");
    custom_config_load();

    display_post_init();
}

void suspend_power_down_user(void) {
    display_powerdown();
}

void suspend_wakeup_init_user(void) {
    display_wakeup();
}

void housekeeping_task_user(void) {
    track_scan_rate();
    display_housekeeping();
}

uint32_t read_custom_config(void *data, uint32_t offset, uint32_t length) {
    return eeconfig_read_kb_datablock(data, offset, length);
}

uint32_t write_custom_config(const void *data, uint32_t offset, uint32_t length) {
    return eeconfig_update_kb_datablock(data, offset, length);
}

void read_host_led_state(void) {
    led_t led_state = host_keyboard_led_state();
    if (led_state.num_lock) {
        if (led_numlock == false) {
            led_numlock = true;
        }
    } else {
        if (led_numlock == true) {
            led_numlock = false;
        }
    }
    if (led_state.caps_lock) {
        if (led_capslock == false) {
            led_capslock = true;
        }
    } else {
        if (led_capslock == true) {
            led_capslock = false;
        }
    }
    if (led_state.scroll_lock) {
        if (led_scrolllock == false) {
            led_scrolllock = true;
        }
    } else {
        if (led_scrolllock == true) {
            led_scrolllock = false;
        }
    }
}

layer_state_t layer_state_set_kb(layer_state_t state) {
    state = layer_state_set_user(state);
    layer = get_highest_layer(state);
    display_wakeup();
    return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    display_wakeup();
    switch (keycode) {
        case OLED_TOGG:
            if (!clock_set_mode) {
                if (record->event.pressed) {
                    change_dm_mode((dm_mode + 1) % _NUM_DM_MODES);
                }
            }
            return false;
        case CLOCK_SET:
            if (record->event.pressed) {
                if (clock_set_mode) {
                    pre_encoder_mode_change();
                    clock_set_mode = false;
                    encoder_mode   = previous_encoder_mode;
                    post_encoder_mode_change();

                } else {
                    previous_encoder_mode = encoder_mode;
                    pre_encoder_mode_change();
                    clock_set_mode = true;
                    encoder_mode   = ENC_MODE_CLOCK_SET;
                    post_encoder_mode_change();
                }
            }
            return false;
        case ENC_PRESS:
            if (record->event.pressed) {
                uint16_t mapped_code        = handle_encoder_press();
                uint16_t held_keycode_timer = timer_read();
                if (mapped_code != 0) {
                    register_code16(mapped_code);
                    while (timer_elapsed(held_keycode_timer) < MEDIA_KEY_DELAY) { /* no-op */
                    }
                    unregister_code16(mapped_code);
                }
            } else {
                // Do something else when release
            }
            return false;
        default:
            break;
    }

    return process_record_user(keycode, record);
}

bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;
    display_wakeup();
    encoder_value = (encoder_value + (clockwise ? 1 : -1)) % 64;
    if (index == 0) {
        if (layer == 0) {
            uint16_t mapped_code = 0;
            if (clockwise) {
                mapped_code = handle_encoder_clockwise();
            } else {
                mapped_code = handle_encoder_ccw();
            }
            uint16_t held_keycode_timer = timer_read();
            if (mapped_code != 0) {
                register_code16(mapped_code);
                while (timer_elapsed(held_keycode_timer) < MEDIA_KEY_DELAY) { /* no-op */
                }
                unregister_code16(mapped_code);
            }
        } else {
            if (clockwise) {
                change_encoder_mode(false);
            } else {
                change_encoder_mode(true);
            }
        }
    }
    return true;
}

void custom_config_reset(void) {
    eeconfig_init_kb_datablock();
}

void custom_config_load(void) {
#ifdef DYNAMIC_KEYMAP_ENABLE
    uint8_t new_dm_mode = dm_mode;
    read_custom_config(&new_dm_mode, EEPROM_DEFAULT_OLED_OFFSET, 1);
    read_custom_config(&enabled_encoder_modes, EEPROM_ENABLED_ENCODER_MODES_OFFSET, 1);
    change_dm_mode(new_dm_mode);
#endif
}

void eeconfig_init_kb_datablock(void) {
    uint8_t default_data[EECONFIG_KB_DATA_SIZE] = {0};

    default_data[EEPROM_ENABLED_ENCODER_MODES_OFFSET] = 0x1F;

    eeconfig_update_kb_datablock(default_data, 0, sizeof(default_data));
}

void matrix_init_kb(void) {
    rtcGetTime(&RTCD1, &last_timespec);
    matrix_init_user();
    display_wakeup();
    dprintf("Current year: %d\n", last_timespec.year);
}

void housekeeping_task_kb(void) {
    rtcGetTime(&RTCD1, &last_timespec);
    uint16_t minutes_since_midnight = last_timespec.millisecond / 1000 / 60;

    if (minutes_since_midnight != last_minute) {
        last_minute = minutes_since_midnight;
        // oled_request_repaint();
    }
}
