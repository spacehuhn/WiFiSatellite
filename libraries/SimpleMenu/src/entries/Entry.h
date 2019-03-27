#ifndef SimpleMenu_Entry_h
#define SimpleMenu_Entry_h

#include <string>    // std::string

#include <Arduino.h> // millis()

#include "events/Event.h"
#include "configs/Config.h"

#define ENTRY_STILL_TIME 2000
#define ENTRY_SCROLL_SPEED 350

namespace simplemenu {
    class Menu; // let's imagine we know Menu already :D
    class Entry {
        public:
            virtual ~Entry();

			virtual void set_config(Config* config);

            virtual bool draw(unsigned int row, bool selected);
            virtual bool draw(unsigned int x, unsigned int y, bool selected) = 0;
            virtual void reset();

            virtual bool is_hidden() const;
            virtual void hide();
            virtual void unhide();

            virtual std::string get_name() const = 0;
            virtual std::string get_name(unsigned int max_len);
            virtual std::string get_name(unsigned int max_len, bool scrolling);

            virtual Event* get_on_clicked() const;
            virtual Event* get_on_doubleclicked() const;
            virtual Event* get_on_holding() const;

            virtual void set_on_click(Event* e);
            virtual void set_on_click(Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual void set_on_click(SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);

            virtual void set_on_doubleclick(Event* e);
            virtual void set_on_doubleclick(Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual void set_on_doubleclick(SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);

            virtual void set_on_hold(Event* e);
            virtual void set_on_hold(Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual void set_on_hold(SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);

        protected:
            Event* click_event       { nullptr };
            Event* doubleclick_event { nullptr };
            Event* hold_event        { nullptr };

            Config* config { nullptr };

            uint16_t char_index { 0 };

            unsigned long still_time  { 0 };
            unsigned long scroll_time { 0 };

            bool hidden { false };
    };
}

#endif // ifndef SimpleMenu_Entry_h