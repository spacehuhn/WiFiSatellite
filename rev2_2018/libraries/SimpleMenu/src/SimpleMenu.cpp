#include "SimpleMenu.h"

namespace simplemenu {
    SimpleMenu::SimpleMenu() {}

    SimpleMenu::SimpleMenu(Menu* main_menu) {
        set_main_menu(main_menu);
    }

    SimpleMenu::~SimpleMenu() {}

    bool SimpleMenu::update() {
        if (!this->config || !current_menu || !current_menu->ready()) return false;

        uint32_t currentTime = millis();

        if (currentTime - frame_time >= 1000) {
            frames     = tmp_frames;
            tmp_frames = 0;
            frame_time = currentTime;
        }

        if ((display_timeout > 0) && config->is_on() && (currentTime - display_time >= display_timeout)) {
            config->off();
        }

        if ((currentTime - update_time >= update_interval) && (tmp_frames < config->get_fps())) {
            update_time = currentTime;

            config->exec_prefix();
            current_menu->draw(get_cursor());
            config->exec_suffix();

            tmp_frames++;

            return true;
        }

        return false;
    }

    void SimpleMenu::set_main_menu(Menu* m) {
        if (m) {
            main_menu = m;

            config          = m->get_config();
            update_interval = 1000 / this->config->get_fps();

            if (!current_menu) current_menu = m;
        }
    }

    void SimpleMenu::set_menu(Menu* m) {
        if (m) {
            if (!main_menu) main_menu = m;
            current_menu = m;
        }
    }

    void SimpleMenu::set_display_timeout(unsigned long display_timeout) {
        this->display_timeout = display_timeout;
        display_time          = millis();
    }

    void SimpleMenu::set_cursor_timeout(unsigned long cursor_timeout) {
        this->cursor_timeout = cursor_timeout;
        cursor_time          = millis();
    }

    Config* SimpleMenu::get_config() {
        return this->config;
    }

    Menu*  SimpleMenu::get_menu() {
        return this->current_menu;
    }

    Menu*  SimpleMenu::get_main_menu() {
        return this->main_menu;
    }

    void SimpleMenu::up() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu) set_menu(current_menu->up());
        display_time = millis();
        cursor_time  = millis();
    }

    void SimpleMenu::down() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu) set_menu(current_menu->down());
        display_time = millis();
        cursor_time  = millis();
    }

    void SimpleMenu::click() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu) set_menu(current_menu->click());
        display_time = millis();
        cursor_time  = millis();
    }

    void SimpleMenu::doubleclick() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu) set_menu(current_menu->doubleclick());
        display_time = millis();
        cursor_time  = millis();
    }

    void SimpleMenu::hold() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu) set_menu(current_menu->hold());
        display_time = millis();
        cursor_time  = millis();
    }

    void SimpleMenu::goBack() {
        if (!config->is_on()) config->on();
        else if (get_cursor() && current_menu && current_menu->has_parent()) set_menu(current_menu->get_parent());
        display_time = millis();
        cursor_time  = millis();
    }

    unsigned int SimpleMenu::get_fps() {
        return this->frames;
    }

    bool SimpleMenu::get_cursor() {
        if (cursor_timeout == 0) return true;
        return millis() - cursor_time <= cursor_timeout;
    }
}