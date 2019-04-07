#include "SettingTime.h"

namespace simplesetting {
    SettingTime::SettingTime(std::string name, unsigned long default_value, unsigned long step, unsigned long min, unsigned long max, std::string comment) {
        this->name    = name;
        this->comment = comment;

        set_default(default_value);
        set_step(step);
        set_min(min);
        set_max(max);

        reset();
    }

    SettingTime::~SettingTime() {}

    void SettingTime::set_default(unsigned long default_value) {
        this->default_value = default_value;
    }

    void SettingTime::set_step(unsigned long step) {
        this->step = step;
    }

    void SettingTime::set_min(unsigned long min) {
        this->min = min;
    }

    void SettingTime::set_max(unsigned long max) {
        this->max = max;
    }

    unsigned long SettingTime::get_milli() const {
        return value;
    }

    unsigned long SettingTime::get_second() const {
        return value / 1000;
    }

    unsigned long SettingTime::get_minute() const {
        return get_second() / 60;
    }

    unsigned long SettingTime::get_hour() const {
        return get_minute() / 60;
    }

    std::string SettingTime::get_value() const {
        unsigned long time = value;

        unsigned long hours        = time / (1000 * 60 * 60);
        unsigned long minutes      = (time / (1000 * 60)) - (hours * 60);
        unsigned long seconds      = (time / 1000) - (hours * 60 * 60) - (minutes * 60);
        unsigned long milliseconds = time - (hours * 60 * 60 * 1000) - (minutes * 60 * 1000) - (seconds * 1000);

        std::stringstream ss;

        if (hours > 0) ss << ' ' << hours << 'h';
        if (minutes > 0) ss << ' ' << minutes << 'm';
        if (seconds > 0) ss << ' ' << seconds << 's';
        if (milliseconds > 0) ss << ' ' << milliseconds << "ms";

        std::string str = ss.str();
        str.erase(0, 1);

        return str;
    }

    bool SettingTime::set(unsigned long value) {
        if (value < min) value = max;
        if (value > max) value = min;
        this->value = value;
        exec_on_change();
        return true;
    }

    bool SettingTime::set_value(std::string s) {
        unsigned long res = 0;

        std::string tmp;

        int  len = s.size();
        char c;

        for (int i = 0; i <= len; ++i) {
            char c = (i < len) ? s.at(i) : ' ';

            if (c != ' ') {
                tmp += c;
            } else {
                int val = atoi(tmp.c_str());

                if (val != 0) {
                    char ending = tmp.at(tmp.find_last_not_of(' '));

                    switch (ending) {
                        case 'h':
                            val *= 60;
                        case 'm':
                            val *= 60;
                            break;
                        case 's':
                            val *= 1000;
                    }
                }

                res += val;
                tmp.clear();
            }
        }

        return set(res);
    }

    void SettingTime::next() {
        up();
    }

    void SettingTime::up() {
        set(get_next_val());
    }

    void SettingTime::down() {
        set(get_prev_val());
    }

    void SettingTime::reset() {
        set(default_value);
    }

