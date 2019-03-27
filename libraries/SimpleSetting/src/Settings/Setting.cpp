#include "Setting.h"

namespace simplesetting {
    void Setting::set_on_change(std::function<void(Setting*)> on_change) {
        this->on_change = on_change;
    }

    const std::string& Setting::get_name() const {
        return this->name;
    }

    const std::string& Setting::get_comment() const {
        return this->comment;
    }

    std::string Setting::to_ini() const {
        std::stringstream ss;

        ss << get_name() << '=' << get_value();
        if (comment.size() > 0) ss << ';' << ' ' << get_comment();

        return ss.str();
    }

    std::string Setting::to_json_arr() const {
        std::stringstream ss;

        ss << '[';
        ss << '"' << get_name() << '"' << ',';
        ss << '"' << get_value() << '"' << ',';
        ss << '"' << get_comment() << '"';
        ss << ']';

        return ss.str();
    }

    std::string Setting::to_json_obj() const {
        std::stringstream ss;

        ss << '{';
        ss << '"' << "name" << '"' << ':' << '"' << get_name() << '"' << ',';
        ss << '"' << "value" << '"' << ':' << '"' << get_value() << '"' << ',';
        ss << '"' << "comment" << '"' << ':' << '"' << get_comment() << '"';
        ss << '}';

        return ss.str();
    }

    bool Setting::equals(const Setting& setting) const {
        return this->name.compare(setting.get_name()) == 0;
    }

    bool Setting::equals(std::string name) const {
        return this->name.compare(name) == 0;
    }

    bool Setting::operator==(const Setting& setting) const {
        return this->equals(setting);
    }

    bool Setting::operator==(std::string name) const {
        return this->equals(name);
    }

    void Setting::exec_on_change() {
        if (on_change) on_change(this);
    }

    std::ostream& operator<<(std::ostream& os, const Setting& setting) {
        os << setting.to_ini();
    }
}