#include "Section.h"

namespace simplesetting {
    Section::Section(std::string name, std::string comment) {
        this->name    = name;
        this->comment = comment;
    }

    Section::~Section() {
        sl.remove_if([](Setting* s) {
            delete s; return true;
        });
    }

    const std::string& Section::get_name() const {
        return this->name;
    }

    const std::string& Section::get_comment() const {
        return this->comment;
    }

    void Section::add(Setting* s) {
        if (!get(s->get_name())) sl.push_back(s);
    }

    Setting* Section::get(std::string name) const {
        for (auto it = sl.begin(); it != sl.end(); ++it) {
            if ((*it)->equals(name)) return *it;
        }

        return nullptr;
    }

    bool Section::set(std::string name, std::string value) {
        Setting* s = get(name);

        return s ? s->set_value(value) : false;
    }

    std::string Section::to_ini() const {
        std::stringstream ss;

        ss << '[' << get_name() << ']';
        if (comment.size() > 0) ss << ';' << ' ' << get_comment();
        ss << '\r' << '\n';

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            ss << (*it)->to_ini();
            ss << '\r' << '\n';
        }

        return ss.str();
    }

    std::string Section::to_json_arr() const {
        std::stringstream ss;

        ss << '[';
        ss << '"' << get_name() << '"' << ',';
        ss << '"' << get_comment() << '"' << ',';
        ss << '[';

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            if (it != sl.begin()) ss << ',';
            ss << (*it)->to_json_arr();
        }

        ss << ']';
        ss << ']';

        return ss.str();
    }

    std::string Section::to_json_obj() const {
        std::stringstream ss;

        ss << '{';
        ss << '"' << "name" << '"' << ':' << '"' << get_name() << '"' << ',';
        ss << '"' << "comment" << '"' << ':' << '"' << get_comment() << '"' << ',';
        ss << '"' << "settings" << '"' << ':' << '[';

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            if (it != sl.begin()) ss << ',';
            ss << (*it)->to_json_obj();
        }

        ss << ']';
        ss << '}';

        return ss.str();
    }

    const std::list<Setting*>& Section::get_setting_list() {
        return this->sl;
    }

    bool Section::equals(const Section& section) const {
        return this->name.compare(section.get_name()) == 0;
    }

    bool Section::equals(std::string name) const {
        return this->name.compare(name) == 0;
    }

    bool Section::operator==(const Section& section) {
        return this->equals(section);
    }

    bool Section::operator==(std::string name) {
        return this->equals(name);
    }

    std::ostream& operator<<(std::ostream& os, const Section& section) {
        os << section.to_ini();
    }
}