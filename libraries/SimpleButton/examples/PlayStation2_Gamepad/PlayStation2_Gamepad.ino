#include <SimpleButton.h>

using namespace simplebutton;

/*
   // Arduino Nano
 #define DATA_PIN        13
 #define COMMAND_PIN     11
 #define ATTENTION_PIN   10
 #define CLOCK_PIN       12
 */

// Wemos D1 mini
#define DATA_PIN       5 // D1
#define COMMAND_PIN   12 // D6
#define ATTENTION_PIN 14 // D5
#define CLOCK_PIN     13 // D7

// enables to read analog values from buttons and activate the vibration motors
// is on "true" by default, but some older/special controllers might not support it
#define ENABLE_ANALOG true

PS2Gamepad* gamepad = NULL;

void setup() {
    Serial.begin(115200);
    Serial.println();

    // create gamepad
    gamepad = new PS2Gamepad();

    // try connecting to it
    while (!gamepad->connected()) {
        Serial.print("Connecting to PlayStation-2 Controller...");

        // setup
        gamepad->setup(CLOCK_PIN, COMMAND_PIN, ATTENTION_PIN, DATA_PIN /*, ENABLE_ANALOG*/);

        // print OK or error message
        Serial.println(gamepad->getError());

        // retry if connection failed
        if (!gamepad->connected()) {
            Serial.println("Retrying...");
            delay(1000);
        }
    }

    Serial.println("Started");
}

void loop() {
    gamepad->update();

    // print analog values

    /*
       uint8_t left_x = gamepad->analogLeft->left->getValue();
       uint8_t left_y = gamepad->analogLeft->up->getValue();
       uint8_t right_x = gamepad->analogRight->left->getValue();
       uint8_t right_y = gamepad->analogRight->up->getValue();
       Serial.printf("Left-X: %+3u, Left-Y: %+3u, Right-X: %+3u, Right-Y: %+3u\n", left_x, left_y, right_x, right_y);
     */

    // d-pad
    if (gamepad->up->clicked()) Serial.println("up clicked");
    if (gamepad->up->holding()) Serial.println("up holding, value: " + String(gamepad->up->getValue()));

    if (gamepad->down->clicked()) Serial.println("down clicked");
    if (gamepad->down->holding()) Serial.println("down holding, value: " + String(gamepad->down->getValue()));

    if (gamepad->left->clicked()) Serial.println("left clicked");
    if (gamepad->left->holding()) Serial.println("left holding, value: " + String(gamepad->left->getValue()));

    if (gamepad->right->clicked()) Serial.println("right clicked");
    if (gamepad->right->holding()) Serial.println("right holding, value: " + String(gamepad->right->getValue()));

    // L and R Buttons
    if (gamepad->l1->clicked()) Serial.println("L1 clicked");
    if (gamepad->l1->holding()) Serial.println("L1 holding, value: " + String(gamepad->l1->getValue()));

    if (gamepad->l2->clicked()) Serial.println("L2 clicked");
    if (gamepad->l2->holding()) Serial.println("L2 holding, value: " + String(gamepad->l2->getValue()));

    if (gamepad->r1->clicked()) Serial.println("R1 clicked");
    if (gamepad->r1->holding()) Serial.println("R1 holding, value: " + String(gamepad->r1->getValue()));

    if (gamepad->r2->clicked()) Serial.println("R2 clicked");
    if (gamepad->r2->holding()) Serial.println("R2 holding, value: " + String(gamepad->r2->getValue()));

    // triangle, circle, cross, square
    if (gamepad->square->clicked()) Serial.println("Square clicked");
    if (gamepad->square->holding()) Serial.println("Square holding, value: " + String(gamepad->square->getValue()));

    if (gamepad->triangle->clicked()) Serial.println("Triangle clicked");
    if (gamepad->triangle->holding()) Serial.println("Triangle holding, value: " +
                                                     String(gamepad->triangle->getValue()));

    if (gamepad->cross->clicked()) Serial.println("Cross clicked");
    if (gamepad->cross->holding()) Serial.println("Cross holding, value: " + String(gamepad->cross->getValue()));

    if (gamepad->circle->clicked()) Serial.println("Circle clicked");
    if (gamepad->circle->holding()) Serial.println("Circle holding, value: " + String(gamepad->circle->getValue()));

    // start & select
    if (gamepad->select->clicked()) Serial.println("Select clicked");
    if (gamepad->select->holding()) Serial.println("Select holding");

    if (gamepad->start->clicked()) Serial.println("Start clicked");
    if (gamepad->start->holding()) Serial.println("Start holding");

    // analog sticks
    if (gamepad->analogLeft->button->clicked()) Serial.println("Stick-Left clicked");
    if (gamepad->analogLeft->button->holding()) Serial.println("Stick-Left holding");

    if (gamepad->analogLeft->up->holding()) Serial.println("Stick-Left up, value: " +
                                                           String(gamepad->analogLeft->up->getValue()));
    if (gamepad->analogLeft->down->holding()) Serial.println("Stick-Left down, value: " +
                                                             String(gamepad->analogLeft->down->getValue()));
    if (gamepad->analogLeft->left->holding()) Serial.println("Stick-Left left, value: " +
                                                             String(gamepad->analogLeft->left->getValue()));
    if (gamepad->analogLeft->right->holding()) Serial.println("Stick-Left right, value: " +
                                                              String(gamepad->analogLeft->right->getValue()));


    if (gamepad->analogRight->button->clicked()) Serial.println("Stick-Right clicked");
    if (gamepad->analogRight->button->holding()) Serial.println("Stick-Right holding");

    if (gamepad->analogRight->up->holding()) Serial.println("Stick-Right up, value: " +
                                                            String(gamepad->analogRight->up->getValue()));
    if (gamepad->analogRight->down->holding()) Serial.println("Stick-Right down, value: " +
                                                              String(gamepad->analogRight->down->getValue()));
    if (gamepad->analogRight->left->holding()) Serial.println("Stick-Right left, value: " +
                                                              String(gamepad->analogRight->left->getValue()));
    if (gamepad->analogRight->right->holding()) Serial.println("Stick-Right right, value: " +
                                                               String(gamepad->analogRight->right->getValue()));
}
