#include <string>
#include <iostream>
#include <fstream>

namespace emu {
    class Loader {
    public:
        enum option {
            HEX = 0x1
        };
        
        Loader(int count, char** args) {
            using std::cout;
            using std::endl;
            
            if (count < 2) {
                cout << "Usage: chip8 <option> path_to_rom" << endl;
            }
            
            for (int i = 1; i < count; ++i) {
                cout << args[i] << endl;
            }
        }
    };
}

int main(int argc, char** argv) {
    emu::Loader(argc, argv);
}