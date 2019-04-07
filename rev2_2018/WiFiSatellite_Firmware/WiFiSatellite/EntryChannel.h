#ifndef SimpleMenu_EntryChannel_h
#define SimpleMenu_EntryChannel_h

#include <sstream>     // std::stringstream
#include "lib/strdo.h" // strdo::build
#include "entries/Entry.h"
#include "Channel.h"

namespace simplemenu {
    class EntryChannel: public Entry {
        public:
            EntryChannel(std::string name, Config* config, Channel* ch, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            virtual ~EntryChannel();

            void update();
            virtual void set_name(std::string name);
            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;

        protected:
            std::string prefix {};
            std::string suffix {};
            std::string name {};
            Channel   * ch { nullptr };
    };
}
#endif // ifndef SimpleMenu_EntryChannel_h