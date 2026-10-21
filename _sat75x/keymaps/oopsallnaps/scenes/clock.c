#include "../oopsallnaps_core.h"
#include <qp.h>
#include <math.h>
#include <printf.h>

int  clock_pos_x = 0;
int  clock_pos_y = 0;
int  clock_vel_x = 1;
int  clock_vel_y = 1;
bool invert      = false;

int oled_hid_get_hour(void) {
    return last_minute / 60;
}

int oled_hid_get_minute(void) {
    return last_minute % 60;
}

#if 0
static char *get_time(void) {
    uint8_t  hour   = last_minute / 60;
    uint16_t minute = last_minute % 60;

    if (encoder_mode == ENC_MODE_CLOCK_SET) {
        hour   = hour_config;
        minute = minute_config;
    }

    bool is_pm = (hour / 12) > 0;
    hour       = hour % 12;
    if (hour == 0) {
        hour = 12;
    }

    static char time_str[8] = "";
    snprintf(time_str, sizeof(time_str), "%02hhu:%02hu%s", hour, minute, is_pm ? "pm" : "am");

    return time_str;
}

static char *get_date(void) {
    int16_t year  = last_timespec.year + 1980;
    int8_t  month = last_timespec.month;
    int8_t  day   = last_timespec.day;

    if (encoder_mode == ENC_MODE_CLOCK_SET) {
        year  = year_config + 1980;
        month = month_config;
        day   = day_config;
    }

    static char date_str[11] = "";
    snprintf(date_str, sizeof(date_str), "%04hd-%02hhd-%02hhd", year, month, day);

    return date_str;
}
#endif

void setup_clock(void) {
    clock_pos_x = 0;
    clock_pos_y = 0;
    clock_vel_x = 1;
    clock_vel_y = 1;
    invert      = false;
}

void teardown_clock(void) {
    return;
}

void scene_clock(painter_device_t display) {
    if (!main_font) {
        return;
    }

    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > QP_UPDATE_INTERVAL) {
        last_draw = timer_read32();

        qp_clear(display);

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

        if (!invert) {
            qp_drawtext(display, clock_pos_x, clock_pos_y, main_font, time_str);
        } else {
            qp_drawtext_recolor(display, clock_pos_x, clock_pos_y, main_font, time_str, 0, 0, 0, 255, 255, 255);
        }

        if (clock_pos_x + clock_width >= DISPLAY_WIDTH || clock_pos_x <= 0) {
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
