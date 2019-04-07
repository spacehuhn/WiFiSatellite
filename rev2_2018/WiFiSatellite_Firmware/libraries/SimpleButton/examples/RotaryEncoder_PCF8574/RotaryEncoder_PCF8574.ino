#include <SimpleButton.h>

using namespace simplebutton;

/*
   1 Rotary encoder is connected to a PCF8574 on a Wemos D1 mini (ESP8266)
   - PCF8574 is connected with SDA to GPIO 5 (D1) and with SCL to GPIO 4 (D2)
   - Rotary encoder CLK is connected to the PCF pin 0 - that's channel A
   - Rotary encoder DT is connected to PCF pin 1 - that's channel B
   - Rotary encoder SW (switch) is unconnected and therefor set to 255
   (the labels on your rotary encoder might be different!)
 */

PCF8574* myPCF           = NULL;
RotaryEncoder* myEncoder = NULL;
int32_t previousPosition = 0;

void setup() {
    // Wire.begin()
    Wire.begin(5, 4); // <- for esp8266 (5 = SDA pin, 4 = SCL pin)

    Serial.begin(115200);
    Serial.println();

    myPCF = new PCF8574(0x20); // 0x20 = i2c address (use a i2c scanner sketch to find the right address)

    // check connection
    do {
        Serial.print("Connecting to PCF8574...");
        myPCF->write(0);
        Serial.println(myPCF->getError());

        if (!myPCF->connected()) {
            Serial.println("Please check the wiring, the i2c address and restart the device!");
            delay(2000);
        }
    } while (!myPCF->connected());

    myEncoder = new RotaryEncoder(myPCF, 0, 1, 255); // GPIOExpander, Channel-A, Channel-B, push button (255 = not used)

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