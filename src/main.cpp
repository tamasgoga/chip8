#include <iostream>
#include <exception>
#include "chip8.hpp"

// OPTIONS:
// hex
// asm
// noexec

void Run(int argc, char** argv) {
    using std::cout;
    using std::endl;

    ch8::Chip8 state;
    ch8::Interface interface;
    ch8::Program program(state, interface, argc, argv);

    if (program.arguments.path.empty()) {
        cout << "Usage: chip8 <options> path_to_rom" << endl;
        return;
    }

    if (os::HasFileError()) {
        cout << os::GetFileError() << endl;
        return;
    }

    cout << endl;

    if (program.arguments.IsEnabled(ch8::OPTIONS_HEX)) {
        program.DumpHex();
        cout << endl;
    }

    if (program.arguments.IsEnabled(ch8::OPTIONS_CODE)) {
        program.Disassemble();
        cout << endl;
    }

    if (!program.arguments.IsEnabled(ch8::OPTIONS_NOEXEC)) {
        program.Execute();
    }

    cout << program.arguments.options << ' ' << program.arguments.path << endl;
}

int QuickTest(int argc, char** argv) {
    using std::cout;
    using std::endl;

    ch8::Chip8 state;
    ch8::Interface interface;
    ch8::Program program(state, interface, "roms/games/Paddles.ch8", 1);
    // emu::Program program("roms/programs/SQRT Test [Sergey Naydenov, 2010].ch8", 1);
    cout << os::GetFileError() << endl;

    // program.DumpHex();
    program.Disassemble();
    cout << '\n' << program.arguments.path << ", " << program.arguments.options << endl;
    
    // Ignore, this is just to supress the unused varaible warning
    return (unsigned long long)(argc) == (unsigned long long)(argv[0]);
}

int main(int argc, char** argv) {
    using std::cerr;
    using std::endl;

    try {
#ifdef TEST
        QuickTest(argc, argv);
#else
        Run(argc, argv);
#endif
    } catch (const std::exception& e) {
        cerr << "<<MAIN>> " << e.what() << endl;
    }
}

/** RULES:
 * (1) Exceptions should not stop execution or disassembly.
 * (2) Separate platform-dependent functionality, clearly.
 * (3) All public methods and functions begin with upper-case. "main" is the only exception, for obvious reasons.
 * (4) If it's a class: use its methods. If it's a struct: the properties may be used directly (or the convenience methods, if provided).
 * (5) Don't add useless accessor methods.
**/

/*
od is your friend:
od -x roms/games/Paddles.ch8
od -x roms/programs/SQRT Test [Sergey Naydenov, 2010].ch8

./chip8 hex asm roms/games/Pong\ \(1\ player\).ch8

http://emulator101.com/

http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
*/