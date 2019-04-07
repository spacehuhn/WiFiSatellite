#include <SimpleButton.h>

using namespace simplebutton;

/*
   1 Rotary encoder is connected to a Wemos D1 mini (ESP8266)
   - Rotary encoder "Key" (that's the button) is connected to gpio 12 (D6)
   - Rotary encoder "S1" is connected to gpio 5 (D1) - that's channel A
   - Rotary encoder "S4" is connected to gpio 4 (D2) - that's channel B
   (the labels on your rotary encoder might be different!)
 */

RotaryEncoder* myEncoder = NULL;
int32_t previousPosition = 0;

void setup() {
    Serial.begin(115200);
    Serial.println();

    myEncoder = new RotaryEncoder(5, 4, 12); // channel-A, channel-B, push button (255 = not used)
    // myEncoder->setEncoding(2); // <- if it used x2 encoding (x1 is default)
    myEncoder->setMin(-128);
    myEncoder->setMax(127);
    // myEncoder->setInverted(true);
    // myEncoder->enableLoop(true);

    Serial.println("Started");
}

void loop() {
    myEncoder->update();

    int32_t currentPosition = myEncoder->getPos();

    if (currentPosition != previousPosition) {
        previousPosition = currentPosition;
        Serial.print(currentPosition);
        if (myEncoder->incremented()) Serial.println(" up");
        if (myEncoder->decremented()) Serial.println(" down");
    }

    if (myEncoder->clicked()) {
        Serial.println("clicked");
    }
}