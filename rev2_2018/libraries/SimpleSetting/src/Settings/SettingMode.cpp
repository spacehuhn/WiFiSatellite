#include "SettingMode.h"

namespace simplesetting {
    SettingMode::SettingMode(std::string name, std::vector<std::string> values, std::string comment) {
        this->name    = name;
        this->comment = comment;
        this->values  = values;
    }

    SettingMode::~SettingMode() {}

    unsigned int SettingMode::get() const {
        return value_index;
    }

    std::string SettingMode::get_value() const {
        return values.at(value_index);
    }

    bool SettingMode::set(int value) {
        if (value < 0) value = values.size() - 1;
        else if (value > values.size() - 1) value = 0;

        this->value_index = value;
        exec_on_change();

        return true;
    }

    bool SettingMode::set_value(std::string s) {
        for (int i = 0; i<values.size(); i++) {
            if (values.at(i) == s) return set(i);
        }

        return false;
    }

    void SettingMode::next() {
        up();
    }

    void SettingMode::up() {
        set(value_index + 1);
    }

    void SettingMode::down() {
        set(value_index - 1);
    }

    void SettingMode::reset() {
        value_index = 0;
    }
}