#include "EntryStatic.h"

namespace simplemenu {
    EntryStatic::EntryStatic(Config* config, std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        set_config(config);
        set_name(name);
        set_on_click(new Event(m, event_fnct));
    }

    EntryStatic::EntryStatic(Config* config, std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        set_config(config);
        set_name(name);
        set_on_click(new Event(m, event_fnct));
    }

    EntryStatic::EntryStatic(Config* config, std::string name, Event* click, Event* doublclick, Event* hold) {
        set_config(config);
        set_name(name);
        set_on_click(click);
        set_on_doubleclick(doublclick);
        set_on_hold(hold);
    }

    EntryStatic::~EntryStatic() {}

    void EntryStatic::set_name(std::string name) {
        this->name = name;
    }

    bool EntryStatic::draw(unsigned int x, unsigned int y, bool selected) {
        std::string str = Entry::get_name(config->get_max_len(), selected);

        config->draw_entry(x, y, selected, str);
    }

    std::string EntryStatic::get_name() const {
        return name;
    }
}