#include <cstdio>

#include "instructions.hpp"

// Constants

static constexpr u16 MEM_START = 0x200;

// Bit twiddling

// rename to getRightNibble, getLeftNibble
static inline u8 getRightNibble(u8 x) { return x & 0x0f; }
static inline u8 getLeftNibble(u8 x)  { return (x & 0xf0) >> 4;}

static inline u16 get16BitAddress (u8 x, u8 y) {
    return ((u16(x) & 0x00f) << 8) | y;
}

// Instructions

void ch8::Instruction::PrintInstruction(u16 i, const char* name) const noexcept {
    printf("%.4x:   <%.2x%.2x>   %-8s ", MEM_START + i * 2, l, r, name);
}

void ch8::Instruction::Execute() noexcept {
    printf("<pc = %.4x> Unknown instruction, nothing executed.\n", this->state.pc);
}

void ch8::Instruction::Disassemble(u16 i) const noexcept {
    PrintInstruction(i, "; unknown");
}

std::vector<std::unique_ptr<ch8::Instruction>> ch8::ParseBytes(State state, std::vector<u8> bytes) noexcept {
    using std::make_unique;

    auto program = std::vector<std::unique_ptr<Instruction>>();
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

    return program;
}


// 0x0: Sys calls.

void ch8::SystemInstruction::Execute() noexcept {}

void ch8::SystemInstruction::Disassemble(u16 i) const noexcept {
    // clear screen, return, sys calls
    switch (r) {
    case 0xe0:
        PrintInstruction(i, "CLS");
        break;
    case 0xee:
        PrintInstruction(i ,"RET");
        break;
    default: {
            // 0nnn: This instruction is only used on the old computers on which Chip-8 was originally implemented.
            // It is ignored by modern interpreters.
            u16 nnn = get16BitAddress(l, r);
            PrintInstruction(i, "SYS");
            printf("%X", nnn);
        }
        break;
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
}


// 1nnn: goto nnn; Jumps to address nnn.

void ch8::JumpInstruction::Execute() noexcept {}

void ch8::JumpInstruction::Disassemble(u16 i) const noexcept {
    u16 nnn = get16BitAddress(l, r);
    PrintInstruction(i, "JMP");
    printf("%X", nnn);
}


// 2nnn: *(nnn)(); Calls subroutine at nnn.

void ch8::CallInstruction::Execute() noexcept {}

void ch8::CallInstruction::Disassemble(u16 i) const noexcept {
    u16 nnn = get16BitAddress(l, r);
    PrintInstruction(i, "CALL");
    printf("%X", nnn);
}


// 3xnn: if(Vx == nn); Skips the next instruction if Vx equals nn.
// Usually the next instruction is a jump to skip a code block. Applies for 4, 5 & 9, too.

void ch8::SkipEqualInstruction::Execute() noexcept {}

void ch8::SkipEqualInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    i8 nn = r;
    PrintInstruction(i, "SE");
    printf("V%X, %d", vx, nn);
}


// 4xnn: if(Vx != nn); Skips the next instruction if Vx doesn't equal nn.

void ch8::SkipNotEqualInstruction::Execute() noexcept {}

void ch8::SkipNotEqualInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    i8 nn = r;
    PrintInstruction(i, "SNE");
    printf("V%X, %d", vx, nn);
}


// 5xy0: if(Vx == Vy); Skips the next instruction if Vx equals Vy.

void ch8::SkipRegisterEqualInstruction::Execute() noexcept {}

void ch8::SkipRegisterEqualInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    u8 vy= getLeftNibble(r);
    PrintInstruction(i, "SRE");
    printf("V%X, V%X", vx, vy);
}


// 6xnn: Vx = nn; Sets VX to NN.

void ch8::MoveInstruction::Execute() noexcept {}

void ch8::MoveInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    i8 nn = r;
    PrintInstruction(i, "MOV");
    printf("V%X, %d", vx, nn);
}


// 7xnn: Vx += nn; Adds nn to Vx. (Carry flag is not changed.)

