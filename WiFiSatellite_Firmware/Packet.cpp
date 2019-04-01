#include "Packet.h"

Packet::Packet(const wifi_promiscuous_pkt_t& pkt, uint32_t size) {
    wifi_pkt_rx_ctrl_t ctrl = static_cast<wifi_pkt_rx_ctrl_t>(pkt.rx_ctrl);

    // Size
    this->size          = size;
    this->original_size = ctrl.sig_len;

    if ((this->size > 0) && (this->size == this->original_size) && (ctrl.rx_state == 0)) {
        this->size          -= 4;
        this->original_size -= 4;
    }

    // Buffer
    this->buffer = (uint8_t*)heap_caps_malloc(this->size, MALLOC_CAP_8BIT);
    memcpy(this->buffer, pkt.payload, this->size);

    // Packet Type
    this->control_field = ((uint16_t(this->buffer[0]) << 8) | this->buffer[1]);
    this->type          = get_type(this->control_field);
    this->subtype       = get_subtype(this->control_field);

    // Channel
    this->channel        = ctrl.channel;
    this->second_channel = ctrl.secondary_channel;
    this->channel_freq  += (ctrl.channel - 1) * 5;
    if (ctrl.channel == 14) this->channel_freq += 7;

    // Antenna
    this->antenna = ctrl.ant;
    this->noise   = ctrl.noise_floor > 0 ? (ctrl.noise_floor / 4) : 0;

    // Other Packet Data
    this->failed_fcs = ctrl.rx_state != 0;
    this->short_gi   = ctrl.sgi;
    this->fec        = ctrl.fec_coding;
    this->mcs        = ctrl.mcs;
    this->ampdu      = ctrl.aggregation;
    this->rssi       = ctrl.rssi;

    this->time_s  = (esp_timer_get_time() / 1000000);
    this->time_us = ctrl.timestamp % 1000000;

    this->protocol  = get_protocol(ctrl.sig_mode);
    this->bandwidth = get_bandwidth(ctrl.cwb);
    this->data_rate = get_rate(ctrl.rate);
}

Packet::Packet(const Packet& p) {
    // Size
    this->size          = p.size;
    this->original_size = p.original_size;

    // Buffer
    this->buffer = (uint8_t*)heap_caps_malloc(this->size, MALLOC_CAP_8BIT);
    memcpy(this->buffer, p.buffer, this->size);

    // Packet Type
    this->control_field = p.control_field;
    this->type          = p.type;
    this->subtype       = p.subtype;

    // Channel
    this->channel        = p.channel;
    this->second_channel = p.second_channel;
    this->channel_freq   = p.channel_freq;

    // Antenna
    this->antenna = p.antenna;
    this->noise   = p.noise;

    // Other Packet Data
    this->failed_fcs = p.failed_fcs;
    this->short_gi   = p.short_gi;
    this->fec        = p.fec;
    this->mcs        = p.mcs;
    this->ampdu      = p.ampdu;
    this->rssi       = p.rssi;

    this->time_s  = p.time_s;
    this->time_us = p.time_us;

    this->protocol  = p.protocol;
    this->bandwidth = p.bandwidth;
    this->data_rate = p.data_rate;
}

Packet::Packet(Packet&& p) {
    // Size
    this->size          = p.size;
    this->original_size = p.original_size;

    // Buffer
    this->buffer = p.buffer;

    // Packet Type
    this->control_field = p.control_field;
    this->type          = p.type;
    this->subtype       = p.subtype;

    // Channel
    this->channel        = p.channel;
    this->second_channel = p.second_channel;
    this->channel_freq   = p.channel_freq;

    // Antenna
    this->antenna = p.antenna;
    this->noise   = p.noise;

    // Other Packet Data
    this->failed_fcs = p.failed_fcs;
    this->short_gi   = p.short_gi;
    this->fec        = p.fec;
    this->mcs        = p.mcs;
    this->ampdu      = p.ampdu;
    this->rssi       = p.rssi;

    this->time_s  = p.time_s;
    this->time_us = p.time_us;

    this->protocol  = p.protocol;
    this->bandwidth = p.bandwidth;
    this->data_rate = p.data_rate;
}

Packet::~Packet() {
    if (this->buffer) free(this->buffer);
}

