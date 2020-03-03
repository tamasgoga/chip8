#ifndef GOGA_TAMAS_CHIP_8_CHIP8_HPP
#define GOGA_TAMAS_CHIP_8_CHIP8_HPP

#include "os.hpp"

namespace emu {
    struct State {
        int mem; // placeholder
    };

    class Program {
    public:
        using size_type = std::vector<u8>::size_type;
        const os::Arguments arguments;
        
        Program(const char* path, u32 options);
        Program(int argc, char** argv);

        void DumpHex() const;
        void Disassemble() const;

    private:
        std::vector<u8> program;
    };
}

#endif // GOGA_TAMAS_CHIP_8_CHIP8_HPP