void ch8::AddInstruction::Execute() noexcept {}

void ch8::AddInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    i8 nn = r;
    PrintInstruction(i, "ADD");
    printf("V%X, %d", vx, nn);
}

// 8xyk: Register operations.

void ch8::RegisterInstruction::Execute() noexcept {}

void ch8::RegisterInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    u8 vy = getLeftNibble(r);

    switch (r & 0x0f) {
    case 0x0:
        // 8xy0: Vx = Vy; Sets Vx to the value of Vy.
        PrintInstruction(i, "MOVR");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x1:
        // 8xy1: Vx |= Vy; Sets Vx to Vx or Vy. (Bitwise OR operation)
        PrintInstruction(i, "OR");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x2:
        // 8xy2: Vx &= Vy; Sets Vx to Vx and Vy. (Bitwise AND operation)
        PrintInstruction(i, "AND");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x3:
        // 8xy3: Vx ^= Vy; Sets Vx to Vx xor Vy.
        PrintInstruction(i, "XOR");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x4:
        // 8xy4: Vx += Vy; Adds Vxy to Vy. Vf is set to 1 when there's a carry, and to 0 when there isn't.
        PrintInstruction(i, "ADDR");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x5:
        // 8xy5: Vx -= Vy; Vy is subtracted from Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.
        PrintInstruction(i, "SUB");
        printf("V%X, V%X", vx, vy);
        break;
    case 0x6:
        // 8xy6: Vx >>= 1; Stores the least significant bit of Vx in Vf and then shifts Vx to the right by 1.
        PrintInstruction(i, "SHR");
        printf("V%X", vx);
        break;
    case 0x7:
        // 8xy7: Vx = Vy - Vx; Sets Vx to Vy minus Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.
        PrintInstruction(i, "SUBINV");
        printf("V%X, V%X", vx, vy);
        break;
    case 0xe:
        // 8xyE: Vx <<= 1; Stores the most significant bit of Vx in Vf and then shifts Vx to the left by 1.
        PrintInstruction(i, "SHL");
        printf("V%X", vx);
        break;
    default:
        PrintInstruction(i, "; unknown (8)");
        break;
    }
}


// 9xy0: if(Vx != Vy); Skips the next instruction if Vx doesn't equal Vy.

void ch8::SkipRegisterNotEqualInstruction::Execute() noexcept {}

void ch8::SkipRegisterNotEqualInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    u8 vy = getLeftNibble(r);
    PrintInstruction(i, "SRNE");
    printf("V%X, V%X", vx, vy);
}


// Annn: i = nnn; Sets i to the address nnn.

void ch8::MoveAddressInstruction::Execute() noexcept {}

void ch8::MoveAddressInstruction::Disassemble(u16 i) const noexcept {
    u16 nnn = get16BitAddress(l, r);
    PrintInstruction(i, "MOVI");
    printf("%X", nnn);
}


// Bnnn: PC = V0 + nnn; Jumps to the address nnn plus V0.

void ch8::JumpRegisterInstruction::Execute() noexcept {}

void ch8::JumpRegisterInstruction::Disassemble(u16 i) const noexcept {
    i16 nnn = get16BitAddress(l, r);
    PrintInstruction(i, "JMPV");
    printf("%X", nnn);
}


// Cxnn: Vx = rand() & nn; Sets Vx to the result of a bitwise and operation on a random number (Typically: 0 to 255) and nn.

void ch8::RandomMaskInstruction::Execute() noexcept {}

void ch8::RandomMaskInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    u8 nn = r;
    PrintInstruction(i, "RNDMSK");
    printf("V%X, V%u", vx, nn);
}


// Dxyn: draw(Vx, Vy, n); Draws a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a height of n pixels.
// Each row of 8 pixels is read as bit-coded starting from memory location i; i value doesn’t change after the execution of this instruction.
// As described above, Vf is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
// and to 0 if that doesn’t happen.

void ch8::DrawInstruction::Execute() noexcept {}

