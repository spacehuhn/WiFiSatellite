#include "updater_file_list_mgmt.h"

u_file_list* get_u_file_list(fs::FS& fs, const char* path) {
    u_file_list* list = create_u_file_list();
    File dir          = fs.open(path);

    if (!dir) return list;

    u_file_node* tmp_file = create_u_file_node("Cancel");
    list->begin = tmp_file;
    list->end   = tmp_file;
    list->size++;

    File f = dir.openNextFile();

    while (f) {
        if (!f.isDirectory() && (strcmp(strrchr(f.name(), '.'), ".bin") == 0)) {
            tmp_file        = create_u_file_node(f.name());
            list->end->next = tmp_file;
            list->end       = tmp_file;
            list->size++;
        }

        f = dir.openNextFile();
    }

    return list;
}

const char* get_file_path(u_cursor* cursor, int entries_per_page) {
    if (cursor && cursor->list) {
        u_file_node* tmp_node = cursor->list->begin;
        int i                 = 0;
        int j                 = 0;

        while (tmp_node && i < cursor->page * entries_per_page) {
            tmp_node = tmp_node->next;
            ++i;
        }

        while (tmp_node && j < cursor->pos) {
            tmp_node = tmp_node->next;
            ++j;
        }

        if (tmp_node) return tmp_node->path;
    }
    return NULL;
}