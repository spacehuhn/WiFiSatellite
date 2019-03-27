#ifndef MenuGraph_h
#define MenuGraph_h

#include <list>         // std::list
#include <string>       // std::string
#include <sstream>      // std::stringstream

#include <menus/Menu.h> // Menu parent class
#include <lib/strdo.h>  // strdo::num_to_str


namespace simplemenu {
    class MenuGraph: public Menu {
        public:
            MenuGraph(Menu& parent);
            ~MenuGraph();

            void set_mode(std::string mode);

            void set_width(size_t size);
            void push(uint32_t);
            //void pop();

			void reset_max();
            void set_max(uint32_t max);
            uint32_t get_max() const;

            bool draw(bool cursor);
            void reset();

            Menu* up();
            Menu* down();
            Menu* click();
            Menu* doubleclick();
            Menu* hold();

            void set_up(Menu* m);
            void set_down(Menu* m);

        private:
            uint16_t max_y { 0 };
            std::list<uint32_t> data {};
			uint32_t max{ 0 };
			uint32_t overwrite_max{ 0 };
            double   multiplicator { 1.0 };
            std::string mode { "" };

            Menu* up_menu { nullptr };
            Menu* down_menu { nullptr };

            double calc_multiplicator();
            std::string build_headline();
    };
}
 #endif /* ifndef MenuGraph_h */