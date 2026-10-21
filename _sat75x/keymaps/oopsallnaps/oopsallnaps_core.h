#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <qp.h>
#include "fonts/pixeloid_mono_9px.qff.h"
#include "fonts/pixeloid_mono_18px.qff.h"

// core funcs

uint32_t get_scan_rate(void);



// 128x32 usable space
#define DISPLAY_WIDTH_ACTUAL 128
#define DISPLAY_HEIGHT_ACTUAL 32
#define DISPLAY_I2C_ADDRESS 0x3C

// display funcs
#define DISPLAY_WIDTH (DISPLAY_WIDTH_ACTUAL-1)
#define DISPLAY_HEIGHT (DISPLAY_HEIGHT_ACTUAL-1)

enum DISPLAY_MODES {
  DM_DEFAULT,
  DM_TRAIL,
  DM_BONGO,
  DM_OFF,
  _NUM_DM_MODES
};



void display_post_init(void);
void display_powerdown(void);
void display_wakeup(void);
void display_housekeeping(void);

// scenes

void setup_wave_trail(void);
void teardown_wave_trail(void);
void scene_wave_trail(painter_device_t display);

void setup_bongo(void);
void teardown_bongo(void);
void scene_bongo(painter_device_t display, bool minimal);