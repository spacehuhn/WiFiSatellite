#ifndef PacketWriter_h
#define PacketWriter_h

#include <iostream>   // cout
#include <functional> // lambda

#include <esp_log.h>  // ESP_LOG

#include "Packet.h"   // Packet

class PacketWriter {
    public:
        virtual void set_on_error(std::function<void()> on_error);

        virtual void set_time(uint32_t timestamp, int32_t timezone);

        virtual bool begin();
        virtual bool end();
        virtual bool pause(bool error = false);
        virtual bool resume();

        virtual bool is_paused() const;
        virtual bool is_errored() const;
        virtual bool header_enabled() const;

        virtual bool write_pkt(const Packet& p);

        virtual bool write(uint8_t* data, uint64_t size) = 0;

    protected:
        bool paused  = false;
        bool errored = false;

        std::function<void()> on_error = nullptr;

        uint32_t timestamp { 0 };
        int32_t  timezone { 0 };
};

#endif /* ifndef PacketWriter_h */