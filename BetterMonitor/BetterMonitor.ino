#include <string>           // std::string
#include <iostream>         // std::cout

#include "Settings.h"       // Settings
#include "Scan.h"           // Scan
#include "SDPacketWriter.h" // Write packets to SD/MMC
#include "DisplayUI.h"      // OLED
#include "CLI.h"            // Serial
#include "RGBLED.h"         // LED

#define LED_PIN 18
#define LED_BRIGHTNESS 10

using namespace std;

simplesetting::Settings settings {};
Scan scan {};
SDPacketWriter sd {};
simplemenu::DisplayUI ui {};
Channel ch { 1, 0, 1000 };
simplecli::CLI cli {};
RGBLED led {};

Interval update_interval { 1000000 };

void setup() {  
    cli.init(115200, ui, ch, settings, sd, led); // start serial communication with given baudrate

    settings.init();                             // start SPIFFS
    settings.load();                             // restore settings from memory

    scan.init();                                 // initialize scan

    //led.init(LED_PIN, LED_BRIGHTNESS);

    // set sniffer callback
    esp_wifi_set_promiscuous_rx_cb([](void* buf, wifi_promiscuous_pkt_type_t type) {
        scan.sniffer(buf, type);
    });

    //scan.set_packet_writer(&sd); // write sniffed packets to SD/MMC

    // set sniffer statistics callback to menu graph
    scan.set_on_new_stats([](SnifferStats stats, SnifferStats new_stats) {
        ui.update_entry_data(stats.all_pkts, stats.misc_pkts, stats.dropped_pkts, stats.bytes, stats.rssi);
        ui.add_graph_data(new_stats.all_pkts, new_stats.misc_pkts, new_stats.dropped_pkts, new_stats.bytes, new_stats.rssi);
    });

    // set channel
    ch.set(settings.channel.get(), settings.channel_mode.get());

    // set channel change callback
    ch.set_on_changed([]() {
        ui.update_channel_entry(); // update UI

        // update settings
        settings.channel.set(ch.get_main_channel());
        settings.channel_mode.set(ch.get_mode());

        // save settings
        settings.save();
    });

    ui.init(settings, settings.flip_screen.get(), ch, &sd, &scan); // initialize display UI
    ui.set_display_timeout(0);

    scan.start();                            // start scan

    cout << "sync" << endl;
}

void loop() {
    scan.update(); // update sniffer
    ui.update();   // update display UI
    cli.update();  // listen for new commands
    led.update();

    if (update_interval.update()) {
        ui.print_max_values();

        /*
           cout << endl;

           if (!sd.is_paused()) {
           cout << sd.get_file_size() << "b written in " << sd.get_file_time() << "us" << endl;
           cout << sd.get_speed() << "mb/s" << endl;
           }

           auto stats = scan.get_all_time_stats();

           cout << '{' << stats.all_pkts << ',' << stats.dropped_pkts << ',' << stats.misc_pkts << ',' << stats.bytes <<  '}';

           cout << endl;

           cout << scan.get_pkts_num() << "pkts in queue" << endl;

           cout << "min. free: " << heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT) << " Byte | free: " << heap_caps_get_free_size(MALLOC_CAP_8BIT) << " Byte | " << heap_caps_get_free_size(MALLOC_CAP_DMA) << " DMA" << endl;

           if (sd.get_file_size() >= 10000000) scan.set_packet_writer(nullptr);
         */
    }
}
