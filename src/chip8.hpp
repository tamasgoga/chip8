#ifndef GOGA_TAMAS_CHIP_8_CHIP8_HPP
#define GOGA_TAMAS_CHIP_8_CHIP8_HPP

#include "os.hpp"
#include "instructions.hpp"

namespace ch8 {
    class Program {
    public:
        using size_type = std::vector<Instruction>::size_type;
        const os::Arguments arguments;
        
        Program(const char* path, u32 options);
        Program(int argc, char** argv);

        void DumpHex() const;
        void Disassemble() const;

    private:
        std::vector<std::unique_ptr<Instruction>> program;
        State state;
    };
}

#endif // GOGA_TAMAS_CHIP_8_CHIP8_HPP