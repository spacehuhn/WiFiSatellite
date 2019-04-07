#include "Config.h"

namespace simplemenu {
    Config::~Config() {}

    void Config::set_fps(unsigned int fps) {
        this->fps = fps;
    }

    void Config::set_res(unsigned int width, unsigned int height) {
        set_width(width);
        set_height(height);
    }

    void Config::set_width(unsigned int width) {
        this->width = width;
    }

    void Config::set_height(unsigned int height) {
        this->height = height;
    }

    void Config::set_entries_per_page(unsigned int entries_per_page) {
        this->entries_per_page = entries_per_page;
        this->entry_height     = this->height / this->entries_per_page;
    }

    void Config::set_entry_height(unsigned int entry_height) {
        this->entry_height     = entry_height;
        this->entries_per_page = this->height / this->entry_height;
    }

    void Config::set_max_len(unsigned int max_len) {
        this->max_len = max_len;
    }

    unsigned int Config::get_fps() const {
        return this->fps;
    }

    unsigned int Config::get_width() const {
        return this->width;
    }

    unsigned int Config::get_height() const {
        return this->height;
    }

    unsigned int Config::get_entries_per_page() const {
        return this->entries_per_page;
    }

    unsigned int Config::get_entry_height() const {
        return this->entry_height;
    }

    unsigned int Config::get_max_len() const {
        return this->max_len;
    }

    bool Config::is_on() const {
        return turned_on;
    }

    void Config::on() {
        this->turned_on = true;
    }

    void Config::off() {
        this->turned_on = false;
    }

    unsigned int Config::fix_x(unsigned int x) {
        return (x > width) ? width : x;
    }

    unsigned int Config::fix_y(unsigned int y) {
        return (y > height) ? height : y;
    }
}