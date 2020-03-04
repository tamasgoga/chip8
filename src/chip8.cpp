#include "chip8.hpp"
#include <cstdio>

// C-tors

ch8::Program::Program(const char* path, u32 options)
    : arguments(path, options)
{
    ParseBytes(os::ReadChip8File(arguments.path));
}

ch8::Program::Program(int argc, char** argv)
    : arguments(argc, argv)
{
    ParseBytes(os::ReadChip8File(arguments.path));
}

void ch8::Program::ParseBytes(std::vector<u8> bytes) noexcept {
    using std::make_unique;

    program.clear();
    program.reserve(bytes.size() / 2u);

    for (std::vector<u8>::size_type i = 0; i < bytes.size(); i += 2) {
        u8 l = bytes[i];
        u8 r = bytes[i + 1];
    
        switch (l >> 4) {
        case 0x00:
            program.push_back(make_unique<SystemInstruction>(state, l, r));
            break;
        case 0x01:
            program.push_back(make_unique<JumpInstruction>(state, l, r));
            break;
        case 0x02:
            program.push_back(make_unique<CallInstruction>(state, l, r));
            break;
        case 0x03:
            program.push_back(make_unique<SkipEqualInstruction>(state, l, r));
            break;
        case 0x04:
            program.push_back(make_unique<SkipNotEqualInstruction>(state, l, r));
            break;
        case 0x05:
            program.push_back(make_unique<SkipRegisterEqualInstruction>(state, l, r));
            break;
        case 0x06:
            program.push_back(make_unique<MoveInstruction>(state, l, r));
            break;
        case 0x07:
            program.push_back(make_unique<AddInstruction>(state, l, r));
            break;
        case 0x08:
            program.push_back(make_unique<RegisterInstruction>(state, l, r));
            break;
        case 0x09:
            program.push_back(make_unique<SkipRegisterNotEqualInstruction>(state, l, r));
            break;
        case 0x0a:
            program.push_back(make_unique<MoveAddressInstruction>(state, l, r));
            break;
        case 0x0b:
            program.push_back(make_unique<JumpRegisterInstruction>(state, l, r));
            break;
        case 0x0c:
            program.push_back(make_unique<RandomMaskInstruction>(state, l, r));
            break;
        case 0x0d:
            program.push_back(make_unique<DrawInstruction>(state, l, r));
            break;
        case 0x0e:
            program.push_back(make_unique<SkipKeyInstruction>(state, l, r));
            break;
        case 0x0f:
            program.push_back(make_unique<FunInstruction>(state, l, r));
            break;

        default:
            program.push_back(make_unique<Instruction>(state, l, r));
            break;
        }
    }
}

void ch8::Program::DumpHex() const {
    if (program.size() < 2) {
        return;
    }

    auto iter = program.cbegin();
    auto handle = iter->get();
    printf("%.4x:   %.2x%.2x ", State::MEM_START, handle->l, handle->r);
    iter++;
    u16 opCount = 1u;

    for (; iter != program.cend(); iter++, opCount++) {
        handle = iter->get();

        if (opCount % 8 == 0u) {
            printf("\n%.4x:   ", State::MEM_START + opCount * 2u);
        }

        printf("%.2x%.2x ", handle->l, handle->r);
    }

    putchar('\n');
}

void ch8::Program::Execute() {
    state.Reset();

    for (auto& instr: program) {
        instr->Execute();
    }
}

void ch8::Program::Disassemble() {
    state.Reset();

    for (const auto& instr: program) {
        instr->Disassemble();
        putchar('\n');
        state.pc += 2;
    }
}