bool Packet::get_to_ds() const {
    if ((this->type == PacketType::Ctrl) && (this->subtype > SubType::CFEndCFAck)) return 0;

    return (this->control_field) & 0x01;
}

bool Packet::get_from_ds() const {
    if ((this->type == PacketType::Ctrl) && (this->subtype > SubType::CFEndCFAck)) return 0;

    return (this->control_field >> 1) & 0x01;
}

bool Packet::get_fragments() const {
    if ((this->type == PacketType::Ctrl) && (this->subtype > SubType::CFEndCFAck)) return 0;

    return (this->control_field >> 2) & 0x01;
}

bool Packet::get_retry() const {
    if ((this->type == PacketType::Ctrl) && (this->subtype > SubType::CFEndCFAck)) return 0;

    return (this->control_field >> 3) & 0x01;
}

bool Packet::get_pwr_mgmt() const {
    return (this->control_field >> 4) & 0x01;
}

bool Packet::get_more_data() const {
    return (this->control_field >> 5) & 0x01;
}

bool Packet::get_htc_order() const {
    return (this->control_field >> 7) & 0x01;
}

bool Packet::get_amsdu() const {
    if (this->type != PacketType::Data) return false;

    if ((this->subtype < SubType::QoSData) || (this->subtype > SubType::QoSCFAckCFPoll)) return false;

    if (this->size < 31) return false;

    uint8_t qosControlField = (get_from_ds() && get_to_ds()) ? buffer[31] : buffer[25];
    bool    aMSDU           = (qosControlField >> 7) & 0x01;

    return aMSDU;
}

bool Packet::get_short_gi() const {
    return this->short_gi;
}

bool Packet::get_fec() const {
    return this->fec;
}

bool Packet::get_stbc() const {
    return this->stbc;
}

bool Packet::get_ampdu() const {
    return this->ampdu;
}

bool Packet::valid_fcs() const {
    return !this->failed_fcs;
}

bool Packet::holds_data() const {
    return
        this->type == PacketType::Data &&
        (this->subtype == SubType::Data ||
         this->subtype == SubType::DataCFAck ||
         this->subtype == SubType::DataCFPoll ||
         this->subtype == SubType::DataCFAck ||
         this->subtype == SubType::DataCFAckCFPoll ||
         this->subtype == SubType::QoSData ||
         this->subtype == SubType::QoSDataCFAck ||
         this->subtype == SubType::QoSDataCFPoll ||
         this->subtype == SubType::QoSDataCFAckCFPoll);
}

bool Packet::has_ssid() const {
    return this->type == PacketType::Mgmt && this->size > 37 &&
           (this->subtype == SubType::Beacon ||
            this->subtype == SubType::ProbeReq ||
            this->subtype == SubType::ProbeRes ||
            this->subtype == SubType::AssocReq ||
            this->subtype == SubType::ReassocReq);
}

bool Packet::is_protected() const {
    return (this->control_field >> 6) & 0x01;
}

uint8_t* Packet::get_buffer() const {
    return &(this->buffer[0]);
}

uint8_t* Packet::get_addr_1() const {
    if (this->size < 10) return nullptr;

    return &(this->buffer[4]);
}

uint8_t* Packet::get_addr_2() const {
    if (this->size < 16) return nullptr;

    switch (this->type) {
        case PacketType::Ctrl:
            if ((this->subtype == SubType::RTS)
                || (this->subtype == SubType::PSPoll)
                || (this->subtype == SubType::CFEnd)
                || (this->subtype == SubType::CFEndCFAck)
                || (this->subtype == SubType::BlockAckReq)
                || (this->subtype == SubType::BlockAck)) {
                return &(this->buffer[10]);
            }
            break;
        case PacketType::Data:
            return &(this->buffer[10]);
        case PacketType::Mgmt:
            return &(this->buffer[10]);
    }

    return nullptr;
}

uint8_t* Packet::get_addr_3() const {
    if (this->size < 22) return nullptr;

    switch (this->type) {
        case PacketType::Data:
            return &(this->buffer[16]);
        case PacketType::Mgmt:
            return &(this->buffer[16]);
    }

    return nullptr;
}

