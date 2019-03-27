#include "Entry.h"

namespace simplemenu {
    Entry::~Entry() {
        if (click_event) delete click_event;
        if (doubleclick_event) delete doubleclick_event;
        if (hold_event) delete hold_event;
    }

    void Entry::set_config(Config* config) {
        this->config = config;
    }

    bool Entry::draw(unsigned int row, bool selected) {
        return draw(0, config->get_entry_height() * row, selected);
    }

    void Entry::reset() {
        char_index = 0;

        still_time  = millis();
        scroll_time = millis();
    }

    bool Entry::is_hidden() const {
        return hidden;
    }

    void Entry::hide() {
        hidden = true;
    }

    void Entry::unhide() {
        hidden = false;
    }

    std::string Entry::get_name(unsigned int max_len) {
        return get_name().substr(0, max_len);
    }

    std::string Entry::get_name(unsigned int max_len, bool scrolling) {
        if (!scrolling && (char_index == 0)) {
            still_time = millis();
            return get_name(max_len);
        }

        std::string s = get_name();

        if (s.size() <= max_len) return s;

        unsigned long current_ms = millis();

        if (char_index > s.size()) {
            still_time = current_ms;
            char_index = 0;
        }

        s = s + ' ' + s;
        s = s.substr(char_index, max_len);

        if ((current_ms - still_time > ENTRY_STILL_TIME) && (current_ms - scroll_time > ENTRY_SCROLL_SPEED)) {
            char_index++;
            scroll_time = current_ms;
        }

        return s;
    }

    Event* Entry::get_on_clicked() const {
        return click_event;
    }

    Event* Entry::get_on_doubleclicked() const {
        return doubleclick_event;
    }

    Event* Entry::get_on_holding() const {
        return hold_event;
    }

    void Entry::set_on_click(Event* e) {
        click_event = e;
    }

    void Entry::set_on_click(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        click_event = new Event(m, event_fnct);
    }

    void Entry::set_on_click(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        click_event = new Event(m, event_fnct);
    }

    void Entry::set_on_doubleclick(Event* e) {
        doubleclick_event = e;
    }

    void Entry::set_on_doubleclick(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        doubleclick_event = new Event(m, event_fnct);
    }

    void Entry::set_on_doubleclick(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        doubleclick_event = new Event(m, event_fnct);
    }

    void Entry::set_on_hold(Event* e) {
        hold_event = e;
    }

    void Entry::set_on_hold(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        hold_event = new Event(m, event_fnct);
    }

    void Entry::set_on_hold(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        hold_event = new Event(m, event_fnct);
    }
}