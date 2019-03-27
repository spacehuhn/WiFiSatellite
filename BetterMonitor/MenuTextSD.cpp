#include "MenuTextSD.h"

namespace simplemenu {
    MenuTextSD::MenuTextSD(SDPacketWriter* sd, Scan* scan, Menu* parent) : MenuText("SD", parent) {
        this->sd   = sd;
        this->scan = scan;

        MenuText::add_dynamic_entry([this]()->std::string {
            if (this->sd->is_errored()) return "ERROR";
            else if (this->sd->is_paused()) return "PAUSED";
            return "OK";
        });

		MenuText::add_dynamic_entry([this]()->std::string {
			std::stringstream ss;
			ss << "Size: " << this->sd->total_bytes() << " Byte";
			return ss.str();
		});

        MenuText::add_dynamic_entry([this]()->std::string {
            std::stringstream ss;
            ss << "Speed: ";
            ss << this->sd->get_speed();
            ss << " MB/s";
            return ss.str();
        });

		MenuText::add_dynamic_entry([this]()->std::string {
			std::stringstream ss;
			ss << "Free: " << this->sd->free_bytes() << " Byte";
			return ss.str();
		});

		MenuText::add_dynamic_entry([this]()->std::string {
			std::stringstream ss;
			ss << "Used: " << this->sd->used_bytes() << " Byte";
			return ss.str();
		});

        MenuText::add_dynamic_entry([this]()->std::string {
            return this->sd->is_paused() ? "ENABLE" : "DISABLE";
        }, nullptr, [this](Menu* p, Menu* n)->Menu* {
            if (this->sd->is_paused()) this->sd->pause();
            else this->sd->resume();
            return nullptr;
        });
    }

    MenuTextSD::~MenuTextSD() {}

    void MenuTextSD::on() {
        this->sd->resume();
        scan->set_packet_writer(sd);
    }

    void MenuTextSD::off() {
        this->sd->pause();
    }
}