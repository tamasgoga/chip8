#ifndef GOGA_TAMAS_CHIP_8_INSTR_HPP
#define GOGA_TAMAS_CHIP_8_INSTR_HPP

#include <array>
#include <vector>
#include <memory>

#include "def.hpp"

namespace emu {
    struct State {
        std::array<u8, 16>     v;      // registers
        u16                    i;      // address register
        u16                    sp;     // stack pointer
        u16                    pc;     // program counter
        u8                     delay;  //
        u8                     sound;  //
        std::array<u8, 4096>   memory; //
        // u8*                    screen; // points to memory[0xF00]

        State() {
            memory.fill(1u);
            // screen = memory[0xf00];    
            sp = 0xfa0;    
            pc = 0x200;  
        }
    };

    // Base instruction.
    class Instruction {
    protected:
        emu::State& state;

    public:
        const u8 l, r;

        Instruction(State& state, u8 left, u8 right)
            : state(state)
            , l(left)
            , r(right)
        {}

        virtual ~Instruction() {}

        virtual void Execute() noexcept;
        virtual void Disassemble(u16 i) const noexcept;

        void PrintInstruction(u16 i, const char* name) const noexcept;
    };

    std::vector<std::unique_ptr<Instruction>> ParseBytes(State state, std::vector<u8> bytes) noexcept;

    // 0x0
    class SystemInstruction: public Instruction {
    public:
        SystemInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x1
    class JumpInstruction: public Instruction {
    public:
        JumpInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };


    // 0x2
    class CallInstruction: public Instruction {
    public:
        CallInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x3
    class SkipEqualInstruction: public Instruction {
    public:
        SkipEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x4
    class SkipNotEqualInstruction: public Instruction {
    public:
        SkipNotEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x5
    class SkipRegisterEqualInstruction: public Instruction {
    public:
        SkipRegisterEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x6
    class MoveInstruction: public Instruction {
    public:
        MoveInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x7
    class AddInstruction: public Instruction {
    public:
        AddInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x8
    class RegisterInstruction: public Instruction {
    public:
        RegisterInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0x9
    class SkipRegisterNotEqualInstruction: public Instruction {
    public:
        SkipRegisterNotEqualInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xA
    class MoveAddressInstruction: public Instruction {
    public:
        MoveAddressInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xB
    class JumpRegisterInstruction: public Instruction {
    public:
        JumpRegisterInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xC
    class RandomMaskInstruction: public Instruction {
    public:
        RandomMaskInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xD
    class DrawInstruction: public Instruction {
    public:
        DrawInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xE
    class SkipKeyInstruction: public Instruction {
    public:
        SkipKeyInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };

    // 0xF -- F stands for Fun!
    class FunInstruction: public Instruction {
    public:
        FunInstruction(State& s, u8 l, u8 r): Instruction(s,l,r) {}
        void Execute() noexcept override;
        void Disassemble(u16 i) const noexcept override;
    };
}

#endif // GOGA_TAMAS_CHIP_8_INSTR_HPP