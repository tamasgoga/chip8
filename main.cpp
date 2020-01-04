// streams
#include <iostream>
#include <fstream>

// data structures
#include <string>
#include <vector>
#include <iterator>

// the 300 iq header
#include <algorithm>

// c headers
#include <cstring>
#include <cstdint>

using i16 = int_least16_t;
using u16 = uint_least16_t;

namespace emu {
    enum PROGRAM_OPTIONS {
        HEX = 0x1
    };
}

// To the best of my knowledge, I will separate OS specific things here.
namespace os {
    struct Arguments {
        int         options = 0;
        std::string path    = "";
    };

    Arguments ProcessArguments(int count, char** args) {
        Arguments ret;

        if (count < 2) {
            return ret;
        }
        
        // Usage implies last arg is rom
        ret.path = args[--count];

        // Parse options
        for (int i = 1; i < count; ++i) {
            if (strcmp("dump", args[i]) == 0) {
                ret.options |= emu::PROGRAM_OPTIONS::HEX;
            }
        }

        return ret;
    }

    // We will assume that the file can be stored in memory all at once
    std::vector<u16> ReadChip8File(std::string path) {        
        std::ifstream file(path, std::ios::binary);

        if (!file.is_open()) {
            return std::vector<u16>();
        }

        // https://stackoverflow.com/questions/5420317/reading-and-writing-binary-file
        return std::vector<u16>(std::istreambuf_iterator<char>(file), {});
    }
}

namespace emu {
    /*
    Od is your friend:
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
        const int         options = 0;
        const std::string path    = "";
        
        Program(const os::Arguments& args)
            : options(args.options)
            , path(args.path)
        {
            program = os::ReadChip8File(path);
        }

        void PrintContent() {
            using std::cout;
            using std::endl;

            unsigned int lineBuf = 0;
            unsigned int opBuf = 1;

            for (u16 op: program) {
                if (lineBuf++ % 16 == 0) {
                    cout << endl;
                    opBuf = 1;
                } else if (opBuf++ % 2 == 0) {
                    putchar(' ');
                }

                printf("%x", op);
            }

            cout << endl;
        }

    private:
        std::vector<u16> program;
    };
}

void Run(int argc, char** argv) {
    using std::cout;
    using std::endl;

    auto args = os::ProcessArguments(argc, argv);
    emu::Program program(args);

    if (program.path.empty()) {
        cout << "Usage: chip8 <options> path_to_rom" << endl;
        return;
    }

    cout << program.options << ' ' << program.path << endl;
}

int QuickTest(int argc, char** argv) {
    using std::cout;
    using std::endl;

    emu::Program program({ 1, "roms/games/Paddles.ch8" });
    program.PrintContent();
    
    // Ignore, this is just to supress the unused varaible warning
    return (unsigned long long)(argc) == (unsigned long long)(argv[0]);
}

int main(int argc, char** argv) {
    // Run(argc, argv);
    QuickTest(argc, argv);
}