#ifndef updater_file_list_mgmt_h
#define updater_file_list_mgmt_h

#include <FS.h> // File System

#include "updater_file_list/updater_file_node.h"
#include "updater_file_list/updater_file_list.h"
#include "updater_file_list/updater_cursor.h"

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

u_file_list* get_u_file_list(fs::FS& fs, const char* path);
const char* get_file_path(u_cursor* cursor, int entries_per_page);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef updater_file_list_mgmt_h */