uint8_t* Packet::get_addr_4() const {
    if (this->size < 30) return nullptr;

    switch (this->type) {
        case PacketType::Data:
            if (get_from_ds() && get_to_ds()) return &(this->buffer[24]);
            break;
    }

    return nullptr;
}

uint8_t* Packet::get_addr_ra() const {
    switch (this->type) {
        case PacketType::Ctrl:
            if (this->subtype != SubType::Wrapper) return get_addr_1();
            break;
        case PacketType::Data:
            return get_addr_1();
        case PacketType::Mgmt:
            return get_addr_1();
    }

    return nullptr;
}

uint8_t* Packet::get_addr_ta() const {
    switch (this->type) {
        case PacketType::Ctrl:
            if ((this->subtype == SubType::RTS) ||
                (this->subtype == SubType::PSPoll) ||
                (this->subtype == SubType::CFEnd) ||
                (this->subtype == SubType::CFEndCFAck) ||
                (this->subtype == SubType::BlockAckReq) ||
                (this->subtype == SubType::BlockAck) ||
                (this->subtype == SubType::BeamRepPoll) ||
                (this->subtype == SubType::Poll) ||
                (this->subtype == SubType::SPR) ||
                (this->subtype == SubType::Grant) ||
                (this->subtype == SubType::DMGCTS) ||
                (this->subtype == SubType::SSW) ||
                (this->subtype == SubType::SSWFeedback) ||
                (this->subtype == SubType::SSWAck) ||
                (this->subtype == SubType::GrantAck) ||
                (this->subtype == SubType::VHTNDPAnnoucement)) {
                return get_addr_2();
            }
            break;
        case PacketType::Data:
            return get_addr_2();
        case PacketType::Mgmt:
            return get_addr_2();
    }

    return nullptr;
}

uint8_t* Packet::get_addr_bssid() const {
    switch (this->type) {
        case PacketType::Ctrl:
            if (this->subtype == SubType::PSPoll) return get_addr_ra();
            else if ((this->subtype == SubType::CFEnd) ||
                     (this->subtype == SubType::CFEndCFAck)) {
                return get_addr_ta();
            }
            break;
        case PacketType::Data:
            if (!get_from_ds() && !get_to_ds()) return get_addr_3();
            else if (!get_from_ds() && get_to_ds()) return get_addr_2();
            else if (get_from_ds() && !get_to_ds()) return get_addr_1();
            else if (get_from_ds() && get_to_ds() && get_amsdu()) return get_addr_3();
            break;
        case PacketType::Mgmt:
            return get_addr_3();
    }

    return nullptr;
}

uint8_t* Packet::get_addr_da() const {
    switch (this->type) {
        case PacketType::Data:
            if (!get_from_ds()) return get_addr_1();
            else if (!get_amsdu()) return get_addr_3();
            break;
        case PacketType::Mgmt:
            return get_addr_1();
    }

    return nullptr;
}

uint8_t* Packet::get_addr_sa() const {
    switch (this->type) {
        case PacketType::Data:
            if (!get_from_ds()) return get_addr_2();
            else if (!get_amsdu()) {
                if (!get_to_ds()) return get_addr_3();
                else return get_addr_4();
            }
            break;
        case PacketType::Mgmt:
            return get_addr_2();
    }

    return nullptr;
}

uint8_t* Packet::get_ssid() const {
    if (!has_ssid()) return nullptr;

    uint8_t* ssid_str = nullptr;
    uint8_t  tagID;
    uint8_t  tagLen;
    uint16_t i;

    switch (this->subtype) {
        case SubType::Beacon:
        case SubType::ProbeRes:
            i = 36;
            break;
        case SubType::ProbeReq:
            i = 24;
            break;
        case SubType::AssocReq:
            i = 28;
            break;
        case SubType::ReassocReq:
            i = 34;
            break;
        default:
            i = 0;
    }

    do {
        tagID  = this->buffer[i];
        tagLen = this->buffer[i + 1];

        if (tagID == 0)
            ssid_str = &(this->buffer[i+2]);

        i += tagLen + 2;
    } while ((i + 2) < this->size && !ssid_str);

    return ssid_str;
}

