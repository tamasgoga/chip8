#ifndef GOGA_TAMAS_CHIP_8_CHIP8_HPP
#define GOGA_TAMAS_CHIP_8_CHIP8_HPP

#include "os.hpp"
#include "instructions.hpp"

namespace ch8 {
    class Program {
    public:
        using instruction_vector = std::vector<std::unique_ptr<Instruction>>;
        using size_type          = instruction_vector::size_type;

        const os::Arguments arguments;
        
        Program(ch8::Chip8& state, const char* path, u32 options);
        Program(ch8::Chip8& state, int argc, char** argv);

        void DumpHex() const noexcept;

        void Disassemble() noexcept;
        void Execute() noexcept;

    private:
        void ParseBytes(std::vector<u8> bytes);

        Chip8& state;
        instruction_vector program;
    };
}

#endif // GOGA_TAMAS_CHIP_8_CHIP8_HPP