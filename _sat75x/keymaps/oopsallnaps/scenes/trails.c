#include "../oopsallnaps_core.h"
#include <qp.h>
#include <math.h>
#include <printf.h>

void setup_wave_trail(void) {
    return;
}

void teardown_wave_trail(void) {
    return;
}

void scene_wave_trail(painter_device_t display) {
    uint32_t     amp   = 10;
    float        freq  = 0.15;
    static float phase = 0.0;

    static uint32_t last_draw = 0;
    static uint32_t last_x    = 0;

    const uint32_t  wave_demo_invalid = 0xFFFF;
    static uint32_t wave_demo_x[16]   = {0xFFFF};
    static uint32_t wave_demo_y[16]   = {0xFFFF};
    static uint32_t wave_demo_idx     = 0;
    if (timer_elapsed32(last_draw) > QP_UPDATE_INTERVAL) {
        last_draw = timer_read32();

        uint32_t y = floor(amp * sin(freq * last_x + phase)) + amp;
        y += 8; // Offset the height so we'e sort of in the middle of the display

        qp_clear(display);

        wave_demo_x[wave_demo_idx]   = last_x;
        wave_demo_y[wave_demo_idx++] = y;
        if (wave_demo_idx > 15) {
            wave_demo_idx = 0;
        }

        last_x++;
        if (last_x >= 127) {
            last_x = 0;
        }

        for (int i = 0; i < ARRAY_SIZE(wave_demo_x); i++) {
            if (wave_demo_x[i] != wave_demo_invalid) {
                qp_setpixel(display, wave_demo_x[i], wave_demo_y[i], 255, 255, 128);
            }
        }
        qp_setpixel(display, last_x, y, 255, 255, 255);

        phase += 0.15;
        qp_flush(display);
    }
}
