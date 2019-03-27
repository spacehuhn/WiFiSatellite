#ifndef flasher_h
#define flasher_h

#include <FS.h>     // File System
#include <Update.h> // Update

#ifdef __cplusplus
extern "C" {
#endif /* ifdef __cplusplus */

const __FlashStringHelper* flash_bin(fs::FS& fs, const char* path);

#ifdef __cplusplus
}
#endif /* ifdef __cplusplus */

#endif /* ifndef flasher_h */