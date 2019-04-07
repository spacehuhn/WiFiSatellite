#include <SH1106Wire.h>
#include <DstikeUpdater.h>

// #define UPDATER_PATH "/update" // folder where bin files are located
// #define LOADING_DELAY 3500 // time to press button at startup

/****** DSTIKE dduino-32 SD ******/
#define BUTTON_UP 32
#define BUTTON_DOWN 25
#define BUTTON_SELECT 33
#define OLED_SDA 26
#define OLED_SCK 27

SH1106Wire display(0x3c, OLED_SDA, OLED_SCK);

void setup() {
    DstikeUpdater::run(display, BUTTON_UP, BUTTON_DOWN, BUTTON_SELECT /*,UPDATER_PATH, LOADING_DELAY*/);
}

void loop() {
    display.clear();
    display.drawString(0, 0, "Test Sketch");
    display.display();
    delay(1000);
}