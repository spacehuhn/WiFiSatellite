#include "DstikeUpdater.h"

// Including things here, so they are not accessible from outside
#include "memory/updater_file_list_mgmt.h" // get_u_file_list(), get_file_path()
#include "memory/flasher.h"                // flash_bin()
#include "io/gpio.h"                       // init_buttons(), pressed(), pressed_any()
#include "io/sd.h"                         // init_sd(), get_sd_fs()
#include "io/display.h"                    // init_display()

// ========== INTERNAL ========== //
void display_u_file_list(OLEDDisplay& display, u_file_list* list, u_cursor* cursor) {
    if (list && cursor) {
        int i          = 0;
        int j          = 0;
        u_file_node* n = list->begin;

        // go to page of cursor
        while (n && i < cursor->page * 4) {
            n = n->next;
            ++i;
        }

        // draw entries
        while (n && j < 4) {
            String entry_str(j == cursor->pos ? '>' : ' ');
            entry_str += String(n->name);

            draw_string(display, j, entry_str);
            n = n->next;
            ++i;
            ++j;
        }

        display_show(display);
    }
}

bool loading_screen(OLEDDisplay& display, int up, int down, int select, unsigned long loading_time) {
    unsigned long start_time = millis();
    unsigned long end_time   = start_time + loading_time;

    while (millis() < end_time) {
        if (pressed_any(up, down, select)) return true;

        draw_bar(display, (millis() - start_time) / (loading_time/128));
        display_show(display);

        delay(50);
    }

    return false;
}

bool update_u_cursor(u_cursor* cursor, int up, int down, int entries_per_page) {
    if (pressed(up)) {
        u_cursor_up(cursor, entries_per_page);
        return true;
    }

    if (pressed(down)) {
        u_cursor_down(cursor, entries_per_page);
        return true;
    }

    return false;
}

// ========== EXTERNAL ========== //
void DstikeUpdater::run(OLEDDisplay& display, int up, int down, int select, const char* path, int loadingDelay) {
    init_display(display);          // Initialize display
    init_buttons(up, down, select); // Initialize buttons as INPUT_PULLUP

    display_clear(display);
    draw_headline(display);

    // Check if SD is connected
    if (!init_sd()) { // Initialize SD/MMC Card
        draw_no_sd(display);
        draw_loading_bar(display, loadingDelay);
        display_clear(display);

        return;
    }

    draw_update(display);
    display_show(display);

    // Show loading screen and check for button press
    if (!loading_screen(display, up, down, select, loadingDelay)) {
        display_clear(display);
        return;
    };

    fs::FS& fs      = get_sd_fs();
    u_file_list* fl = get_u_file_list(fs, path); // load all bin files into a list
    u_cursor   * c  = create_u_cursor(fl);       // create a cursor

    // show file list on display
    display_clear(display);
    draw_headline(display);
    display_u_file_list(display, fl, c);
    display_show(display);

    // Wait until buttons are released
    while (pressed_any(up, down, select)) delay(10);

    // run updater
    bool running          = true;
    const char* file_path = NULL;

    while (running) {
        // Up and Down
        if (update_u_cursor(c, up, down, 4)) {
            display_clear(display);
            draw_headline(display);
            display_u_file_list(display, fl, c);
            display_show(display);
            delay(200);
        }

        // Select
        if (pressed(select)) {
            file_path = get_file_path(c, 4);

            if (strcmp(file_path, "Cancel") == 0) {
                running = false;

                display_clear(display);
                draw_headline(display);
                draw_canceled(display);
                draw_loading_bar(display, loadingDelay);
                display_show(display);
            } else {
                display_clear(display);
                draw_headline(display);
                draw_flashing(display);
                display_show(display);

                draw_string_f(display, 1, flash_bin(fs, file_path));
                draw_rebooting(display);
                draw_loading_bar(display, loadingDelay);

                ESP.restart();
            }
        }
    }

    // free memory of file list and cursor
    delete_u_file_list(fl);
    delete_u_cursor(c);

    display_clear(display);
}

void DstikeUpdater::runSH1106(int sda, int sck, int up, int down, int select, const char* path, int loadingDelay) {
    SH1106Wire display(0x3c, sda, sck);

    run(display, up, down, select, path, loadingDelay);
    display.end();
}

void DstikeUpdater::runSSD1306(int sda, int sck, int up, int down, int select, const char* path, int loadingDelay) {
    SSD1306 display(0x3c, sda, sck);

    run(display, up, down, select, path, loadingDelay);
    display.end();
}