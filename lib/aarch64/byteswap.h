#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#include <features.h>
#include <stdint.h>

static __inline uint16_t __bswap_16(uint16_t __x) {
    return (__x << 8) | (__x >> 8);
}

static __inline uint32_t __bswap_24(uint32_t __x) {
    return (__x & 0xFF00) | (((__x & 0xFF) << 16) & 0xFFFFFF00) | ((__x >> 16) & 0xFF);
}

static __inline uint32_t __bswap_32_ignore_last(uint32_t __x) {
    return (__x >> 24) | (__x >> 8 & 0xff00) | (__x << 8 & 0xff0000);
}

static __inline uint32_t __bswap_32(uint32_t __x) {
    return (__x >> 24) | (__x >> 8 & 0xff00) | (__x << 8 & 0xff0000) | (__x << 24);
}

static __inline uint64_t __bswap_32_64(uint64_t x) {
    return ((((x)&0xff00000000000000ull) >> 56) | (((x)&0x00ff000000000000ull) >> 40) |
            (((x)&0x0000ff0000000000ull) >> 24) | (((x)&0x000000ff00000000ull) >> 8) |
            (((x)&0x00000000ff000000ull) << 8) | (((x)&0x0000000000ff0000ull) << 24) |
            (((x)&0x000000000000ff00ull) << 40) | (((x)&0x00000000000000ffull) << 56));
}

static __inline uint64_t __bswap_64(uint64_t __x) {
    return ((__bswap_32(__x) + 0ULL) << 32) | __bswap_32(__x >> 32);
}

#define bswap_16(x) __bswap_16(x)
#define bswap_24(x) __bswap_24(x)
#define bswap_32_ignore_last(x) __bswap_32_ignore_last(x)
#define bswap_32(x) __bswap_32(x)
#define bswap_32_64(x) __bswap_32_64(x)
#define bswap_64(x) __bswap_64(x)

#endif
