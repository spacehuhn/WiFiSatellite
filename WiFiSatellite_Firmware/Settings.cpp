#include "Settings.h"

namespace simplesetting {
    Settings::Settings() {
        // this->packet_section = new Section("Packets");

        // this->sd            = new SettingFlag("SD", true, "Save recorded Wi-Fi traffic to SD card");
        // this->radiotap      = new SettingFlag("Radiotap", true, "Adds radiotapheader to PCAP (for metadata like RSSI)");
        // this->dropMalformed = new SettingFlag("DropMalformed", true, "Drop malformed Wi-Fi packets");
        // this->dropPayload   = new SettingFlag("DropPayload", false, "Save only the packet headers");

        // packet_section.add(&sd);

        /*
           packet_section.add(&radiotap);
           packet_section.add(&dropMalformed);
           packet_section.add(&dropPayload);
         */
        // this->add(&packet_section);
        this->add(&sd);
        this->add(&channel);
        this->add(&channel_mode);
		this->add(&flip_screen);
		this->add(&id);
    }

    Settings::~Settings() {}

    void Settings::init() {
        if (!SPIFFS.begin(true)) {
            // ERROR
        }
    }

    void Settings::load() {
        File file = SPIFFS.open("/settings.ini");

        if (file && !file.isDirectory()) {
            char* content = new char[file.size()]; // allocate memory

            file.readBytes(content, file.size());  // read data

            std::string data { content, file.size() }; // write it to a string

            parse(data);                           // parse data

            delete[] content;                      // release memory

            file.close();                          // close file
        }
    }

    void Settings::save() {
        File file = SPIFFS.open("/settings.ini", FILE_WRITE);

        if (file) {
            // get data
            std::string data { to_ini() };
            uint8_t   * buffer = (uint8_t*)data.c_str();

            file.write(buffer, data.size()); // write data

            file.close();                    // close file
        }
    }
}