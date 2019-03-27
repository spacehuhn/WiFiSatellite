#include <SimpleButton.h>
#include <EEPROM.h>

using namespace simplebutton;

/*
   One I2C rotary encoder is connected to an Wemos D1 mini
   - SDA is connected to D1 (GPIO 5)
   - SCL is connected to D2 (GPIO 4)
   - Interrupt pin is connected to D6 (GPIO 12)
 */

RotaryEncoderI2C* myEncoder = NULL;

void setup() {
    // EEPROM.begin();
    EEPROM.begin(4096); // <- for ESP8266

    Serial.begin(115200);
    Serial.println();

    // Wire.begin();
    Wire.begin(D1, D2); // <- for ESP8266

    // setup the rotary encoder
    myEncoder = new RotaryEncoderI2C(0x30);

    // set the interrupt pin at D6
    myEncoder->enableInterrupt(12, true);

    myEncoder->setMin(-128);      // set the minimum counter value
    myEncoder->setMax(127);       // set the maximum counter value
    myEncoder->enableLed(true);   // enable LED
    // myEncoder->setEncoding(2);    // enable x2 encoding
    // myEncoder->enableLoop(true);
    myEncoder->setInverted(true); // invert direction of counting

    // read out the last counter value from the EEPROM and set it
    myEncoder->setPos((int8_t)EEPROM.read(0));

    myEncoder->begin();

    // check connction
    do {
        Serial.print("Connecting to I2C Encoder...");
        Serial.println(myEncoder->getError());

        if (!myEncoder->connected()) {
            Serial.println("Please check the wiring, the i2c address and restart the device!");
            delay(2000);
        }
    } while (!myEncoder->connected());

    Serial.println("Started!");
    Serial.print("Value: ");
    Serial.println(myEncoder->getPos());
}

void loop() {
    // check if interrupt pin has changed
    if (myEncoder->update()) {
        int val = myEncoder->getPos(); // get the value

        // save value in the eeprom
        EEPROM.write(0, (int8_t)val);
        EEPROM.commit(); // <- for ESP8266

        // print infos
        Serial.print(val);

        if (myEncoder->decremented()) {
            Serial.print(" down");
            myEncoder->setLed(255, 0);
        } else if (myEncoder->incremented()) {
            Serial.print(" up");
            myEncoder->setLed(0, 255);
        }

        if (myEncoder->clicked()) {
            Serial.print(" clicked");
            myEncoder->setLed(0, 0);
        }

        Serial.println();
    }
}