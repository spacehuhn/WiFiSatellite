#ifndef SimpleSetting_SimpleSetting_h
#define SimpleSetting_SimpleSetting_h

#define SIMPLE_SETTING_VERSION_MAJOR 0
#define SIMPLE_SETTING_VERSION_MINOR 1

#include <list>    // list
#include <string>  // string
#include <sstream> // string stream

#include "Settings/Setting.h"
#include "Section/Section.h"

#include "Settings/SettingFlag.h"
#include "Settings/SettingMemory.h"
#include "Settings/SettingMode.h"
#include "Settings/SettingConst.h"
#include "Settings/SettingValue.h"
#include "Settings/SettingTime.h"
#include "Settings/SettingString.h"

namespace simplesetting {
    class SimpleSetting {
        public:
            SimpleSetting();
            ~SimpleSetting();

            void add(Section* s);
            void add(Setting* s);

            Setting* get(std::string name) const;
            Section* get_section(std::string name) const;

            bool set(std::string name, std::string value);

            std::string to_ini() const;
            std::string to_json_arr() const;
            std::string to_json_obj() const;

            void parse(const std::string& s);

            const std::list<Section*>& get_section_list() const;

        protected:
            std::list<Section*> sl {};
    };

    std::ostream& operator<<(std::ostream& os, const SimpleSetting& settings);
}

#endif /* ifndef SimpleSetting_SimpleSetting_h */