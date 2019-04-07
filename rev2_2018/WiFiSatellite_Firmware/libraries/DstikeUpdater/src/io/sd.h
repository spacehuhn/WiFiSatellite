#ifndef sd_h
#define sd_h

#include <FS.h>     // File System
#include <SD_MMC.h> // SD/MMC Card

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

fs::FS& get_sd_fs();
bool init_sd();

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef sd_h */