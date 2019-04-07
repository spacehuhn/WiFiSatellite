#include "SimpleSetting.h"

namespace simplesetting {
    SimpleSetting::SimpleSetting()  {
        sl.push_back(new Section("default"));
    }

    SimpleSetting::~SimpleSetting() {
        sl.remove_if([](Section* s) {
            delete s; return true;
        });
    }

    void SimpleSetting::add(Section* s) {
        sl.push_back(s);
    }

    void SimpleSetting::add(Setting* s) {
        sl.front()->add(s);
    }

    Setting* SimpleSetting::get(std::string name) const {
        for (auto it = sl.begin(); it != sl.end(); ++it) {
            Setting* setting = (*it)->get(name);
            if (setting) return setting;
        }

        return nullptr;
    }

    Section* SimpleSetting::get_section(std::string name) const {
        for (auto it = sl.begin(); it != sl.end(); ++it) {
            Section* section = *it;
            if (*section == name) return section;
        }

        return NULL;
    }

    bool SimpleSetting::set(std::string name, std::string value) {
        Setting* s = get(name);

        return s ? s->set_value(value) : false;
    }

    std::string SimpleSetting::to_ini() const {
        std::stringstream ss;

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            auto settings_list = (*it)->get_setting_list();
            if (settings_list.size() > 0) ss << (*it)->to_ini() << '\r' << '\n';
        }

        return ss.str();
    }

    std::string SimpleSetting::to_json_arr() const {
        std::stringstream ss;

        ss << '[';

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            if (it != sl.begin()) ss << ',';
            ss << (*it)->to_json_arr();
        }

        ss << ']';

        return ss.str();
    }

    std::string SimpleSetting::to_json_obj() const {
        std::stringstream ss;

        ss << '{' << '"' << "sections" << ':' << '[';

        for (auto it = sl.begin(); it != sl.end(); ++it) {
            if (it != sl.begin()) ss << ',';
            ss << (*it)->to_json_obj();
        }

        ss << ']' << '}';

        return ss.str();
    }

    void SimpleSetting::parse(const std::string& s) {
        std::string tmp;

        Setting* setting = nullptr;

        auto trim = [](std::string& str) {
                        str.erase(0, str.find_first_not_of(" \n\r\t")); // remove leading spaces
                        str.erase(str.find_last_not_of(" \n\r\t")+1);   // remove tailing spaces
                    };

        for (size_t i = 0; i <= s.size(); i++) {
            char c = i < s.size() ? s.at(i) : '\n';

            // setting variable
            if (c == '=') {
                trim(tmp);          // remove spaces
                setting = get(tmp); // get setting from list
                tmp.clear();        // reset string
            }

            // end of line
            else if ((c == '\r') || (c == '\n')) {
                // when setting and value was found
                if (setting && (tmp.length() > 0)) {
                    trim(tmp); // remove spaces

                    // get position of quotes
                    int quoteA    = tmp.find_first_of('"');
                    int quoteB    = tmp.find_last_of('"');
                    int semicolon = tmp.find_first_of(';');

                    // find the first semicolon after quotes
                    while (semicolon >= 0 && quoteB > semicolon) {
                        semicolon = tmp.find_first_of(';', quoteB + 1);
                    }

                    // remove comment
                    if (quoteA < semicolon) {
                        tmp.erase(semicolon);
                    }

                    // remove surrounding quotes if existing
                    if ((quoteA < quoteB) && (quoteA == 0) && (quoteB == tmp.length() - 1)) {
                        tmp.erase(quoteA, 1);
                        tmp.erase(quoteB, 1);
                    }

                    setting->set_value(tmp);
                }

                tmp.clear();
                setting = nullptr;
            }

            // add to string
            else tmp += c;
        }
    }

    const std::list<Section*>& SimpleSetting::get_section_list() const {
        return this->sl;
    }

    std::ostream& operator<<(std::ostream& os, const SimpleSetting& settings) {
        os << settings.to_ini();
    }
}