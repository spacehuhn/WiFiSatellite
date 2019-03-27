#ifndef MenuTextSD_h
#define MenuTextSD_h

#include <menus/MenuText.h>
#include "SDPacketWriter.h"
#include "Scan.h"

namespace simplemenu {
    class MenuTextSD : public MenuText {
        public:
            MenuTextSD(SDPacketWriter* sd, Scan* scan, Menu* parent);
            ~MenuTextSD();

            void on();
            void off();

        private:
            SDPacketWriter* sd{ nullptr };
            Scan* scan{ nullptr };
    };
}

#endif // ifndef MenuTextSD_h