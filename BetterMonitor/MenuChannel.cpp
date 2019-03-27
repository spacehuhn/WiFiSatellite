#include "MenuChannel.h"

namespace simplemenu {
    MenuChannel::MenuChannel(std::string name, Menu* parent, Channel* ch) {
        assert(parent);
        assert(ch);

        set_name(name);
        set_parent_menu(parent);
        set_config(parent->get_config());
        this->ch              = ch;
        this->header_entry    = new EntryMenuHeader(this->config, name);
        this->channel_entry   = new EntryValue(this->config, "Channel Number");
        this->bandwidth_entry = new EntryValue(this->config, "Mode");
        this->info_entry      = new EntryStatic(this->config, strdo::build("", "click to continue", "", config->get_max_len()));

        this->channel_entry->set_value<int>(ch->get_main_channel());
        this->bandwidth_entry->set_value<std::string>(ch->get_bandwidth());
    }

    MenuChannel::~MenuChannel() {
        delete header_entry;
        delete channel_entry;
        delete bandwidth_entry;
        delete info_entry;
    }

    void MenuChannel::update() {
        Menu::update();
    }

    bool MenuChannel::draw(bool cursor) {
        ((Entry*)header_entry)->draw(0, false);
        ((Entry*)channel_entry)->draw(1, index == 1);
        ((Entry*)bandwidth_entry)->draw(2, index == 2);
        ((Entry*)info_entry)->draw(4, false);
    }

    void MenuChannel::reset() {
        index = 1;
    }

    Menu* MenuChannel::up() {
        switch (index) {
            case 1:
                ch->set(ch->get_main_channel() + 1, ch->get_mode());
				break;
			case 2:
				ch->set(ch->get_main_channel(), ch->get_mode() + 1);
                break;
        }

        this->channel_entry->set_value<int>(ch->get_main_channel());
        this->bandwidth_entry->set_value<std::string>(ch->get_bandwidth());

        return Menu::up();
    }

    Menu* MenuChannel::down() {
        switch (index) {
            case 1:
                ch->set(ch->get_main_channel() - 1, ch->get_mode());
                break;
            case 2:
                ch->set(ch->get_main_channel(), ch->get_mode() - 1);
                break;
        }

        this->channel_entry->set_value<int>(ch->get_main_channel());
        this->bandwidth_entry->set_value<std::string>(ch->get_bandwidth());

        return Menu::down();
    }

    Menu* MenuChannel::click() {
        index++;

        if (index > 2) {
            reset();
            return parent;
        }

        return Menu::click();
    }

    Menu* MenuChannel::doubleclick() {
        return Menu::doubleclick();
    }

    Menu* MenuChannel::hold() {
        return Menu::hold();
    }
}