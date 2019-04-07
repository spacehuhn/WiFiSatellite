#include "SettingString.h"

namespace simplesetting {
    SettingString::SettingString(std::string name, std::string default_value, std::string comment) {
        this->name    = name;
        this->comment = comment;

        this->default_value = default_value;

        reset();
    }

    SettingString::~SettingString() {}

    std::string SettingString::get_value() const {
        return value;
    }

    bool SettingString::set_value(std::string s) {
        this->value = s;
        exec_on_change();
        return true;
    }

    void SettingString::next() {}

    void SettingString::up() {}

    void SettingString::down() {}

    void SettingString::reset() {
        set_value(default_value);
    }

    void SettingString::add(char c) {
        set_value(this->value + c);
    }

    void SettingString::add(std::string s) {
        set_value(this->value + s);
    }

    void SettingString::remove(int n) {
        std::string tmp = this->value;

        tmp.erase(0, value.size() - n);
        set_value(tmp);
    }
}