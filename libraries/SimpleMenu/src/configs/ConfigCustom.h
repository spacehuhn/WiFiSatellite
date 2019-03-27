#ifndef SimpleMenu_ConfigCustom_h
#define SimpleMenu_ConfigCustom_h

#include "Config.h"

 #define SIMPLEMENU_DISPLAY_ON         std::function<void()>                                                                                     on_f
 #define SIMPLEMENU_DISPLAY_OFF        std::function<void()>                                                                                     off_f
 #define SIMPLEMENU_DISPLAY_PREFIX     std::function<void()>                                                                                     prefix_f
 #define SIMPLEMENU_DISPLAY_SUFFIX     std::function<void()>                                                                                     suffix_f
 #define SIMPLEMENU_DISPLAY_DRAW       std::function<void(unsigned int x, unsigned int y, std::string s)>                                        draw_str_f
 #define SIMPLEMENU_DISPLAY_HEADER     std::function<void(unsigned int x, unsigned int y, bool selected, std::string s)>                         draw_header_f
 #define SIMPLEMENU_DISPLAY_ENTRY      std::function<void(unsigned int x, unsigned int y, bool selected, std::string s)>                         draw_entry_f
 #define SIMPLEMENU_DISPLAY_PIXEL      std::function<void(unsigned int x, unsigned int y, bool on)>                                              draw_pixel_f
 #define SIMPLEMENU_DISPLAY_LINE       std::function<void(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)>                   draw_line_f
 #define SIMPLEMENU_DISPLAY_RECT       std::function<void(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled)> draw_rect_f
 #define SIMPLEMENU_DISPLAY_CIRCLE     std::function<void(unsigned int x, unsigned int y, unsigned int radius, bool filled)>                     draw_circle_f

namespace simplemenu {
    class ConfigCustom : public Config {
        public:
            ConfigCustom();
            ConfigCustom(unsigned int width, unsigned int height, unsigned int entries_per_page, unsigned int max_len);

            virtual ~ConfigCustom();

            virtual void set_display_on(SIMPLEMENU_DISPLAY_ON);
            virtual void set_display_off(SIMPLEMENU_DISPLAY_OFF);
            virtual void set_update_prefix(SIMPLEMENU_DISPLAY_PREFIX);
            virtual void set_update_suffix(SIMPLEMENU_DISPLAY_SUFFIX);
            virtual void set_draw_str(SIMPLEMENU_DISPLAY_DRAW);
            virtual void set_draw_header(SIMPLEMENU_DISPLAY_HEADER);
            virtual void set_draw_entry(SIMPLEMENU_DISPLAY_ENTRY);
            virtual void set_draw_pixel(SIMPLEMENU_DISPLAY_PIXEL);
            virtual void set_draw_line(SIMPLEMENU_DISPLAY_LINE);
            virtual void set_draw_rect(SIMPLEMENU_DISPLAY_RECT);
            virtual void set_draw_circle(SIMPLEMENU_DISPLAY_CIRCLE);

            virtual void exec_prefix();
            virtual void exec_suffix();

            virtual void draw(unsigned int x, unsigned int y, std::string s);
            virtual void draw_header(unsigned int x, unsigned int y, bool selected, std::string s);
            virtual void draw_entry(unsigned int x, unsigned int y, bool selected, std::string s);
            virtual void draw_pixel(unsigned int x, unsigned int y, bool on);
            virtual void draw_line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
            virtual void draw_rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height, bool filled);
            virtual void draw_circle(unsigned int x, unsigned int y, unsigned int radius, bool filled);

            virtual void on();
            virtual void off();

        protected:
            SIMPLEMENU_DISPLAY_ON         { nullptr };
            SIMPLEMENU_DISPLAY_OFF        { nullptr };
            SIMPLEMENU_DISPLAY_PREFIX     { nullptr };
            SIMPLEMENU_DISPLAY_SUFFIX     { nullptr };
            SIMPLEMENU_DISPLAY_DRAW       { nullptr };
            SIMPLEMENU_DISPLAY_HEADER     { nullptr };
            SIMPLEMENU_DISPLAY_ENTRY      { nullptr };
            SIMPLEMENU_DISPLAY_PIXEL      { nullptr };
            SIMPLEMENU_DISPLAY_LINE       { nullptr };
            SIMPLEMENU_DISPLAY_RECT       { nullptr };
            SIMPLEMENU_DISPLAY_CIRCLE     { nullptr };
    };
}
#endif /* ifndef SimpleMenu_ConfigCustom_h */
