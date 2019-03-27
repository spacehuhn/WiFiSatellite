#include <SimpleButton.h>
#include <Wire.h>

using namespace simplebutton;

/*
   This example is for the Lameboy by @davedarko
   https://hackaday.io/project/26823-lameboy-another-esp12-handheld
   https://www.tindie.com/products/davedarko/lameboy-esp8266-handheld
 */

Button* up    = NULL;
Button* down  = NULL;
Button* left  = NULL;
Button* right = NULL;
Button* a     = NULL;
Button* b     = NULL;
Button* sd    = NULL;

PCF8574* myPCF = NULL;

void setup() {
    Serial.begin(115200);
    Serial.println();

    Wire.begin();

    myPCF = new PCF8574(0x3C); // 0x3c = i2c address (use a i2c scanner sketch to find the right address)

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

    up    = new ButtonPullupGPIOExpander(myPCF, 7);
    down  = new ButtonPullupGPIOExpander(myPCF, 5);
    left  = new ButtonPullupGPIOExpander(myPCF, 6);
    right = new ButtonPullupGPIOExpander(myPCF, 4);
    a     = new ButtonPullupGPIOExpander(myPCF, 3);
    b     = new ButtonPullupGPIOExpander(myPCF, 2);
    sd    = new ButtonPullupGPIOExpander(myPCF, 1);

    Serial.println("Started");
}

void loop() {
    up->update();
    down->update();
    left->update();
    right->update();
    a->update();
    b->update();
    sd->update();

    if (up->doubleClicked()) Serial.println("up doubleclicked");
    if (up->clicked()) Serial.println("up clicked");
    if (up->holding()) Serial.println("up holding");

    if (down->doubleClicked()) Serial.println("down doubleclicked");
    if (down->clicked()) Serial.println("down clicked");
    if (down->holding()) Serial.println("down holding");

    if (left->doubleClicked()) Serial.println("left doubleclicked");
    if (left->clicked()) Serial.println("left clicked");
    if (left->holding()) Serial.println("left holding");

    if (right->doubleClicked()) Serial.println("right doubleclicked");
    if (right->clicked()) Serial.println("right clicked");
    if (right->holding()) Serial.println("right holding");

    if (a->doubleClicked()) Serial.println("a doubleclicked");
    if (a->clicked()) Serial.println("a clicked");
    if (a->holding()) Serial.println("a holding");

    if (b->doubleClicked()) Serial.println("b doubleclicked");
    if (b->clicked()) Serial.println("b clicked");
    if (b->holding()) Serial.println("b holding");

    if (sd->pushed()) Serial.println("sd card inserted");
    if (sd->released()) Serial.println("sd card removed");
}