#include <cstdio>
#include "instructions.hpp"

// Base

void ch8::Instruction::PrintInstruction(const char* name) const noexcept {
    printf("%.4x:   <%.2x%.2x>   %-8s ", state.pc, l, r, name);
}

void ch8::Instruction::Execute() noexcept {
    // printf("<pc = %.4x> Ignored instruction, nothing executed.\n", this->state.pc);
}

void ch8::Instruction::Disassemble() noexcept {
    PrintInstruction("; ignored");
}


// 00e0: Clear screen.

void ch8::ClearScreenInstruction::Execute() noexcept {
    interface.ClearScreen();
}

void ch8::ClearScreenInstruction::Disassemble() noexcept {
    PrintInstruction("CLS");
}


// 00ee: Return.

void ch8::ReturnInstruction::Execute() noexcept {}

void ch8::ReturnInstruction::Disassemble() noexcept {
    PrintInstruction("RET");
}


// 1nnn: goto nnn; Jumps to address nnn.

void ch8::JumpInstruction::Execute() noexcept {}

void ch8::JumpInstruction::Disassemble() noexcept {
    u16 nnn = Get16BitAddress();

    PrintInstruction("JMP");
    printf("%X", nnn);
}


// 2nnn: *(nnn)(); Calls subroutine at nnn.

void ch8::CallInstruction::Execute() noexcept {}

void ch8::CallInstruction::Disassemble() noexcept {
    u16 nnn = Get16BitAddress();

    PrintInstruction("CALL");
    printf("%X", nnn);
}


// 3xnn: if(Vx == nn); Skips the next instruction if Vx equals nn.
// Usually the next instruction is a jump to skip a code block. Applies for 4, 5 & 9, too.

void ch8::SkipEqualInstruction::Execute() noexcept {}

void ch8::SkipEqualInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    i8 nn = r;

    PrintInstruction("SE");
    printf("V%X, %d", vx, nn);
}


// 4xnn: if(Vx != nn); Skips the next instruction if Vx doesn't equal nn.

void ch8::SkipNotEqualInstruction::Execute() noexcept {}

void ch8::SkipNotEqualInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    i8 nn = r;

    PrintInstruction("SNE");
    printf("V%X, %d", vx, nn);
}


// 5xy0: if(Vx == Vy); Skips the next instruction if Vx equals Vy.

void ch8::SkipRegisterEqualInstruction::Execute() noexcept {}

void ch8::SkipRegisterEqualInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy= GetLeftNibble(r);

    PrintInstruction("SRE");
    printf("V%X, V%X", vx, vy);
}


// 6xnn: Vx = nn; Sets VX to NN.

void ch8::MoveInstruction::Execute() noexcept {}

void ch8::MoveInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    i8 nn = r;

    PrintInstruction("MOV");
    printf("V%X, %d", vx, nn);
}


// 7xnn: Vx += nn; Adds nn to Vx. (Carry flag is not changed.)

void ch8::AddInstruction::Execute() noexcept {}

void ch8::AddInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    i8 nn = r;

    PrintInstruction("ADD");
    printf("V%X, %d", vx, nn);
}


// 8xy0: Vx = Vy; Sets Vx to the value of Vy.

void ch8::MoveRegisterInstruction::Execute() noexcept {}

void ch8::MoveRegisterInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("MOVR");
    printf("V%X, V%X", vx, vy);
}


// 8xy1: Vx |= Vy; Sets Vx to Vx or Vy. (Bitwise OR operation)

void ch8::OrInstruction::Execute() noexcept {}

void ch8::OrInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("OR");
    printf("V%X, V%X", vx, vy);
}


// 8xy2: Vx &= Vy; Sets Vx to Vx and Vy. (Bitwise AND operation)

void ch8::AndInstruction::Execute() noexcept {}

void ch8::AndInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("AND");
    printf("V%X, V%X", vx, vy);
}


// 8xy3: Vx ^= Vy; Sets Vx to Vx xor Vy.

void ch8::XorInstruction::Execute() noexcept {}

void ch8::XorInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("XOR");
    printf("V%X, V%X", vx, vy);
}


// 8xy4: Vx += Vy; Adds Vxy to Vy. Vf is set to 1 when there's a carry, and to 0 when there isn't.

void ch8::AddRegisterInstruction::Execute() noexcept {}

void ch8::AddRegisterInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("ADDR");
    printf("V%X, V%X", vx, vy);
}


// 8xy5: Vx -= Vy; Vy is subtracted from Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.

void ch8::SubInstruction::Execute() noexcept {}

void ch8::SubInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("SUB");
    printf("V%X, V%X", vx, vy);
}


// 8xy6: Vx >>= 1; Stores the least significant bit of Vx in Vf and then shifts Vx to the right by 1.

void ch8::ShiftRightInstruction::Execute() noexcept {}

void ch8::ShiftRightInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SHR");
    printf("V%X", vx);
}


// 8xy7: Vx = Vy - Vx; Sets Vx to Vy minus Vx. Vf is set to 0 when there's a borrow, and 1 when there isn't.

void ch8::SubInverseInstruction::Execute() noexcept {}

void ch8::SubInverseInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("SUBINV");
    printf("V%X, V%X", vx, vy);
}


// 8xyE: Vx <<= 1; Stores the most significant bit of Vx in Vf and then shifts Vx to the left by 1.

