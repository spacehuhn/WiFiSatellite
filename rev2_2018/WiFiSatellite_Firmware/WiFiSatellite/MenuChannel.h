#ifndef MenuChannel_h
#define MenuChannel_h

#include <menus/Menu.h>         // Parent class
#include <entries/EntryValue.h> // entries
#include <entries/EntryMenuHeader.h>
#include <entries/EntryStatic.h>
#include "Channel.h"            // Channel

namespace simplemenu {
    class MenuChannel: public Menu {
        public:
            MenuChannel(std::string name, Menu* parent, Channel* ch);
            ~MenuChannel();

            virtual void update();

            virtual bool draw(bool cursor);
            virtual void reset();

            virtual Menu* up();
            virtual Menu* down();
            virtual Menu* click();
            virtual Menu* doubleclick();
            virtual Menu* hold();

        private:
            Channel* ch { nullptr };
            EntryMenuHeader* header_entry { nullptr };
            EntryValue * channel_entry    { nullptr };
            EntryValue * bandwidth_entry  { nullptr };
            EntryStatic* info_entry       { nullptr };
            unsigned int index { 1 };
    };
}

#endif /* ifndef MenuChannel_h */