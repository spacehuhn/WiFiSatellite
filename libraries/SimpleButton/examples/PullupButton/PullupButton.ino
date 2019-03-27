#include <SimpleButton.h>

using namespace simplebutton;

/*
  One button is connected to gpio 12 (D6 on the NodeMCU) and GND 
*/

Button* b = NULL;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  // ButtonPullup will do "pinMode(12, INPUT_PULLUP)" isntead of "pinMode(12, INPUT)".
  // That way you can connect it between the pin and GND and don't need the additional resistor.
  b = new ButtonPullup(12);
  
  Serial.println("Started");
}

void loop() { 
  b->update();
  
  if(b->doubleClicked()) Serial.println("doubleclicked");
  if(b->clicked()) Serial.println("clicked");
  if(b->holded()) Serial.println("holded");
}