void ch8::ShiftLeftInstruction::Execute() noexcept {}

void ch8::ShiftLeftInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SHL");
    printf("V%X", vx);
}


// 9xy0: if(Vx != Vy); Skips the next instruction if Vx doesn't equal Vy.

void ch8::SkipRegisterNotEqualInstruction::Execute() noexcept {}

void ch8::SkipRegisterNotEqualInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);

    PrintInstruction("SRNE");
    printf("V%X, V%X", vx, vy);
}


// Annn: i = nnn; Sets i to the address nnn.

void ch8::MoveAddressInstruction::Execute() noexcept {}

void ch8::MoveAddressInstruction::Disassemble() noexcept {
    u16 nnn = Get16BitAddress();

    PrintInstruction("MOVI");
    printf("%X", nnn);
}


// Bnnn: PC = V0 + nnn; Jumps to the address nnn plus V0.

void ch8::JumpRegisterInstruction::Execute() noexcept {}

void ch8::JumpRegisterInstruction::Disassemble() noexcept {
    i16 nnn = Get16BitAddress();

    PrintInstruction("JMPV");
    printf("%X", nnn);
}


// Cxnn: Vx = rand() & nn; Sets Vx to the result of a bitwise and operation on a random number (Typically: 0 to 255) and nn.

void ch8::RandomMaskInstruction::Execute() noexcept {}

void ch8::RandomMaskInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 nn = r;

    PrintInstruction("RNDMSK");
    printf("V%X, V%u", vx, nn);
}


// Dxyn: draw(Vx, Vy, n); Draws a sprite at coordinate (Vx, Vy) that has a width of 8 pixels and a height of n pixels.
// Each row of 8 pixels is read as bit-coded starting from memory location i; i value doesn’t change after the execution of this instruction.
// As described above, Vf is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
// and to 0 if that doesn’t happen.

void ch8::DrawInstruction::Execute() noexcept {}

void ch8::DrawInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);
    u8 vy = GetLeftNibble(r);
    u8 n = GetRightNibble(r);

    PrintInstruction("DRAW");
    printf("V%X, V%X, %.2u", vx, vy, n);
}


// Ex9E: if(key() == Vx); Skips the next instruction if the key stored in Vx is pressed.

void ch8::SkipKeyEqualsInstruction::Execute() noexcept {}

void ch8::SkipKeyEqualsInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SKE");
    printf("V%X", vx);
}


// ExA1; if(key() != Vx); Skips the next instruction if the key stored in Vx isn't pressed.

void ch8::SkipKeyNotEqualsInstruction::Execute() noexcept {}

void ch8::SkipKeyNotEqualsInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SKNE");
    printf("V%X", vx);
}


// Fx07: Vx = get_delay(); Sets Vx to the value of the delay timer.

void ch8::GetDelayInstruction::Execute() noexcept {}

void ch8::GetDelayInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("GETDLY");
    printf("V%X", vx);
}


// Fx0A: Vx = get_key(); A key press is awaited, and then stored in Vx. (Blocking Operation. All instruction halted until next key event.)

void ch8::GetKeyInstruction::Execute() noexcept {}

void ch8::GetKeyInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("GETKEY");
    printf("V%X", vx);
}


// Fx15: delay_timer(Vx); Sets the delay timer to Vx.

void ch8::SetDelayInstruction::Execute() noexcept {}

void ch8::SetDelayInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SETDLY");
    printf("V%X", vx);
}


// Fx18: sound_timer(Vx); Sets the sound timer to VX.

void ch8::SetSoundInstruction::Execute() noexcept {}

void ch8::SetSoundInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SETSND");
    printf("V%X", vx);
}


// Fx1E: I += Vx; Adds Vx to i. Vf is set to 1 when there is a range overflow (I + Vx > 0xFFF), and to 0 when there isn't.

void ch8::AddToAddressInstruction::Execute() noexcept {}

void ch8::AddToAddressInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("ADDI");
    printf("V%X", vx);
}


// Fx29: I = sprite_addr[Vx]; Sets I to the location of the sprite for the character in Vx.
// Characters 0-F (in hexadecimal) are represented by a 4x5 font.

void ch8::SetSpriteInstruction::Execute() noexcept {}

void ch8::SetSpriteInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SPRITE");
    printf("V%X", vx);
}


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

(BCD = binary coded decimal);
*/

void ch8::SetBcdInstruction::Execute() noexcept {}

void ch8::SetBcdInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("BCD");
    printf("V%X", vx);
}


// Fx55: reg_dump(Vx, &i); Stores V0 to Vx (including Vx) in memory starting at address i.
// The offset from i is increased by 1 for each value written, but i itself is left unmodified.

void ch8::SaveRegistersInstruction::Execute() noexcept {}

void ch8::SaveRegistersInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("SAVE");
    printf("V%X", vx);
}


// Fx65: reg_load(Vx, &i); Fills V0 to Vx (including Vx) with values from memory starting at address i.
// The offset from i is increased by 1 for each value written, but i itself is left unmodified.

void ch8::LoadRegistersInstruction::Execute() noexcept {}

void ch8::LoadRegistersInstruction::Disassemble() noexcept {
    u8 vx = GetRightNibble(l);

    PrintInstruction("LOAD");
    printf("V%X", vx);
}