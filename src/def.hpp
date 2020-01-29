#ifndef GOGA_TAMAS_CHIP_8_DEF_HPP
#define GOGA_TAMAS_CHIP_8_DEF_HPP

#include <cstdint>

using i8  = int_least8_t;
using i16 = int_least16_t;
using i32 = int_least32_t;

using u8 = uint_least8_t;
using u16 = uint_least16_t;
using u32 = uint_least32_t;


namespace emu {
    enum PROGRAM_OPTIONS: u32 {
        OPTIONS_HEX  = 0x1,
        OPTIONS_CODE = 0x2
    };
}

#endif // GOGA_TAMAS_CHIP_8_DEF_HPP