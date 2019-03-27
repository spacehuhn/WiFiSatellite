#include "Scan.h"

Scan::Scan() {}

Scan::~Scan() {}

void Scan::init() {
    // init memory and stuff
    ESP_ERROR_CHECK(nvs_flash_init());
    tcpip_adapter_init();

    // init event loop
    ESP_ERROR_CHECK(esp_event_loop_init([] (void* ctx, system_event_t * event) {
        return ESP_OK;
    }, NULL));

    // set configs
    ESP_ERROR_CHECK(esp_wifi_init(create_wifi_config()));
    ESP_ERROR_CHECK(esp_wifi_set_country(create_wifi_country()));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

    // create semaphores
    pkt_list_semaphore = xSemaphoreCreateMutex();
    stats_semaphore    = xSemaphoreCreateMutex();

    // print info
    cout << "Scan setup complete" << '\r' << '\n';
}

void Scan::sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
    wifi_promiscuous_pkt_t* pkt = static_cast<wifi_promiscuous_pkt_t*>(buf);
    wifi_pkt_rx_ctrl_t ctrl     = static_cast<wifi_pkt_rx_ctrl_t>(pkt->rx_ctrl);

    bool dropped = false;

    if (type == WIFI_PKT_MISC) dropped = true;                 // drop MISC (MIMO etc.) frames
    else dropped = !push_pkt(new Packet(*pkt, ctrl.sig_len));  // add packet to list

    // update stats
    if (xSemaphoreTake(stats_semaphore, 5)) {
        tmp_stats.all_pkts     += 1;
        tmp_stats.misc_pkts    += (type == WIFI_PKT_MISC);
        tmp_stats.dropped_pkts += (dropped);
        tmp_stats.bytes        += ctrl.sig_len;
        tmp_stats.rssi         += ctrl.rssi & 127;
        xSemaphoreGive(stats_semaphore);
    } else {
        // cout << "Stats Blocked\r\n";
    }
}

void Scan::start() {
    // reset stats
    tmp_stats = { 0, 0, 0, 0, 0 };

    // start PacketWriter
    if (pkt_writer) pkt_writer->resume();

    // enable sniffer
    esp_wifi_set_promiscuous(true);
}

void Scan::stop() {
    // stop PacketWriter
    if (pkt_writer) pkt_writer->pause();

    // disable sniffer
    esp_wifi_set_promiscuous(false);
}

void Scan::update() {
    // [Update sniffer statistics]
    if (stats_interval.update()) {
        // set semaphore, wait 1s
        if (xSemaphoreTake(stats_semaphore, 1000)) {
            // increment all-time-stats
            all_time_stats.all_pkts     += tmp_stats.all_pkts;
            all_time_stats.dropped_pkts += tmp_stats.dropped_pkts;
            all_time_stats.misc_pkts    += tmp_stats.misc_pkts;
            all_time_stats.bytes        += tmp_stats.bytes;
            all_time_stats.rssi         += tmp_stats.rssi;

            // add stats to graph menu
            if (stats_fnct) stats_fnct(all_time_stats, tmp_stats);

            tmp_stats = { 0, 0, 0, 0, 0 };   // reset temporary stats
            xSemaphoreGive(stats_semaphore); // give semaphore back
        }
    }

    // [Write packets]
    Packet* p = pop_pkt();

    if (p) {
        if (pkt_writer) {
            pkt_writer->write_pkt(*p);                                 // write packet somewhere
            if (pkt_writer->is_errored()) set_packet_writer(nullptr);  // disable packet writer
        }
        delete p;                                                      // free memory
    }
	/*
    // [Check channel hop]
	if(ch_list.size() > 1){
    uint64_t current_time = esp_timer_get_time() / 1000;
    Channel& current_ch   = ch_list.at(ch_list_index);
    if (current_ch.next(current_time)) {
        // increment index
        ch_list_index++;
        if (ch_list_index == ch_list.size()) ch_list_index = 0;

        // enable new channel
        Channel& next_ch = ch_list.at(ch_list_index);
        next_ch.start(current_ch, current_time);
    }
	}*/
}

