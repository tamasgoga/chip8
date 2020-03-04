#ifndef GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP
#define GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP

#include <array>
#include <vector>
#include <memory>

#include "defines.hpp"

// Contains the Chip-8's CPU layout & all of its instructions.

namespace ch8 {
    // The Chip-8's CPU.
    struct Chip8 {
        static const u16 MEM_START    = 0x200;
        static const u16 STACK_START  = 0xfa0;
        static const u16 SCREEN_START = 0xf00;

        std::array<u8, 16>     v;      // registers
        u16                    i;      // address register
        u16                    sp;     // stack pointer
        u16                    pc;     // program counter
        u8                     dt;     // delay timer
        u8                     st;     // sound timer
        std::array<u8, 4096>   memory; //
        // u8*                    screen; // points to memory[0xF00]

        Chip8() {
            Reset();
        }

        void Reset() {
            // screen = memory[SCREEN_START];
            sp = STACK_START;
            pc = MEM_START;
        }
    };

    // Bit twiddling.
    inline u8 GetRightNibble(u8 x) { return x & 0x0f; }
    inline u8 GetLeftNibble(u8 x)  { return (x & 0xf0) >> 4;}

    // Base instruction.
    class Instruction {
    public:
        const u8 l, r;

        Instruction(Chip8& state, u8 left, u8 right)
            : l(left)
            , r(right)
            , state(state)
        {}

        virtual ~Instruction() {}

        u16 Get16BitAddress() const noexcept {
            return ((u16(l) & 0x00f) << 8) | r;
        }

        void PrintInstruction(const char* name) const noexcept;

        virtual void Execute() noexcept;
        virtual void Disassemble() noexcept;

    protected:
        Chip8& state;
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
    class MoveRegisterInstruction: public Instruction {
    public:
        MoveRegisterInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class OrInstruction: public Instruction {
    public:
        OrInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class AndInstruction: public Instruction {
    public:
        AndInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class XorInstruction: public Instruction {
    public:
        XorInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class AddRegisterInstruction: public Instruction {
    public:
        AddRegisterInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SubInstruction: public Instruction {
    public:
        SubInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class ShiftRightInstruction: public Instruction {
    public:
        ShiftRightInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SubInverseInstruction: public Instruction {
    public:
        SubInverseInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class ShiftLeftInstruction: public Instruction {
    public:
        ShiftLeftInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
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
    class SkipKeyEqualsInstruction: public Instruction {
    public:
        SkipKeyEqualsInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SkipKeyNotEqualsInstruction: public Instruction {
    public:
        SkipKeyNotEqualsInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    // 0xF -- F stands for Fun!
    class GetDelayInstruction: public Instruction {
    public:
        GetDelayInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class GetKeyInstruction: public Instruction {
    public:
        GetKeyInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SetDelayInstruction: public Instruction {
    public:
        SetDelayInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SetSoundInstruction: public Instruction {
    public:
        SetSoundInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class AddToAddressInstruction: public Instruction {
    public:
        AddToAddressInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SetSpriteInstruction: public Instruction {
    public:
        SetSpriteInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SetBcdInstruction: public Instruction {
    public:
        SetBcdInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class SaveRegistersInstruction: public Instruction {
    public:
        SaveRegistersInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    };

    class LoadRegistersInstruction: public Instruction {
    public:
        LoadRegistersInstruction(Chip8& s, u8 l, u8 r): Instruction(s,l,r) {}
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

Dxy0 - DRW Vx, Vy, 0

Fx30 - LD HF, Vx
Fx75 - LD R, Vx
Fx85 - LD Vx, R
*/

#endif // GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP