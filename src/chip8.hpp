#ifndef GOGA_TAMAS_CHIP_8_CHIP8_HPP
#define GOGA_TAMAS_CHIP_8_CHIP8_HPP

#include <array>
#include <vector>
#include <memory>

#include "defines.hpp"
#include "sdl.hpp"

// Contains the Chip-8's CPU layout & all of its instructions.

// Note: I decided to implement the stack separately, to make my life easier.
//       I couldn't find any authoritative resource that prohibited me from doing so.

namespace ch8 {
    // The Chip-8's CPU.
    struct Chip8 {
        std::array<u8, 16>          v;      // Registers
        u16                         i;      // Big register, usually for storing an address
        u16                         sp;     // Stack pointer
        u16                         pc;     // Program counter
        u8                          dt;     // Delay timer
        u8                          st;     // Sound timer
        std::array<u16, STACK_SIZE> stack;  // Decided to implement the stack separately, to make my life easier
        // std::array<u8, MEM_SIZE>    memory; // Memory available to programs (first 512 bytes unused, they were reserved for the interpreter)

        Chip8() {
            Reset();
        }

        void Reset() {
            stack.fill(0);
            sp = 0;
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
        ClearScreenInstruction(Chip8& s, Interface& i, u8 l, u8 r): Instruction(s,l,r), interface(i) {}
        void Execute() noexcept override;
        void Disassemble() noexcept override;
    private:
        Interface& interface;
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

#endif // GOGA_TAMAS_CHIP_8_CHIP8_HPP