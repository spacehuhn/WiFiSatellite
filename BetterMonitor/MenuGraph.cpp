#include "MenuGraph.h"

namespace simplemenu {
    MenuGraph::MenuGraph(Menu& parent) {
        set_parent_menu(&parent);
        set_config(parent.get_config());

        set_width(config->get_width());

        max_y = config->get_height() - config->get_entry_height() - 1;
        max   = max_y;
    }

    MenuGraph::~MenuGraph() {}

    void MenuGraph::set_mode(std::string mode) {
        this->mode = mode;
    }

    void MenuGraph::set_width(size_t size) {
        data.resize(size, 0);
    }

    void MenuGraph::push(uint32_t value) {
        data.pop_front();

        data.push_back(value);

        max = 1;

        for (auto x : data)
            if (x > max) max = x;

        multiplicator = calc_multiplicator();
    }

    void MenuGraph::reset_max() {
        overwrite_max = 1;
        max           = 1;

        for (auto x : data)
            if (x > max) max = x;

        multiplicator = calc_multiplicator();
    }

    void MenuGraph::set_max(uint32_t max) {
        this->overwrite_max = max;
        this->multiplicator = calc_multiplicator();
    }

    uint32_t MenuGraph::get_max() const {
        return this->max;
    }

    bool MenuGraph::draw(bool cursor) {
        std::string headline = build_headline();

        config->draw(0, 0, headline);

        int x { 0 };
        int y { config->get_height() - 1 };
        int height { 0 };

        for (auto value : data) {
            height = (value > 0) ? value : 1;

            height *= multiplicator;
            if (height == max_y) height--;

            config->draw_line(x, y, x, y - height);

            ++x;
        }
    }

    void MenuGraph::reset() {}

    Menu* MenuGraph::up() {
        return up_menu;
    }

    Menu* MenuGraph::down() {
        return down_menu;
    }

    Menu* MenuGraph::click() {
        return this->parent;
    }

    Menu* MenuGraph::doubleclick() {
        return nullptr;
    }

    Menu* MenuGraph::hold() {
        return nullptr;
    }

    void MenuGraph::set_up(Menu* m) {
        this->up_menu = m;
    }

    void MenuGraph::set_down(Menu* m) {
        this->down_menu = m;
    }

    double MenuGraph::calc_multiplicator() {
        uint32_t tmp_max = this->max;

        if (this->overwrite_max > tmp_max) tmp_max = overwrite_max;

        if (tmp_max > max_y)
            return (double)max_y / (double)tmp_max;
        else
            return 1;
    }

    std::string MenuGraph::build_headline() {
        unsigned int len { config->get_width() };
        unsigned int spaces { len };

        std::string left { this->mode };
        std::string right {};
        std::string result {};

        uint32_t value = *(--data.end());

        right = strdo::num_to_str(value);

        spaces -= left.size() + right.size();

        result += left;

        while (spaces > 0) {
            result += ' ';
            spaces--;
        }

        result += right;

        return result.substr(0, len);
    }
}