#ifndef Scan_h
#define Scan_h

#include <iostream>         // std::cout
#include <list>             // std::list
#include <vector>           // std::vector
#include <functional>       // std::function

#include "esp_event_loop.h" // for init() method
#include "nvs_flash.h"      // for init() method

#include "Packet.h"         // for parsing raw WiFi frames
#include "PacketWriter.h"   // for saving Packet objects
#include "Interval.h"       // for the packet graph
#include "SnifferStats.h"   // for counting packets

using namespace std;

static xSemaphoreHandle pkt_list_semaphore = 0;
static xSemaphoreHandle stats_semaphore    = 0;

class Scan {
    public:
        Scan();
        ~Scan();

        void init();
        void sniffer(void* buf, wifi_promiscuous_pkt_type_t type);

        void start();
        void stop();

        void update();

        void set_packet_writer(PacketWriter* packet_writer);
        void set_stats_interval(int64_t us);

        size_t get_pkts_num() const;

        Packet* pop_pkt();

        SnifferStats get_all_time_stats() const;

        void set_on_new_stats(function<void(SnifferStats, SnifferStats)> stats_fnct);

    private:
        wifi_init_config_t config;
        wifi_country_t     country;

        list<Packet*> pkt_list {};

        PacketWriter* pkt_writer { nullptr };

        SnifferStats tmp_stats { 0, 0, 0, 0, 0 };
        SnifferStats all_time_stats { 0, 0, 0, 0, 0 };
        uint32_t     stats_list_size { 32 };
        Interval     stats_interval { 1000000 };

        function<void(SnifferStats, SnifferStats)> stats_fnct { nullptr };

        wifi_init_config_t* create_wifi_config();
        wifi_country_t* create_wifi_country();

        bool push_pkt(Packet* p);

        size_t ch_list_index { 0 };
};
#endif /* ifndef Scan_h */