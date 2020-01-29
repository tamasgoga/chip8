// streams
#include <iostream>

// data structures
#include <string>
#include <vector>
#include <iterator>

// the 300 iq header
#include <algorithm>

// c headers
#include <cstring>

// my headers
#include "def.hpp"
#include "os.hpp"

namespace emu {
    /*
    od is your friend:
    od -x roms/games/Paddles.ch8

    http://emulator101.com/
    TODO: Read through it.

    http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

    3.1 - Standard Chip-8 Instructions
            00E0 - CLS
            00EE - RET
            0nnn - SYS addr
            1nnn - JP addr
            2nnn - CALL addr
            3xkk - SE Vx, byte
            4xkk - SNE Vx, byte
            5xy0 - SE Vx, Vy
            6xkk - LD Vx, byte
            7xkk - ADD Vx, byte
            8xy0 - LD Vx, Vy
            8xy1 - OR Vx, Vy
            8xy2 - AND Vx, Vy
            8xy3 - XOR Vx, Vy
            8xy4 - ADD Vx, Vy
            8xy5 - SUB Vx, Vy
            8xy6 - SHR Vx {, Vy}
            8xy7 - SUBN Vx, Vy
            8xyE - SHL Vx {, Vy}
            9xy0 - SNE Vx, Vy
            Annn - LD I, addr
            Bnnn - JP V0, addr
            Cxkk - RND Vx, byte
            Dxyn - DRW Vx, Vy, nibble
            Ex9E - SKP Vx
            ExA1 - SKNP Vx
            Fx07 - LD Vx, DT
            Fx0A - LD Vx, K
            Fx15 - LD DT, Vx
            Fx18 - LD ST, Vx
            Fx1E - ADD I, Vx
            Fx29 - LD F, Vx
            Fx33 - LD B, Vx
            Fx55 - LD [I], Vx
            Fx65 - LD Vx, [I]
      3.2 - Super Chip-48 Instructions
            00Cn - SCD nibble
            00FB - SCR
            00FC - SCL
            00FD - EXIT
            00FE - LOW
            00FF - HIGH
            Dxy0 - DRW Vx, Vy, 0
            Fx30 - LD HF, Vx
            Fx75 - LD R, Vx
            Fx85 - LD Vx, R
    */

    class Program {
    public:
        using size_type = std::vector<u8>::size_type;
        const os::Arguments arguments;
        
        Program(const char* path, u32 options)
            : arguments(path, options)
        {
            program = os::ReadChip8File(arguments.path);
        }

        Program(int argc, char** argv)
            : arguments(argc, argv)
        {
            program = os::ReadChip8File(arguments.path);
        }

        void DumpHex() {
            using std::cout;
            using std::endl;

            unsigned int lineBuf = 0;
            unsigned int opBuf = 1;

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

        void Disassemble() {
            using std::cout;
            using std::endl;

            cout << endl;

            for (size_type i = 0; i < program.size(); i += 2) {
                switch (program[i] >> 4) {
                case 0x00: printf("0 not handled yet"); break;
                case 0x01: printf("1 not handled yet"); break;
                case 0x02: printf("2 not handled yet"); break;
                case 0x03: printf("3 not handled yet"); break;
                case 0x04: printf("4 not handled yet"); break;
                case 0x05: printf("5 not handled yet"); break;
                case 0x06: {
                        uint8_t reg = program[0] & 0x0f;
                        printf("%-10s V%01X,#$%02x", "MVI", reg, program[1]);
                    }
                    break;
                case 0x07: printf("7 not handled yet"); break;
                case 0x08: printf("8 not handled yet"); break;
                case 0x09: printf("9 not handled yet"); break;
                case 0x0a: {
                        uint8_t addresshi = program[0] & 0x0f;
                        printf("%-10s I,#$%01x%02x", "MVI", addresshi, program[1]);
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

    private:
        std::vector<u8> program;
    };
}

void Run(int argc, char** argv) {
    using std::cout;
    using std::endl;

    emu::Program program(argc, argv);

    if (program.arguments.path.empty()) {
        cout << "Usage: chip8 <options> path_to_rom" << endl;
        return;
    }

    if (program.arguments.options & emu::OPTIONS_HEX) {
        program.DumpHex();
    }

    if (program.arguments.options & emu::OPTIONS_CODE) {
        program.Disassemble();
    }

    cout << program.arguments.options << ' ' << program.arguments.path << endl;
}

int QuickTest(int argc, char** argv) {
    using std::cout;
    using std::endl;

    emu::Program program("roms/games/Paddles.ch8", 1);
    program.DumpHex();
    
    // Ignore, this is just to supress the unused varaible warning
    return (unsigned long long)(argc) == (unsigned long long)(argv[0]);
}

int main(int argc, char** argv) {
    Run(argc, argv);
    // QuickTest(argc, argv);
}