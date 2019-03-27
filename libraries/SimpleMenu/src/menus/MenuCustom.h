#ifndef SimpleMenu_MenuCustom_h
#define SimpleMenu_MenuCustom_h

#include "menus/Menu.h"
#include "events/Event.h"

#define SIMPLEMENU_DRAW_FNCT std::function<void(Config* config, bool cursor)> draw_fnct

namespace simplemenu {
    class MenuCustom : public Menu {
        public:
            MenuCustom(std::string title, Config* config);
            MenuCustom(std::string title, Menu* parent);

            virtual ~MenuCustom();

            virtual bool draw(bool cursor);
            virtual void reset();

            virtual Menu* up();
            virtual Menu* down();
            virtual Menu* click();
            virtual Menu* doubleclick();
            virtual Menu* hold();

            virtual void set_draw_fnct(SIMPLEMENU_DRAW_FNCT);

            virtual void setOnUp(Event* e);
            virtual void setOnDown(Event* e);
            virtual void setOnClicked(Event* e);
            virtual void setOnDoubleclicked(Event* e);
            virtual void setOnHolding(Event* e);

            virtual void setOnUp(Menu* m, SIMPLEMENU_EVENT_FNCT            = nullptr);
            virtual void setOnDown(Menu* m, SIMPLEMENU_EVENT_FNCT          = nullptr);
            virtual void setOnClicked(Menu* m, SIMPLEMENU_EVENT_FNCT       = nullptr);
            virtual void setOnDoubleclicked(Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual void setOnHolding(Menu* m, SIMPLEMENU_EVENT_FNCT       = nullptr);

            virtual void setOnUp(SIMPLEMENU_EVENT_FNCT, Menu* m            = nullptr);
            virtual void setOnDown(SIMPLEMENU_EVENT_FNCT, Menu* m          = nullptr);
            virtual void setOnClicked(SIMPLEMENU_EVENT_FNCT, Menu* m       = nullptr);
            virtual void setOnDoubleclicked(SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            virtual void setOnHolding(SIMPLEMENU_EVENT_FNCT, Menu* m       = nullptr);

        protected:
            SIMPLEMENU_DRAW_FNCT { nullptr };

            Event* upEvent          { nullptr };
            Event* downEvent        { nullptr };
            Event* clickEvent       { nullptr };
            Event* doubleClickEvent { nullptr };
            Event* holdEvent        { nullptr };
    };
}

#endif // ifndef SimpleMenu_MenuCustom_h