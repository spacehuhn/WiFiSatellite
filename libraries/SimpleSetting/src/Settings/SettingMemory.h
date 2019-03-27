#ifndef SimpleSetting_SettingMemory_h
#define SimpleSetting_SettingMemory_h

#include <stdlib.h> // atoi

#include "Setting.h"

namespace simplesetting {
    class SettingMemory : public Setting {
        public:
            SettingMemory(std::string name, uint32_t default_value = 0, uint32_t step = 1, uint32_t min = 0, uint32_t max = UINT32_MAX, std::string comment = "");
            ~SettingMemory();

            virtual void set_min(uint32_t min);
            virtual void set_max(uint32_t max);
            virtual void set_step(uint32_t step);
            virtual void set_default(uint32_t default_value);

            virtual uint32_t get_byte();
            virtual uint32_t get_kb();
            virtual uint32_t get_mb();
            virtual uint32_t get_gb();

            virtual uint32_t get_kib();
            virtual uint32_t get_mib();
            virtual uint32_t get_gib();

            virtual uint32_t get_bit();
            virtual uint32_t get_kbit();
            virtual uint32_t get_mbit();
            virtual uint32_t get_gbit();

            virtual std::string get_value() const;

            virtual bool set(uint32_t value);
            virtual bool set_value(std::string s);

            virtual void next();
            virtual void up();
            virtual void down();
            virtual void reset();

        protected:
            uint32_t min { 0 };
            uint32_t max { UINT32_MAX };
            uint32_t step { 1 };
            uint32_t value { 0 };
            uint32_t default_value { 0 };
    };
}

#endif // ifndef SimpleSetting_SettingMemory_h