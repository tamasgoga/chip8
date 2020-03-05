Chip-8 interpreter/emulator that uses [SDL2](https://www.libsdl.org/) for graphics & sound. Thank you, [Lazy Foo](https://lazyfoo.net/tutorials/SDL/index.php)!

### Rules:
1. Exceptions should not stop execution or disassembly.
1. Separate platform-dependent functionality, clearly.
1. All public methods and functions begin with upper-case. "main" is the only exception, for obvious reasons.
1. If it's a class: use its methods. If it's a struct: the properties may be used directly (or the convenience methods, if provided).
1. Don't add useless accessor methods.

### Resources:
+ [Chip-8 specs;](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
+ [a tutorial;](http://emulator101.com/)
+ [a Bisqwit video (read the description as well);](https://www.youtube.com/watch?v=rpLoS7B6T94)
+ [and, of course, the Wikipedia article.](https://en.wikipedia.org/wiki/CHIP-8)

__Copied from the description for *Bisqwit's* video:__
> NOTE: It's been brought to my attention that the document that I was reading has several misconceptions of the Chip8 architecture. Though I also read the Wikipedia article and a few other documents, there's a few things I missed. Here's a list of errors in my interpreter. Note that most of these "errors" have no practical bearing to executing pure chip8 programs, but are merely pedantic compatibility issues targeting the platform where the Chip-8 interpreter was originally released.
> 
> * I and PC are 16-bit registers, not 12. That means that the "& 0xFFF" parts in my code should not be there, or should be replaced with "& 0xFFFF". This applies also to how the carry flag is handled in the "add I, Vx" instruction.
> * The memory map is 64 kilobytes, not 4 kilobytes. However, only the first 4 kilobytes are RAM. It is in fact possible to have more RAM (or less), from 2 to 32 kB. RAM is mirrored in the whole $0000-$7FFF range. A conforming implementation should perhaps use a 64-kilobyte array, and permit modifications only within the first 4 kilobytes. Ideally, part of the array should contain a verbatim copy of the Telmac 1800 ROM...
> * The built-in font should not be stored in the first 4 kilobytes, but within the "ROM", which is mapped at $8000-$81FF and probably mirrored throughout $8000-$FFFF. This memory should also be non-modifiable. See http://www.chip8.com/cosmacvip/VIP_Manual.pdf for information about the exact location and layout.
> * The 12-element stack should be stored at the particular address of $0FA0-$0FCF (or in an equivalent address of whichever bank is the last RAM bank)
> * The 16 V registers should be stored at the particular addresses of $0FF0-$0FFF (same note as above).
> * The 256-byte display memory should be stored at $0E00-$0EFF (same note as above).
> * The timer registers and stack pointer should not be stored in RAM (the COSMAC VIP stores them in R8 and R2 registers respectively).
> * The "sys" instruction should do emulation of RCA 1802 (COSMAC). This is deliberately left out from almost all modern Chip8 interpreters, including mine. On a similar note, the "cls" and "ret" instructions (00E0 and 00EE respectively) should be implemented as RCA 1802 code pre-initialized in those RAM addresses, and treated as the "sys" instruction instead. But then you could just create a COSMAC #emulator and forget about Chip8, as it's just a program running within the first 256 bytes of RAM and utilizing the 512-byte ROM. More information: http://datasheets.chipdb.org/RCA/MPM-201B_CDP1802_Users_Manual_Nov77.pdf
