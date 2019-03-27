#include <SimpleButton.h>

using namespace simplebutton;

/*
   A 4x4 Button matrix connected to an ESP8266 (Wemos D1 mini) on pin A0.
 */

#define MATRIX_SIZE 16

Button* matrix[MATRIX_SIZE];

// these are the analog values of each button, they probably are different on your matrix, so test and change the
// values!
int addresses[] = { 1023, 960, 875, 810, 695, 650, 610, 575, 515, 490, 470, 450, 410, 330, 275, 240 };
int tolerance   = 5;

void setup() {
    Serial.begin(115200);
    Serial.println();

    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = new ButtonAnalog(A0, addresses[i] - tolerance, addresses[i] + tolerance);
    }

    Serial.println("Started");
}

void loop() {
    // Serial.println(analogRead(A0)); // <- you can use this to read out the value of each button

    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i]->update();
        if (matrix[i]->clicked()) {
            Serial.println("clicked " + String(i + 1));
        }
    }
}