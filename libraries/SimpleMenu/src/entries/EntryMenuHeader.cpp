#include "EntryMenuHeader.h"

namespace simplemenu {
    EntryMenuHeader::EntryMenuHeader(Config* config, std::string name, Menu* parent, std::string parent_name) {
        this->name        = name;
        this->parent      = parent;
        this->parent_name = parent_name;
        set_config(config);
        set_on_click(new Event(parent));
    }

    EntryMenuHeader::~EntryMenuHeader() {}

    bool EntryMenuHeader::draw(unsigned int x, unsigned int y, bool selected) {
        std::string str = generate_name(config->get_max_len(), selected);

        config->draw_header(x, y, selected, str);
    }

    void EntryMenuHeader::set_page(unsigned int page) {
        this->page_str = strdo::to_str(page);
    }

    std::string EntryMenuHeader::get_name() const {
        return generate_name(32, false);
    }

    std::string EntryMenuHeader::generate_name(int max_len, bool selected) const {
        std::string left   = (selected && parent) ? "<" : "#";
        std::string center = (selected && parent) ? parent_name : name;
        std::string right  = (selected && parent) ? "" : page_str;

        return strdo::build(left, center, right, max_len);
    }
}