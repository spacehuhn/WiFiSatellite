#ifndef Channel_h
#define Channel_h

#include <stdint.h>   // uint8_t, int8_t
#include <string>     // std::string
#include <sstream>    // std::stringstream
#include <functional> // std::function

#include <esp_err.h>  // esp_err_t
#include <esp_wifi.h> // esp_wifi_set_channel


#define CHANNEL_ON_CHANGE_FNCT std::function<void()> on_change_fnct

class Channel {
    public:
        Channel(int channel = 1, int second = 0, unsigned long time = 300);
        ~Channel();

        void set_on_changed(CHANNEL_ON_CHANGE_FNCT);
        int get_main_channel() const;
        int get_second_channel() const;
        int get_mode() const;
        std::string get_bandwidth() const;
        std::string to_str() const;

        void set(int channel, int mode);
        void set_time(unsigned long time);

        void start(const Channel& prev_ch, unsigned long current_time);
        bool next(unsigned long current_time);

        bool operator !=(const Channel& b) const;

    private:
        uint8_t main_channel { 1 };     // Channel number 1 - 14
        int8_t  second_channel { 0 };   // Second channel 0 = none, >0 = above, <0 = below
        unsigned long time { 300 };     // channel time in ms

        unsigned long start_time { 0 }; // time channel was set

        CHANNEL_ON_CHANGE_FNCT { nullptr };

        void apply_changes();
};

#endif /* ifndef Channel_h */
