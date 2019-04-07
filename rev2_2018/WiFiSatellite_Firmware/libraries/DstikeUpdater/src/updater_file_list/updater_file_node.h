#ifndef updater_file_node_h
#define updater_file_node_h

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

typedef struct u_file_node {
    char              * name;
    char              * path;
    struct u_file_node* next;
} u_file_node;

u_file_node* create_u_file_node(const char* path);
void delete_u_file_node(u_file_node* n);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef updater_file_node_h */