#ifndef GOGA_TAMAS_CHIP_8_SDL_HPP
#define GOGA_TAMAS_CHIP_8_SDL_HPP

#include <SDL2/SDL.h>
#include <string>
#include "defines.hpp"

// Contains all interactive parts of the project (graphics, sound & keyboard input).

namespace ch8 {
    struct Interface {
        SDL_Window* window;
        SDL_Renderer* renderer;

        std::string error;

        Interface();
        ~Interface();

        // Don't forget copy & move.

        bool start(const char* title, i32 width, i32 height) noexcept;
        void stop() noexcept;

        // The rest of the functions only make sense if start was already called.
        // I hereby declare that calling any of these without starting first UNDEFINED BEHAVIOUR.

        void ClearScreen() noexcept {
            SDL_SetRenderDrawColor(renderer, 0,0,0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    };
}


#endif // GOGA_TAMAS_CHIP_8_SDL_HPP