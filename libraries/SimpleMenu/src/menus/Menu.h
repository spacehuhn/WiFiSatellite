#ifndef SimpleMenu_Menu_h
#define SimpleMenu_Menu_h

#include "configs/ConfigCustom.h"

#include <Arduino.h> // millis()

namespace simplemenu {
    class Menu {
        public:
            virtual ~Menu() = default;

            virtual Config* get_config() const;
            virtual void set_config(Config* config);

            void set_name(std::string name);
            virtual std::string get_name();

            virtual bool has_parent();
            virtual Menu* get_parent()  const;
            virtual void set_parent_menu(Menu* parent);

            virtual void set_fps(unsigned int fps);
            virtual unsigned int get_fps() const;

            virtual bool ready() const;
            virtual void update();

            virtual bool draw(bool cursor) = 0;
            virtual void reset()           = 0;

            virtual Menu* up();
            virtual Menu* down();
            virtual Menu* click();
            virtual Menu* doubleclick();
            virtual Menu* hold();

        protected:
            std::string name {};

            Config* config { nullptr };
            Menu* parent   { nullptr };

            unsigned int fps         { 30 };
            unsigned long update_time { 0 };
    };
}

#endif // ifndef SimpleMenu_Menu_h