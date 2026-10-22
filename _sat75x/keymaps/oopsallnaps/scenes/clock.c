#include "../oopsallnaps_core.h"
#include <qp.h>
#include <printf.h>

int  clock_pos_x = 0;
int  clock_pos_y = 0;
int  clock_vel_x = 1;
int  clock_vel_y = 1;
bool invert      = false;

rect dirty_text;

int oled_hid_get_hour(void) {
    return last_minute / 60;
}

int oled_hid_get_minute(void) {
    return last_minute % 60;
}

void setup_clock(void) {
    clock_pos_x = 0;
    clock_pos_y = 0;
    clock_vel_x = 1;
    clock_vel_y = 1;
    invert      = false;
    dirty_text  = (rect){0};
}

void teardown_clock(void) {
    return;
}

void bounds_changed_clock(painter_device_t display) {
    if (clock_pos_x + dirty_text.width >= scene_get_usable_width()) {
        qp_rect(display, dirty_text.pos.x, dirty_text.pos.y, dirty_text.pos.x + dirty_text.width, dirty_text.pos.y + dirty_text.height, 0, 0, 0, true);
        clock_pos_x = (scene_get_usable_width() - (dirty_text.width + 1));
        clock_vel_x *= -1;
        invert = !invert;
    }
}

void scene_clock(painter_device_t display) {
    if (!main_font) {
        return;
    }

    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > QP_UPDATE_INTERVAL) {
        last_draw = timer_read32();

        qp_rect(display, dirty_text.pos.x, dirty_text.pos.y, dirty_text.pos.x + dirty_text.width, dirty_text.pos.y + dirty_text.height, 0, 0, 0, true);

        int hour   = oled_hid_get_hour();
        int minute = oled_hid_get_minute();

        clock_pos_x += clock_vel_x;
        clock_pos_y += clock_vel_y;

        int hour_1 = hour >= 10 ? hour / 10 : 0;
        int hour_2 = hour % 10;
        int min_1  = minute >= 10 ? minute / 10 : 0;
        int min_2  = minute % 10;

        char time_str[32] = {""};
        sprintf(time_str, "%d%d:%d%d", hour_1, hour_2, min_1, min_2);
        int16_t clock_width  = qp_textwidth(main_font, time_str);
        int16_t clock_height = main_font->line_height;

        dirty_text = (rect){(vec2){clock_pos_x, clock_pos_y}, clock_width, clock_height};

        if (!invert) {
            qp_drawtext(display, clock_pos_x, clock_pos_y, main_font, time_str);
        } else {
            qp_drawtext_recolor(display, clock_pos_x, clock_pos_y, main_font, time_str, 0, 0, 0, 255, 255, 255);
        }

        if (clock_pos_x + clock_width >= scene_get_usable_width() || clock_pos_x <= 0) {
            clock_vel_x *= -1;
            invert = !invert;
        }
        if (clock_pos_y + clock_height >= DISPLAY_HEIGHT || clock_pos_y <= 0) {
            clock_vel_y *= -1;
            invert = !invert;
        }

        qp_flush(display);
    }
}
