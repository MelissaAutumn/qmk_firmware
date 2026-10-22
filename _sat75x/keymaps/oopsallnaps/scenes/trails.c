#include "../oopsallnaps_core.h"
#include "deferred_exec.h"
#include "util.h"
#include <qp.h>
#include <math.h>
#include <printf.h>
#include <stdbool.h>

bool           wave_is_setup = false;
vec2           wave_trail[64];
uint32_t       wave_demo_idx        = 0;
const uint32_t wave_demo_invalid    = 0xFF;
uint32_t       wave_frame_last_draw = 0;
uint32_t       wave_last_x          = 0;
bool           wave_invert          = false;
bool           wave_invert_y        = false;

const uint32_t wave_amp    = 10;
const float    wave_freq   = 5.0f;
const uint32_t half_height = (float)DISPLAY_HEIGHT / 2.0f;
float          wave_phase  = 0.0f;

void setup_wave_trail(painter_device_t display) {
    // scene does not clear by itself, so clear here to remove previous scene!
    qp_clear(display);

    if (wave_is_setup) {
        return;
    }

    // We only need to this once!
    memset(wave_trail, wave_demo_invalid, sizeof(wave_trail));
    wave_demo_idx        = 0;
    wave_frame_last_draw = 0;
    wave_last_x          = 0;
    wave_phase           = 0.0f;
    wave_is_setup        = true;
    wave_invert          = false;
    wave_invert_y        = false;
}

void teardown_wave_trail(void) {
    return;
}

void bounds_changed_trail(painter_device_t display) {
    for (int i = 0; i < ARRAY_SIZE(wave_trail); i++) {
        // invalidate any trails past usable width
        if (wave_trail[i].x > scene_get_usable_width()) {
            //qp_setpixel(display, wave_trail[i].x, wave_trail[i].y, 0, 0, 0);

            wave_trail[i].x = wave_demo_invalid;
            wave_trail[i].y = wave_demo_invalid;
        }
    }
    if (wave_last_x > scene_get_usable_width()) {
        wave_last_x = 0;
    }
    qp_rect(display, 0, 0, scene_get_usable_width()+1, DISPLAY_HEIGHT, 0, 0, 0, true);
}

void scene_wave_trail(painter_device_t display) {
    if (timer_elapsed32(wave_frame_last_draw) > 16) {
        wave_frame_last_draw     = timer_read32();
        const uint32_t max_width = scene_get_usable_width();

        // Manually dirty the oldest demo idx
        const uint8_t oldest_demo_idx = wave_demo_idx + 1 % ARRAY_SIZE(wave_trail) - 1;
        qp_setpixel(display, wave_trail[oldest_demo_idx].x, wave_trail[oldest_demo_idx].y, 0, 0, 0);

        if (wave_trail[oldest_demo_idx].x + 1 <= max_width) qp_setpixel(display, (wave_trail[oldest_demo_idx].x + 1) % max_width, wave_trail[oldest_demo_idx].y - 1, 0, 0, 0);
        if (wave_trail[oldest_demo_idx].x + 2 <= max_width) qp_setpixel(display, (wave_trail[oldest_demo_idx].x + 2) % max_width, wave_trail[oldest_demo_idx].y + 1, 0, 0, 0);
        if (wave_trail[oldest_demo_idx].x + 3 <= max_width) qp_setpixel(display, (wave_trail[oldest_demo_idx].x + 3) % max_width, wave_trail[oldest_demo_idx].y, 0, 0, 0);

        // Kudos to https://gist.github.com/gkhays/e264009c0832c73d5345847e673a64ab
        uint32_t y = half_height + wave_amp * sin(wave_phase + wave_last_x / wave_freq);

        wave_last_x++;
        if (wave_last_x > scene_get_usable_width()) {
            wave_last_x = 0;
        }

        wave_trail[wave_demo_idx++] = (vec2){.x = wave_last_x, .y = y};
        if (wave_demo_idx > ARRAY_SIZE(wave_trail) - 1) {
            wave_demo_idx = 0;
        }

        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 0, 0);

        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 1, -1);
        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 2, 1);

        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 3, 0);

        wave_phase += 0.1;
        qp_flush(display);
    }
}
