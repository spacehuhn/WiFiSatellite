#include "SettingFlag.h"

namespace simplesetting {
    SettingFlag::SettingFlag(std::string name, bool default_value, std::string comment) {
        this->name          = name;
        this->value         = default_value;
        this->default_value = default_value;
        this->comment       = comment;
    }

    SettingFlag::~SettingFlag() {}

    bool SettingFlag::get() const {
        return value;
    }

    std::string SettingFlag::get_value() const {
        return value ? "true" : "false";
    }

    bool SettingFlag::set(bool value) {
        this->value = value;
        exec_on_change();
        return true;
    }

    bool SettingFlag::set_value(std::string s) {
        return set(!(s == "false" || s == "FALSE" || s == "0"));
    }

    void SettingFlag::next() {
        set(!value);
    }

    void SettingFlag::up() {
        next();
    }

    void SettingFlag::down() {
        next();
    }

    void SettingFlag::reset() {
        value = default_value;
    }
}