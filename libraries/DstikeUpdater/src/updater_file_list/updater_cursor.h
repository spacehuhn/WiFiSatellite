#ifndef updater_cursor_h
#define updater_cursor_h

#include <stdlib.h>
#include "updater_file_list/updater_file_list.h"

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

typedef struct u_cursor {
    int                 page;
    int                 pos;
    struct u_file_list* list;
} u_cursor;

u_cursor* create_u_cursor(u_file_list* l);
void delete_u_cursor(u_cursor* c);
void u_cursor_up(u_cursor* cursor, int entries_per_page);
void u_cursor_down(u_cursor* cursor, int entries_per_page);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef updater_cursor_h */