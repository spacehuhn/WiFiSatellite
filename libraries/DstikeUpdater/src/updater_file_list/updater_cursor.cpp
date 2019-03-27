#include "updater_cursor.h"

u_cursor* create_u_cursor(u_file_list* l) {
    if (!l) return NULL;
    u_cursor* c = (u_cursor*)malloc(sizeof(u_cursor));
    c->page = 0;
    c->pos  = 0;
    c->list = l;
    return c;
}

void delete_u_cursor(u_cursor* c) {
    free(c);
}

void u_cursor_up(u_cursor* cursor, int entries_per_page) {
    if (cursor && ((cursor->page > 0) || (cursor->pos > 0))) {
        cursor->pos--;
        if (cursor->pos < 0) {
            cursor->pos = entries_per_page - 1;
            cursor->page--;
        }
    }
}

void u_cursor_down(u_cursor* cursor, int entries_per_page) {
    if (cursor && cursor->list && (cursor->page * entries_per_page + cursor->pos < cursor->list->size - 1)) {
        cursor->pos++;
        if (cursor->pos >= entries_per_page) {
            cursor->pos = 0;
            cursor->page++;
        }
    }
}