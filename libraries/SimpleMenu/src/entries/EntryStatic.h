#ifndef SimpleMenu_EntryStatic_h
#define SimpleMenu_EntryStatic_h

#include "entries/Entry.h"

namespace simplemenu {
    class EntryStatic : public Entry {
        public:
            EntryStatic(Config* config, std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            EntryStatic(Config* config, std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            EntryStatic(Config* config, std::string name, Event* click                   = nullptr, Event* doublclick = nullptr, Event* hold = nullptr);

            virtual ~EntryStatic();

            virtual void set_name(std::string name);
            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;

        protected:
            std::string name {};
    };
}
#endif // ifndef SimpleMenu_EntryStatic_h