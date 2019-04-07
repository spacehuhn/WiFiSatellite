#include "SDPacketWriter.h"

SDPacketWriter::SDPacketWriter() {}

SDPacketWriter::~SDPacketWriter() {
    end();
}

bool SDPacketWriter::begin() {
    if (!SD_MMC.begin()) {
        cout << "SD/MMC mount failed" << '\r' << '\n';
        pause(true);
        return false;
    }

    if (SD_MMC.cardType() == CARD_NONE) {
        cout << "No SD/MMC card attached" << '\r' << '\n';
        pause(true);
        return false;
    }

    if (buffer) free(buffer);
    buffer = (uint8_t*)heap_caps_malloc(BUFFER_SIZE, MALLOC_CAP_DMA);

    update_file_path();

    cout << "Started SD/MMC writer" << '\r' << '\n';

    return true;
}

bool SDPacketWriter::end() {
    flush();
    file.close();
    SD_MMC.end();
    paused = true;
    if (buffer) free(buffer);
    buffer = nullptr;
    cout << "Stopped SD/MMC writer" << '\r' << '\n';

    return true;
}

bool SDPacketWriter::pause(bool error) {
    if (!error) flush();
    file.close();
    cout  << "SD card writer paused"  << '\r' << '\n';
    return PacketWriter::pause(error);
}

bool SDPacketWriter::resume() {
    cout  << "SD card writer resumed"  << '\r' << '\n';
    id_minor++;
    update_file_path();
    return PacketWriter::resume();
}

uint64_t SDPacketWriter::used_bytes() const {
    return SD_MMC.usedBytes();
}

uint64_t SDPacketWriter::free_bytes() const {
    return SD_MMC.totalBytes() - SD_MMC.usedBytes();
}

uint64_t SDPacketWriter::total_bytes() const {
    return SD_MMC.totalBytes();
}

uint64_t SDPacketWriter::get_file_size() const {
    return this->file_size;
}

int64_t SDPacketWriter::get_file_time() const {
    return this->file_time;
}

double SDPacketWriter::get_speed() const {
    if ((file_size == 0) || (file_time == 0)) return 0;

    double bytes = file_size;
    double us    = file_time;

    double speed = file_size/us;

    return (int)(speed * 1000.0)/1000.0;
}

bool SDPacketWriter::header_enabled() const {
    return buffer_size == 0 && file_size == 0;
}

bool SDPacketWriter::exists(const char* path) const {
    return SD_MMC.exists(path);
}

bool SDPacketWriter::write(uint8_t* data, uint64_t size) {
    if (paused) return false;

    if (buffer_size + size >= BUFFER_SIZE) {
        uint64_t remaining_size = BUFFER_SIZE - buffer_size;

        memcpy(&buffer[buffer_size], data, remaining_size);
        data  = &data[remaining_size];
        size -= remaining_size;

        buffer_size += remaining_size;

        if (!flush()) return false;
    }

    memcpy(&buffer[buffer_size], data, size);
    buffer_size += size;

    return true;
}

bool SDPacketWriter::flush() {
    if (!file) {
        cout  << "ERROR opening file"  << '\r' << '\n';
        pause(true);
        return false;
    }

    int64_t write_time = esp_timer_get_time();

    bool success = file.write(buffer, buffer_size);

    if (success) {
        file_size += buffer_size;
        file_time += esp_timer_get_time() - write_time;

        if (file_size >= MAX_PCAP_FILE_SIZE) {
            id_minor++;
            update_file_path();
        }

        buffer_size = 0;

        return true;
    } else {
        cout  << "ERROR writing file"  << '\r' << '\n';
        pause(true);
        return false;
    }
}

string SDPacketWriter::create_file_path() const {
    stringstream ss;

    ss << "/capture_" << id_major << '_' << id_minor << ".pcap";
    return ss.str();
}

void SDPacketWriter::update_file_path() {
    file.close();

    file_path = create_file_path();

    while (exists(file_path.c_str())) {
        id_major++;
        id_minor  = 0;
        file_path = create_file_path();
    }

    file      = SD_MMC.open(file_path.c_str(), FILE_APPEND);
    file_size = 0;
    file_time = 0;

    cout << "Writing to " << file_path << "\r\n";
}