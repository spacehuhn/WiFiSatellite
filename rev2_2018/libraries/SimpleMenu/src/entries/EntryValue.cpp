#include "EntryValue.h"

namespace simplemenu {
    EntryValue::EntryValue(Config* config, std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        this->prefix = name;
        set_config(config);
        set_name(name);
        set_on_click(new Event(m, event_fnct));
    }

    EntryValue::EntryValue(Config* config, std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        this->prefix = name;
        set_config(config);
        set_name(name);
        set_on_click(new Event(m, event_fnct));
    }

    EntryValue::EntryValue(Config* config, std::string name, Event* click, Event* doublclick, Event* hold) {
        this->prefix = name;
        set_config(config);
        set_name(name);
        set_on_click(click);
        set_on_doubleclick(doublclick);
        set_on_hold(hold);
    }

    EntryValue::~EntryValue() {}

    void EntryValue::set_name(std::string name) {
        this->name = name;
    }

    bool EntryValue::draw(unsigned int x, unsigned int y, bool selected) {
        std::string str = Entry::get_name(config->get_max_len(), selected);

        config->draw_entry(x, y, selected, str);
    }

    std::string EntryValue::get_name() const {
        return name;
    }
}