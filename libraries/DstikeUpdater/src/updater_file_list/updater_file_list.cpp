#include "updater_file_list.h"

u_file_list* create_u_file_list() {
    u_file_list* l = (u_file_list*)malloc(sizeof(u_file_list));

    l->begin = NULL;
    l->end   = NULL;
    l->size  = 0;
    return l;
}

void delete_u_file_list(u_file_list* l) {
    if (l) {
        delete_u_file_node(l->begin);
        free(l);
    }
}