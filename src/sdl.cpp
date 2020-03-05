#include <stdexcept>
#include "sdl.hpp"

// Starting & stopping SDL is done statically, since we only want to do those operations once.

static u32 interfaceCount = 0u;

static inline void startSDL() {
    if (interfaceCount > 0u || SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) == 0) {
        ++interfaceCount;
        return;
    }

    throw std::runtime_error(SDL_GetError());
}

static inline void stopSDL() noexcept {
    if (interfaceCount > 1u) {
        --interfaceCount;
        return;
    }

    interfaceCount = 0u;
    SDL_Quit();
}


// The actual interface

ch8::Interface::Interface() {
    startSDL();
}

ch8::Interface::~Interface() {
    Stop();
    stopSDL();
}

// Copy
ch8::Interface::Interface(const Interface& other) {
    this->operator=(other);
}

ch8::Interface& ch8::Interface::operator=(const Interface& other) {
    startSDL();

    if (other.window != nullptr) {
        i32 x, y, w, h;
        SDL_GetWindowPosition(other.window, &x, &y);
        SDL_GetWindowSize(other.window, &w, &h);

        window = SDL_CreateWindow(SDL_GetWindowTitle(other.window), x, y, w, h, SDL_GetWindowFlags(other.window));
        if (window == nullptr) {
            throw std::runtime_error(SDL_GetError());
        }
    }

    if (other.renderer != nullptr && window != nullptr) {
        SDL_RendererInfo info;
        SDL_GetRendererInfo(other.renderer, &info);

        renderer = SDL_CreateRenderer(window, -1, info.flags);
        if (renderer == nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
            throw(std::runtime_error(SDL_GetError()));
        }
    }

    return *this;
}

// Move
ch8::Interface::Interface(Interface&& other) {
    this->operator=(std::move(other));
}

ch8::Interface& ch8::Interface::operator=(Interface&& other) {
    startSDL();

    window = other.window;
    other.window = nullptr;

    renderer = other.renderer;
    other.renderer = nullptr;

    return *this;
}

void ch8::Interface::Start(const char* title, i32 width, i32 height) {
    if (window == nullptr) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            throw(std::runtime_error(SDL_GetError()));
        }

        // Make sure to delete any previous renderers.
        if (renderer != nullptr) {
            SDL_DestroyRenderer(renderer);
            renderer = nullptr;
        }
    }

    if (renderer == nullptr) {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            SDL_DestroyWindow(window);
            window = nullptr;
            throw(std::runtime_error(SDL_GetError()));
        }
    }
}

void ch8::Interface::Stop() noexcept {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}