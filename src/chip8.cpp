#include "chip8.hpp"
#include <cstdio>

// Constants

static constexpr u16 MEM_START = 0x200;

// C-tors

ch8::Program::Program(const char* path, u32 options)
    : arguments(path, options)
{
    program = ParseBytes(state, os::ReadChip8File(arguments.path));
}

ch8::Program::Program(int argc, char** argv)
    : arguments(argc, argv)
{
    program = ParseBytes(state, os::ReadChip8File(arguments.path));
}

// Dump program hexa
void ch8::Program::DumpHex() const {
    if (program.size() < 2) {
        return;
    }

    auto iter = program.cbegin();
    auto handle = iter->get();
    printf("%.4x:   %.2x%.2x ", MEM_START, handle->l, handle->r);
    iter++;
    u16 opCount = 1u;

    for (; iter != program.cend(); iter++, opCount++) {
        handle = iter->get();

        if (opCount % 8 == 0u) {
            printf("\n%.4x:   ", MEM_START + opCount * 2u);
        }

        printf("%.2x%.2x ", handle->l, handle->r);
    }

    putchar('\n');
}

// Disassemble program
void ch8::Program::Disassemble() const {
    for (size_type i = 0; i < program.size(); ++i) {
        program[i]->Disassemble(i);
        putchar('\n');
    }
}