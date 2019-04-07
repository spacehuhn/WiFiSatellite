#ifndef SimpleSetting_SettingFlag_h
#define SimpleSetting_SettingFlag_h

#include "Setting.h"

namespace simplesetting {
    class SettingFlag : public Setting {
        public:
            SettingFlag(std::string name, bool default_value = false, std::string comment = "");
            ~SettingFlag();

            virtual bool get() const;
            virtual std::string get_value() const;

            virtual bool set(bool value);
            virtual bool set_value(std::string value);

            virtual void next();
            virtual void up();
            virtual void down();
            virtual void reset();

        protected:
            bool default_value { false };
            bool value         { false };
    };
}

#endif // ifndef SimpleSetting_SettingFlag_h