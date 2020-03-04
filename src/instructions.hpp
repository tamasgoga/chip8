#ifndef GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP
#define GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP

#include <array>
#include <vector>
#include <memory>

#include "defines.hpp"

namespace ch8 {
    struct Chip8 {
        static const u16 MEM_START    = 0x200;
        static const u16 STACK_START  = 0xfa0;
        static const u16 SCREEN_START = 0xf00;

        std::array<u8, 16>     v;      // registers
        u16                    i;      // address register
        u16                    sp;     // stack pointer
        u16                    pc;     // program counter
        u8                     delay;  //
        u8                     sound;  //
        std::array<u8, 4096>   memory; //
        // u8*                    screen; // points to memory[0xF00]

        Chip8() {
            Reset();
        }

        void Reset() {
            // memory.fill(1u); // probably not required
            // screen = memory[SCREEN_START];
            sp = STACK_START;
            pc = MEM_START;
        }
    };

    // Base instruction.
    class Instruction {
    protected:
        ch8::Chip8& state;

    public:
        const u8 l, r;

        Instruction(Chip8& state, u8 left, u8 right)
            : state(state)
            , l(left)
            , r(right)
        {}

        virtual ~Instruction() {}

        void PrintInstruction(const char* name) const noexcept;

        virtual void Execute() noexcept;
        virtual void Disassemble() noexcept;
    };

    // 0x0
    class ClearScreenInstruction: public Instruction {
    public:
        ClearScreenInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class ReturnInstruction: public Instruction {
    public:
        ReturnInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x1
    class JumpInstruction: public Instruction {
    public:
        JumpInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x2
    class CallInstruction: public Instruction {
    public:
        CallInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x3
    class SkipEqualInstruction: public Instruction {
    public:
        SkipEqualInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x4
    class SkipNotEqualInstruction: public Instruction {
    public:
        SkipNotEqualInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x5
    class SkipRegisterEqualInstruction: public Instruction {
    public:
        SkipRegisterEqualInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x6
    class MoveInstruction: public Instruction {
    public:
        MoveInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x7
    class AddInstruction: public Instruction {
    public:
        AddInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x8
    class RegisterInstruction: public Instruction {
    public:
        RegisterInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0x9
    class SkipRegisterNotEqualInstruction: public Instruction {
    public:
        SkipRegisterNotEqualInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xA
    class MoveAddressInstruction: public Instruction {
    public:
        MoveAddressInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xB
    class JumpRegisterInstruction: public Instruction {
    public:
        JumpRegisterInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xC
    class RandomMaskInstruction: public Instruction {
    public:
        RandomMaskInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xD
    class DrawInstruction: public Instruction {
    public:
        DrawInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xE
    class SkipKeyInstruction: public Instruction {
    public:
        SkipKeyInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xF -- F stands for Fun!
    class FunInstruction: public Instruction {
    public:
        FunInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };
}

/*
Extra Super Chip-48 stuff:

00Cn - SCD nibble
00FB - SCR
00FC - SCL
00FD - EXIT
00FE - LOW
00FF - HIGH
*/

#endif // GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP