#include "DisplayUI.h"

namespace simplemenu {
    DisplayUI::DisplayUI() {}

    DisplayUI::~DisplayUI() {
        delete config;
        delete button_up;
        delete button_down;
        delete button_select;

        delete main_menu;
        delete pkt_graph;
        delete channel_menu;
        delete byte_graph;
        delete rssi_graph;
        delete drop_graph;
        delete misc_graph;
    }

    void DisplayUI::init(Settings& settings, bool flipped, Channel& ch, SDPacketWriter* sd, Scan* scan) {
        this->settings = &settings;
        this->ch       = &ch;

        config    = new ConfigOLED(flipped);           // create config
        main_menu = new MenuText("Main Menu", config); // create menu

        pkt_graph  = new MenuGraph(* main_menu);       // create graph menu for pkts
        byte_graph = new MenuGraph(* main_menu);       // create graph menu for bytes
        rssi_graph = new MenuGraph(* main_menu);       // create graph menu for average RSSI
        drop_graph = new MenuGraph(* main_menu);       // create graph menu for dropped pkts
        misc_graph = new MenuGraph(* main_menu);       // create graph menu for misc pkts

        pkt_graph->set_up(main_menu);
        byte_graph->set_up(pkt_graph);
        rssi_graph->set_up(byte_graph);
        drop_graph->set_up(rssi_graph);
        misc_graph->set_up(drop_graph);

        pkt_graph->set_down(byte_graph);
        byte_graph->set_down(rssi_graph);
        rssi_graph->set_down(drop_graph);
        drop_graph->set_down(misc_graph);
        misc_graph->set_down(main_menu);

        // create channel menu & entry
        channel_menu       = new MenuChannel("Channel", main_menu, & ch);
        channel_menu_entry = new EntryChannel("Channel", config, & ch, channel_menu);
        main_menu->add_entry(channel_menu_entry);

        pkt_graph->set_mode("Pkts/s");
        byte_graph->set_mode("Byte/s");
        rssi_graph->set_mode("RSSI (-dBm)");
        drop_graph->set_mode("Dropped/s");
        misc_graph->set_mode("Misc/s");

        // add entries
        pkt_graph_entry  = main_menu->add_value_entry("Pkts", pkt_graph);
        byte_graph_entry = main_menu->add_value_entry("Byte", byte_graph);
        rssi_graph_entry = main_menu->add_value_entry("RSSI", rssi_graph);
        drop_graph_entry = main_menu->add_value_entry("Drop", drop_graph);
        misc_graph_entry = main_menu->add_value_entry("Misc", misc_graph);

        runtime_entry = main_menu->add_value_entry("Time");

        //sd_menu = new MenuTextSD(sd, scan, main_menu);
        //main_menu->add_static_entry("SD", sd_menu);

        set_main_menu(main_menu); // set main menu

        // create buttons
        button_up     = new simplebutton::ButtonPullup(BUTTON_UP);
        button_down   = new simplebutton::ButtonPullup(BUTTON_DOWN);
        button_select = new simplebutton::ButtonPullup(BUTTON_SELECT);

        id_menu = new MenuCustom("ID", config);
        id_menu->set_draw_fnct([this](Config* config, bool cursor) {
            config->draw(50, 20, this->settings->id.get_value());
        });
    }

    bool DisplayUI::update() {
        // update button states
        button_up->update();
        button_down->update();
        button_select->update();

        // react on button clicks
        if (button_up->clicked() || button_up->holding()) up();
        if (button_down->clicked() || button_down->holding()) down();

        if (button_select->holding()) hold();
        else if (button_select->doubleClicked()) doubleclick();
        else if (button_select->clicked()) click();

        // update UI
        return SimpleMenu::update();
    }

    void DisplayUI::up() {
        SimpleMenu::up();
    }

    void DisplayUI::down() {
        SimpleMenu::down();
    }

    void DisplayUI::click() {
        SimpleMenu::click();
    }

    void DisplayUI::doubleclick() {
        SimpleMenu::doubleclick();
    }

    void DisplayUI::hold() {
        SimpleMenu::hold();
    }

    void DisplayUI::add_graph_data(uint64_t pkts, uint64_t misc, uint64_t dropped, uint64_t bytes, uint64_t rssi) {
        if (pkt_graph) pkt_graph->push(pkts);
        if (byte_graph) byte_graph->push(bytes);
        if (rssi_graph) rssi_graph->push(rssi > 0 ? rssi / pkts : 0);
        if (misc_graph) misc_graph->push(misc);
        if (drop_graph) drop_graph->push(dropped);
    }

    void DisplayUI::update_entry_data(uint64_t pkts, uint64_t misc, uint64_t dropped, uint64_t bytes, uint64_t rssi) {
        if (pkt_graph_entry) pkt_graph_entry->set_num_value(pkts);
        if (byte_graph_entry) byte_graph_entry->set_num_value(bytes);
        if (rssi_graph_entry) rssi_graph_entry->set_value('-', rssi > 0 ? rssi/pkts : 0, "dBm");
        if (runtime_entry) runtime_entry->set_time_value(esp_timer_get_time() / 1000);
        if (drop_graph_entry) drop_graph_entry->set_num_value(misc);
        if (misc_graph_entry) misc_graph_entry->set_num_value(dropped);
    }

    void DisplayUI::update_channel_entry() {
        channel_menu_entry->update();
    }

    void DisplayUI::reset_values() {
        pkt_graph->reset_max();
        byte_graph->reset_max();
        rssi_graph->reset_max();
        misc_graph->reset_max();
        drop_graph->reset_max();
    }

    void DisplayUI::set_max_values(int pkt_max, int byte_max, int rssi_max, int misc_max, int drop_max) {
        pkt_graph->set_max(pkt_max);
        byte_graph->set_max(byte_max);
        rssi_graph->set_max(rssi_max);
        misc_graph->set_max(misc_max);
        drop_graph->set_max(drop_max);
    }

    void DisplayUI::print_max_values() const {
        std::cout << "max " << ch->get_main_channel() << ' ' << '[' << pkt_graph->get_max() << ',' << byte_graph->get_max() << ',' << rssi_graph->get_max() << ',' << misc_graph->get_max() << ',' << drop_graph->get_max() << ']' << std::endl;
    }

    void DisplayUI::goto_main_menu() {
        set_menu(main_menu);
    }

    void DisplayUI::goto_pkt_graph() {
        set_menu(pkt_graph);
    }

    void DisplayUI::goto_byte_graph() {
        set_menu(byte_graph);
    }

    void DisplayUI::goto_rssi_graph() {
        set_menu(rssi_graph);
    }

    void DisplayUI::goto_sd_menu() {
        set_menu(sd_menu);
    }

    void DisplayUI::goto_channel_menu() {
        set_menu(channel_menu);
    }

    void DisplayUI::goto_id_menu() {
        set_menu(id_menu);
    }
}
