#ifndef SimpleMenu_EntryValue_h
#define SimpleMenu_EntryValue_h

#include <sstream>     // std::stringstream
#include "lib/strdo.h" // strdo::build
#include "entries/Entry.h"

namespace simplemenu {
    class EntryValue : public Entry {
        public:
            EntryValue(Config* config, std::string name, SIMPLEMENU_EVENT_FNCT, Menu* m = nullptr);
            EntryValue(Config* config, std::string name, Menu* m, SIMPLEMENU_EVENT_FNCT = nullptr);
            EntryValue(Config* config, std::string name, Event* click                   = nullptr, Event* doublclick = nullptr, Event* hold = nullptr);

            virtual ~EntryValue();

            template<typename T>
            std::string get_value(T value) {
                std::stringstream ss;

                ss << value;
                std::string value_str = ss.str();

                return ss.str();
            }

            template<typename T, typename ... Args>
            std::string get_value(T value, Args ... args) {
                return get_value(value) + get_value(args ...);
            }

            template<typename ... Args>
            void set_value(Args ... args) {
                set_name(strdo::build(prefix, "", get_value(args ...), config->get_max_len()));
            }

            template<typename T>
            void set_num_value(T value) {
                set_value(strdo::num_to_str(value));
            }

            template<typename T>
            void set_time_value(T value) {
                set_value(strdo::time_to_str(value));
            }

            virtual void set_name(std::string name);
            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;

        protected:
            std::string prefix {};
            std::string suffix {};
            std::string name {};
    };
}
#endif // ifndef SimpleMenu_EntryValue_h