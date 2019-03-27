#ifndef updater_file_list_h
#define updater_file_list_h


#include <stdlib.h>
#include "updater_file_list/updater_file_node.h"

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

typedef struct u_file_list {
    struct u_file_node* begin;
    struct u_file_node* end;
    int                 size;
} u_file_list;

u_file_list* create_u_file_list();
void delete_u_file_list(u_file_list* l);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef updater_file_list_h */