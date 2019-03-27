#ifndef SimpleSetting_SettingValue_h
#define SimpleSetting_SettingValue_h

#include <stdlib.h> // atoi

#include "Setting.h"

namespace simplesetting {
    template<typename T>
    class SettingValue : public Setting {
        public:
            SettingValue(std::string name, T default_value, T step, T min, T max, std::string comment = "") {
                this->name    = name;
                this->comment = comment;

                set_default(default_value);
                set_step(step);
                set_min(min);
                set_max(max);

                reset();
            }

            ~SettingValue() {}

            virtual void set_default(T default_value) {
                this->default_value = default_value;
            }

            virtual void set_step(T step) {
                this->step = step;
            }

            virtual void set_min(T min) {
                this->min = min;
            }

            virtual void set_max(T max) {
                this->max = max;
            }

            virtual T get() const {
                return value;
            }

            virtual std::string get_value() const {
                std::stringstream ss;

                ss << get();
                return ss.str();
            }

            virtual bool set(T value) {
                if (value < min) value = max;
                if (value > max) value = min;
                this->value = value;
                exec_on_change();
                return true;
            }

            virtual bool set_value(std::string s) {
                return set(atoi(s.c_str()));
            }

            virtual void next() {
                up();
            }

            virtual void up() {
                set(value + step);
            }

            virtual void down() {
                set(value - step);
            }

            virtual void reset() {
                set(default_value);
            }

        protected:
            T min;
            T max;
            T step;
            T value;
            T default_value;
    };
}

#endif // ifndef SimpleSetting_SettingValue_h