#include "EntryCustom.h"

namespace simplemenu {
    EntryCustom::EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        this->draw_fnct = draw_fnct;
        set_config(config);
        set_on_click(m, event_fnct);
    }

    EntryCustom::EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        this->draw_fnct = draw_fnct;
        set_config(config);
        set_on_click(m, event_fnct);
    }

    EntryCustom::EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, Event* click, Event* doubleclick, Event* hold) {
        this->draw_fnct = draw_fnct;
        set_config(config);
        set_on_click(click);
        set_on_doubleclick(doubleclick);
        set_on_hold(hold);
    }

    EntryCustom::~EntryCustom() {}

    bool EntryCustom::draw(unsigned int x, unsigned int y, bool selected) {
        if (this->draw_fnct && !hidden) {
            this->draw_fnct(x, y, selected);
            return true;
        }
        return false;
    }

    std::string EntryCustom::get_name() const {
        return std::string();
    }
}