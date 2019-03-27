#ifndef SimpleMenu_SimpleMenu_h
#define SimpleMenu_SimpleMenu_h

#include "menus/MenuText.h"
#include "menus/MenuCustom.h"

namespace simplemenu {
    class SimpleMenu {
        public:
            SimpleMenu();
            SimpleMenu(Menu* main_menu);

            virtual ~SimpleMenu();

            virtual bool update();

            virtual void set_main_menu(Menu* m);
            virtual void set_menu(Menu* m);
            virtual void set_display_timeout(unsigned long display_timeout);
            virtual void set_cursor_timeout(unsigned long cursor_timeout);

            virtual Config* get_config();
            virtual Menu* get_menu();
            virtual Menu* get_main_menu();

            virtual void up();
            virtual void down();
            virtual void click();
            virtual void doubleclick();
            virtual void hold();
            virtual void goBack();

            virtual unsigned int get_fps();
            virtual bool get_cursor();

        protected:
            Menu* main_menu    { nullptr };
            Menu* current_menu { nullptr };

            unsigned long update_time     { 0 };
            unsigned long update_interval { 0 };
            unsigned long frame_time      { 0 };

            uint16_t frames     { 0 };
            uint16_t tmp_frames { 0 };

            Config* config { nullptr };

            // turn display off after given amount of time
            unsigned long display_timeout { 300000 };
            unsigned long display_time    { 0 };

            // hide cursor after given amount of time
            unsigned long cursor_timeout  { 30000 };
            unsigned long cursor_time     { 0 };
    };
}

#endif // ifndef SimpleMenu_SimpleMenu_h