#ifndef MacAddr_h
#define MacAddr_h

#include <cstring>   // memcmp
#include <cinttypes> // uint8_t
#include <array>     // std::array
#include <sstream>   // std::stringstream
#include <string>    // std::string
#include <iomanip>   // std::setfill, std::setw, std::hex

class MacAddr {
    public:
        MacAddr();
        MacAddr(uint8_t * buffer);
        MacAddr(std::array<uint8_t, 6> arr);

        ~MacAddr();

        void set(uint8_t* buffer);
        void set(std::array<uint8_t, 6> arr);
        void set_broadcast();
        void set_zero();

        void copy_in(uint8_t* buffer) const;

        bool equals(const MacAddr& mac) const;
        int compare(const MacAddr& mac) const;

        bool is_zero() const;
        bool is_broadcast() const;
        bool is_group() const;
        bool is_multicast() const;

        std::string to_string() const;

        bool operator==(const MacAddr& mac) const;
        bool operator!=(const MacAddr& mac) const;
        bool operator>(const MacAddr& mac) const;
        bool operator<(const MacAddr& mac) const;
        operator bool() const;

    private:
        uint8_t arr[6] { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
        bool    valid = false;
};

std::ostream& operator << (std::ostream& os, const MacAddr& mac);

#endif /* ifndef MacAddr_h */