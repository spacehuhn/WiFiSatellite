#ifndef Packet_h
#define Packet_h

#include "esp_wifi.h"    // for structure definitions

#include "BufferIO.h"    // write8, write16, write32
#include "PacketTypes.h" // Packet type & subtype enums
#include "MACAddr.h"     // MAC Address Object

using namespace bufferio;

class Packet {
    public:
        Packet(const wifi_promiscuous_pkt_t& pkt, uint32_t size);
        Packet(const Packet& p);
        Packet(Packet && p);
        ~Packet();

        bool get_to_ds() const;
        bool get_from_ds() const;
        bool get_fragments() const;
        bool get_retry() const;
        bool get_pwr_mgmt() const;
        bool get_more_data() const;
        bool get_htc_order() const;
        bool get_amsdu() const;
        bool get_short_gi() const;
        bool get_fec() const;
        bool get_stbc() const;
        bool get_ampdu() const;
        bool valid_fcs() const;
        bool holds_data() const;
        bool has_ssid() const;
        bool is_protected() const;

        uint8_t* get_buffer() const;

        uint8_t* get_addr_1() const;
        uint8_t* get_addr_2() const;
        uint8_t* get_addr_3() const;
        uint8_t* get_addr_4() const;

        uint8_t* get_addr_ra() const;
        uint8_t* get_addr_ta() const;
        uint8_t* get_addr_bssid() const;
        uint8_t* get_addr_da() const;
        uint8_t* get_addr_sa() const;

        uint8_t* get_ssid() const;

        uint8_t get_ssid_size() const;

        uint8_t get_channel() const;
        uint8_t get_2nd_channel() const;
        uint8_t get_antenna() const;
        uint8_t get_mcs() const;

        int8_t get_rssi() const;
        int8_t get_noise() const;

        uint16_t get_size() const;
        uint16_t get_original_size() const;
        uint16_t get_channel_freq() const;
        uint16_t get_rate() const;
        uint16_t get_control_field() const;

        uint32_t get_seconds() const;
        uint32_t get_microseconds() const;

        uint32_t calc_pcap_size(bool header = false, bool radiotap = true) const;

        std::string get_type_str() const;
        std::string get_subtype_str() const;
        std::string get_ssid_str() const;

        MacAddr get_mac_addr_1() const;
        MacAddr get_mac_addr_2() const;
        MacAddr get_mac_addr_3() const;
        MacAddr get_mac_addr_4() const;

        MacAddr get_mac_addr_ra() const;
        MacAddr get_mac_addr_ta() const;
        MacAddr get_mac_addr_bssid() const;
        MacAddr get_mac_addr_da() const;
        MacAddr get_mac_addr_sa() const;

        PacketType get_type() const;
        SubType get_subtype() const;
        Protocol get_protocol() const;
        Bandwidth get_bandwidth() const;

        uint32_t write_pcap(uint8_t* buffer, uint32_t timstamp, int32_t timezone, bool header = false, bool radiotap = true) const;

    private:
        PacketType type      { PacketType::Unknown };
        SubType    subtype   { SubType::Unknown };
        Protocol   protocol  { Protocol::Unknown };
        Bandwidth  bandwidth { Bandwidth::HT20 };

        bool failed_fcs { false };
        bool short_gi   { false };
        bool fec        { false };
        bool stbc       { false };
        bool ampdu      { false };

        uint8_t* buffer { nullptr };

        uint8_t channel        { 0 };
        uint8_t second_channel { 0 }; //  0: none; 1: above; 2: below
        uint8_t antenna        { 0 };
        uint8_t mcs            { 0 };

        int8_t rssi  { 0 };
        int8_t noise { 0 };

        uint16_t size          { 0 };
        uint16_t original_size { 0 };
        uint16_t channel_freq  { 2412 };
        uint16_t data_rate     { 0 };
        uint16_t control_field { 0 };

        uint32_t time_s  { 0 };
        uint32_t time_us { 0 };

        PacketType get_type(uint16_t control_field) const;
        SubType get_subtype(uint16_t control_field) const;
        Protocol get_protocol(uint8_t sig_mode) const;
        Bandwidth get_bandwidth(uint8_t cwb) const;
        uint16_t get_rate(uint8_t rate) const;

        uint32_t write_pcap_header(uint8_t* buffer, int32_t timeZone = 0, bool radiotap = true) const;
        uint32_t calc_radiotap_size() const;
        uint32_t write_radiotap(uint8_t* buffer) const;

        bool check_type(uint8_t type, uint8_t bitmask, bool inverted       = false) const;
        bool check_subtype(uint8_t subType, uint8_t bitmask, bool inverted = false) const;
};
#endif // ifndef Packet_h