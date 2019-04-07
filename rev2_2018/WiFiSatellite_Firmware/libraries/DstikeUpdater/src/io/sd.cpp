#include "sd.h"

fs::FS& get_sd_fs() {
    return SD_MMC;
}

bool init_sd() {
    if (!SD_MMC.begin()) return false;
    if (SD_MMC.cardType() == CARD_NONE) return false;

    return true;
}