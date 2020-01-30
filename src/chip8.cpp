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
    static constexpr u32 MEM_START = 0x200;

    auto printInstr = [this] (size_type i, const char* name) {
        printf("%.8x   <%.2x%.2x>   %-8s ", (u32(i) + MEM_START), program[i], program[i + 1], name);
    };

    cout << endl;

    for (size_type i = 0; i < program.size(); i += 2) {
        switch (program[i] >> 4) {
        case 0x00: {
                // clear screen, return, sys calls
                switch (program[i + 1]) {
                    case 0xe0: printInstr(i, "CLS"); break;
                    case 0xee: printInstr(i, "RET"); break;
                    default: {
                            // 0nnn: This instruction is only used on the old computers on which Chip-8 was originally implemented.
                            // It is ignored by modern interpreters.
                            u16 nnn = get16BitAddress(program[i], program[i + 1]);
                            printInstr(i, "SYS");
                            printf("%X", nnn);
                        }
                        break;

                    /*
                    Super Chip-48:
                    00Cn - SCD nibble
                    00FB - SCR
                    00FC - SCL
                    00FD - EXIT
                    00FE - LOW
                    00FF - HIGH
                    */
                }
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
                // register operations
                u8 vx = get4BitAddressRight(program[i]);
                u8 vy = get4BitAddressLeft(program[i + 1]);

                switch (program[i + 1] & 0x0f) {
                case 0x0:
                    // 8xy0: Vx = Vy; Sets Vx to the value of Vy.
                    printInstr(i, "MOVR");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x1:
                    // 8xy1: Vx = Vx|Vy; Sets Vx to Vx or Vy. (Bitwise OR operation)
                    printInstr(i, "OR");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x2:
                    // 8xy2: Vx = Vx & Vy; Sets Vx to Vx and Vy. (Bitwise AND operation)
                    printInstr(i, "AND");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x3:
                    // 8xy3: Vx = Vx ^ Vy; Sets Vx to Vx xor Vy.
                    printInstr(i, "XOR");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x4:
                    // 8xy4: Vx += Vy; Adds Vxy to Vy. Vf is set to 1 when there's a carry, and to 0 when there isn't.
                    printInstr(i, "ADDR");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x5:
                    // 8xy5: Vx -= Vy; Vy is subtracted from Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.
                    printInstr(i, "SUB");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0x6:
                    // 8xy6: Vx >>= 1; Stores the least significant bit of Vx in Vf and then shifts Vx to the right by 1.
                    printInstr(i, "SHR");
                    printf("V%X", vx);
                    break;
                case 0x7:
                    // 8xy7: Vx = Vy - Vx; Sets Vx to Vy minus Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.
                    printInstr(i, "SUBINV");
                    printf("V%X, V%X", vx, vy);
                    break;
                case 0xe:
                    // 8xyE: Vx <<= 1; Stores the most significant bit of Vx in Vf and then shifts Vx to the left by 1.
                    printInstr(i, "SHL");
                    printf("V%X", vx);
                    break;
                default:
                    printInstr(i, "; unknown (8)");
                    break;
                }
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
                printInstr(i, "MOVI");
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
                // As described above, Vf is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
                // and to 0 if that doesn’t happen.
                u8 vx = get4BitAddressRight(program[i]);
                u8 vy = get4BitAddressLeft(program[i + 1]);
                u8 n = get4BitAddressRight(program[i + 1]);
                printInstr(i, "DRAW");
                printf("V%X, V%X, %.2u", vx, vy, n);

                // Super Chip-48: Dxy0 - DRW Vx, Vy, 0
            }
            break;
        case 0x0e: {
                // skip (Usually the next instruction is a jump to skip a code block)
                u8 vx = get4BitAddressRight(program[i]);

                switch (program[i + 1]) {
                case 0x9e:
                    // Ex9E: if(key() == Vx); Skips the next instruction if the key stored in Vx is pressed.
                    printInstr(i, "SKE");
                    printf("V%X", vx);
                    break;
                case 0xa1:
                    // ExA1; if(key() != Vx); Skips the next instruction if the key stored in Vx isn't pressed.
                    printInstr(i, "SKNE");
                    printf("V%X", vx);
                    break;
                default:
                    printInstr(i, "; unknown (E)");
                    break;
                }
            }
            break;
        case 0x0f: {
                u8 vx = get4BitAddressRight(program[i]);

                switch (program[i + 1]) {
                case 0x07:
                    // Fx07: Vx = get_delay(); Sets Vx to the value of the delay timer.
                    printInstr(i, "GETDLY");
                    printf("V%X", vx);
                    break;
                case 0x0a:
                    // Fx0A: Vx = get_key(); A key press is awaited, and then stored in Vx.
                    // (Blocking Operation. All instruction halted until next key event)
                    printInstr(i, "GETKEY");
                    printf("V%X", vx);
                    break;
                case 0x15:
                    // Fx15: delay_timer(Vx); Sets the delay timer to Vx.
                    printInstr(i, "SETDLY");
                    printf("V%X", vx);
                    break;
                case 0x18:
                    // Fx18: sound_timer(Vx); Sets the sound timer to VX.
                    printInstr(i, "SETSND");
                    printf("V%X", vx);
                    break;
                case 0x1e:
                    // Fx1E: I += Vx; Adds Vx to i. Vf is set to 1 when there is a range overflow (I + Vx > 0xFFF), and to 0 when there isn't.
                    printInstr(i, "ADDI");
                    printf("V%X", vx);
                    break;
                case 0x29:
                    // Fx29: I = sprite_addr[Vx]; Sets I to the location of the sprite for the character in Vx.
                    // Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    printInstr(i, "DRAW");
                    printf("V%X", vx);
                    break;
                case 0x33:
                    /*
                    Fx33:
                    - set_BCD(Vx);
                    - *(I + 0) = BCD(3);
                    - *(I + 1) = BCD(2);
                    - *(I + 2) = BCD(1);
                    Stores the binary-coded decimal representation of Vx, with the most significant of three digits at the address in i,
                    the middle digit at i plus 1, and the least significant digit at i plus 2.
                    (In other words, take the decimal representation of Vx, place the hundreds digit in memory at location in i,
                    the tens digit at location i+1, and the ones digit at location i+2.)
                    */
                    printInstr(i, "BCD");
                    printf("V%X", vx);
                    break;
                case 0x55:
                    // Fx55: reg_dump(Vx, &i); Stores V0 to Vx (including Vx) in memory starting at address i.
                    // The offset from i is increased by 1 for each value written, but i itself is left unmodified.
                    printInstr(i, "SAVE");
                    printf("V%X", vx);
                    break;
                case 0x65:
                    // Fx65: reg_load(Vx, &i); Fills V0 to Vx (including Vx) with values from memory starting at address i.
                    // The offset from i is increased by 1 for each value written, but i itself is left unmodified.
                    printInstr(i, "LOAD");
                    printf("V%X", vx);
                    break;
                default:
                    printInstr(i, "; unknown (F)");
                    break;

                /*
                Super Chip-48:
                Fx30 - LD HF, Vx
                Fx75 - LD R, Vx
                Fx85 - LD Vx, R
                */
                }
            }
            break;
        }

        putchar('\n');
    }

    cout << endl;
}
