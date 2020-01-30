#include <iostream>

#include "chip8.hpp"

emu::Program::Program(const char* path, u32 options)
    : arguments(path, options)
{
    program = os::ReadChip8File(arguments.path);
}

emu::Program::Program(int argc, char** argv)
    : arguments(argc, argv)
{
    program = os::ReadChip8File(arguments.path);
}

void emu::Program::DumpHex() const {
    using std::cout;
    using std::endl;

    u32 lineBuf = 0;
    u32 opBuf = 1;

    for (u8 op: program) {
        if (lineBuf++ % 16 == 0) {
            cout << endl;
            opBuf = 1;
        } else if (opBuf++ % 2 == 0) {
            putchar(' ');
        }

        printf("%.2x", op);
    }

    cout << endl << endl;
}

void emu::Program::Disassemble() const {
    using std::cout;
    using std::endl;

    cout << endl;

    auto printInstr = [this] (size_type i, const char* name) {
        printf("%.2x%-8.2x %-10s ", program[i], program[i + 1], name);
    };

    for (size_type i = 0; i < program.size(); i += 2) {
        switch (program[i] >> 4) {
        case 0x00: printf("0 not handled yet"); break;
        case 0x01: {
                printInstr(i, "JMP");
                u16 addr = ((u16(program[i]) & 0x00f) << 8) | program[i + 1];
                printf("%.3x", addr);
            }
            break;
        case 0x02: printf("2 not handled yet"); break;
        case 0x03: printf("3 not handled yet"); break;
        case 0x04: printf("4 not handled yet"); break;
        case 0x05: printf("5 not handled yet"); break;
        case 0x06: {
                u8 reg = program[i] & 0x0f;
                i8 con = program[i + 1];
                printInstr(i, "MOV");
                printf("V%.2u, %d", reg, con);
            }
            break;
        case 0x07: {
                //7XNN	Const	Vx += NN	Adds NN to VX. (Carry flag is not changed)
                u8 reg = program[i] & 0x0f;
                i8 con = program[i + 1];
                printInstr(i, "ADD");
                printf("V%.2u, %d", reg, con);
            }
            break;
        case 0x08: printf("8 not handled yet"); break;
        case 0x09: printf("9 not handled yet"); break;
        case 0x0a: {
                u8 addresshi = program[i] & 0x0f;
                printf("%-10s I,#$%01x%02x", "MVI", addresshi, program[i + 1]);
            }
            break;
        case 0x0b: printf("b not handled yet"); break;
        case 0x0c: printf("c not handled yet"); break;
        case 0x0d: printf("d not handled yet"); break;
        case 0x0e: printf("e not handled yet"); break;
        case 0x0f: printf("f not handled yet"); break;
        }

        putchar('\n');
    }

    cout << endl;
}
