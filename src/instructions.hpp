#ifndef GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP
#define GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP

#include <array>
#include <vector>
#include <memory>

#include "defines.hpp"

namespace ch8 {
    // Used as a struct, since the purpose is to modify its properties directly, without the use of methods.
    struct State {
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

        State() {
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
        ch8::State& state;

    public:
        const u8 l, r;

        Instruction(State& state, u8 left, u8 right)
            : state(state)
            , l(left)
            , r(right)
        {}

        virtual ~Instruction() {}

        virtual void Execute() noexcept;
        virtual void Disassemble() const noexcept;

        void PrintInstruction(const char* name) const noexcept;
    };

    // 0x0
    class SystemInstruction: public Instruction {
    public:
        SystemInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x1
    class JumpInstruction: public Instruction {
    public:
        JumpInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };


    // 0x2
    class CallInstruction: public Instruction {
    public:
        CallInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x3
    class SkipEqualInstruction: public Instruction {
    public:
        SkipEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x4
    class SkipNotEqualInstruction: public Instruction {
    public:
        SkipNotEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x5
    class SkipRegisterEqualInstruction: public Instruction {
    public:
        SkipRegisterEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x6
    class MoveInstruction: public Instruction {
    public:
        MoveInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x7
    class AddInstruction: public Instruction {
    public:
        AddInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x8
    class RegisterInstruction: public Instruction {
    public:
        RegisterInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0x9
    class SkipRegisterNotEqualInstruction: public Instruction {
    public:
        SkipRegisterNotEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xA
    class MoveAddressInstruction: public Instruction {
    public:
        MoveAddressInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xB
    class JumpRegisterInstruction: public Instruction {
    public:
        JumpRegisterInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xC
    class RandomMaskInstruction: public Instruction {
    public:
        RandomMaskInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xD
    class DrawInstruction: public Instruction {
    public:
        DrawInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xE
    class SkipKeyInstruction: public Instruction {
    public:
        SkipKeyInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };

    // 0xF -- F stands for Fun!
    class FunInstruction: public Instruction {
    public:
        FunInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble() const noexcept override;
    };
}

#endif // GOGA_TAMAS_CHIP_8_INSTRUCTIONS_HPP