#include "PacketWriter.h"

void PacketWriter::set_on_error(std::function<void()> on_error) {
    this->on_error = on_error;
}

void PacketWriter::set_time(uint32_t timestamp, int32_t timezone) {
    uint64_t corrected_timestamp = uint64_t(timestamp) - (esp_timer_get_time() / uint64_t(1000000));

    this->timestamp = corrected_timestamp + (esp_timer_get_time() % 1000000);
    this->timezone  = timezone;
}

bool PacketWriter::begin() {
    return true;
}

bool PacketWriter::end() {
    return true;
}

bool PacketWriter::pause(bool error) {
    if (!errored && error) {
        this->errored = true;
        if (on_error) on_error();
    }
    paused = true;
    return true;
}

bool PacketWriter::resume() {
    paused = false;
    return true;
}

bool PacketWriter::is_paused() const {
    return this->paused;
}

bool PacketWriter::is_errored() const {
    return this->errored;
}

bool PacketWriter::header_enabled() const {
    return false;
}

bool PacketWriter::write_pkt(const Packet& p) {
    if (paused) return false;

    // pcap settings
    bool pcap_header   = this->header_enabled();
    bool pcap_radiotap = true;

    // calcuate pcap entry size
    const uint32_t pcap_size = p.calc_pcap_size(pcap_header, pcap_radiotap);

    // allocate memory for buffer
    uint8_t* buffer = (uint8_t*)heap_caps_malloc(pcap_size, MALLOC_CAP_DMA);

    // write into buffer
    uint32_t written_size = p.write_pcap(buffer, timestamp, timezone, pcap_header, pcap_radiotap);

    // error check
    if (pcap_size != written_size) ESP_LOGE("PacketWriter", "Written packet size not equals calculated size");

    // write memory
    bool success = write(buffer, pcap_size);

    // release memory
    free(buffer);

    return success;
}