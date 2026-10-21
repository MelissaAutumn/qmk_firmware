#include "../oopsallnaps_core.h"
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

const uint32_t wave_amp   = 10;
const float    wave_freq  = 0.05f;
float          wave_phase = 0.0f;

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
    wave_is_setup        = false;
    wave_invert          = false;
    wave_invert_y        = false;
}

void teardown_wave_trail(void) {
    return;
}

void scene_wave_trail(painter_device_t display) {
    if (timer_elapsed32(wave_frame_last_draw) > 16) {
        wave_frame_last_draw = timer_read32();

        // Manually dirty the oldest demo idx
        const uint8_t oldest_demo_idx = wave_demo_idx + 1 % ARRAY_SIZE(wave_trail)-1;
        qp_setpixel(display, wave_trail[oldest_demo_idx].x, wave_trail[oldest_demo_idx].y, 0, 0, 0);
        qp_setpixel(display, wave_trail[oldest_demo_idx].x + 1, wave_trail[oldest_demo_idx].y + 1, 0, 0, 0);
        qp_setpixel(display, wave_trail[oldest_demo_idx].x + 2, wave_trail[oldest_demo_idx].y + 1, 0, 0, 0);
        qp_setpixel(display, wave_trail[oldest_demo_idx].x + 3, wave_trail[oldest_demo_idx].y, 0, 0, 0);

        uint32_t y = (wave_amp * sin(wave_freq * wave_last_x + wave_phase)) + wave_amp;
        y += 8; // Offset the height so we'e sort of in the middle of the display

        wave_last_x++;
        if (wave_last_x >= DISPLAY_WIDTH) {
            wave_last_x = 0;
        }

        wave_trail[wave_demo_idx++] = (vec2){.x = wave_last_x, .y = y};
        if (wave_demo_idx > ARRAY_SIZE(wave_trail) - 1) {
            wave_demo_idx = 0;
        }


#if 1
        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 0, 0);
        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 1, 1);
        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 2, 1);
        batch_setpixel(display, wave_trail, ARRAY_SIZE(wave_trail), 3, 0);

#else

        for (int i = 0; i < ARRAY_SIZE(wave_trail); i++) {
            const vec2 *trail_vec2 = &wave_trail[i];
            if (trail_vec2->x != wave_demo_invalid) {
                qp_setpixel(display, trail_vec2->x, trail_vec2->y, 255, 255, 255);
#    if 0
                wave_invert = trail_vec2->x % 3;
                wave_invert_y = trail_vec2->y % 3;

                qp_setpixel(display, trail_vec2->x - (wave_invert ? 2 : 1), trail_vec2->y + (wave_invert_y ? 0 : 1), 255, 255, 255);
                qp_setpixel(display, trail_vec2->x - (wave_invert ? 3 : 4), trail_vec2->y + (wave_invert_y ? 1 : 0), 255, 255, 255);
                qp_setpixel(display, trail_vec2->x - (wave_invert ? 5 : 6), trail_vec2->y + (wave_invert_y ? 1 : 0), 255, 255, 255);
                qp_setpixel(display, trail_vec2->x - (wave_invert ? 8 : 7), trail_vec2->y + (wave_invert_y ? 0 : 1), 255, 255, 255);
#    endif
            }
        }
#endif

        // wave_invert = !wave_invert;

        //qp_setpixel(display, wave_last_x, y, 255, 255, 255);

        wave_phase += 0.1;
        qp_flush(display);
    }
}
