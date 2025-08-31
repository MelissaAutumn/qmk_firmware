// Copyright 2023 Andrew Kannan (@awkannan)
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#ifdef OLED_HID_ENABLE
#include "raw_hid.h"
#include "../../lib/satisfaction75/oled_hid.h"
#endif

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
    QK_REBOOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MEDIA_PREV_TRACK, KC_MEDIA_PLAY_PAUSE, KC_MEDIA_NEXT_TRACK, OLED_TOGG,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, CLOCK_SET,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    QK_BOOT, _______, _______,                   _______,                            _______, _______, _______, _______, _______, _______
  )
};



void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable=true;
    //debug_matrix=true;
    //debug_keyboard=true;
    //debug_mouse=true;
}

#ifdef OLED_HID_ENABLE
/*
Src: https://github.com/fcambus/spleen
Copyright (c) 2018-2024, Frederic Cambus
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#define SPLEEN_DIGITS_LENGTH 11
#define SPLEEN_DIGITS_SIZE 36
#define SPLEEN_DIGITS_ROW_SIZE SPLEEN_DIGITS_SIZE / 3
static const char PROGMEM spleen_digits[SPLEEN_DIGITS_LENGTH][SPLEEN_DIGITS_SIZE] = {
// 0
{
    0x00, 0xC0, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0xC0, 0x00,
    0x00, 0xFF, 0xFF, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0xFF, 0xFF, 0x00,
    0x00, 0x01, 0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// 1
{
    0x00, 0x00, 0xC0, 0x60, 0x30, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x04, 0x04, 0x07, 0x07, 0x04, 0x04, 0x04, 0x00, 0x00,
},
// 2
{
    0x00, 0x40, 0x60, 0x30, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0xC0, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xC0, 0x60, 0x30, 0x18, 0x0C, 0x07, 0x03, 0x00,
    0x00, 0x06, 0x07, 0x05, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x06, 0x00,
},
// 3
{
    0x00, 0x40, 0x60, 0x30, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0xC0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x04, 0x0E, 0xFB, 0xF1, 0x00,
    0x00, 0x01, 0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// 4
{
    0x00, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x7F, 0x7F, 0x40, 0x40, 0x40, 0x40, 0xFF, 0xFF, 0x40, 0x40, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00,
},
// 5
{
    0x00, 0xF0, 0xF0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x30, 0x00,
    0x00, 0x07, 0x07, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0C, 0xF8, 0xF0, 0x00,
    0x00, 0x01, 0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// 6
{
    0x00, 0xC0, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x20, 0x00,
    0x00, 0xFF, 0xFF, 0x04, 0x04, 0x04, 0x04, 0x04, 0x0C, 0xF8, 0xF0, 0x00,
    0x00, 0x01, 0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// 7
{
    0x00, 0x30, 0x30, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xF0, 0xF0, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xE0, 0xF0, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
},
// 8
{
    0x00, 0xC0, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0xC0, 0x00,
    0x00, 0xF1, 0xFB, 0x0E, 0x04, 0x04, 0x04, 0x04, 0x0E, 0xFB, 0xF1, 0x00,
    0x00, 0x01, 0x03, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// 9
{
    0x00, 0xC0, 0xE0, 0x30, 0x10, 0x10, 0x10, 0x10, 0x30, 0xE0, 0xC0, 0x00,
    0x00, 0x07, 0x0F, 0x18, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFF, 0xFF, 0x00,
    0x00, 0x02, 0x06, 0x04, 0x04, 0x04, 0x04, 0x04, 0x06, 0x03, 0x01, 0x00,
},
// :
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
}
};


char test_str[32] = {0};
int cpu_perc = 0;
int gpu_perc = 0;
int ram_perc = 0;

#define PERF_MULT_BY OLED_DISPLAY_HEIGHT / 100.0f
#define PERF_BAR_WIDTH 4

static inline void draw_large_digit(int idx, int col, int line) {
	const char* digit_row_ptr = spleen_digits[idx];
	oled_set_cursor(col, line);

	oled_write_raw(digit_row_ptr, SPLEEN_DIGITS_ROW_SIZE);
	oled_set_cursor(col, line+1);

	digit_row_ptr += SPLEEN_DIGITS_ROW_SIZE;
	oled_write_raw(digit_row_ptr, SPLEEN_DIGITS_ROW_SIZE);
	oled_set_cursor(col, line+2);

	digit_row_ptr += SPLEEN_DIGITS_ROW_SIZE;
	oled_write_raw(digit_row_ptr, SPLEEN_DIGITS_ROW_SIZE);
}

void oled_hid_draw(void) {
//void oled_draw_perf(void) {
/*
	int cpu_perc_scaled = cpu_perc * PERF_MULT_BY;
	int gpu_perc_scaled = gpu_perc * PERF_MULT_BY;
	int ram_perc_scaled = ram_perc * PERF_MULT_BY;

 // Didn't fit :(
	int start_x = 2;
	if (cpu_perc_scaled > 0) {
	for (int x = start_x; x < start_x + PERF_BAR_WIDTH; x++) {
		for (int y = cpu_perc_scaled; y > 0 ; y--) {
			oled_write_pixel(x, y, true);
		}
	}
	}

	start_x = 2 + PERF_BAR_WIDTH + 2;
	if (gpu_perc_scaled > 0) {
	for (int x = start_x; x < PERF_BAR_WIDTH + start_x; x++) {
		for (int y = gpu_perc_scaled; y > 0 ; y--) {
			oled_write_pixel(x, y, true);
		}
	}
	}

	start_x = 4 + (PERF_BAR_WIDTH*2) + 4;
	if (ram_perc_scaled > 0) {
	for (int x = start_x; x < PERF_BAR_WIDTH + start_x; x++) {
		for (int y = ram_perc_scaled; y > 0 ; y--) {
			oled_write_pixel(x, y, true);
		}
	}
	}

	// draw divider
	start_x += 2 + PERF_BAR_WIDTH;
	for (int x = start_x; x < start_x + 1; x++) {
			for (int y = 0; y < OLED_DISPLAY_HEIGHT; y++) {
				oled_write_pixel(x, y, true);
			}
		}
*/

	// Get past divider

	// start_x needs to be at 25 pixels


	// Write stats
	char buf[32] = {0};
	oled_set_cursor(0, 0);

	sprintf(buf, "CPU: %d%%", cpu_perc);
	oled_write(buf, false);

	oled_set_cursor(0, 1);
	sprintf(buf, "GPU: %d%%", gpu_perc);
	oled_write(buf, false);

	oled_set_cursor(0, 2);
	sprintf(buf, "RAM: %d%%", ram_perc);
	oled_write(buf, false);


	// Draw digits
	int hour = oled_hid_get_hour();
	int minute = oled_hid_get_minute();

	int start_col = 9;
	int start_row = 0;

	if (hour >= 10) {
		draw_large_digit((hour / 10) % 10, start_col, start_row);
		draw_large_digit(hour % 10, start_col+2, start_row);
	} else {
		draw_large_digit(0, start_col, start_row);
		draw_large_digit(hour % 10, start_col+2, start_row);
	}

	// :
	draw_large_digit(10, start_col+4, start_row);

	if (minute >= 10) {
		draw_large_digit((minute / 10) % 10, start_col+6, start_row);
		draw_large_digit(minute % 10, start_col+8, start_row);
	} else {
		draw_large_digit(0, start_col+6, start_row);
		draw_large_digit(minute % 10, start_col+8, start_row);
	}
}

