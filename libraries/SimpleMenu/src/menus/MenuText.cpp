#include "MenuText.h"

namespace simplemenu {
    MenuText::MenuText(std::string title, Config* config) {
        assert(config);

        set_name(title);
        set_config(config);

        set_header_entry(title);

        reset();
    }

    MenuText::MenuText(std::string title, Menu* parent) {
        assert(parent);

        set_name(title);
        set_config(parent->get_config());
        set_parent_menu(parent);

        set_header_entry(title, parent);

        reset();
    }

    MenuText::~MenuText() {
        entry_list.remove_if([](Entry* e) {
            if (e) delete e;
            return true;
        });

        if (header_entry) delete header_entry;
    }

    void MenuText::set_config(Config* config) {
        for (auto e:entry_list) e->set_config(config);
        Menu::set_config(config);
    }

    bool MenuText::draw(bool cursor) {
        update();

        if (!config) return false;

        // relative entry index (i.e. 0 - 4)
        int i    = 0;                              // index
        int r    = 0;                              // row of page
        int rows = config->get_entries_per_page(); // rows per page

        // start index
        int start = 0;
        int page  = get_page();                           // page (starts with 0)
        if (page > 0) start = 5 + ((page-1) * (rows-1));  // first page includes header and therefor one entry longer

        for (auto it = entry_list.begin(); it != entry_list.end() && r < rows; ++it) {
            // check that index is in range of the current page
            if (i >= start) {
                if ((r == 0) && (i>0)) { // draw header (first page has the header as entry)
                    header_entry->draw(0, 0, false);
                    ++r;                 // increment row number
                }

                Entry* e = *it;

                // if object isn't null and not hidden
                if (e && !e->is_hidden()) {
                    bool selected = (i == this->cursor);
                    e->draw(0, r * config->get_entry_height(), selected && cursor);
                }

                ++r; // increment row number
            }

            ++i;     // increment index number
        }

        return true;
    }

    void MenuText::reset() {
        for (auto i = entry_list.begin(); i != entry_list.end(); ++i) {
            (*i)->reset();
        }

        cursor = 1;
    }

    Menu* MenuText::up() {
        set_cursor(get_cursor() - 1);

        return nullptr;
    }

    Menu* MenuText::down() {
        set_cursor(get_cursor() + 1);

        return nullptr;
    }

    Menu* MenuText::click() {
        Entry* selected_entry = get_entry(cursor);

        if (selected_entry) {
            Event* e = selected_entry->get_on_clicked();

            if (e) {
                Menu* next_menu = e->run(this);
                if ((cursor == 0) && next_menu) reset();
                return next_menu;
            }
        }

        return nullptr;
    }

    Menu* MenuText::doubleclick() {
        Entry* selected_entry = get_entry(cursor);

        if (selected_entry) {
            Event* e = selected_entry->get_on_doubleclicked();

            if (e) return e->run(this);
        }

        return nullptr;
    }

    Menu* MenuText::hold() {
        Entry* selected_entry = get_entry(cursor);

        if (selected_entry) {
            Event* e = selected_entry->get_on_holding();

            if (e) return e->run(this);
        }

        return nullptr;
    }

    int MenuText::get_cursor() const {
        return cursor;
    }

    void MenuText::set_cursor(int i) {
        if (((i == 0) && !parent) || (i < 0)) i = entry_list.size() - 1;
        else if (i >= entry_list.size()) i = 1;

        cursor = i;

        if (header_entry) header_entry->set_page(get_page() + 1);
    }

    int MenuText::get_page() const {
        int entries_per_page = config->get_entries_per_page();

        // because the first page has one entry more (the header)
        if (cursor < entries_per_page) return 0;

        // .. we gonna have to do some maths
        int tmp_cursor = cursor - entries_per_page;               // minus entries of first page
        int page       = (tmp_cursor / (entries_per_page-1)) + 1; // divide to get page number + 1 for the first page
        return page;
    }

    int MenuText::get_pages() const {
        return entry_list.size() / (config->get_entries_per_page() - 1);
    }

    void MenuText::add_entry(Entry* e) {
        entry_list.push_back(e);
    }

    void MenuText::add_entries(std::list<Entry*> entry_list) {
        for (auto i = entry_list.begin(); i != entry_list.end(); ++i) {
            this->entry_list.push_back(*i);
        }
    }

    EntryStatic* MenuText::add_static_entry(std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        EntryStatic* e = new EntryStatic(config, name, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryDynamic* MenuText::add_dynamic_entry(SIMPLEMENU_NAME_FNCT,  Menu* m, SIMPLEMENU_EVENT_FNCT) {
        EntryDynamic* e = new EntryDynamic(config, name_fnct, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryCustom* MenuText::add_custom_entry(SIMPLEMENU_DRAW_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        EntryCustom* e = new EntryCustom(config, draw_fnct, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryValue* MenuText::add_value_entry(std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT) {
        EntryValue* e = new EntryValue(config, name, event_fnct, m);

        add_entry(e);
        return e;
    }

    EntryStatic* MenuText::add_static_entry(std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        EntryStatic* e = new EntryStatic(config, name, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryDynamic* MenuText::add_dynamic_entry(SIMPLEMENU_NAME_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        EntryDynamic* e = new EntryDynamic(config, name_fnct, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryCustom* MenuText::add_custom_entry(SIMPLEMENU_DRAW_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        EntryCustom* e = new EntryCustom(config, draw_fnct, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryValue* MenuText::add_value_entry(std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m) {
        EntryValue* e = new EntryValue(config, name, m, event_fnct);

        add_entry(e);
        return e;
    }

    EntryStatic* MenuText::add_static_entry(std::string name, Event* click, Event* doubleclick, Event* hold) {
        EntryStatic* e = new EntryStatic(config, name, click, doubleclick, hold);

        add_entry(e);
        return e;
    }

    EntryDynamic* MenuText::add_dynamic_entry(SIMPLEMENU_NAME_FNCT, Event* click, Event* doubleclick, Event* hold) {
        EntryDynamic* e = new EntryDynamic(config, name_fnct, click, doubleclick, hold);

        add_entry(e);
        return e;
    }

    EntryCustom* MenuText::add_custom_entry(SIMPLEMENU_DRAW_FNCT, Event* click, Event* doubleclick, Event* hold) {
        EntryCustom* e = new EntryCustom(config, draw_fnct, click, doubleclick, hold);

        add_entry(e);
        return e;
    }

    EntryValue* MenuText::add_value_entry(std::string name, Event* click, Event* doublclick, Event* hold) {
        EntryValue* e = new EntryValue(config, name, click, doublclick, hold);

        add_entry(e);
        return e;
    }

    Entry* MenuText::get_entry(int index) const {
        if (index < 0) return header_entry;

        if (index < entry_list.size()) {
            int j = 0;

            for (auto i = entry_list.begin(); i != entry_list.end(); ++i) {
                if (j == index) return *i;
                ++j;
            }
        }

        return nullptr;
    }

    const std::list<Entry*>& MenuText::get_entry_list() const {
        return entry_list;
    }

    void MenuText::set_header_entry(std::string title, Menu* parent) {
        if (header_entry) {
            entry_list.pop_front();
            delete header_entry;
        }

        if (parent) header_entry = new EntryMenuHeader(config, title, parent, parent->get_name());
        else header_entry = new EntryMenuHeader(config, title);

        entry_list.push_front(header_entry);
    }
}