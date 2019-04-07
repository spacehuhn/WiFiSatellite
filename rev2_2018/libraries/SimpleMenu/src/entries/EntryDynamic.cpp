#include "EntryDynamic.h"

namespace simplemenu {
    EntryDynamic::EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        this->name_fnct = name_fnct;
        set_config(config);
        set_on_click(new Event(m, event_fnct));
    }

    EntryDynamic::EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        this->name_fnct = name_fnct;
        set_config(config);
        set_on_click(new Event(m, event_fnct));
    }

    EntryDynamic::EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, Event* click, Event* doubleclick, Event* hold) {
        this->name_fnct = name_fnct;
        set_config(config);
        set_on_click(click);
        set_on_doubleclick(doubleclick);
        set_on_hold(hold);
    }

    EntryDynamic::~EntryDynamic() {}

    bool EntryDynamic::draw(unsigned int x, unsigned int y, bool selected) {
        std::string str = Entry::get_name(config->get_max_len(), selected);

        config->draw_entry(x, y, selected, str);
    }

    std::string EntryDynamic::get_name() const {
        return (name_fnct) ? name_fnct() : std::string();
    }
}