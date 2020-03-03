#include <cstring>
#include <fstream>

#include "os.hpp"

// Arguments

os::Arguments::Arguments(int count, char** args) {
    if (count < 2) {
        return;
    }

    // Usage implies last arg is rom
    path = args[--count];
    
    // Parse options
    for (int i = 1; i < count; ++i) {
        if (strcmp("hex", args[i]) == 0) {
            options |= emu::OPTIONS_HEX;
        } else if (strcmp("asm", args[i]) == 0) {
            options |= emu::OPTIONS_CODE;
        }
    }
}

// Files

static const char* fileError = nullptr;

std::vector<u8> os::ReadChip8File(std::string path) {        
    fileError = nullptr;
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        fileError = strerror(errno);
        return std::vector<u8>();
    }

    // https://stackoverflow.com/questions/5420317/reading-and-writing-binary-file
    auto program = std::vector<u8>(std::istreambuf_iterator<char>(file), {});
    return program.size() % 2 == 0 ? program : std::vector<u8>();
}

bool os::HasFileError() noexcept {
    return fileError != nullptr;
}

const char* os::GetFileError() noexcept {
    return fileError != nullptr ? fileError : ""; 
}