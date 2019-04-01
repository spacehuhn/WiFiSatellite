#ifndef CLI_h
#define CLI_h

#include <SimpleCLI.h> // SimpleCLI library and parent class
#include <iostream>    // std::cout
#include <functional>  // std:.function
#include "DisplayUI.h"
#include "Settings.h"
#include "PacketWriter.h"
#include "RGBLED.h"

using namespace std;
using namespace simplemenu;
using namespace simplesetting;

namespace simplecli {
    class CLI: public SimpleCLI {
        public:
            CLI();
            ~CLI();

            void init(int baudrate, DisplayUI& ui, Channel& ch, Settings& settings, PacketWriter& sd, RGBLED& led);
            void update();

        private:
            DisplayUI* ui { nullptr };
            Channel  * ch { nullptr };
            Settings * settings { nullptr };
			PacketWriter* sd{ nullptr };
			RGBLED* led { nullptr };
    };
}

#endif /* ifndef CLI_h */