#include "gpio.h"
#include "Arduino.h" // pinMode(), digitalRead()

void init_buttons(int up, int down, int select) {
    pinMode(up, INPUT_PULLUP);
    pinMode(down, INPUT_PULLUP);
    pinMode(select, INPUT_PULLUP);
}

bool pressed(int pin) {
    return digitalRead(pin) == LOW;
}

bool pressed_any(int up, int down, int select) {
    return pressed(up) || pressed(down) || pressed(select);
}