uint8_t Packet::get_ssid_size() const {
    uint8_t* ssid_ptr  = get_ssid();
    uint8_t  ssid_size = ssid_ptr ? *(ssid_ptr - 1) : 0;

    if ((ssid_size > 0) && (ssid_ptr[0] == '\0')) ssid_size = 0;

    return ssid_size;
}

uint8_t Packet::get_channel() const {
    return this->channel;
}

uint8_t Packet::get_2nd_channel() const {
    return this->second_channel;
}

uint8_t Packet::get_antenna() const {
    return this->antenna;
}

uint8_t Packet::get_mcs() const {
    return this->mcs;
}

int8_t Packet::get_rssi() const {
    return this->rssi;
}

int8_t Packet::get_noise() const {
    return this->noise;
}

uint16_t Packet::get_size() const {
    return this->size;
}

uint16_t Packet::get_original_size() const {
    return this->original_size;
}

uint16_t Packet::get_channel_freq() const {
    return this->channel_freq;
}

uint16_t Packet::get_rate() const {
    return this->data_rate;
}

uint16_t Packet::get_control_field() const {
    return this->control_field;
}

uint32_t Packet::get_seconds() const {
    return this->time_s;
}

uint32_t Packet::get_microseconds() const {
    return this->time_us;
}

uint32_t Packet::calc_pcap_size(bool header, bool radiotap) const {
    uint32_t i = 0;

    if (radiotap)
        i += calc_radiotap_size();

    if (header) i += 24;  // fixed PCAP File header size
    i += 16;              // fixed header size for every packet
    i += this->size;

    return i;
}

std::string Packet::get_type_str() const {
    switch (this->type) {
        case PacketType::Mgmt: return "Mgmt";
        case PacketType::Ctrl: return "Ctrl";
        case PacketType::Data: return "Data";
        default: return "Unknown";
    }
}

std::string Packet::get_subtype_str() const {
    if (this->type == PacketType::Mgmt) {
        switch (this->subtype) {
            case SubType::AssocReq: return "Association Request";
            case SubType::AssocRes: return "Association Response";
            case SubType::ReassocReq: return "Reassociation Request";
            case SubType::ReassocRes: return "Reassociation Response";
            case SubType::ProbeReq: return "Probe Response";
            case SubType::ProbeRes: return "Probe Response";
            case SubType::TimingAdv: return "Timing Advertisement";
            case SubType::Beacon: return "Beacon";
            case SubType::ATIM: return "ATIM";
            case SubType::Disassoc: return "Disassociation";
            case SubType::Auth: return "Authentication";
            case SubType::Deauth: return "Deauthentication";
            case SubType::Action: return "Action";
            case SubType::ActionNoAck: return "Action No Ack ";
            default: return "Unknown";
        }
    }

    else if (this->type == PacketType::Ctrl) {
        switch (this->subtype) {
            case SubType::BeamRepPoll: return "Beamforming Report Poll";
            case SubType::VHTNDPAnnoucement: return "VHT NDP Announcement";
            case SubType::Wrapper: return "Control Wrapper";
            case SubType::BlockAckReq: return "Block Ack Request";
            case SubType::BlockAck: return "Block Ack";
            case SubType::PSPoll: return "PS-Poll";
            case SubType::RTS: return "RTS";
            case SubType::CTS: return "CTS";
            case SubType::Ack: return "Ack";
            case SubType::CFEnd: return "CF-End";
            case SubType::CFEndCFAck: return "CF-End + CF-Ack";
            case SubType::Poll: return "Poll";
            case SubType::Grant: return "Grant";
            case SubType::DMGCTS: return "DMG CTS";
            case SubType::DMGDTS: return "DMG DTS";
            case SubType::GrantAck: return "Grant Ack";
            case SubType::SSW: return "SSW";
            case SubType::SSWFeedback: return "SSW Feedback";
            case SubType::SSWAck: return "SSW Ack";
            default: return "Unknown";
        }
    }

    else if (this->type == PacketType::Data) {
        switch (this->subtype) {
            case SubType::Data: return "Data";
            case SubType::DataCFAck: return "Data + CF-Ack";
            case SubType::DataCFPoll:  return "Data + CF-Poll";
            case SubType::DataCFAckCFPoll: return "Data + CF-Ack + CF-Poll";
            case SubType::Null: return "Null";
            case SubType::CFAck: return "CF-Ack";
            case SubType::CFPoll: return "CF-Poll";
            case SubType::CFAckCFPoll: return "CF-Ack +CF-Poll";
            case SubType::QoSData:  return "QoS Data";
            case SubType::QoSDataCFAck: return "QoS Data + CF-Ack";
            case SubType::QoSDataCFPoll: return "QoS Data +CF-Poll";
            case SubType::QoSDataCFAckCFPoll:  return "QoS Data +CF-Ack +CF-Poll ";
            case SubType::QoSNull: return "QoS Null";
            case SubType::QoSCFPoll: return "QoS CF-Poll";
            case SubType::QoSCFAckCFPoll: return "QoS CF-Ack +CF-Poll";
            default: return "Unknown";
        }
    }
}

