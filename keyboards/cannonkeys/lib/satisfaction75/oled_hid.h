void oled_hid_draw(void);
int oled_hid_get_hour(void);
int oled_hid_get_minute(void);
/**
* Sets the RTC with a given unix timestamp
*/
void oled_hid_set_time(time_t unix_time);
// Make it accessible
char* get_enc_mode(void);