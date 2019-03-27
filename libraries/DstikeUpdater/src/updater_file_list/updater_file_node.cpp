#include "updater_file_node.h"

u_file_node* create_u_file_node(const char* path) {
    u_file_node* n = (u_file_node*)malloc(sizeof(u_file_node));

    char* name = strrchr(path, '/');

    if (!name) name = (char*)path;

    n->name = (char*)malloc(strlen(name) + 1);
    n->path = (char*)malloc(strlen(path) + 1);
    n->next = NULL;

    strcpy(n->name, name);
    strcpy(n->path, path);

    return n;
}

void delete_u_file_node(u_file_node* n) {
    if (n) {
        free(n->name);
        free(n->path);
        delete_u_file_node(n->next);
        free(n);
    }
}