std::string Packet::get_ssid_str() const {
    uint8_t* ssid_ptr  = get_ssid();
    uint8_t  ssid_size = ssid_ptr ? *(ssid_ptr - 1) : 0;

    return std::string((char*)ssid_ptr, ssid_size);
}

MacAddr Packet::get_mac_addr_1() const {
    return MacAddr(get_addr_1());
}

MacAddr Packet::get_mac_addr_2() const {
    return MacAddr(get_addr_2());
}

MacAddr Packet::get_mac_addr_3() const {
    return MacAddr(get_addr_3());
}

MacAddr Packet::get_mac_addr_4() const {
    return MacAddr(get_addr_4());
}

MacAddr Packet::get_mac_addr_ra() const {
    return MacAddr(get_addr_ra());
}

MacAddr Packet::get_mac_addr_ta() const {
    return MacAddr(get_addr_ta());
}

MacAddr Packet::get_mac_addr_bssid() const {
    return MacAddr(get_addr_bssid());
}

MacAddr Packet::get_mac_addr_da() const {
    return MacAddr(get_addr_da());
}

MacAddr Packet::get_mac_addr_sa() const {
    return MacAddr(get_addr_sa());
}

PacketType Packet::get_type() const {
    return this->type;
}

SubType Packet::get_subtype() const {
    return this->subtype;
}

Protocol Packet::get_protocol() const {
    return this->protocol;
}

Bandwidth Packet::get_bandwidth() const {
    return this->bandwidth;
}

uint32_t Packet::write_pcap(uint8_t* buffer, uint32_t timstamp, int32_t timezone, bool header, bool radiotap) const {
    uint32_t i = 0;

    if (header) i += write_pcap_header(buffer, timezone);

    i += write32(&buffer[i], timstamp + this->time_s);
    i += write32(&buffer[i], this->time_us);

    if (radiotap) {
        uint32_t radioTapHeaderSize = calc_radiotap_size();

        i += write32(&buffer[i], this->size + radioTapHeaderSize);
        i += write32(&buffer[i], this->original_size + radioTapHeaderSize);
        i += write_radiotap(&buffer[i]);
    }
    else {
        i += write32(&buffer[i], this->size);
        i += write32(&buffer[i], this->original_size);
    }

    memcpy(&buffer[i], this->buffer, this->size);
    i += this->size;

    return i;
}

PacketType Packet::get_type(uint16_t controlField) const {
    uint8_t type = (controlField >> 10) & 0x03;

    if (check_type(type, 0x00)) return PacketType::Mgmt;
    if (check_type(type, 0x01)) return PacketType::Ctrl;
    if (check_type(type, 0x02)) return PacketType::Data;

    return PacketType::Unknown;
}

