#ifndef Settings_h
#define Settings_h

#include <SimpleSetting.h> // settings library
#include <SPIFFS.h>
#include <FS.h>

namespace simplesetting {
    class Settings: public SimpleSetting {
        public:
            Settings();
            ~Settings();

            // Section packet_section { "Packets" };

            SettingFlag sd { "SD", true, "Save recorded Wi-Fi traffic to SD card" };
			SettingValue<int> channel{ "Ch", 1, 1, 1, 14, "Main 2.4GHz channel (1-14)" };
            SettingValue<int> channel_mode { "ChMode", 0, 1, -1, 1, "-1 Below(HT40-), 0 none(HT20), 1 Above(HT40+)" };
            SettingFlag flip_screen { "FlipScreen", true, "Flip Screen Vertically" };
			SettingValue<int> id{ "id", 0, 1, 0, 999, "ID for UART communication" };

            /*
               SettingFlag radiotap { "Radiotap", true, "Adds radiotapheader to PCAP (for metadata like RSSI)" };
               SettingFlag dropMalformed { "DropMalformed", true, "Drop malformed Wi-Fi packets" };
               SettingFlag dropPayload { "DropPayload", false, "Save only the packet headers" };
             */

            void init();
            void load();
            void save();
    };
}

#endif /* ifndef Settings_h */