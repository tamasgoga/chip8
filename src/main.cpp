#include <iostream>

#include "chip8.hpp"

// OPTIONS:
// hex
// asm

void Run(int argc, char** argv) {
    using std::cout;
    using std::endl;

    ch8::Program program(argc, argv);

    if (program.arguments.path.empty()) {
        cout << "Usage: chip8 <options> path_to_rom" << endl;
        return;
    }

    if (os::HasFileError()) {
        cout << os::GetFileError() << endl;
        return;
    }

    cout << endl;

    if (program.arguments.options & emu::OPTIONS_HEX) {
        program.DumpHex();
        cout << endl;
    }

    if (program.arguments.options & emu::OPTIONS_CODE) {
        program.Disassemble();
        cout << endl;
    }

    cout << program.arguments.options << ' ' << program.arguments.path << endl;
}

int QuickTest(int argc, char** argv) {
    using std::cout;
    using std::endl;

    ch8::Program program("roms/games/Paddles.ch8", 1);
    // emu::Program program("roms/programs/SQRT Test [Sergey Naydenov, 2010].ch8", 1);
    cout << os::GetFileError() << endl;

    // program.DumpHex();
    program.Disassemble();
    cout << program.arguments.path << ", " << program.arguments.options << endl;
    
    // Ignore, this is just to supress the unused varaible warning
    return (unsigned long long)(argc) == (unsigned long long)(argv[0]);
}

int main(int argc, char** argv) {
#ifdef TEST
    QuickTest(argc, argv);
#else
    Run(argc, argv);
#endif
}

/*
od is your friend:
od -x roms/games/Paddles.ch8
od -x roms/programs/SQRT Test [Sergey Naydenov, 2010].ch8

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