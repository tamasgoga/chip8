#include <cstdio>

#include "program.hpp"

// C-tors

ch8::Program::Program(ch8::Chip8& state, ch8::Interface& interface, const char *path, u32 options)
    : arguments(path, options)
    , state(state)
    , interface(interface)
{
    ParseBytes(os::ReadChip8File(arguments.path));
}

ch8::Program::Program(ch8::Chip8& state, ch8::Interface& interface, int argc, char **argv)
    : arguments(argc, argv)
    , state(state)
    , interface(interface)
{
    ParseBytes(os::ReadChip8File(arguments.path));
}

void ch8::Program::ParseBytes(std::vector<u8> bytes) {
    using std::make_unique;

    program.clear();
    program.reserve(bytes.size() / 2u);

    for (std::vector<u8>::size_type i = 0; i < bytes.size(); i += 2) {
        u8 l = bytes[i];
        u8 r = bytes[i + 1];

        switch (l >> 4) {
        case 0x00:
            switch (r) {
            case 0xe0:
                program.push_back(make_unique<ClearScreenInstruction>(state, l, r));
                break;
            case 0xee:
                program.push_back(make_unique<ReturnInstruction>(state, l, r));
                break;

            default:
                // 0nnn: This instruction is only used on the old computers on which Chip-8 was originally implemented.
                // It is ignored by modern interpreters.
                program.push_back(make_unique<Instruction>(state, l, r));
                break;
            }
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
            switch (GetRightNibble(r)) {
            case 0x0:
                program.push_back(make_unique<MoveRegisterInstruction>(state, l, r));
                break;
            case 0x1:
                program.push_back(make_unique<OrInstruction>(state, l, r));
                break;
            case 0x2:
                program.push_back(make_unique<AndInstruction>(state, l, r));
                break;
            case 0x3:
                program.push_back(make_unique<XorInstruction>(state, l, r));
                break;
            case 0x4:
                program.push_back(make_unique<AddRegisterInstruction>(state, l, r));
                break;
            case 0x5:
                program.push_back(make_unique<SubInstruction>(state, l, r));
                break;
            case 0x6:
                program.push_back(make_unique<ShiftRightInstruction>(state, l, r));
                break;
            case 0x7:
                program.push_back(make_unique<SubInverseInstruction>(state, l, r));
                break;
            case 0xe:
                program.push_back(make_unique<ShiftLeftInstruction>(state, l, r));
                break;

            default:
                program.push_back(make_unique<Instruction>(state, l, r));
                break;
            }
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
            switch (r) {
            case 0x9e:
                program.push_back(make_unique<SkipKeyEqualsInstruction>(state, l, r));
                break;
            case 0xa1:
                program.push_back(make_unique<SkipKeyNotEqualsInstruction>(state, l, r));
                break;

            default:
                program.push_back(make_unique<Instruction>(state, l, r));
                break;
            }
            break;
        case 0x0f:
            switch (r) {
            case 0x07:
                program.push_back(make_unique<GetDelayInstruction>(state, l, r));
                break;
            case 0x0a:
                program.push_back(make_unique<GetKeyInstruction>(state, l, r));
                break;
            case 0x15:
                program.push_back(make_unique<SetDelayInstruction>(state, l, r));
                break;
            case 0x18:
                program.push_back(make_unique<SetSoundInstruction>(state, l, r));
                break;
            case 0x1e:
                program.push_back(make_unique<AddToAddressInstruction>(state, l, r));
                break;
            case 0x29:
                program.push_back(make_unique<SetSpriteInstruction>(state, l, r));
                break;
            case 0x33:
                program.push_back(make_unique<SetBcdInstruction>(state, l, r));
                break;
            case 0x55:
                program.push_back(make_unique<SaveRegistersInstruction>(state, l, r));
                break;
            case 0x65:
                program.push_back(make_unique<LoadRegistersInstruction>(state, l, r));
                break;

            default:
                program.push_back(make_unique<Instruction>(state, l, r));
                break;
            }
            break;

        default:
            program.push_back(make_unique<Instruction>(state, l, r));
            break;
        }
    }
}

void ch8::Program::DumpHex() const noexcept {
    if (program.size() == 0) {
        return;
    }

    auto handle = program[0].get();
    printf("%.4x:   %.2x%.2x ", Chip8::MEM_START, handle->l, handle->r);

    for (u32 i = 1u; i < program.size(); i++) {
        handle = program[i].get();

        if (i % 8 == 0u) {
            printf("\n%.4x:   ", Chip8::MEM_START + i * 2u);
        }

        printf("%.2x%.2x ", handle->l, handle->r);
    }

    putchar('\n');
}

void ch8::Program::Execute() noexcept {
    state.Reset();

    bool isRunning = true;
    SDL_Event event;

    interface.start("Chip-8", 800, 600);
    SDL_SetRenderDrawColor(interface.renderer, 0,0,0, 255);
    SDL_RenderClear(interface.renderer);
    SDL_RenderPresent(interface.renderer);

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }
    }
}

void ch8::Program::Disassemble() noexcept {
    state.Reset();

    for (const auto &instr: program) {
        instr->Disassemble();
        putchar('\n');
        state.pc += 2;
    }
}