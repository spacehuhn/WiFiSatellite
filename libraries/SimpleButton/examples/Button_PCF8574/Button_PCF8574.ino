#include <SimpleButton.h>

using namespace simplebutton;

/*
   2 buttons are connected to a PCF8574 on a Wemos D1 mini (ESP8266)
   - PCF8574 is connected with SDA to GPIO 5 (D1) and with SCL to GPIO 4 (D2)
   - Button A is connected between the PCF pin 0 and GND
   - Button B is connected between the PCF pin 1 and 3.3V (VCC)
 */

GPIOExpander* myPCF   = NULL;
Button * buttonA = NULL;
Button * buttonB = NULL;

void setup() {
    // Wire.begin()
    Wire.begin(5, 4); // <- for esp8266 (5 = SDA pin, 4 = SCL pin)

    Serial.begin(115200);
    Serial.println();

    myPCF = new PCF8574(0x38); // 0x38 = i2c address (use a i2c scanner sketch to find the right address)

    // check connction
    do {
        Serial.print("Connecting to PCF8574...");
        myPCF->write(0);
        Serial.println(myPCF->getError());

        if (!myPCF->connected()) {
            Serial.println("Please check the wiring, the i2c address and restart the device!");
            delay(2000);
        }
    } while (!myPCF->connected());

    buttonA = new ButtonPullupGPIOExpander(myPCF, 0);
    buttonB = new ButtonGPIOExpander(myPCF, 1);

    Serial.println("Started");
}

void loop() {
    buttonA->update();
    buttonB->update();

    if (buttonA->doubleClicked()) Serial.println("Button A doubleclicked");
    if (buttonA->clicked()) Serial.println("Button A clicked");
    if (buttonA->holding()) Serial.println("Button A holding");

    if (buttonB->doubleClicked()) Serial.println("Button B doubleclicked");
    if (buttonB->clicked()) Serial.println("Button B clicked");
    if (buttonB->holding()) Serial.println("Button B holding");
}