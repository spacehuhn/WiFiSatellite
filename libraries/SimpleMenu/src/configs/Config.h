#ifndef SimpleMenu_Config_h
#define SimpleMenu_Config_h

#include <functional> // std::function
#include <string>     // std::string

namespace simplemenu {
    class Config {
        public:
            virtual ~Config();

            virtual void set_fps(unsigned int fps);
            virtual void set_res(unsigned int width, unsigned int height);
            virtual void set_width(unsigned int width);
            virtual void set_height(unsigned int height);
            virtual void set_entries_per_page(unsigned int entries_per_page);
            virtual void set_entry_height(unsigned int entry_height);
            virtual void set_max_len(unsigned int max_len);

            virtual unsigned int get_fps() const;
            virtual unsigned int get_width() const;
            virtual unsigned int get_height() const;
            virtual unsigned int get_entries_per_page() const;
            virtual unsigned int get_entry_height() const;
            virtual unsigned int get_max_len() const;

            virtual bool is_on() const;

            virtual void exec_prefix() = 0;
            virtual void exec_suffix() = 0;

            virtual void draw(unsigned int x, unsigned int y, std::string s)                                             = 0;
            virtual void draw_entry(unsigned int x, unsigned int y, bool selected, std::string s)                        = 0;
            virtual void draw_header(unsigned int x, unsigned int y, bool selected, std::string s)                       = 0;
            virtual void draw_pixel(unsigned int x, unsigned int y, bool on)                                             = 0;
            virtual void draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)                   = 0;
            virtual void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled) = 0;
            virtual void draw_circle(unsigned int x, unsigned int y, unsigned int radius, bool filled)                   = 0;

            virtual void on();
            virtual void off();

        protected:
            unsigned int fps             { 30 };
            unsigned int width            { 0 };
            unsigned int height           { 0 };
            unsigned int entries_per_page { 1 };
            unsigned int entry_height     { 0 };
            unsigned int max_len          { 0 };

            bool turned_on { true };

            unsigned int fix_x(unsigned int x);
            unsigned int fix_y(unsigned int y);
    };
}
#endif /* ifndef SimpleMenu_Config_h */