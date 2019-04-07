#ifndef SimpleSetting_SettingConst_h
#define SimpleSetting_SettingConst_h

#include "Setting.h"

namespace simplesetting {
    template<typename T>
    class SettingConst : public Setting {
        public:
            SettingConst(std::string name, T value, std::string comment = "") {
                this->name    = name;
                this->value   = value;
                this->comment = comment;
            }

            ~SettingConst() {}

            virtual T get() const {
                return value;
            }

            virtual std::string get_value() const {
                std::stringstream ss;

                ss << get();
                return ss.str();
            }

            virtual bool set_value(std::string s) {
                return false;
            }

            virtual void next() {}

            virtual void up() {}

            virtual void down() {}

            virtual void reset() {}

        protected:
            T value;
    };
}

#endif // ifndef SimpleSetting_SettingConst_hs