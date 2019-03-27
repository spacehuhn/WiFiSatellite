#ifndef SimpleSetting_SettingTime_h
#define SimpleSetting_SettingTime_h

#include <stdlib.h> // atoi
#include <limits.h>   // ULONG_MAX

#include "Setting.h"

namespace simplesetting {
    class SettingTime: public Setting {
        public:
            SettingTime(std::string name, unsigned long default_value, unsigned long step = 0, unsigned long min = 0, unsigned long max = ULONG_MAX, std::string comment = "");
            ~SettingTime();

            virtual void set_default(unsigned long default_value);
            virtual void set_step(unsigned long step);
            virtual void set_min(unsigned long min);
            virtual void set_max(unsigned long max);

            virtual unsigned long get_milli() const;
            virtual unsigned long get_second() const;
            virtual unsigned long get_minute() const;
            virtual unsigned long get_hour() const;

            virtual std::string get_value() const;

            virtual bool set(unsigned long value);
            virtual bool set_value(std::string s);

            virtual void next();
            virtual void up();
            virtual void down();
            virtual void reset();

        protected:
            unsigned long get_next_val();
            unsigned long get_prev_val();

            unsigned long step { 0 };

            unsigned long min           { 0 };
            unsigned long max           { ULONG_MAX };
            unsigned long value         { 0 };
            unsigned long default_value { 0 };
    };
}

#endif // ifndef SimpleSetting_SettingTime_h