    unsigned long SettingTime::get_next_val() {
        if (step != 0) return value + step;

        unsigned long tmpValue = value;

        if (tmpValue >= 345600000) tmpValue = 604800000;       // 4d -> 1w
        else if (tmpValue >= 172800000) tmpValue = 345600000;  // 2d -> 4d
        else if (tmpValue >= 86400000) tmpValue = 172800000;   // 1d -> 2d

        else if (tmpValue >= 43200000) tmpValue = 86400000;    // 12h -> 24h
        else if (tmpValue >= 21600000) tmpValue = 43200000;    // 6h -> 12h
        else if (tmpValue >= 7200000) tmpValue = 21600000;     // 2h -> 6h
        else if (tmpValue >= 3600000) tmpValue = 7200000;      // 1h -> 2h

        else if (tmpValue >= 1800000) tmpValue = 3600000;      // 30m -> 1h
        else if (tmpValue >= 1200000) tmpValue = 1800000;      // 20m -> 30m
        else if (tmpValue >= 600000) tmpValue = 1200000;       // 10m -> 20m
        else if (tmpValue >= 300000) tmpValue = 600000;        // 5m -> 10m
        else if (tmpValue >= 120000) tmpValue = 300000;        // 2m -> 5m
        else if (tmpValue >= 60000) tmpValue = 120000;         // 1m -> 2m

        else if (tmpValue >= 30000) tmpValue = 60000;          // 30s -> 1m
        else if (tmpValue >= 20000) tmpValue = 30000;          // 20s -> 30s
        else if (tmpValue >= 10000) tmpValue = 20000;          // 10s -> 20s
        else if (tmpValue >= 5000) tmpValue = 10000;           // 5s -> 10s
        else if (tmpValue >= 2000) tmpValue = 5000;            // 2s -> 5s
        else if (tmpValue >= 1000) tmpValue = 2000;            // 1s -> 2s

        else if (tmpValue >= 500) tmpValue = 1000;             // 500ms -> 1s
        else if (tmpValue >= 200) tmpValue = 500;              // 200ms -> 500ms
        else if (tmpValue >= 100) tmpValue = 200;              // 100ms -> 200ms
        else if (tmpValue >= 50) tmpValue = 100;               // 50ms -> 100ms
        else if (tmpValue >= 20) tmpValue = 50;                // 20ms -> 50ms
        else if (tmpValue >= 10) tmpValue = 20;                // 10ms -> 20ms
        else if (tmpValue >= 5) tmpValue = 10;                 // 5ms -> 10ms
        else if (tmpValue >= 2) tmpValue = 5;                  // 2ms -> 5ms
        else if (tmpValue >= 1) tmpValue = 2;                  // 1ms -> 2ms
        else tmpValue = 1;                                     // 1ms

        return tmpValue;
    }

    unsigned long SettingTime::get_prev_val() {
        if (step != 0) return value - step;

        unsigned long tmpValue = value;

        if (tmpValue > 345600000) tmpValue = 345600000;       // 1w -> 4d
        else if (tmpValue > 172800000) tmpValue = 172800000;  // 4d -> 2d
        else if (tmpValue > 86400000) tmpValue = 86400000;    // 2d -> 1d

        else if (tmpValue > 43200000) tmpValue = 43200000;    // 24h -> 12h
        else if (tmpValue > 21600000) tmpValue = 21600000;    // 12h -> 6h
        else if (tmpValue > 7200000) tmpValue = 7200000;      // 6h -> 2h
        else if (tmpValue > 3600000) tmpValue = 3600000;      // 2h -> 1h

        else if (tmpValue > 1800000) tmpValue = 1800000;      // 1h -> 30m
        else if (tmpValue > 1200000) tmpValue = 1200000;      // 30m -> 20m
        else if (tmpValue > 600000) tmpValue = 600000;        // 20m -> 10m
        else if (tmpValue > 300000) tmpValue = 300000;        // 10m -> 5m
        else if (tmpValue > 120000) tmpValue = 120000;        // 5m -> 2m
        else if (tmpValue > 60000) tmpValue = 60000;          // 2m -> 1m

        else if (tmpValue > 30000) tmpValue = 30000;          // 1m -> 30s
        else if (tmpValue > 20000) tmpValue = 20000;          // 30s -> 20s
        else if (tmpValue > 10000) tmpValue = 10000;          // 20s -> 10s
        else if (tmpValue > 5000) tmpValue = 5000;            // 10s -> 5s
        else if (tmpValue > 2000) tmpValue = 2000;            // 5s -> 2s
        else if (tmpValue > 1000) tmpValue = 1000;            // 2s -> 1s

        else if (tmpValue > 500) tmpValue = 500;              // 1s -> 500ms
        else if (tmpValue > 200) tmpValue = 200;              // 500ms -> 200ms
        else if (tmpValue > 100) tmpValue = 100;              // 200ms -> 100ms
        else if (tmpValue > 50) tmpValue = 50;                // 100ms -> 50ms
        else if (tmpValue > 20) tmpValue = 20;                // 50ms -> 20ms
        else if (tmpValue > 10) tmpValue = 10;                // 20ms -> 10ms
        else if (tmpValue > 5) tmpValue = 5;                  // 10ms -> 5ms
        else if (tmpValue > 2) tmpValue = 2;                  // 5ms -> 2ms
        else tmpValue = 1;                                    // 2ms -> 1ms

        return tmpValue;
    }
}