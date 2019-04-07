#include "Event.h"

namespace simplemenu {
    Event::Event(Menu* m) {
        this->menu = m;
    }

    Event::Event(SIMPLEMENU_EVENT_FNCT) {
        this->event_fnct = event_fnct;
    }

    Event::Event(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        this->menu       = m;
        this->event_fnct = event_fnct;
    }

    Event::~Event() {}

    Menu* Event::get_menu() const {
        return this->menu;
    }

    void Event::set_menu(Menu* m) {
        this->menu = m;
    }

    void Event::set_fnct(SIMPLEMENU_EVENT_FNCT) {
        this->event_fnct = event_fnct;
    }

    Menu* Event::run(Menu* currentMenu) {
        if (event_fnct) return event_fnct(currentMenu, get_menu());

        return get_menu();
    }
}