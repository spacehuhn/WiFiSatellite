#include "ConfigCustom.h"

namespace simplemenu {
    ConfigCustom::ConfigCustom() {}

    ConfigCustom::ConfigCustom(unsigned int width, unsigned int height, unsigned int entries_per_page, unsigned int max_len) {
        set_res(width, height);
        set_entries_per_page(entries_per_page);
        set_max_len(max_len);
    }

    ConfigCustom::~ConfigCustom() {}

    void ConfigCustom::set_display_on(SIMPLEMENU_DISPLAY_ON) {
        this->on_f = on_f;
    }

    void ConfigCustom::set_display_off(SIMPLEMENU_DISPLAY_OFF) {
        this->off_f = off_f;
    }

    void ConfigCustom::set_update_prefix(SIMPLEMENU_DISPLAY_PREFIX) {
        this->prefix_f = prefix_f;
    }

    void ConfigCustom::set_update_suffix(SIMPLEMENU_DISPLAY_SUFFIX) {
        this->suffix_f = suffix_f;
    }

    void ConfigCustom::set_draw_str(SIMPLEMENU_DISPLAY_DRAW) {
        this->draw_str_f = draw_str_f;
    }

    void ConfigCustom::set_draw_header(SIMPLEMENU_DISPLAY_HEADER) {
        this->draw_header_f = draw_header_f;
    }

    void ConfigCustom::set_draw_entry(SIMPLEMENU_DISPLAY_ENTRY) {
        this->draw_entry_f = draw_entry_f;
    }

    void ConfigCustom::set_draw_pixel(SIMPLEMENU_DISPLAY_PIXEL) {
        this->draw_pixel_f = draw_pixel_f;
    }

    void ConfigCustom::set_draw_line(SIMPLEMENU_DISPLAY_LINE) {
        this->draw_line_f = draw_line_f;
    }

    void ConfigCustom::set_draw_rect(SIMPLEMENU_DISPLAY_RECT) {
        this->draw_rect_f = draw_rect_f;
    }

    void ConfigCustom::set_draw_circle(SIMPLEMENU_DISPLAY_CIRCLE) {
        this->draw_circle_f = draw_circle_f;
    }

    void ConfigCustom::exec_prefix() {
        if (this->prefix_f) this->prefix_f();
    }

    void ConfigCustom::exec_suffix() {
        if (this->suffix_f) this->suffix_f();
    }

    void ConfigCustom::draw(unsigned int x, unsigned int y, std::string s) {
        if (this->draw_str_f) this->draw_str_f(x, y, s);
    }

    void ConfigCustom::draw_header(unsigned int x, unsigned int y, bool selected, std::string s) {
        if (this->draw_header_f) this->draw_header_f(x, y, selected, s);
        else draw_entry(x, y, selected, s);
    }

    void ConfigCustom::draw_entry(unsigned int x, unsigned int y, bool selected, std::string s) {
        if (this->draw_entry_f) this->draw_entry_f(x, y, selected, s);
        else draw(x, y, s);
    }

    void ConfigCustom::draw_pixel(unsigned int x, unsigned int y, bool on) {
        x = fix_x(x);
        y = fix_y(y);
        if (this->draw_pixel_f) this->draw_pixel_f(x, y, on);
    }

    void ConfigCustom::draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
        x1 = fix_x(x1);
        y2 = fix_y(y2);
        x2 = fix_x(x2);
        y2 = fix_y(y2);
        if (this->draw_line_f) this->draw_line_f(x1, y1, x2, y2);
    }

    void ConfigCustom::draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled) {
        x = fix_x(x);
        y = fix_y(y);
        if (this->draw_rect_f) this->draw_rect_f(x, y, width, height, filled);
    }

    void ConfigCustom::draw_circle(unsigned int x, unsigned int y, unsigned int radius, bool filled) {
        x = fix_x(x);
        y = fix_y(y);
        if (this->draw_circle_f) this->draw_circle_f(x, y, radius, filled);
    }

    void ConfigCustom::on() {
        Config::on();
        if (on_f) on_f();
    }

    void ConfigCustom::off() {
        Config::off();
        if (off_f) off_f();
    }
}