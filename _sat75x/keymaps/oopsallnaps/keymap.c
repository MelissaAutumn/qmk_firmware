// Copyright 2023 Andrew Kannan (@awkannan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "oopsallnaps_core.h"
#include "printf.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   ENC_PRESS,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,  KC_DEL,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_BSLS,  KC_DEL,
    KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_ENTER, KC_PGUP,
    KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_RSFT,  KC_UP,    KC_PGDN,
    KC_LCTL,  KC_LGUI,  KC_LALT,                      KC_SPC,                                 KC_RCTL,  MO(1),    KC_RALT,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),
  [1] = LAYOUT_all(
    QK_REBOOT, DB_TOGG, _______, _______, _______, _______, _______, _______, _______, _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, OLED_TOGG,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, MS_BTN1, MS_UP, MS_BTN2, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CLOCK_SET,
    _______, MS_LEFT, MS_DOWN, MS_RGHT, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    QK_BOOT, _______, _______,                   _______,                            _______, _______, _______, _______, _______, _______
  )
};

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

void keyboard_post_init_user(void) {
  debug_enable=true;
  printf("[oopsallnaps] keyboard_post_init_user ran !\n");
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