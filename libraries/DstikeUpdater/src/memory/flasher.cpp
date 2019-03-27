#include "flasher.h"

const __FlashStringHelper* flash_bin(fs::FS& fs, const char* path) {
    File bin = fs.open(path);

    if (!bin) {
        bin.close();
        return F("File could not be opened");
    }

    if (bin.isDirectory()) {
        bin.close();
        return F("File is a directory");
    }

    size_t  updateSize   = bin.size();
    Stream& updateSource = bin;

    if (!Update.begin(updateSize)) {
        bin.close();
        return F("Not enough space to update");
    }

    size_t written = Update.writeStream(updateSource);

    if (!Update.end()) {
        bin.close();
        return F("Error"); // Update.getError();
    }

    if (!Update.isFinished()) {
        bin.close();
        return F("Couldn't finish update");
    }

    bin.close();
    return F("Update finished");
}