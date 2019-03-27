#ifndef DisplayUI_h
#define DisplayUI_h

#include <iostream>

#include <SimpleMenu.h>   // SimpleMenu library
#include <SimpleButton.h> // SimpleButton library

#include "ConfigOLED.h"   // Config class for OLED
#include "MenuGraph.h"    // Packet Graph Menu
#include "MenuChannel.h"  // Channel Menu
#include "EntryChannel.h"
#include "MenuTextSD.h"
#include "Settings.h"

// DSTIKE ESP32 Wristband
// #define BUTTON_UP 19
// #define BUTTON_DOWN 5
// #define BUTTON_SELECT 18

// DSTIKE D-DUINO32 SD
#define BUTTON_UP 32
#define BUTTON_DOWN 25
#define BUTTON_SELECT 33

using namespace simplesetting;

namespace simplemenu {
    class DisplayUI: public SimpleMenu {
        public:
            DisplayUI();
            ~DisplayUI();

            void init(Settings& settings, bool flipped, Channel& ch, SDPacketWriter* sd, Scan* scan);
            bool update();

            void up();
            void down();
            void click();
            void doubleclick();
            void hold();

            void add_graph_data(uint64_t pkts, uint64_t misc, uint64_t dropped, uint64_t bytes, uint64_t rssi);
            void update_entry_data(uint64_t pkts, uint64_t misc, uint64_t dropped, uint64_t bytes, uint64_t rssi);
            void update_channel_entry();

            void reset_values();
            void set_max_values(int pkt_max, int byte_max, int rssi_max, int misc_max, int drop_max);
            void print_max_values() const;

            void goto_main_menu();
            void goto_pkt_graph();
            void goto_byte_graph();
            void goto_rssi_graph();
            void goto_sd_menu();
            void goto_channel_menu();
            void goto_id_menu();

            void flip();

        private:
            MenuText* main_menu        { nullptr };
            MenuChannel* channel_menu  { nullptr };
            MenuTextSD * sd_menu       { nullptr };
            MenuCustom * id_menu       { nullptr };
            MenuGraph  * pkt_graph     { nullptr };
            MenuGraph  * byte_graph    { nullptr };
            MenuGraph  * rssi_graph    { nullptr };
            MenuGraph  * drop_graph    { nullptr };
            MenuGraph  * misc_graph    { nullptr };

            EntryValue  * pkt_graph_entry    { nullptr };
            EntryValue  * byte_graph_entry   { nullptr };
            EntryValue  * rssi_graph_entry    { nullptr };
            EntryValue  * runtime_entry { nullptr };
            EntryValue  * drop_graph_entry { nullptr };
            EntryValue  * misc_graph_entry    { nullptr };
            EntryChannel* channel_menu_entry { nullptr };

            simplebutton::Button* button_up;
            simplebutton::Button* button_down;
            simplebutton::Button* button_select;

            Settings* settings { nullptr };

            Channel* ch { nullptr };
    };
}

#endif /* ifndef DisplayUI_h */
