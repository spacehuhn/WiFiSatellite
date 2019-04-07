#include <SimpleButton.h>

using namespace simplebutton;

/*
   Analog stick connected to an Arduino micro
   - x axis is connected to pin A0
   - y axis is connected to pin A1
   - Switch is connected to pin 5
 */

AnalogStick* analogStick = NULL;

void setup() {
    Serial.begin(115200);
    Serial.println();

    analogStick = new AnalogStick(A0, A1, 5);

    /* === check your analogStick if it returns values from 0 - 255 or from 0 - 1024! === */
    // analogStick->setLogic(256, 25); // logic => 256: values range from 0 to 255, tolarance => 25%
    analogStick->setLogic(1024, 25); // logic => 1024: values range from 0 to 1023, tolarance => 25%

    Serial.println("Started!");
}

void loop() {
    // In case you want to read out the values manually for testing:
    // Serial.print(analogRead(A0));
    // Serial.print(' ');
    // Serial.println(analogRead(A1));

    analogStick->update();

    // or read them out using the object
    // uint8_t x = analogStick->left->getValue();
    // uint8_t y = analogStick->up->getValue();

    if (analogStick->button->doubleClicked()) Serial.println("doubleClicked");
    if (analogStick->button->clicked()) Serial.println("clicked");
    if (analogStick->button->holding()) Serial.println("holding");

    if (analogStick->left->clicked()) Serial.println("left clicked");
    if (analogStick->right->clicked()) Serial.println("right clicked");
    if (analogStick->up->clicked()) Serial.println("up clicked");
    if (analogStick->down->clicked()) Serial.println("down clicked");

    if (analogStick->left->holding()) Serial.println("left holding");
    if (analogStick->right->holding()) Serial.println("right holding");
    if (analogStick->up->holding()) Serial.println("up holding");
    if (analogStick->down->holding()) Serial.println("down holding");
}