void oled_hid_draw_old(void) {
	oled_set_cursor(0, 0);
	oled_write_P(test_str, false);
}

enum OLED_HID_COMMANDS {
	OLED_HID_NULL = 0x0,
	OLED_HID_OK = 0x1,
	OLED_HID_ERR = 0x2,
	OLED_HID_CPU_PERC = 0x3,
	OLED_HID_GPU_PERC = 0x4,
	OLED_HID_RAM_PERC = 0x5,

	// Special command
	OLED_HID_TIME_SYNC = 0x99,
};

void raw_hid_receive(uint8_t *data, uint8_t length) {
	//printf("--raw_hid_recieve(%p, %d)\n",data,length);
	//memcpy((void*)test_str, data, length);
	uint8_t* cursor = data;

	while (*cursor != OLED_HID_NULL) {
		switch(*(cursor++)) { // inc cursor as we go
			default:
			case OLED_HID_NULL:
				break;
			case OLED_HID_CPU_PERC:
				cpu_perc = (int)*(cursor++);
				break;
			case OLED_HID_GPU_PERC:
				gpu_perc = (int)*(cursor++);
				break;
			case OLED_HID_RAM_PERC:
				ram_perc = (int)*(cursor++);
				break;
#ifdef HID_TIMESYNC_ENABLE
			case OLED_HID_TIME_SYNC:
			{
				int byte1 = (int)*(cursor++);
				int byte2 = (int)*(cursor++);
				int byte3 = (int)*(cursor++);
				int byte4 = (int)*(cursor++);
				int unix_time = byte1 << 24 | byte2 << 16 | byte3 << 8 | byte4;
				printf("--unix timestamp <%d>\n", unix_time);
				oled_hid_set_time((time_t)unix_time);

				//int year = (int)*(cursor++);
				//int month = (int)*(cursor++);
				//int day = (int)*(cursor++);
				//int hour = (int)*(cursor++);
				//int min = (int)*(cursor++);
				//int sec = (int)*(cursor++);
				//oled_hid_set_time(year, month, day, hour, min, sec);
			}
			break;
#endif
		}
	}

	//printf("--raw_hid_recieve (%d, %d, %d)\n", cpu_perc, gpu_perc, ram_perc);

    uint8_t response[length];
    memset(response, 0, length);
    response[0] = OLED_HID_OK;
    raw_hid_send(response, length);
}



#endif