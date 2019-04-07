#include "Channel.h"

Channel::Channel(int channel, int second, unsigned long time) {
    set(channel, second);
    set_time(time);
}

Channel::~Channel() {}

void Channel::set_on_changed(CHANNEL_ON_CHANGE_FNCT) {
    this->on_change_fnct = on_change_fnct;
}

int Channel::get_main_channel() const {
    return main_channel;
}

int Channel::get_second_channel() const {
    if (second_channel == 0) return main_channel;
    else if (second_channel > 0) return main_channel + 4;
    else return main_channel - 4;
}

int Channel::get_mode() const {
    return second_channel;
}

std::string Channel::get_bandwidth() const {
    std::stringstream ss;

    if (second_channel == 0) ss << "HT20";
    else if (second_channel < 0) ss << "HT40-";
    else ss << "HT40+";

    return ss.str();
}

std::string Channel::to_str() const {
    std::stringstream ss;

    ss << get_main_channel();

    if (second_channel > 0) ss << '+';
    else if (second_channel < 0) ss << '-';

    return ss.str();
}

void Channel::set(int channel, int mode) {
    if ((get_main_channel() == channel) && (get_mode() == mode)) return;
    if ((channel < 1) || (channel > 14)) return;
	if (mode < 0) mode = -1;
	if (mode > 0) mode = 1;

    this->main_channel = (uint8_t)channel;

    if ((mode == 0) || ((mode == 1) && (this->main_channel <= 10)) || ((mode == -1) && (this->main_channel >= 5)))
        this->second_channel = (int8_t)mode;
    else
        this->second_channel = 0;

    apply_changes();
}

void Channel::set_time(unsigned long time) {
    this->time = time;
}

void Channel::start(const Channel& prev_ch, unsigned long current_time) {
    this->start_time = current_time;

    if (*this != prev_ch) apply_changes();
}

bool Channel::next(unsigned long current_time) {
    return current_time - start_time > this->time;
}

bool Channel::operator !=(const Channel& b) const {
    return b.get_main_channel() != get_main_channel() || b.get_second_channel() != get_second_channel();
}

void Channel::apply_changes() {
    esp_err_t res;

    if (on_change_fnct) on_change_fnct();

    // loop through different ht40 options
    switch (second_channel) {
        case -1:
            res = esp_wifi_set_channel(main_channel, WIFI_SECOND_CHAN_BELOW);
            break;
        case 0:
            res = esp_wifi_set_channel(main_channel, WIFI_SECOND_CHAN_NONE);
            break;
        case 1:
            res = esp_wifi_set_channel(main_channel, WIFI_SECOND_CHAN_ABOVE);
            break;
        default:
            res = ESP_ERR_INVALID_ARG;
    }

    if (res != ESP_OK) {
        // error
    }
}