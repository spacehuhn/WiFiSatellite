#ifndef RGBLED_H
#define RGBLED_H

#include <Adafruit_NeoPixel.h>

class RGBLED {
    public:
        RGBLED();
        ~RGBLED();

        void init(int pin, int brightness);

        void set(uint8_t r, uint8_t g, uint8_t b);

		void update();
    private:
        Adafruit_NeoPixel* strip { nullptr };
		unsigned long update_time{ 0 };
};

#endif // ifndef RGBLED_H