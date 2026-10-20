// Copyright 2023 Andrew Kannan (@awkannan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "print.h"

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

// oled_init seems pretty slow, give it some extra time to warm up
static uint32_t i2c_timeout = 500;

void keyboard_post_init_user(void) {
  printf("[oopsallnaps] keyboard_post_init_user ran !\n");

  // We're after oled_init so set it to the normal timeout!
  i2c_timeout = OLED_I2C_TIMEOUT;
}

#include "i2c_master.h"

bool oled_send_cmd(const uint8_t *data, uint16_t size) {
    i2c_status_t status = i2c_transmit((OLED_DISPLAY_ADDRESS << 1), data, size, i2c_timeout);
    // If we timed out, try again immediately...
    if (status == I2C_STATUS_TIMEOUT) {
      status = i2c_transmit((OLED_DISPLAY_ADDRESS << 1), data, size, i2c_timeout);
      dprintf("[oopsallnaps.oled_send_cmd] returned = %d with timeout of %ld\n", status, i2c_timeout);

    }

    return (status == I2C_STATUS_SUCCESS);
}