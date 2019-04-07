#ifndef SimpleMenu_MenuText_h
#define SimpleMenu_MenuText_h

#include <list>   // std::list
#include <string> // std::string

#include "menus/Menu.h"
#include "entries/EntryStatic.h"
#include "entries/EntryValue.h"
#include "entries/EntryDynamic.h"
#include "entries/EntryCustom.h"
#include "entries/EntryMenuHeader.h"

namespace simplemenu {
    class MenuText : public Menu {
        public:
            MenuText(std::string title, Config* config);
            MenuText(std::string title, Menu* parent);

            virtual ~MenuText();

            virtual void set_config(Config* config);

            virtual bool draw(bool cursor);

            virtual void reset();

            virtual Menu* up();
            virtual Menu* down();
            virtual Menu* click();
            virtual Menu* doubleclick();
            virtual Menu* hold();

            virtual int get_cursor() const;
            virtual void set_cursor(int i);

            virtual int get_page() const;
            virtual int get_pages() const;

            virtual void add_entry(Entry* e);
            virtual void add_entries(std::list<Entry*> entry_list);

            virtual EntryStatic* add_static_entry(std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT       = nullptr);
            virtual EntryDynamic* add_dynamic_entry(SIMPLEMENU_NAME_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual EntryCustom* add_custom_entry(SIMPLEMENU_DRAW_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT   = nullptr);
            virtual EntryValue* add_value_entry(std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT         = nullptr);

            virtual EntryStatic* add_static_entry(std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m       = nullptr);
            virtual EntryDynamic* add_dynamic_entry(SIMPLEMENU_NAME_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            virtual EntryCustom* add_custom_entry(SIMPLEMENU_DRAW_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m   = nullptr);
            virtual EntryValue* add_value_entry(std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m        = nullptr);

            virtual EntryStatic* add_static_entry(std::string name, Event* click = nullptr, Event* doubleclick = nullptr, Event* hold = nullptr);
            virtual EntryDynamic* add_dynamic_entry(SIMPLEMENU_NAME_FNCT, Event* click = nullptr, Event* doubleclick = nullptr, Event* hold = nullptr);
            virtual EntryCustom* add_custom_entry(SIMPLEMENU_DRAW_FNCT, Event* click   = nullptr, Event* doubleclick = nullptr, Event* hold = nullptr);
            virtual EntryValue* add_value_entry(std::string name, Event* click = nullptr, Event* doublclick = nullptr, Event* hold = nullptr);

            virtual const std::list<Entry*>& get_entry_list() const;
            virtual Entry* get_entry(int index) const;

        protected:
            EntryMenuHeader* header_entry { nullptr };
            std::list<Entry*> entry_list {};
            int cursor { 1 };

            virtual void set_header_entry(std::string title, Menu* parent = nullptr);
    };
}

#endif // ifndef SimpleMenu_MenuText_h