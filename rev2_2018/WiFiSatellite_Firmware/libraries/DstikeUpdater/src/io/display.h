#ifndef display_h
#define display_h

#include <OLEDDisplay.h> // OLED

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

bool init_display(OLEDDisplay& display);

void display_show(OLEDDisplay& display);
void display_clear(OLEDDisplay& display);

void draw_headline(OLEDDisplay& display);

void draw_string(OLEDDisplay& display, int row, const String& str);
void draw_string_f(OLEDDisplay& display, int row, const __FlashStringHelper* str);

void draw_bar(OLEDDisplay& display, int len);
void draw_invalid_path(OLEDDisplay& display);
void draw_no_sd(OLEDDisplay& display);
void draw_update(OLEDDisplay& display);
void draw_canceled(OLEDDisplay& display);
void draw_flashing(OLEDDisplay& display);
void draw_rebooting(OLEDDisplay& display);
void draw_loading_bar(OLEDDisplay& display, unsigned long loading_time);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef sd_h */