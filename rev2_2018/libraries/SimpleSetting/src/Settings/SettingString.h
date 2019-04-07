#ifndef SimpleSetting_SettingString_h
#define SimpleSetting_SettingString_h

#include "Setting.h"

namespace simplesetting {
    class SettingString : public Setting {
        public:
            SettingString(std::string name, std::string default_value, std::string comment = "");
            ~SettingString();

            virtual std::string get_value() const;
            virtual bool set_value(std::string s);

            virtual void next();
            virtual void up();
            virtual void down();
            virtual void reset();

            virtual void add(char c);
            virtual void add(std::string s);

            virtual void remove(int n = 1);

        protected:
            std::string value { "" };
            std::string default_value { "" };
    };
}

#endif // ifndef SimpleSetting_SettingString_h