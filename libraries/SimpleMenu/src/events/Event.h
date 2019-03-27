#ifndef SimpleMenu_Event_h
#define SimpleMenu_Event_h

#include <functional> // std::function

#define SIMPLEMENU_EVENT_FNCT std::function<Menu*(Menu* current, Menu* next)> event_fnct

namespace simplemenu {
    class Menu; // let's imagine we know Menu already :D
    class Event {
        public:
            Event(Menu* m);
            Event(SIMPLEMENU_EVENT_FNCT);
            Event(Menu* m, SIMPLEMENU_EVENT_FNCT);

            virtual ~Event();

            virtual Menu* get_menu() const;
            virtual void set_menu(Menu* m);

            virtual void set_fnct(SIMPLEMENU_EVENT_FNCT);

            virtual Menu* run(Menu* currentMenu);

        protected:
            Menu* menu            { nullptr };
            SIMPLEMENU_EVENT_FNCT { nullptr };
    };
}

#endif // ifndef SimpleMenu_Event_h