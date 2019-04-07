#include "MenuCustom.h"

namespace simplemenu {
    MenuCustom::MenuCustom(std::string title, Config* config) {
        assert(config);
        set_name(title);
        set_config(config);
    }

    MenuCustom::MenuCustom(std::string title, Menu* parent) {
        assert(parent);
        set_name(title);
        set_config(parent->get_config());
        set_parent_menu(parent);
    }

    MenuCustom::~MenuCustom() {}

    bool MenuCustom::draw(bool cursor) {
        this->update();
        if (this->draw_fnct) {
            this->draw_fnct(this->config, cursor);
            return true;
        }
        return false;
    }

    void MenuCustom::reset() {}

    Menu* MenuCustom::up() {
        if (this->upEvent) return this->upEvent->run(this);

        return nullptr;
    }

    Menu* MenuCustom::down() {
        if (this->downEvent) return this->downEvent->run(this);

        return nullptr;
    }

    Menu* MenuCustom::click() {
        if (this->clickEvent) return this->clickEvent->run(this);

        return nullptr;
    }

    Menu* MenuCustom::doubleclick() {
        if (this->doubleClickEvent) return this->doubleClickEvent->run(this);

        return nullptr;
    }

    Menu* MenuCustom::hold() {
        if (this->holdEvent) return this->holdEvent->run(this);

        return nullptr;
    }

    void MenuCustom::set_draw_fnct(SIMPLEMENU_DRAW_FNCT) {
        this->draw_fnct = draw_fnct;
    }

    void MenuCustom::setOnUp(Event* e) {
        this->upEvent = e;
    }

    void MenuCustom::setOnDown(Event* e) {
        this->downEvent = e;
    }

    void MenuCustom::setOnClicked(Event* e) {
        this->clickEvent = e;
    }

    void MenuCustom::setOnDoubleclicked(Event* e) {
        this->doubleClickEvent = e;
    }

    void MenuCustom::setOnHolding(Event* e) {
        this->holdEvent = e;
    }

    void MenuCustom::setOnUp(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        setOnUp(new Event(m, event_fnct));
    }

    void MenuCustom::setOnDown(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        setOnDown(new Event(m, event_fnct));
    }

    void MenuCustom::setOnClicked(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        setOnClicked(new Event(m, event_fnct));
    }

    void MenuCustom::setOnDoubleclicked(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        setOnDoubleclicked(new Event(m, event_fnct));
    }

    void MenuCustom::setOnHolding(Menu* m, SIMPLEMENU_EVENT_FNCT) {
        setOnHolding(new Event(m, event_fnct));
    }

    void MenuCustom::setOnUp(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        setOnUp(m, event_fnct);
    }

    void MenuCustom::setOnDown(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        setOnDown(m, event_fnct);
    }

    void MenuCustom::setOnClicked(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        setOnClicked(m, event_fnct);
    }

    void MenuCustom::setOnDoubleclicked(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        setOnDoubleclicked(m, event_fnct);
    }

    void MenuCustom::setOnHolding(SIMPLEMENU_EVENT_FNCT, Menu* m) {
        setOnHolding(m, event_fnct);
    }
}