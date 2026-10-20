#include "oopsallnaps_core.h"

#include <qp.h>
#include <math.h>
#include <printf.h>

static painter_device_t display       = 0;
bool                    display_is_on = false;
static uint8_t          current_mode  = DM_OFF;
//static painter_font_handle_t large_font = NULL;
static painter_font_handle_t main_font = NULL;


bool should_draw(void) {
    return display_is_on;
}

void display_post_init(void) {
    display = qp_sh1106_make_i2c_device(DISPLAY_WIDTH_ACTUAL, DISPLAY_HEIGHT_ACTUAL, DISPLAY_I2C_ADDRESS);
    if (!display) {
        printf("[oopsallnaps] failed to make oled display !\n");
    }
    // qp_set_viewport_offsets(display, 0, 32);
    if (!qp_init(display, QP_ROTATION_0)) {
        printf("[oopsallnaps] failed to init oled display !\n");
    }

    dprintf("Created display[%p] with [w=%d/h=%d]\n", display, qp_get_width(display), qp_get_height(display));
    display_is_on = true;

    current_mode = DM_BONGO;

    main_font = qp_load_font_mem(font_pixeloid_mono_9px);
    if (main_font) {
        static const char *text = "Booting!";
        int16_t width = qp_textwidth(main_font, text);
        qp_drawtext(display, ((DISPLAY_WIDTH/2) - (width/2)), ((DISPLAY_HEIGHT/2) - (main_font->line_height/2)), main_font, text);
    }
}

void display_powerdown(void) {
    if (!display) {
        printf("[display_powerdown] no display??\n");
        return;
    }
    qp_power(display, false);
    display_is_on = false;
    printf("[display_powerdown] display is off\n");
}

void display_wakeup(void) {
    if (!display) {
        printf("[display_wakeup] no display??\n");
        return;
    }
    qp_power(display, true);
    display_is_on = true;
    printf("[display_wakeup] display is off\n");
}

void display_housekeeping(void) {
    if (!display) {
        return;
    }
    if (!should_draw()) {
        return;
    }

    uint32_t scan_rate = get_scan_rate();

    // If we're hit with terrible performance, just turn it off!
    if (scan_rate < QP_SCANRATE_THRESHOLD) {
        dprintf("Scan rate under %d, skipping!\n", QP_SCANRATE_THRESHOLD);
        return;
    }

    /*
    if (main_font && scan_rate % 100) {
        char text[32] = {""};
        sprintf(text, "SCANS: %ld", scan_rate);
        //int16_t width = qp_textwidth(main_font, text);
        qp_drawtext(display, 0, 0, main_font, text);
    }
    */

    switch (current_mode) {
        case DM_TRAIL:
            scene_wave_trail(display);
            break;
        case DM_BONGO:
            scene_bongo(display, true);
            break;
        default:
            break;
    }
}