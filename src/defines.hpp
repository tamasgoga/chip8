#ifndef GOGA_TAMAS_CHIP_8_DEFINES_HPP
#define GOGA_TAMAS_CHIP_8_DEFINES_HPP

#include <cstdint>

// Commonly used definitions.

using i8  = int_least8_t;
using i16 = int_least16_t;
using i32 = int_least32_t;
using i64 = int_least64_t;

using u8  = uint_least8_t;
using u16 = uint_least16_t;
using u32 = uint_least32_t;
using u64 = uint_least64_t;

namespace ch8 {
    enum PROGRAM_OPTIONS: u32 {
        OPTIONS_HEX    = 0x1,
        OPTIONS_CODE   = 0x2,
        OPTIONS_NOEXEC = 0x4
    };

    constexpr u16 MEM_START    = 0x200;
    constexpr u16 MEM_SIZE     = 0x1000;
    constexpr u16 SCREEN_START = 0xf00;
    constexpr u16 STACK_SIZE   = 0x10;
    constexpr u16 MAX_PROG_LEN = MEM_SIZE - MEM_START - (MEM_SIZE - SCREEN_START);
}

#endif // GOGA_TAMAS_CHIP_8_DEFINES_HPP