#ifndef bufferio_h
#define bufferio_h

#include <cstdint>

namespace bufferio {
	inline uint32_t write8(uint8_t* buffer, uint8_t value) {
		buffer[0] = value;
		return 1;
	}

	inline uint32_t write16(uint8_t* buffer, uint16_t value) {
		buffer[0] = uint8_t(value);
		buffer[1] = uint8_t(value >> 8);
		return 2;
	}

	inline uint32_t write32(uint8_t* buffer, uint32_t value) {
		buffer[0] = uint8_t(value);
		buffer[1] = uint8_t(value >> 8);
		buffer[2] = uint8_t(value >> 16);
		buffer[3] = uint8_t(value >> 24);
		return 4;
	}
}

#endif /* ifndef BufferIO_h */