SubType Packet::get_subtype(uint16_t controlField) const {
    uint8_t subtype       = (controlField >> 12) & 0x0F;
    uint8_t extensionType = (controlField >> 4) & 0x0F; // Ctrl Extension

    switch (this->type) {
        case PacketType::Ctrl:
            if (check_subtype(subtype, 0x04)) return SubType::BeamRepPoll;
            if (check_subtype(subtype, 0x05)) return SubType::VHTNDPAnnoucement;

            if (check_subtype(subtype, 0x06) && (this->size > 1)) {
                if (check_subtype(extensionType, 0x02)) return SubType::Poll;
                if (check_subtype(extensionType, 0x03)) return SubType::SPR;
                if (check_subtype(extensionType, 0x04)) return SubType::Grant;
                if (check_subtype(extensionType, 0x05)) return SubType::DMGCTS;
                if (check_subtype(extensionType, 0x06)) return SubType::DMGDTS;
                if (check_subtype(extensionType, 0x07)) return SubType::GrantAck;
                if (check_subtype(extensionType, 0x08)) return SubType::SSW;
                if (check_subtype(extensionType, 0x09)) return SubType::SSWFeedback;
                if (check_subtype(extensionType, 0x0A)) return SubType::SSWAck;
            }

            if (check_subtype(subtype, 0x07)) return SubType::Wrapper;
            if (check_subtype(subtype, 0x08)) return SubType::BlockAckReq;
            if (check_subtype(subtype, 0x09)) return SubType::BlockAck;
            if (check_subtype(subtype, 0x0A)) return SubType::PSPoll;
            if (check_subtype(subtype, 0x0B)) return SubType::RTS;
            if (check_subtype(subtype, 0x0C)) return SubType::CTS;
            if (check_subtype(subtype, 0x0D)) return SubType::Ack;
            if (check_subtype(subtype, 0x0E)) return SubType::CFEnd;
            if (check_subtype(subtype, 0x0F)) return SubType::CFEndCFAck;

            break;

        case PacketType::Mgmt:
            if (check_subtype(subtype, 0x00)) return SubType::AssocReq;
            if (check_subtype(subtype, 0x01)) return SubType::AssocRes;
            if (check_subtype(subtype, 0x02)) return SubType::ReassocReq;
            if (check_subtype(subtype, 0x03)) return SubType::ReassocRes;
            if (check_subtype(subtype, 0x04)) return SubType::ProbeReq;
            if (check_subtype(subtype, 0x05)) return SubType::ProbeRes;
            if (check_subtype(subtype, 0x06)) return SubType::TimingAdv;
            if (check_subtype(subtype, 0x08)) return SubType::Beacon;
            if (check_subtype(subtype, 0x09)) return SubType::ATIM;
            if (check_subtype(subtype, 0x0A)) return SubType::Disassoc;
            if (check_subtype(subtype, 0x0B)) return SubType::Auth;
            if (check_subtype(subtype, 0x0C)) return SubType::Deauth;
            if (check_subtype(subtype, 0x0D)) return SubType::Action;
            if (check_subtype(subtype, 0x0E)) return SubType::ActionNoAck;

            break;

        case PacketType::Data:
            if (check_subtype(subtype, 0x00)) return SubType::Data;
            if (check_subtype(subtype, 0x01)) return SubType::DataCFAck;
            if (check_subtype(subtype, 0x02)) return SubType::DataCFPoll;
            if (check_subtype(subtype, 0x03)) return SubType::DataCFAckCFPoll;
            if (check_subtype(subtype, 0x04)) return SubType::Null;
            if (check_subtype(subtype, 0x05)) return SubType::CFAck;
            if (check_subtype(subtype, 0x06)) return SubType::CFPoll;
            if (check_subtype(subtype, 0x07)) return SubType::CFAckCFPoll;
            if (check_subtype(subtype, 0x08)) return SubType::QoSData;
            if (check_subtype(subtype, 0x09)) return SubType::QoSDataCFAck;
            if (check_subtype(subtype, 0x0A)) return SubType::QoSDataCFPoll;
            if (check_subtype(subtype, 0x0B)) return SubType::QoSDataCFAckCFPoll;
            if (check_subtype(subtype, 0x0C)) return SubType::QoSNull;
            if (check_subtype(subtype, 0x0E)) return SubType::QoSCFPoll;
            if (check_subtype(subtype, 0x0F)) return SubType::QoSCFAckCFPoll;

            break;
    }

    return SubType::Unknown;
}

Protocol Packet::get_protocol(uint8_t sig_mode) const {
    switch (sig_mode) {
        case 0: return Protocol::bg;
        case 1: return Protocol::n;
        case 3: return Protocol::ac;
        default: return Protocol::Unknown;
    }
}

