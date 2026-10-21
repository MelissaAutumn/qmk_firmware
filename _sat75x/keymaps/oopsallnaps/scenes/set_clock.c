#include "../oopsallnaps_core.h"
#include <qp.h>
#include <math.h>
#include <printf.h>

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

void setup_set_clock(void) {
    return;
}

void teardown_set_clock(void) {
    return;
}

void scene_set_clock(painter_device_t display) {
    if (!main_font) {
        return;
    }

    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > QP_UPDATE_INTERVAL) {
        last_draw = timer_read32();

        qp_clear(display);

        char *time = get_time();
        char *date = get_date();

        int16_t time_width = qp_textwidth(main_font, time);
        int16_t date_width = qp_textwidth(main_font, date);

        qp_drawtext(display, ((DISPLAY_WIDTH / 2) - (date_width / 2)), (((DISPLAY_HEIGHT / 2) - (main_font->line_height / 2))), main_font, date);
        qp_drawtext(display, ((DISPLAY_WIDTH / 2) - (time_width / 2)), (((DISPLAY_HEIGHT / 2) - (main_font->line_height / 2) + main_font->line_height)), main_font, time);

        qp_flush(display);
    }
}
