# SimpleButton
A simple Arduino library to make interfacing and reacting on button events easier.

**Overview:**  
- [Features](#features)
- [To-Do](#to-do)
- [Installation](#installation)
- [Usage](#usage)
  - [Include the library](#include-the-library)
  - [Create a button](#create-a-button)
    - [Push Button](#push-button-setup)
    - [On/Off Switch](#onoff-switch-setup)
    - [Rotary Encoder](#rotary-encoder-setup)
    - [Analog Button](#analog-button-setup)
    - [Analog Stick](#analog-stick-setup)
    - [PlayStation2 Gamepad](#playstation2-gamepad-setup)
    - [GPIO Expander](#gpio-expander-setup)
  - [Read out status](#read-out-status)
    - [Button](button-status)
    - [Rotary Encoder](#rotary-encoder-status)
    - [Analog Stick](#analog-stick-status)
    - [PlayStation2 Gamepad](#playstation2-gamepad-status)
  - [Use events](#use-events)
- [License](#license)

## Features

This library supports:  
- Push buttons (with or without pullup or inverted logic)
- On/Off Switches
- Rotary Encoders
- The [Lameboy](https://hackaday.io/project/26823-lameboy-another-esp12-handheld) :D
- Any Analog input (i.e. ButtonMatrix)
- Analog-Stick
- PlayStation2 Gamepad
- [I2C Encoder](https://www.tindie.com/products/Saimon/i2c-encoder-connect-rotary-encoders-on-i2c-bus/)

You can not only read out the current state of the button, but also if it's:    
- pushed
- released
- clicked
- double clicked
- holding

It also works with buttons that are connected to a PCF8574, PCF8575 or MCP23017 GPIO expander!  

## Installation

1) [Download](https://github.com/spacehuhn/SimpleButton/archive/master.zip) the source code from GitHub.  
2) Unzip and rename the Folder name to "SimpleButton".  
3) Paste it in your Library folder (Usually located somewhere at documents/Arduino/libraries).  
4) Restart the Arduino IDE.  

## Usage

Also have a look at the [examples](https://github.com/spacehuhn/SimpleButton/tree/master/examples).  

### Include the library
```c++
#include <SimpleButton.h>

using namespace simplebutton;
```

### Create a button

#### Push Button Setup

**Normal logic:**  
The usual way of connecting a push button as described [here](https://www.arduino.cc/en/Tutorial/Button).  
```c++
// creates button on pin 12
Button* b = new Button(12);
```

**Inverted logic:**  
If you got any special kind of button that is HIGH on idle and goes to LOW when it's pressed, you can invert the logic.  
```c++
// creates inverted button on pin 12
Button* b = new Button(12, true);
```

**Pullup button:**  
This will use the internal resistor and you won't need to add an external one. Also described [here](https://www.arduino.cc/en/Tutorial/InputPullupSerial).  
```c++
// creates pullup button on pin 12
Button* b = new ButtonPullup(12);
```

#### On/Off Switch Setup
You can use a switch as a button. Whenever you switch it, it will count it as a button click.  
```c++
// creates switch on pin 12
Button* b = new Switch(12);
```

#### Rotary Encoder Setup
```c++
// creates a rotary encoder connected to pin 5 and pin 4 and switch connected pin 3 (set switch to 255 to disable it)
RotaryEncoder* myEncoder = new RotaryEncoder(5, 4, 3);

// rotary encoder connected to the PCF on pin 2 and pin 3
RotaryEncoder* myEncoder = new RotaryEncoder(exp, 2, 3, 255);

// I2C encoder at address 0x30
RotaryEncoderI2C* myEncoder = new RotaryEncoderI2C(0x30);

// in case you have a rotary encoder that does 2 steps with each turn:
// (default is 1 step per turn)
myEncoder->setEncoding(2);

// set a start position for the counter
myEncoder->setPos(10);

// set a minimum value threshold
myEncoder->setMin(-20);

// set a maximum value threshold
myEncoder->setMax(20);

// invert the directions
myEncoder->setInverted(true);

// enable looping (when the counter goes below the minimum it will be set to the maximum and vice versa)
myEncoder->enableLoop(true);

// ===== for the I2C encoder only =====

// enables a interrupt pin at gpio 12 with pullup enabled (true)
myEncoder->enableInterrupt(12, true);

// enables the dual-color led
myEncoder->enableLed(true);

// set LED-A to 255 (on) and LED-B to 0 (off)
myEncoder->setLed(255, 0);

//  !!!!! IMPORTANT !!!!!
// to enable all our config changes
myEncoder->begin();
```

#### Analog Button Setup
```c++
// create an analog button on pin A0 that is pushed when it reads a value between 0 and 20
ButtonAnalog* b = ButtonAnalog(A0, 0, 20);
```

#### Analog Stick Setup
```c++
// creates an analog stick that has X connected to A0, Y connected to A1 and the switch connected to pin 5
// (set switch to 255 to disable it)
AnalogStick* analogStick = new AnalogStick(A0, A1, 5);

// set the logic to read values from 0 to 1024 with 25% tolerance
analogStick->setLogic(1024, 25);

// set the logic to read values from 0 to 256 with 25% tolerance
analogStick->setLogic(256, 25);
```

#### PlayStation2 Gamepad Setup

To learn more about the wiring, protocol and usage of the PlayStation2 controller, please have a look [here](http://store.curiousinventor.com/guides/PS2/).  
It doesn't really matter (on most Arduino's) what pins you use to connect the controller.  

```c++
// create the gamepad
PS2Gamepad* gamepad = new PS2Gamepad();

// connect to it
gamepad->setup(CLOCK_PIN, COMMAND_PIN, ATTENTION_PIN, DATA_PIN);

// check for errros
bool isConnected = gamepad->connected();
String errorMessage = gamepad->getError();
```

#### GPIO Expander Setup
```c++
// start i2c
Wire.begin();

// 0x20 = i2c address (use a i2c scanner sketch to find the right address)
// create a PCF8574
GPIOExpander* exp = new PCF8574(0x20);
// create a PCF8575
GPIOExpander* exp = new PCF8575(0x20);
// create a MCP23017
GPIOExpander* exp = new MCP23017(0x20);

// creates a push button connected to the PCF on pin 0
Button* b = new ButtonGPIOExpander(exp, 0);

// creates a pullup button connected to the PCF on pin 1
Button* b = new ButtonPullupGPIOExpander(exp, 1);

// rotary encoder connected to the PCF on pin 2 and pin 3
RotaryEncoder* myEncoder = new RotaryEncoder(exp, 2, 3, 255);

// check for errors
bool isConnected = exp->connected();
String errorMessage = exp->getError();
```

### Read out status
`b` is a pointer to a created button (see above).  

#### Button status

Important status methods a `Button` object has:  
```c++
bool pushed();
bool released();
bool clicked();
bool clicked(uint32_t minPushTime);
bool clicked(uint32_t minPushTime, uint32_t minReleaseTime);
bool doubleClicked();
bool doubleClicked(uint32_t minPushTime);
bool doubleClicked(uint32_t minPushTime, uint32_t timeSpan);
bool doubleClicked(uint32_t minPushTime, uint32_t minReleaseTime, uint32_t timeSpan);
bool holding();
bool holding(uint32_t interval);
```

Example usage:  
```c++
// first update the button
b->update();

// react on double click
if(b->doubleClicked()){ ... }

// with minimum push time in ms (default = 40)
if(b->doubleClicked(uint32_t minPushTime)) { ... }

// with minimum push time in a given time span in ms (default = 650)
if(b->doubleClicked(uint32_t minPushTime, uint32_t timeSpan)) { ... }

// react on a click		
if(b->clicked()){ ...}

// with a minimum push time in ms (default = 40)
if(b->clicked(uint32_t minPushTime)) { ... }

// if button is beeing hold
if(b->holding()){ ... }

// with custom time interval in ms (default = 250)
if(b->holding(uint32_t interval)){ ... };

// when the button is beeing pushed
if(b->pushed()) { ... }

// when the button is released
if(b->released()) { ... }

// you can also read the button state out directly
bool currentButtonState = b->getState();

// read out the analog value
uint8_t value = analogButton->getValue();
```

#### Rotary Encoder status

These are the `Button`s a `RotaryEncoder` object has:  
```c++
Button* button;
Button* clockwise;
Button* anticlockwise;
```

Example usage:  
```c++
// update the encoder
myEncoder->update();

// read out the position counter
int position = myEncoder->getPos();

// if rotary encoder switch was pushed
bool clicked = myEncoder->clicked();
// here using the Button object
bool clicked = myEncoder->button->clicked();

// read out the directions
bool incremented = myEncoder->incremented();
bool decremented = myEncoder->decremented();

// read out the directions using the Button objects
bool incremented = myEncoder->clockwise->clicked();
bool decremented = myEncoder->anticlockwise->clicked();

// read out if the counter hit a threshold
bool hitMinValue = myEncoder->minVal();
bool hitMaxValue = myEncoder->maxVal();

// ==== for I2C encoder only =====

// read out if interrupt pin changed (will always be true if disabled)
bool interrupt = myEncoder->interrupt();

// if the interrupt pin is enabled, you can also use the update function
// to see if something changed
bool changed = myEncoder->update();
```

#### Analog Stick status

These are the `Button`s a `AnalogStick` object has:  
```c++
Button* button;
ButtonAnalog* up;
ButtonAnalog* down;
ButtonAnalog* left;
ButtonAnalog* right;
```

Example usage:  
```c++
// read out the values
uint8_t x = analogStick->left->getValue();
uint8_t y = analogStick->up->getValue();

// access the switch button (if you added one)
if (analogStick->button->clicked()) Serial.println("clicked");

// react on any direction
if (analogStick->left->holding()) Serial.println("left holding");
if (analogStick->right->holding()) Serial.println("right holding");
if (analogStick->up->holding()) Serial.println("up holding");
if (analogStick->down->holding()) Serial.println("down holding");
```

#### PlayStation2 Gamepad status

These are the `Button`s a `PS2Gamepad` object has:  
```c++
ButtonAnalog* up;
ButtonAnalog* down;
ButtonAnalog* left;
ButtonAnalog* right;

ButtonAnalog* l1;
ButtonAnalog* l2;
ButtonAnalog* r1;
ButtonAnalog* r2;

ButtonAnalog* square;
ButtonAnalog* triangle;
ButtonAnalog* x;
ButtonAnalog* circle;

Button* select;
Button* start;

AnalogStick* analogLeft;
AnalogStick* analogRight;
```

Example usage:  
```c++
// getting the analog-stick values
uint8_t left_x = gamepad->analogLeft->left->getValue();
uint8_t left_y = gamepad->analogLeft->up->getValue();

uint8_t right_x = gamepad->analogRight->left->getValue();
uint8_t right_y = gamepad->analogRight->up->getValue();

// d-pad
if (gamepad->up->clicked()) Serial.println("up clicked");
if (gamepad->down->holding()) Serial.println("down clicked");
if (gamepad->left->clicked()) Serial.println("left clicked");
if (gamepad->right->clicked()) Serial.println("right clicked");

// L and R Buttons
if (gamepad->l1->clicked()) Serial.println("L1 clicked");
if (gamepad->l2->clicked()) Serial.println("L2 clicked");
if (gamepad->r1->clicked()) Serial.println("R1 clicked");
if (gamepad->r2->clicked()) Serial.println("R2 clicked");

// start & select
if (gamepad->select->clicked()) Serial.println("Select clicked");
if (gamepad->start->clicked()) Serial.println("Start clicked");

// triangle, circle, cross, square
if (gamepad->square->clicked()) Serial.println("Square clicked");
if (gamepad->triangle->clicked()) Serial.println("Triangle clicked");
if (gamepad->cross->clicked()) Serial.println("Cross clicked");
if (gamepad->circle->clicked()) Serial.println("Circle clicked");

// left analog stick
if (gamepad->analogLeft->button->clicked()) Serial.println("Stick-Left clicked");

if (gamepad->analogLeft->up->holding()) Serial.println("Stick-Left up");
if (gamepad->analogLeft->down->holding()) Serial.println("Stick-Left down");
if (gamepad->analogLeft->left->holding()) Serial.println("Stick-Left left");
if (gamepad->analogLeft->right->holding()) Serial.println("Stick-Left right");

// right analog stick
if (gamepad->analogRight->button->clicked()) Serial.println("Stick-Right clicked");

if (gamepad->analogRight->up->holding()) Serial.println("Stick-Right up");
if (gamepad->analogRight->down->holding()) Serial.println("Stick-Right down");
if (gamepad->analogRight->left->holding()) Serial.println("Stick-Right left");
if (gamepad->analogRight->right->holding()) Serial.println("Stick-Right right");
```

### Use events
Each button can have multiple events, you can add them with following methods:  
```c++
void setOnPushed(void (* fnct)());

void setOnReleased(void (* fnct)());

void setOnClicked(void (* fnct)());
void setOnClicked(void (* fnct)(), uint32_t minPushTime);
void setOnClicked(void (* fnct)(), uint32_t minPushTime, uint32_t minReleaseTime);

void setOnDoubleClicked(void (* fnct)());
void setOnDoubleClicked(void (* fnct)(), uint32_t minPushTime);
void setOnDoubleClicked(void (* fnct)(), uint32_t minPushTime, uint32_t timeSpan);
void setOnDoubleClicked(void (* fnct)(), uint32_t minPushTime, uint32_t minReleaseTime, uint32_t timeSpan);

void setOnHolding(void (* fnct)());
void setOnHolding(void (* fnct)(), uint32_t interval);
```

Here's an example:  
```c++
button->setOnClicked([](){
  Serial.println("Button clicked!");
});

rotaryEncoder->clockwise->setOnClicked([](){
  Serial.println("Moved clockwise");
});

analogStick->left->setOnPushed([](){
  Serial.println("Analog-left");
});

gamepad->up->setOnDoubleClicked([](){
  Serial.println("Double clicked UP button");
});
```

## License

This software is licensed under the MIT License. See the [license file](LICENSE) for details.  