Bandwidth Packet::get_bandwidth(uint8_t cwb) const {
    if (this->protocol != Protocol::n) return Bandwidth::nonHT;

    if (cwb == 0) return Bandwidth::HT20;

    if (this->second_channel == 1) return Bandwidth::HT40plus;
    return Bandwidth::HT40minus;
}

uint16_t Packet::get_rate(uint8_t rate) const {
    // https://github.com/espressif/esp-idf/blob/2a1c74883494b8a47ee4f2aa04a04d895222a5df/components/esp32/include/esp_wifi_types.h#L457
    switch (rate) {
        case 0x00: return 2;                                        // 1 Mbps with long preamble
        case 0x01: return 4;                                        // 2 Mbps with long preamble
        case 0x02: return 11;                                       // 5.5 Mbps with long preamble
        case 0x03: return 22;                                       // 11 Mbps with long preamble
        case 0x05: return 4;                                        // 2 Mbps with short preamble
        case 0x06: return 11;                                       // 5.5 Mbps with short preamble
        case 0x07: return 22;                                       // 11 Mbps with short preamble
        case 0x08: return 96;                                       // 48 Mbps
        case 0x09: return 48;                                       // 24 Mbps
        case 0x0A: return 24;                                       // 12 Mbps
        case 0x0B: return 12;                                       // 6 Mbps
        case 0x0C: return 108;                                      // 54 Mbps
        case 0x0D: return 72;                                       // 36 Mbps
        case 0x0E: return 36;                                       // 18 Mbps
        case 0x0F: return 18;                                       // 9 Mbps
        case 0x10: return bandwidth == Bandwidth::HT20 ? 13 : 27;   // MCS0 with long GI, 6.5 Mbps for 20MHz, 13.5 Mbps for 40MHz
        case 0x11: return bandwidth == Bandwidth::HT20 ? 26 : 54;   // MCS1 with long GI, 13 Mbps for 20MHz, 27 Mbps for 40MHz
        case 0x12: return bandwidth == Bandwidth::HT20 ? 39 : 81;   // MCS2 with long GI, 19.5 Mbps for 20MHz, 40.5 Mbps for 40MHz
        case 0x13: return bandwidth == Bandwidth::HT20 ? 52 : 108;  // MCS3 with long GI, 26 Mbps for 20MHz, 54 Mbps for 40MHz
        case 0x14: return bandwidth == Bandwidth::HT20 ? 78 : 162;  // MCS4 with long GI, 39 Mbps for 20MHz, 81 Mbps for 40MHz
        case 0x15: return bandwidth == Bandwidth::HT20 ? 104 : 216; // MCS5 with long GI, 52 Mbps for 20MHz, 108 Mbps for 40MHz
        case 0x16: return bandwidth == Bandwidth::HT20 ? 117 : 243; // MCS6 with long GI, 58.5 Mbps for 20MHz, 121.5 Mbps for 40MHz
        case 0x17: return bandwidth == Bandwidth::HT20 ? 130 : 270; // MCS7 with long GI, 65 Mbps for 20MHz, 135 Mbps for 40MHz
        case 0x18: return bandwidth == Bandwidth::HT20 ? 14 : 30;   // MCS0 with short GI, 7.2 Mbps for 20MHz, 15 Mbps for 40MHz
        case 0x19: return bandwidth == Bandwidth::HT20 ? 29 : 60;   // MCS1 with short GI, 14.4 Mbps for 20MHz, 30 Mbps for 40MHz
        case 0x1A: return bandwidth == Bandwidth::HT20 ? 43 : 90;   // MCS2 with short GI, 21.7 Mbps for 20MHz, 45 Mbps for 40MHz
        case 0x1B: return bandwidth == Bandwidth::HT20 ? 58 : 120;  // MCS3 with short GI, 28.9 Mbps for 20MHz, 60 Mbps for 40MHz
        case 0x1C: return bandwidth == Bandwidth::HT20 ? 87 : 180;  // MCS4 with short GI, 43.3 Mbps for 20MHz, 90 Mbps for 40MHz
        case 0x1D: return bandwidth == Bandwidth::HT20 ? 116 : 240; // MCS5 with short GI, 57.8 Mbps for 20MHz, 120 Mbps for 40MHz
        case 0x1E: return bandwidth == Bandwidth::HT20 ? 130 : 270; // MCS6 with short GI, 65 Mbps for 20MHz, 135 Mbps for 40MHz
        case 0x1F: return bandwidth == Bandwidth::HT20 ? 144 : 300; // MCS7 with short GI, 72.2 Mbps for 20MHz, 150 Mbps for 40MHz
        default: return 0;
    }
}

