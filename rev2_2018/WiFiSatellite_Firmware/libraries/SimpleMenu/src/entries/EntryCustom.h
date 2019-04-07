#ifndef SimpleMenu_EntryCustom_h
#define SimpleMenu_EntryCustom_h

#include "entries/Entry.h" // parent class

#define SIMPLEMENU_DRAW_FNCT std::function<void(unsigned int x, unsigned int y, bool selected)> draw_fnct

namespace simplemenu {
    class EntryCustom : public Entry {
        public:
            EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            EntryCustom(Config* config, SIMPLEMENU_DRAW_FNCT, Event* click                   = nullptr, Event* doubleclick = nullptr, Event* hold = nullptr);

            virtual ~EntryCustom();

            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;

        protected:
            SIMPLEMENU_DRAW_FNCT { nullptr };
    };
}
#endif // ifndef SimpleMenu_EntryCustom_h