void Scan::set_packet_writer(PacketWriter* packet_writer) {
    // end old writer
    if (this->pkt_writer) this->pkt_writer->end();

    // start and set new writer
    if (packet_writer && packet_writer->begin()) this->pkt_writer = packet_writer;
    else this->pkt_writer = nullptr;
}

void Scan::set_stats_interval(int64_t us) {
    this->stats_interval.set_interval(us);
}

size_t Scan::get_pkts_num() const {
    return this->pkt_list.size();
}

wifi_init_config_t* Scan::create_wifi_config() {
    config.event_handler      = &esp_event_send;
    config.osi_funcs          = &g_wifi_osi_funcs;
    config.wpa_crypto_funcs   = g_wifi_default_wpa_crypto_funcs;
    config.static_rx_buf_num  = CONFIG_ESP32_WIFI_STATIC_RX_BUFFER_NUM;
    config.dynamic_rx_buf_num = CONFIG_ESP32_WIFI_DYNAMIC_RX_BUFFER_NUM;
    config.tx_buf_type        = CONFIG_ESP32_WIFI_TX_BUFFER_TYPE;
    config.static_tx_buf_num  = WIFI_STATIC_TX_BUFFER_NUM;
    config.dynamic_tx_buf_num = WIFI_DYNAMIC_TX_BUFFER_NUM;
    config.csi_enable         = WIFI_CSI_ENABLED;
    config.ampdu_rx_enable    = WIFI_AMPDU_RX_ENABLED;
    config.ampdu_tx_enable    = WIFI_AMPDU_TX_ENABLED;
    config.nvs_enable         = WIFI_NVS_ENABLED;
    config.nano_enable        = WIFI_NANO_FORMAT_ENABLED;
    config.tx_ba_win          = WIFI_DEFAULT_TX_BA_WIN;
    config.rx_ba_win          = WIFI_DEFAULT_RX_BA_WIN;
    config.wifi_task_core_id  = WIFI_TASK_CORE_ID;
    config.magic              = WIFI_INIT_CONFIG_MAGIC;

    return &config;
}

wifi_country_t* Scan::create_wifi_country() {
    country.cc[0]        = 'J';
    country.cc[1]        = 'P';
    country.cc[2]        = 'N';
    country.schan        = 1;
    country.nchan        = 14;
    country.max_tx_power = 127;
    country.policy       = WIFI_COUNTRY_POLICY_MANUAL;

    return &country;
}

bool Scan::push_pkt(Packet* p) {
    // check RAM
    if ((pkt_list.size() > 32)                                       // check packet list
        && (heap_caps_get_minimum_free_size(MALLOC_CAP_DMA) < 16000) // quick RAM check
        && (heap_caps_get_free_size(MALLOC_CAP_DMA) < 32000)) {      // complete RAM check
        // cout << "RAM full\r\n";
        delete p;
        return false;
    }

    // check semaphore, wait 5ms
    if (xSemaphoreTake(pkt_list_semaphore, 5)) {
        pkt_list.push_back(p);
        xSemaphoreGive(pkt_list_semaphore);
        // cout << "Pushed packet in list";
        return true;
    } else {
        // cout << "List push Blocked\r\n";
        delete p;
        return false;
    }
}

Packet* Scan::pop_pkt() {
    if (pkt_list.size() == 0) {
        // cout << "List empty\r\n";
        return nullptr;
    }

    // set packet list semaphore, wait 1s
    if (xSemaphoreTake(pkt_list_semaphore, 1000)) {
        Packet* p = pkt_list.front();       // get first from list
        pkt_list.pop_front();               // remove it from list
        xSemaphoreGive(pkt_list_semaphore); // give semaphore back

        return p;
    } else {
        // cout << "List pop blocked\r\n";
        return nullptr;
    }
}

SnifferStats Scan::get_all_time_stats() const {
    return this->all_time_stats;
}

void Scan::set_on_new_stats(function<void(SnifferStats, SnifferStats)> stats_fnct) {
    this->stats_fnct = stats_fnct;
}