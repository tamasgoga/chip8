#ifndef GOGA_TAMAS_CHIP_8_OS_HPP
#define GOGA_TAMAS_CHIP_8_OS_HPP

#include <string>
#include <vector>
#include "def.hpp"

// To the best of my knowledge, I will separate OS specific things here.
namespace os {
    class Arguments {
    public:
        u32         options = 0;
        std::string path    = "";

        Arguments(int count, char** args);

        Arguments(const char* path, i32 options = 0)
            : options(options)
            , path(path != nullptr ? path : "")
        {}
    };

    // We will assume that the file can be stored in memory all at once
    // Also, the program cannot have an odd number of bytes, as per the spec
    std::vector<u8> ReadChip8File(std::string path);
}

#endif // GOGA_TAMAS_CHIP_8_OS_HPP