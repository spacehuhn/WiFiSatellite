#ifndef SDPacketWriter_h
#define SDPacketWriter_h

#include <cstddef>        // uint32_t
#include <string>         // std::string
#include <sstream>        // std::stringstream

#include <FS.h>           // ESP32 file system library
#include <SD_MMC.h>       // SD/MMC card

#include "PacketWriter.h" // Parent class

using namespace std;

#define MAX_PCAP_FILE_SIZE 4000000000
#define BUFFER_SIZE 64000

class SDPacketWriter: public PacketWriter {
    public:
        SDPacketWriter();
        ~SDPacketWriter();

        virtual bool begin();
        virtual bool end();
        virtual bool pause(bool error = false);
        virtual bool resume();

        virtual uint64_t used_bytes() const;
        virtual uint64_t free_bytes() const;
        virtual uint64_t total_bytes() const;
        virtual uint64_t get_file_size() const;
        virtual int64_t get_file_time() const;
        virtual double get_speed() const;
        virtual bool header_enabled() const;
        virtual bool exists(const char* path) const;

        virtual bool write(uint8_t* data, uint64_t size);
        virtual bool flush();

    protected:
        File file;

        string   file_path { "/capture_1_0.pcap" };
        uint64_t file_size = 0;
        int64_t  file_time { 0.0 };

        uint32_t id_major { 1 };
        uint32_t id_minor { 0 };

        uint8_t* buffer { nullptr };
        uint64_t buffer_size { 0 };

		virtual string create_file_path() const;
		virtual void update_file_path();

};

#endif /* ifndef PacketWriter_h */