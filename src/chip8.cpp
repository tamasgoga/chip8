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

static inline u8 get4BitAddressRight(u8 x) {
    return x & 0x0f;
}

static inline u8 get4BitAddressLeft(u8 x) {
    return (x & 0xf0) >> 4;
}

static inline u16 get16BitAddress (u8 x, u8 y) {
    return ((u16(x) & 0x00f) << 8) | y;
}

void emu::Program::Disassemble() const {
    using std::cout;
    using std::endl;

    cout << endl;

    u32 pc = 0x200;
    auto printInstr = [this, &pc] (size_type i, const char* name) {
        printf("%.8x   <%.2x%.2x>   %-8s ", pc, program[i], program[i + 1], name);
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
                u16 nnn = get16BitAddress(program[i], program[i + 1]);
                printInstr(i, "JMP");
                printf("%X", nnn);
            }
            break;
        case 0x02: {
                // 2nnn: *(nnn)(); Calls subroutine at nnn.
                u16 nnn = get16BitAddress(program[i], program[i + 1]);
                printInstr(i, "CALL");
                printf("%X", nnn);
            }
            break;
        case 0x03: {
                // 3xnn: if(Vx == nn); Skips the next instruction if Vx equals nn.
                // Usually the next instruction is a jump to skip a code block. Applies for 4, 5 & 9, too.
                u8 vx = get4BitAddressRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "SE");
                printf("V%X, %d", vx, nn);
            }
            break;
        case 0x04: {
                // 4xnn: if(Vx != nn); Skips the next instruction if Vx doesn't equal nn.
                u8 vx = get4BitAddressRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "SNE");
                printf("V%X, %d", vx, nn);
            }
            break;
        case 0x05: {
                // 5xy0: if(Vx == Vy); Skips the next instruction if Vx equals Vy.
                u8 vx = get4BitAddressRight(program[i]);
                u8 vy = get4BitAddressLeft(program[i + 1]);
                printInstr(i, "SRE");
                printf("V%X, V%X", vx, vy);
            }
            break;
        case 0x06: {
                // 6xnn: Vx = nn; Sets VX to NN.
                u8 vx = get4BitAddressRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "MOV");
                printf("V%X, %d", vx, nn);
            }
            break;
        case 0x07: {
                // 7xnn: Vx += nn; Adds nn to Vx. (Carry flag is not changed)
                u8 vx = get4BitAddressRight(program[i]);
                i8 nn = program[i + 1];
                printInstr(i, "ADD");
                printf("V%X, %d", vx, nn);
            }
            break;
        case 0x08: {
                printInstr(i, "8");
            }
            break;
        case 0x09: {
                // 9xy0: if(Vx != Vy); Skips the next instruction if Vx doesn't equal Vy.
                u8 vx = get4BitAddressRight(program[i]);
                u8 vy = get4BitAddressLeft(program[i + 1]);
                printInstr(i, "SRNE");
                printf("V%X, V%X", vx, vy);
            }
            break;
        case 0x0a: {
                //Annn: i = nnn; Sets i to the address nnn.
                u16 nnn = get16BitAddress(program[i], program[i + 1]);
                printInstr(i, "MVI");
                printf("%X", nnn);
            }
            break;
        case 0x0b: {
                // Bnnn: PC = V0 + nnn; Jumps to the address nnn plus V0.
                i16 nnn = get16BitAddress(program[i], program[i + 1]);
                printInstr(i, "JMPV");
                printf("%X", nnn);
            }
            break;
        case 0x0c: {
                // Cxnn: Vx = rand() & nn; Sets Vx to the result of a bitwise and operation on a random number (Typically: 0 to 255) and nn.
                u8 vx = get4BitAddressRight(program[i]);
                u8 nn = program[i + 1];
                printInstr(i, "RNDMSK");
                printf("V%X, V%u", vx, nn);
            }
            break;
        case 0x0d: {
                // Dxyn: draw(Vx, Vy, n); Draws a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a height of n pixels.
                // Each row of 8 pixels is read as bit-coded starting from memory location i; i value doesn’t change after the execution of this instruction.
                // As described above, V16 is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
                // and to 0 if that doesn’t happen.
                u8 vx = get4BitAddressRight(program[i]);
                u8 vy = get4BitAddressLeft(program[i + 1]);
                u8 n = get4BitAddressRight(program[i + 1]);
                printInstr(i, "DRAW");
                printf("V%X, V%X, %.2u", vx, vy, n);
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