void ch8::DrawInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);
    u8 vy = getLeftNibble(r);
    u8 n = getRightNibble(r);
    PrintInstruction(i, "DRAW");
    printf("V%X, V%X, %.2u", vx, vy, n);

    // Super Chip-48: Dxy0 - DRW Vx, Vy, 0
}


// Exnn: Skip (usually the next instruction is a jump to skip a code block).

void ch8::SkipKeyInstruction::Execute() noexcept {}

void ch8::SkipKeyInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);

    switch (r) {
    case 0x9e:
        // Ex9E: if(key() == Vx); Skips the next instruction if the key stored in Vx is pressed.
        PrintInstruction(i, "SKE");
        printf("V%X", vx);
        break;
    case 0xa1:
        // ExA1; if(key() != Vx); Skips the next instruction if the key stored in Vx isn't pressed.
        PrintInstruction(i, "SKNE");
        printf("V%X", vx);
        break;
    default:
        PrintInstruction(i, "; unknown (E)");
        break;
    }
}


// Fxkk: F stands for Fun!

void ch8::FunInstruction::Execute() noexcept {}

void ch8::FunInstruction::Disassemble(u16 i) const noexcept {
    u8 vx = getRightNibble(l);

    switch (r) {
    case 0x07:
        // Fx07: Vx = get_delay(); Sets Vx to the value of the delay timer.
        PrintInstruction(i, "GETDLY");
        printf("V%X", vx);
        break;
    case 0x0a:
        // Fx0A: Vx = get_key(); A key press is awaited, and then stored in Vx.
        // (Blocking Operation. All instruction halted until next key event)
        PrintInstruction(i, "GETKEY");
        printf("V%X", vx);
        break;
    case 0x15:
        // Fx15: delay_timer(Vx); Sets the delay timer to Vx.
        PrintInstruction(i, "SETDLY");
        printf("V%X", vx);
        break;
    case 0x18:
        // Fx18: sound_timer(Vx); Sets the sound timer to VX.
        PrintInstruction(i, "SETSND");
        printf("V%X", vx);
        break;
    case 0x1e:
        // Fx1E: I += Vx; Adds Vx to i. Vf is set to 1 when there is a range overflow (I + Vx > 0xFFF), and to 0 when there isn't.
        PrintInstruction(i, "ADDI");
        printf("V%X", vx);
        break;
    case 0x29:
        // Fx29: I = sprite_addr[Vx]; Sets I to the location of the sprite for the character in Vx.
        // Characters 0-F (in hexadecimal) are represented by a 4x5 font.
        PrintInstruction(i, "DRAW");
        printf("V%X", vx);
        break;
    case 0x33:
        /*
        Fx33:
        - set_BCD(Vx);
        - *(I + 0) = BCD(3);
        - *(I + 1) = BCD(2);
        - *(I + 2) = BCD(1);
        Stores the binary-coded decimal representation of Vx, with the most significant of three digits at the address in i,
        the middle digit at i plus 1, and the least significant digit at i plus 2.
        (In other words, take the decimal representation of Vx, place the hundreds digit in memory at location in i,
        the tens digit at location i+1, and the ones digit at location i+2.)
        */
        PrintInstruction(i, "BCD");
        printf("V%X", vx);
        break;
    case 0x55:
        // Fx55: reg_dump(Vx, &i); Stores V0 to Vx (including Vx) in memory starting at address i.
        // The offset from i is increased by 1 for each value written, but i itself is left unmodified.
        PrintInstruction(i, "SAVE");
        printf("V%X", vx);
        break;
    case 0x65:
        // Fx65: reg_load(Vx, &i); Fills V0 to Vx (including Vx) with values from memory starting at address i.
        // The offset from i is increased by 1 for each value written, but i itself is left unmodified.
        PrintInstruction(i, "LOAD");
        printf("V%X", vx);
        break;
    default:
        PrintInstruction(i, "; unknown (F)");
        break;
    }

    /*
    Super Chip-48:
    Fx30 - LD HF, Vx
    Fx75 - LD R, Vx
    Fx85 - LD Vx, R
    */
}