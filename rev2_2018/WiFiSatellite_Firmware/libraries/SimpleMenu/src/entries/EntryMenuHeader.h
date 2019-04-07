#ifndef EntryMenuHeader_h
#define EntryMenuHeader_h

#include "Entry.h"
#include "lib/strdo.h"

namespace simplemenu {
    class EntryMenuHeader : public Entry {
        public:
            EntryMenuHeader(Config* config, std::string name, Menu* parent = nullptr, std::string parent_name = "");

            virtual ~EntryMenuHeader();

            virtual void set_page(unsigned int page);

            virtual bool draw(unsigned int x, unsigned int y, bool selected);
            virtual std::string get_name() const;
            virtual std::string generate_name(int max_len, bool selected) const;

        protected:
            Menu* parent { nullptr };
            std::string name { "" };
            std::string parent_name { "" };
            std::string page_str { "1" };
    };
}

#endif /* ifndef EntryMenuHeader_h */