#ifndef Logger_h
#define Logger_h

#include "Arduino.h"

class Log {
    public:
        static void init(int baudrate = 115200);
        static void deinit();

        template < typename T >
        static void print(T obj) {
            Serial.print(String(obj));
        }

        template < typename T >
        static void println(T obj) {
            Serial.println(String(obj));
        }

        static void print(uint8_t* buffer, uint32_t size);
        static void println(uint8_t* buffer, uint32_t size);
        static void println();

        static void flush();
};

#endif /* ifndef Logger_h */