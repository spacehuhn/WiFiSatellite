#include "CLI.h"

namespace simplecli {
    CLI::CLI() {}

    CLI::~CLI() {}

    void CLI::init(int baudrate, DisplayUI& ui, Channel& ch, Settings& settings, PacketWriter& sd, RGBLED& led) {
        Serial.begin(baudrate); // set baud rate for UART

        this->ui       = &ui;
        this->ch       = &ch;
        this->settings = &settings;
        this->sd       = &sd;
        this->led      = &led;

        this->onNotFound = [] (String str) {
            cout << '"' << str.c_str() << '"' << " not found" << endl;
        };

        addCmd(new Command("help", [this](Cmd* cmd) {
            cout << this->toString().c_str() << endl;
        }));

        addCmd(new Command("menu", [this](Cmd* cmd) {
            this->ui->goto_main_menu();
        }));

        addCmd(new Command("pkts", [this](Cmd* cmd) {
            this->ui->goto_pkt_graph();
        }));

        addCmd(new Command("byte", [this](Cmd* cmd) {
            this->ui->goto_byte_graph();
        }));

        addCmd(new Command("rssi", [this](Cmd* cmd) {
            this->ui->goto_rssi_graph();
        }));

        addCmd(new Command("sd", [this](Cmd* cmd) {
            this->ui->goto_sd_menu();
        }));

        addCmd(new Command("channel", [this](Cmd* cmd) {
            this->ui->goto_channel_menu();
        }));

        addCmd(new Command("reset", [this](Cmd* cmd) {
            this->ui->reset_values();
        }));

        addCmd(new Command("on", [this](Cmd* cmd) {
            this->ui->get_config()->on();
        }));

        addCmd(new Command("off", [this](Cmd* cmd) {
            this->ui->get_config()->off();
        }));

        addCmd(new Command("up", [this](Cmd* cmd) {
            this->ui->up();
        }));

        addCmd(new Command("down", [this](Cmd* cmd) {
            this->ui->down();
        }));

        addCmd(new Command("click", [this](Cmd* cmd) {
            this->ui->click();
        }));

        addCmd(new Command("doubleclick", [this](Cmd* cmd) {
            this->ui->doubleclick();
        }));

        addCmd(new Command("hold", [this](Cmd* cmd) {
            this->ui->hold();
        }));

        Command* id_cmd = new Command("setid", [this](Cmd* cmd) {
            this->settings->id.set(cmd->getValue(0).toInt());
			this->settings->save();
        });
        id_cmd->addArg(new AnonymReqArg());
        addCmd(id_cmd);

        addCmd(new Command("id", [this](Cmd* cmd) {
            this->ui->goto_id_menu();
        }));

        Command* ch_cmd = new Command("ch", [this](Cmd* cmd) {
            int second_ch = 0;
            if (cmd->isSet(0)) {
                if ((cmd->getValue(1) == "above") || (cmd->getValue(1) == "a")) second_ch = 1;
                else if ((cmd->getValue(1) == "below") || (cmd->getValue(1) == "b")) second_ch = -1;
            }
            this->ch->set(cmd->getValue(0).toInt(), second_ch);
        });
        ch_cmd->addArg(new AnonymReqArg());
        ch_cmd->addArg(new AnonymOptArg("0"));
        addCmd(ch_cmd);

        addCmd(new Command("getch", [this](Cmd* cmd) {
            cout << this->ch->to_str() << endl;
        }));

        addCmd(new Command("settings", [this](Cmd* cmd) {
            cout << *(this->settings) << endl;
        }));

        Command* time_cmd = new Command("time", [this](Cmd* cmd) {
            this->sd->set_time(cmd->getValue(0).toInt(), cmd->getValue(1).toInt());
            cout << "set time" << endl;
        });
        time_cmd->addArg(new AnonymReqArg());
        time_cmd->addArg(new AnonymReqArg());
        addCmd(time_cmd);

        Command* setflip_cmd = new Command("setflip", [this](Cmd* cmd) {
            this->settings->flip_screen.set(cmd->getValue(0) == "true");
            this->settings->save();
        });
        setflip_cmd->addArg(new AnonymReqArg());
        addCmd(setflip_cmd);

        Command* sd_cmd = new Command("sd", [this](Cmd* cmd) {
            if (cmd->getValue(0) == "false") {
                this->sd->pause();
                cout << "sd pause" << endl;
            }
            else {
                this->sd->resume();
                cout << "sd resumed" << endl;
            }
        });
        sd_cmd->addArg(new AnonymReqArg());
        addCmd(sd_cmd);

        Command* led_cmd = new Command("led", [this](Cmd* cmd) {
            this->led->set(cmd->getValue(0).toInt(), cmd->getValue(1).toInt(), cmd->getValue(2).toInt());
        });
        led_cmd->addArg(new AnonymReqArg());
        led_cmd->addArg(new AnonymReqArg());
        led_cmd->addArg(new AnonymReqArg());
        addCmd(led_cmd);

        Command* set_max_cmd = new Command("setmax", [this](Cmd* cmd) {
            String value = cmd->getArg(0)->getValue();
            String valueArr[5];

            int j = 0;

            for (int i = 0; i < value.length(); i++) {
                char c = value.charAt(i);
                if ((c == ',') || (c == ']')) j++;
                else if (c != '[') valueArr[j] += c;
            }

            this->ui->set_max_values(valueArr[0].toInt(), valueArr[1].toInt(), valueArr[2].toInt(), valueArr[3].toInt(), valueArr[4].toInt());
        });
        set_max_cmd->addArg(new AnonymReqArg());

        addCmd(set_max_cmd);
    }

    void CLI::update() {
        if (Serial.available()) {
            String tmp = Serial.readStringUntil('\n');

            if (tmp.length() > 0) {
                // print input
                Serial.print("# ");
                Serial.println(tmp);

                // and parse it
                parse(tmp);

                led->set(0, 0, 0);
            }
        }
    }
}