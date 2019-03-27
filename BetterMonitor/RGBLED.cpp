#include "RGBLED.h"

RGBLED::RGBLED() {}

RGBLED::~RGBLED() {
    if (strip) delete strip;
}

void RGBLED::init(int pin, int brightness) {
    strip = new Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ800);
    strip->setBrightness(brightness);
    strip->begin();
    strip->show();
    set(0, 0, 0);
}

void RGBLED::set(uint8_t r, uint8_t g, uint8_t b) {
    if (strip) {
		/*
        Serial.print(r);
        Serial.print(' ');
        Serial.print(g);
        Serial.print(' ');
        Serial.print(b);
        Serial.println();
		*/
        strip->setPixelColor(0, r, g, b);
        strip->show();
        update_time = millis();
        delay(1);
    }
}

void RGBLED::update() {
    if (millis() - update_time >= 5000) {
        set(1, 0, 0);
    }
}