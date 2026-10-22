#include "../oopsallnaps_core.h"
#include "action_layer.h"
#include <qp.h>
#include <printf.h>

typedef struct {
    uint8_t  encoder_mode;
    uint16_t layer_state;
    bool     caps_lock;
    bool     scroll_lock;
} fixed_info_state;

bool force_update = false;
const uint32_t   fixed_info_width = 42;
fixed_info_state cached_state     = {0};



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

static char *get_enc_mode(void) {
    switch (encoder_mode) {
        default:
        case ENC_MODE_VOLUME:
            return "VOL";
        case ENC_MODE_MEDIA:
            return "MED";
        case ENC_MODE_SCROLL:
            return "SCR";
        case ENC_MODE_BRIGHTNESS:
            return "BRT";
        case ENC_MODE_BACKLIGHT:
            return "BKL";
        case ENC_MODE_CLOCK_SET:
            return "CLK";
        case ENC_MODE_CUSTOM0:
            return "CS0";
        case ENC_MODE_CUSTOM1:
            return "CS1";
        case ENC_MODE_CUSTOM2:
            return "CS2";
    }
}

uint32_t get_fixed_info_width(void) {
    return fixed_info_width;
}

void setup_fixed_info(void) {
    return;
}

void teardown_fixed_info(painter_device_t display) {
    qp_rect(display, scene_get_usable_width()+1, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0, true);
}

void bounds_fixed_info(void) {
    force_update = true;
}

__attribute__((unused)) void draw_time(painter_device_t display) {
    char *time = get_time();
    char *date = get_date();

    int16_t time_width = qp_textwidth(main_font, time);
    int16_t date_width = qp_textwidth(main_font, date);

    const uint32_t offset_x = scene_get_usable_width() + 1;
    const vec2     time_pos = (vec2){offset_x + 4, 4};
    const vec2     date_pos = (vec2){offset_x + 4, 4 + main_font->line_height};

    qp_rect(display, time_pos.x, time_pos.y, time_pos.x + time_width, time_pos.y + main_font->line_height, 0, 0, 0, true);
    qp_rect(display, date_pos.x, date_pos.y, date_pos.x + date_width, date_pos.y + main_font->line_height, 0, 0, 0, true);

    qp_drawtext(display, time_pos.x, time_pos.y, main_font, time);
    qp_drawtext(display, date_pos.x, date_pos.y, main_font, date);
}

void scene_fixed_info(painter_device_t display) {
    if (!main_font) {
        return;
    }

    static uint32_t last_draw = 0;
    if (timer_elapsed32(last_draw) > QP_UPDATE_INTERVAL) {
        last_draw = timer_read32();

        char *encoder_text  = get_enc_mode();
        char  encoder_layer = get_highest_layer(layer_state) + 0x30;
        led_t led_state     = host_keyboard_led_state();

        // If nothing has changed then don't redraw anything !
        if (!force_update && encoder_mode == cached_state.encoder_mode && layer_state == cached_state.layer_state && led_state.caps_lock == cached_state.caps_lock && led_state.scroll_lock == cached_state.scroll_lock) {
            return;
        }

        const uint32_t offset_x = scene_get_usable_width() + 1 + 2;
        const uint32_t offset_y = 0;

        // Clear our section
        qp_rect(display, scene_get_usable_width() + 1, offset_y, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0, true);

        
        qp_line(display, scene_get_usable_width() + 1, 0, scene_get_usable_width() + 1, DISPLAY_HEIGHT, 255, 255, 255);

        char buffer[32] = {""};
        sprintf(buffer, "ENC %s", encoder_text);
        qp_drawtext(display, offset_x, offset_y, main_font, buffer);
        memset(buffer, '\0', sizeof(buffer));

        sprintf(buffer, "LYR %c", encoder_layer);
        qp_drawtext(display, offset_x, offset_y + main_font->line_height, main_font, buffer);
        memset(buffer, '\0', sizeof(buffer));

        uint32_t mod_key_offset_x = offset_x;
        if (led_state.caps_lock) {
            sprintf(buffer, " C ");
            qp_drawtext_recolor(display, mod_key_offset_x, offset_y + (main_font->line_height * 2), main_font, buffer, 0, 0, 0, 255, 255, 255);
            mod_key_offset_x += qp_textwidth(main_font, buffer) + 2;
            memset(buffer, '\0', sizeof(buffer));
        }

        if (led_state.scroll_lock) {
            sprintf(buffer, " S ");
            qp_drawtext_recolor(display, mod_key_offset_x, offset_y + (main_font->line_height * 2), main_font, buffer, 0, 0, 0, 255, 255, 255);
            mod_key_offset_x += qp_textwidth(main_font, buffer) + 2;
            memset(buffer, '\0', sizeof(buffer));
        }

        cached_state = (fixed_info_state){encoder_mode, layer_state, led_state.caps_lock, led_state.scroll_lock};

        qp_flush(display);
        force_update = false;
    }
}
