#include "ConfigOLED.h"

namespace simplemenu {
    ConfigOLED::ConfigOLED(bool flipped) {
        display = new DISPLAY_TYPE(I2C_ADDR, SDA, SCL);
        
        DstikeUpdater::run(*display, 19, 5, 18);
        
        display->init();
        display->setFont(DejaVu_Sans_Mono_12);
        if(flipped) display->flipScreenVertically();

        Config::set_fps(30);
        Config::set_res(128, 64);
        Config::set_entries_per_page(5);
        Config::set_max_len(18);
    }

    ConfigOLED::~ConfigOLED() {}

    void ConfigOLED::exec_prefix() {
        display->clear();
    }

    void ConfigOLED::exec_suffix() {
        display->display();
    }

    void ConfigOLED::draw(unsigned int x, unsigned int y, std::string s) {
        display->drawString(x, y, s.c_str());
    }

    void ConfigOLED::draw_header(unsigned int x, unsigned int y, bool selected, std::string s) {
        draw_entry(x, y, selected, s);
        if (!selected) draw_line(x, y + entry_height, width, y + entry_height);
    }

    void ConfigOLED::draw_entry(unsigned int x, unsigned int y, bool selected, std::string s) {
        if (selected) draw_rect(0, y, width, entry_height + 2, false);
        draw(x + 2, y, s);
    }

    void ConfigOLED::draw_pixel(unsigned int x, unsigned int y, bool on) {
        if (on) display->setPixel(x, y);
    }

    void ConfigOLED::draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
        display->drawLine(x1, y1, x2, y2);
    }

    void ConfigOLED::draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled) {
        if (filled) display->fillRect(x, y, width, height);
        else display->drawRect(x, y, width, height);
    }

    void ConfigOLED::draw_circle(unsigned int x, unsigned int y, unsigned int radius, bool filled) {
        if (filled) display->fillCircle(x, y, radius);
        else display->drawCircle(x, y, radius);
    }

    void ConfigOLED::on() {
        Config::on();
        display->displayOn();
    }

    void ConfigOLED::off() {
        Config::off();
        display->displayOff();
    }
}
