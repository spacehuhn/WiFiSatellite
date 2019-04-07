#include "EntryChannel.h"

namespace simplemenu {
    EntryChannel::EntryChannel(std::string name, Config* config, Channel* ch, Menu* m, SIMPLEMENU_EVENT_FNCT) {
		this->prefix = name;
		set_name(name);
		set_config(config);
		this->ch = ch;
		set_on_click(new Event(m, event_fnct));

		update();
    }

    EntryChannel::~EntryChannel() {}

    void EntryChannel::update() {
        if(ch) set_name(strdo::build(prefix, "", ch->to_str(), config->get_max_len()));
    }

    void EntryChannel::set_name(std::string name) {
        this->name = name;
    }

    bool EntryChannel::draw(unsigned int x, unsigned int y, bool selected) {
        std::string str = Entry::get_name(config->get_max_len(), selected);

        config->draw_entry(x, y, selected, str);
    }

    std::string EntryChannel::get_name() const {
        return name;
    }
}