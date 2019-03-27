#include "Log.h"

void Log::init(int baudrate) {
    Serial.begin(baudrate);
    delay(500);
}

void Log::deinit() {
    Serial.end();
}

void Log::print(uint8_t* buffer, uint32_t size) {
    uint32_t i = 0;

    while (i < size) {
        print < const char* > ("0x");
        if (buffer[i] < 0x10) print < uint8_t > (0);
        print < uint8_t > (buffer[i]);
        print < const char* > (", ");
        i++;
    }
}

void Log::println(uint8_t* buffer, uint32_t size) {
    print(buffer, size);
    println();
}

void Log::println() {
    Serial.println();
}

void Log::flush() {
    Serial.flush();
}