#include "sdl.hpp"

ch8::Interface::Interface() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) < 0) {
        error = SDL_GetError();
    }
}

ch8::Interface::~Interface() {
    stop();
    SDL_Quit();
}

bool ch8::Interface::start(const char* title, i32 width, i32 height) noexcept {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        error = SDL_GetError();
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        error = SDL_GetError();
        return false;
    }

    return true;
}

void ch8::Interface::stop() noexcept {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }

    if (window != NULL) {
        SDL_DestroyWindow(window);
    }
}