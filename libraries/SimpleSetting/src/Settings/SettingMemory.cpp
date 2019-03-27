#include "SettingMemory.h"

namespace simplesetting {
    SettingMemory::SettingMemory(std::string name, uint32_t default_value, uint32_t step, uint32_t min, uint32_t max, std::string comment) {
        this->name    = name;
        this->comment = comment;

        set_min(min);
        set_max(max);
        set_step(step);
        set_default(default_value);

        reset();
    }

    void SettingMemory::set_min(uint32_t min) {
        this->min = min;
    }

    void SettingMemory::set_max(uint32_t max) {
        this->max = max;
    }

    void SettingMemory::set_step(uint32_t step) {
        this->step = step;
    }

    void SettingMemory::set_default(uint32_t default_value) {
        this->default_value = default_value;
    }

    SettingMemory::~SettingMemory() {}

    uint32_t SettingMemory::get_byte() {
        return this->value;
    }

    uint32_t SettingMemory::get_kb() {
        return this->value / (1000);
    }

    uint32_t SettingMemory::get_mb() {
        return this->value / (1000 * 1000);
    }

    uint32_t SettingMemory::get_gb() {
        return this->value / (1000 * 1000 * 1000);
    }

    uint32_t SettingMemory::get_kib() {
        return this->value / (1024);
    }

    uint32_t SettingMemory::get_mib() {
        return this->value / (1024 * 1024);
    }

    uint32_t SettingMemory::get_gib() {
        return this->value / (1024 * 1024 * 1024);
    }

    uint32_t SettingMemory::get_bit() {
        return get_byte() * 8;
    }

    uint32_t SettingMemory::get_kbit() {
        return get_kb() * 8;
    }

    uint32_t SettingMemory::get_mbit() {
        return get_mb() * 8;
    }

    uint32_t SettingMemory::get_gbit() {
        return get_gb() * 8;
    }

    std::string SettingMemory::get_value() const {
        std::stringstream ss;

        uint32_t mem = value;

        uint32_t gb = mem / (1000 * 1000 * 1000);
        uint32_t mb = (mem / (1000 * 1000)) - (gb * 1000);
        uint32_t kb = (mem / 1000) - (mb * 1000) - (gb * 1000 * 1000);
        uint32_t b  = mem - (kb * 1000) - (mb * 1000 * 1000) - (gb * 1000 * 1000 * 1000);

        if (gb > 0) ss << ' ' << gb << "GB";
        if (mb > 0) ss << ' ' << mb << "MB";
        if (kb > 0) ss << ' ' << kb << "kB";
        if (b > 0) ss << ' ' << b << "b";

        std::string str = ss.str();
        str.erase(0, 1);

        return str;
    }

    bool SettingMemory::set(uint32_t value) {
        if (value < min) value = max;
        if (value > max) value = min;
        this->value = value;
        exec_on_change();

        return true;
    }

    bool SettingMemory::set_value(std::string s) {
        auto endsWith = [](const std::string& a, const std::string& b) -> bool {
                            if (a.length() < b.length()) return false;
                            return a.compare(a.length() - b.length(), b.length(), b) == 0;
                        };

        uint32_t res = 0;

        std::string tmp;

        int len = s.size();

        for (int i = 0; i <= len; ++i) {
            char c = (i < len) ? s.at(i) :  ' ';

            if (c != ' ') {
                tmp += c;
            } else {
                int val = atoi(tmp.c_str());

                if (val != 0) {
                    if (endsWith(tmp, "bit")) val *= 8;
                    else if (endsWith(tmp, "kB") || endsWith(tmp, "KB")) val *= 1000;
                    else if (endsWith(tmp, "KiB")) val *= 1024;
                    else if (endsWith(tmp, "Kbit")) val *= 8 * 1000;
                    else if (endsWith(tmp, "MB")) val *= 1000 * 1000;
                    else if (endsWith(tmp, "MiB")) val *= 1024 * 1024;
                    else if (endsWith(tmp, "Mbit")) val *= 8 * 1000 * 1000;
                    else if (endsWith(tmp, "GB")) val *= 1000 * 1000 * 1000;
                    else if (endsWith(tmp, "GiB")) val *= 1024 * 1024 * 1024;
                    else if (endsWith(tmp, "Gbit")) val *= 8 * 1000 * 1000 * 1000;
                }

                res += val;
                tmp.clear();
            }
        }

        return set(res);
    }

    void SettingMemory::next() {
        up();
    }

    void SettingMemory::up() {
        set(value + step);
    }

    void SettingMemory::down() {
        set(value - step);
    }

    void SettingMemory::reset() {
        set(default_value);
    }
}