uint32_t Packet::write_pcap_header(uint8_t* buffer, int32_t timeZone, bool radiotap) const {
    uint32_t i = 0;

    i += write32(&buffer[i], 0xa1b23c4d);           // magic number
    i += write16(&buffer[i], 2);                    // major version number
    i += write16(&buffer[i], 4);                    // minor version number
    i += write32(&buffer[i], timeZone);             // GMT to local correction
    i += write32(&buffer[i], 0);                    // accuracy of timestamps
    i += write32(&buffer[i], 65535);                // max length of captured packets
    i += write32(&buffer[i], radiotap ? 127 : 105); // data link type

    return i;
}

uint32_t Packet::calc_radiotap_size() const {
    uint32_t i = 0;

    i += 1; // it_version;
    i += 1; // it_pad
    i += 2; // it_len
    i += 4; // it_present;

    i += 1; // Flags
    i += 1; // Rate
    i += 4; // Channel
    i += 1; // Antenna signal
    i += 1; // Antenna noise
    i += 1; // Antenna
    i += 3; // MCS

    return i;
}

uint32_t Packet::write_radiotap(uint8_t* buffer) const {
    uint32_t i = 0;

    uint8_t  vers = 0;
    uint8_t  pad  = 0;
    uint16_t len  = 0;
    uint32_t pres = 0;

    i += 1;                     // it_version;
    i += 1;                     // it_pad
    i += 2;                     // it_len
    i += 4;                     // it_present;

    // Flags
    pres |= (uint32_t(1) << 1); // enable Flag Field
    i    += write8(&buffer[i], ((failed_fcs ? 0x50 : 0x00) | (short_gi ? 0x80 : 0x00)));

    // Rate
    pres |= (uint32_t(1) << 2);                   // enable Rate Field
    i    += write8(&buffer[i], data_rate);        // in 500kbps

    // Channel
    pres |= (uint32_t(1) << 3);                   // enable Channel Field
    i    += write16(&buffer[i], channel_freq);    // frequency
    i    += write16(&buffer[i], 0x0080 | 0x0400); // 2GHz spectrum channel, Dynamic CCK-OFDM channel

    // Antenna signal
    pres |= (uint32_t(1) << 5);                   // enable Antenna signal Field
    i    += write8(&buffer[i], rssi);

    // Antenna noise
    pres |= (uint32_t(1) << 6); // enable Antenna signal Field
    i    += write8(&buffer[i], noise);


    // Antenna
    pres |= (uint32_t(1) << 11);         // enable Antenna Field
    i    += write8(&buffer[i], antenna); // channel                                                                                                                                       // maxpower

    // MCS
    pres |= (uint32_t(1) << 19);         // enable MCS Field
    i    += write8(&buffer[i], (0x01 | 0x02 | 0x10 | 0x20));
    i    += write8(&buffer[i], (second_channel > 0 ? 0x01 : 0x00) | (short_gi ? 0x04 : 0x00) | (fec ? 0x10 : 0x00) | (stbc ? 0x40 : 0x00));
    i    += write8(&buffer[i], mcs);

    // RadiotapHeader
    len = i;

    write8(&buffer[0], pad);   // it_version
    write8(&buffer[1], vers);  // it_pad
    write16(&buffer[2], len);  // it_len
    write32(&buffer[4], pres); // it_present

    return len;
}

bool Packet::check_type(uint8_t type, uint8_t bitmask, bool inverted) const {
    type    &= 0x03;
    bitmask &= 0x03;

    return (type & bitmask) == bitmask && (inverted ? true : check_type(~type, ~bitmask, true));
}

bool Packet::check_subtype(uint8_t subType, uint8_t bitmask, bool inverted) const {
    subType &= 0x0F;
    bitmask &= 0x0F;

    return (subType & bitmask) == bitmask && (inverted ? true : check_subtype(~subType, ~bitmask, !inverted));
}