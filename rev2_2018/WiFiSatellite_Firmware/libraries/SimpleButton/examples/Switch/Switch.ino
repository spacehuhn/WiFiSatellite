#include <SimpleButton.h>

using namespace simplebutton;

/*
   A on/off switch is connected to gpio 12
   - the switch connects gpio 12 to either VCC (3.3V) or GND
 */

Switch* s = NULL;

void setup() {
    Serial.begin(115200);
    Serial.println();

    s = new Switch(12);

    Serial.println("Started");
}

void loop() {
    s->update();

    if (s->clicked()) Serial.println("clicked");
}