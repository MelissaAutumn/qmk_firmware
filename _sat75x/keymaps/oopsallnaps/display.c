#include "oopsallnaps_core.h"

#include <qp.h>
#include <printf.h>
#include <stdint.h>
#include <stdio.h>

static painter_device_t display       = 0;
bool                    display_is_on = false;
// static painter_font_handle_t large_font = NULL;
painter_font_handle_t main_font = NULL;

const char *low_scan_rate_warning  = "[!]";
bool has_low_scan_rate_warning_drawn = false;

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

    main_font = qp_load_font_mem(font_pixeloid_mono_9px);
    if (main_font) {
        static const char *text  = "Booting!";
        int16_t            width = qp_textwidth(main_font, text);
        qp_drawtext(display, ((DISPLAY_WIDTH / 2) - (width / 2)), ((DISPLAY_HEIGHT / 2) - (main_font->line_height / 2)), main_font, text);
    }

    // Setup our default dm_mode!
    change_dm_mode(dm_mode);

    
}

void display_powerdown(void) {
    if (!display || !display_is_on) {
        return;
    }
    qp_power(display, false);
    display_is_on = false;
}

void display_wakeup(void) {
    if (!display || display_is_on) {
        return;
    }
    qp_power(display, true);
    display_is_on = true;
}

void change_dm_mode(uint8_t new_mode) {
    switch (dm_mode) {
        case DM_CLOCK:
            teardown_clock();
            break;
        case DM_TRAIL:
            teardown_wave_trail();
            break;
        case DM_BONGO:
            teardown_bongo();
            break;
        default:
            break;
    }

    switch (new_mode) {
        case DM_CLOCK:
            setup_clock();
            break;
        case DM_TRAIL:
            setup_wave_trail();
            break;
        case DM_BONGO:
            setup_bongo();
            break;
        case DM_OFF:
            qp_clear(display);
            break;
        default:
            break;
    }

    dm_mode = new_mode;
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
        //dprintf("Scan rate under %d, skipping!\n", QP_SCANRATE_THRESHOLD);
        if (!has_low_scan_rate_warning_drawn) {
            qp_drawtext(display, 0, 0, main_font, low_scan_rate_warning);
            has_low_scan_rate_warning_drawn = true;
        }
        return;
    }

    if (has_low_scan_rate_warning_drawn) {
        qp_clear(display);
        has_low_scan_rate_warning_drawn = false;
    }

    if (clock_set_mode) {
        scene_set_clock(display);
        return;
    }

    switch (dm_mode) {
        case DM_CLOCK:
            scene_clock(display);
            break;
        case DM_TRAIL:
            scene_wave_trail(display);
            break;
        case DM_BONGO:
            scene_bongo(display, true);
            break;
        case DM_OFF:
            break;
        default:
            break;
    }
}