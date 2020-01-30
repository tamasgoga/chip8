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

    u32 pc = 0;
    auto printInstr = [this, &pc] (size_type i, const char* name) {
        printf("%.8x   <%.2x%.2x>   %-7s ", pc, program[i], program[i + 1], name);
        pc += 0x10;
    };

    for (size_type i = 0; i < program.size(); i += 2) {
        switch (program[i] >> 4) {
        case 0x00: {
                printInstr(i, "0");
            }
            break;
        case 0x01: {
                // 1nnn: goto NNN; Jumps to address nnn.
                printInstr(i, "JMP");
                u16 addr = ((u16(program[i]) & 0x00f) << 8) | program[i + 1];
                printf("%.3x", addr);
            }
            break;
        case 0x02: {
                printInstr(i, "2");
            }
            break;
        case 0x03: {
                printInstr(i, "3");
            }
            break;
        case 0x04: {
                printInstr(i, "4");
            }
            break;
        case 0x05: {
                printInstr(i, "5");
            }
            break;
        case 0x06: {
                // 6xnn: Vx = nn; Sets VX to NN.
                u8 reg = program[i] & 0x0f;
                i8 con = program[i + 1];
                printInstr(i, "MOV");
                printf("V%.2u, %d", reg, con);
            }
            break;
        case 0x07: {
                //7xnn: Vx += nn; Adds nn to Vx. (Carry flag is not changed)
                u8 reg = program[i] & 0x0f;
                i8 con = program[i + 1];
                printInstr(i, "ADD");
                printf("V%.2u, %d", reg, con);
            }
            break;
        case 0x08: {
                printInstr(i, "8");
            }
            break;
        case 0x09: {
                printInstr(i, "9");
            }
            break;
        case 0x0a: {
                //Annn: i = nnn; Sets i to the address nnn.
                u16 addr = ((u16(program[i]) & 0x00f) << 8) | program[i + 1];
                printInstr(i, "MVI");
                printf("%.3x", addr);
            }
            break;
        case 0x0b: {
                printInstr(i, "b");
            }
            break;
        case 0x0c: {
                printInstr(i, "c");
            }
            break;
        case 0x0d: {
                printInstr(i, "d");
            }
            break;
        case 0x0e: {
                printInstr(i, "e");
            }
            break;
        case 0x0f: {
                printInstr(i, "f");
            }
            break;
        }

        putchar('\n');
    }

    cout << endl;
}
