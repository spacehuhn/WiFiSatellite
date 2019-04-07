#ifndef SimpleSetting_Section_h
#define SimpleSetting_Section_h

#include <list>
#include <string>
#include <sstream>

#include "Settings/Setting.h"

namespace simplesetting {
    class Section {
        public:
            Section(std::string name, std::string comment = "");
            ~Section();

            const std::string& get_name() const;
            const std::string& get_comment() const;

            void add(Setting* s);

            Setting* get(std::string name) const;

            bool set(std::string name, std::string value);

            std::string to_ini() const;
            std::string to_json_arr() const;
            std::string to_json_obj() const;

            const std::list<Setting*>& get_setting_list();

            virtual bool equals(const Section& section) const;
            virtual bool equals(std::string name) const;

            bool operator==(const Section& section);
            bool operator==(std::string name);

        protected:
            std::list<Setting*> sl {};
            std::string name { "" };
            std::string comment { "" };
    };

    std::ostream& operator<<(std::ostream& os, const Section& section);
}

#endif /* ifndef SimpleSetting_Section_h */