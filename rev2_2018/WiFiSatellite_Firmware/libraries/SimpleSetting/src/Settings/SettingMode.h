#ifndef SimpleSetting_SettingMode_h
#define SimpleSetting_SettingMode_h

#include <vector>

#include "Setting.h"

namespace simplesetting {
    class SettingMode : public Setting {
        public:
            SettingMode(std::string name, std::vector<std::string> values, std::string comment = "");
            ~SettingMode();

            virtual unsigned int get() const;
            virtual std::string get_value() const;

            virtual bool set(int value);
            virtual bool set_value(std::string s);

            virtual void next();
            virtual void up();
            virtual void down();
            virtual void reset();

        protected:
            std::vector<std::string> values {};

            unsigned int value_index { 0 };
    };
}

#endif // ifndef SimpleSetting_SettingMode_h