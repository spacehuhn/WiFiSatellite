#include "display.h"

// [ ===== Init ===== ] //
bool init_display(OLEDDisplay& display) {
    display.init();
    display.flipScreenVertically();
    display.setFont(ArialMT_Plain_10);

    display_clear(display);
    display_show(display);

    return true;
}

// [ ===== Show ===== ] //
void display_show(OLEDDisplay& display) {
    display.display();
}

// [ ===== Clear ===== ] //
void display_clear(OLEDDisplay& display) {
    display.clear();
}

// [ ===== Headline ===== ] //
void draw_headline(OLEDDisplay& display) {
    display.drawString(0, 0, F("[ ==== SD UPDATE ==== ] "));
    display.drawLine(0, 13, 128, 13);
    display_show(display);
}

// [ ===== String ===== ] //
void draw_string(OLEDDisplay& display, int row, const String& str) {
    display.drawString(0, 14 + (row*12), str);
}

// [ ===== String_F ===== ] //
void draw_string_f(OLEDDisplay& display, int row, const __FlashStringHelper* str) {
    display.drawString(0, 14 + (row*12), str);
}

// [ ===== BAR ===== ] //
void draw_bar(OLEDDisplay& display, int len) {
    display.drawLine(0, 62, len, 62);
    display.drawLine(0, 63, len, 63);
}

// [ ===== N0 SD ===== ] //
void draw_no_sd(OLEDDisplay& display) {
    draw_string_f(display, 0, F("No SD detected :("));
    draw_string_f(display, 1, F("Loading default..."));
}

// [ ===== Update ===== ] //
void draw_update(OLEDDisplay& display) {
    draw_string_f(display, 0, F("Click to update"));
}

// [ ===== Canceled ===== ] //
void draw_canceled(OLEDDisplay& display) {
    draw_string_f(display, 0, F("Canceled Update"));
    draw_string_f(display, 1, F("Resuming programm..."));
}

// [ ===== Flashing  ===== ] //
void draw_flashing(OLEDDisplay& display) {
    draw_string_f(display, 0, F("Flashing..."));
}

// [ ===== Rebooting ===== ] //
void draw_rebooting(OLEDDisplay& display) {
    draw_string_f(display, 2, F("Rebooting..."));
}

// [ ===== Loading Bar ===== ] //
void draw_loading_bar(OLEDDisplay& display, unsigned long loading_time) {
    unsigned long start_time = millis();
    unsigned long end_time   = start_time + loading_time;

    while (millis() < end_time) {
        draw_bar(display, (millis() - start_time) / (loading_time/128));
        display.display();
        delay(50);
    }
}