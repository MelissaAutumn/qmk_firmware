void oled_hid_draw(void);
int oled_hid_get_hour(void);
int oled_hid_get_minute(void);
//void oled_hid_set_time(int year, int month, int day, int hour, int minute, int second);
void oled_hid_set_time(time_t unix_time);