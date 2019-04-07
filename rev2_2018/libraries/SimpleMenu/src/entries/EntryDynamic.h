#ifndef SimpleMenu_EntryDynamic_h
#define SimpleMenu_EntryDynamic_h

#include "entries/Entry.h"

#define SIMPLEMENU_NAME_FNCT std::function<std::string()> name_fnct

namespace simplemenu {
    class EntryDynamic : public Entry {
        public:
            EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            EntryDynamic(Config* config, SIMPLEMENU_NAME_FNCT, Event* click                   = nullptr, Event* doubleclick = nullptr, Event* hold = nullptr);

            virtual ~EntryDynamic();

            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;

        protected:
            SIMPLEMENU_NAME_FNCT { nullptr };
    };
}
#endif // ifndef SimpleMenu_EntryDynamic_h