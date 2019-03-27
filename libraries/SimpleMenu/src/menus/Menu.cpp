#include "Menu.h"

namespace simplemenu {
    Config*  Menu::get_config() const {
        return this->config;
    }

    void Menu::set_config(Config* config) {
        this->config = config;
        set_fps(config->get_fps());
    }

    void Menu::set_name(std::string name) {
        this->name = name;
    }

    std::string Menu::get_name() {
        return this->name;
    }

    bool Menu::has_parent() {
        return this->parent;
    }

    Menu* Menu::get_parent() const {
        return this->parent;
    }

    void Menu::set_parent_menu(Menu* parent) {
        this->parent = parent;
    }

    void Menu::set_fps(unsigned int fps) {
        this->fps = fps;
    }

    unsigned int Menu::get_fps() const {
        return fps;
    }

    bool Menu::ready() const {
        return (millis() - update_time) >= (1000 / fps);
    }

    void Menu::update() {
        update_time = millis();
    }

    Menu* Menu::up() {
        return nullptr;
    }

    Menu* Menu::down() {
        return nullptr;
    }

    Menu* Menu::click() {
        return nullptr;
    }

    Menu* Menu::doubleclick() {
        return nullptr;
    }

    Menu* Menu::hold() {
        return nullptr;
    }
}