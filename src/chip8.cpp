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

static inline u8 get4BitOperandRight(u8 x) {
    return x & 0x0f;
}

static inline u8 get4BitOperandLeft(u8 x) {
    return (x & 0xf0) >> 4;
}

static inline u16 get16BitOperand (u8 x, u8 y) {
    return ((u16(x) & 0x00f) << 8) | y;
}

void emu::Program::Disassemble() const {
    using std::cout;
    using std::endl;

    cout << endl;

    u32 pc = 0;
    auto printInstr = [this, &pc] (size_type i, const char* name) {
        printf("%.8x   <%.2x%.2x>   %-7s ", pc, program[i], program[i + 1], name);
        pc += 2;
    };

    for (size_type i = 0; i < program.size(); i += 2) {
        switch (program[i] >> 4) {
        case 0x00: {
                printInstr(i, "0");
            }
            break;
        case 0x01: {
                // 1nnn: goto nnn; Jumps to address nnn.
                u16 nnn = get16BitOperand(program[i], program[i + 1]);
                printInstr(i, "JMP");
                printf("%.3x", nnn);
            }
            break;
        case 0x02: {
                // 2nnn: *(nnn)(); Calls subroutine at nnn.
                u16 nnn = get16BitOperand(program[i], program[i + 1]);
                printInstr(i, "CALL");
                printf("%0.3x", nnn);
            }
            break;
        case 0x03: {
                // 3xnn if(Vx == nn) Skips the next instruction if Vx equals nn.
                // Usually the next instruction is a jump to skip a code block. Applies for 4 % 5, too.
                u8 vx = get4BitOperandRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "SE");
                printf("V%.2u, %d", vx, nn);
            }
            break;
        case 0x04: {
                // 4xnn if(Vx != nn) Skips the next instruction if Vx doesn't equal nn.
                u8 vx = get4BitOperandRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "SNE");
                printf("V%.2u, %d", vx, nn);
            }
            break;
        case 0x05: {
                //5xy0 if(Vx == Vy) Skips the next instruction if Vx equals Vy.
                u8 rx = get4BitOperandRight(program[i]);
                u8 ry = get4BitOperandLeft(program[i + 1]);
                printInstr(i, "SRE");
                printf("V%.2u, V%.2u", rx, ry);
            }
            break;
        case 0x06: {
                // 6xnn: Vx = nn; Sets VX to NN.
                u8 vx = get4BitOperandRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "MOV");
                printf("V%.2u, %d", vx, nn);
            }
            break;
        case 0x07: {
                //7xnn: Vx += nn; Adds nn to Vx. (Carry flag is not changed)
                u8 vx = get4BitOperandRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "ADD");
                printf("V%.2u, %d", vx, nn);
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
                u16 nnn = get16BitOperand(program[i], program[i + 1]);
                printInstr(i, "MVI");
                printf("%.3x", nnn);
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
