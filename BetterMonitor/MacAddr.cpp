#include "MacAddr.h"

MacAddr::MacAddr() {
    // set_zero(); // already zero by default
}

MacAddr::MacAddr(uint8_t* buffer) {
    set(buffer);
}

MacAddr::MacAddr(std::array<uint8_t, 6> arr) {
    set(arr);
}

MacAddr::~MacAddr() {}

void MacAddr::set(uint8_t* buffer) {
    if (buffer)
        for (uint8_t i = 0; i < 6; i++) this->arr[i] = buffer[i];
    this->valid = !this->is_zero();
}

void MacAddr::set(std::array<uint8_t, 6> arr) {
    for (uint8_t i = 0; i < 6; i++) this->arr[i] = arr.at(i);
    this->valid = !this->is_zero();
}

void MacAddr::set_broadcast() {
    for (uint8_t i = 0; i < 6; i++) this->arr[i] = 0xFF;
    this->valid = true;
}

void MacAddr::set_zero() {
    for (uint8_t i = 0; i < 6; i++) this->arr[i] = 0x00;
    this->valid = false;
}

void MacAddr::copy_in(uint8_t* buffer) const {
    for (uint8_t i = 0; i < 6; i++) buffer[i] = this->arr[i];
}

bool MacAddr::equals(const MacAddr& mac) const {
    return memcmp(mac.arr, this->arr, 6) == 0;
}

int MacAddr::compare(const MacAddr& mac) const {
    return memcmp(this->arr, mac.arr, 6);
}

bool MacAddr::is_zero() const {
    for (uint8_t i = 0; i<6; ++i) if (arr[i] != 0x00) return false;
    return true;
}

bool MacAddr::is_broadcast() const {
    for (uint8_t i = 0; i<6; ++i) if (arr[i] != 0xFF) return false;
    return true;
}

bool MacAddr::is_group() const {
    return arr[0] & 0x01;
}

bool MacAddr::is_multicast() const {
    return !is_broadcast() && is_group();
}

std::string MacAddr::to_string() const {
    std::stringstream ss;

    for (uint8_t i = 0; i < 6; ++i) {
        ss << std::setfill('0') << std::setw(2) << std::hex << +arr[i];
        if (i < 5) ss << ':';
    }

    return ss.str();
}

bool MacAddr::operator==(const MacAddr& mac) const {
    return this->equals(mac);
}

bool MacAddr::operator!=(const MacAddr& mac)  const {
    return !this->equals(mac);
}

bool MacAddr::operator>(const MacAddr& mac) const {
    return this->compare(mac) > 0;
}

bool MacAddr::operator<(const MacAddr& mac) const {
    return this->compare(mac) < 0;
}

MacAddr::operator bool() const {
    return this->valid;
}

std::ostream& operator << (std::ostream& os, const MacAddr& mac) {
    os << mac.to_string();
    return os;
}