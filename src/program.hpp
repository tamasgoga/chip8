#ifndef GOGA_TAMAS_CHIP_8_PROGRAM_HPP
#define GOGA_TAMAS_CHIP_8_PROGRAM_HPP

#include <memory>
#include "os.hpp"
#include "chip8.hpp"
#include "sdl.hpp"

namespace ch8 {
    class Program {
    public:
        using instruction_vector = std::vector<std::unique_ptr<Instruction>>;
        using size_type          = instruction_vector::size_type;

        const os::Arguments arguments;
        
        Program(ch8::Chip8& state, ch8::Interface& interface, const char* path, u32 options);
        Program(ch8::Chip8& state, ch8::Interface& interface, int argc, char** argv);

        void DumpHex() const noexcept;

        void Disassemble() noexcept;
        void Execute() noexcept;

    private:
        void ParseBytes(std::vector<u8> bytes);

        Chip8& state;
        Interface& interface;

        instruction_vector program;
    };
}

#endif // GOGA_TAMAS_CHIP_8_PROGRAM_HPP