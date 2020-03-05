#include "sdl.hpp"

// Starting & stopping SDL is done statically, since we only want to do those operations once.

static u32 interfaceCount = 0u;

static inline bool startSDL() {
    if (interfaceCount > 0u || SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) == 0) {
        ++interfaceCount;
        return true;
    }

    return false;
}

static inline bool isSDLRunning() {
    return interfaceCount > 0u;
}

static inline void stopSDL() {
    if (interfaceCount-- == 1u) {
        SDL_Quit();
    }
}


// The actual interface

ch8::Interface::Interface()
    : window(nullptr)
    , renderer(nullptr)
{
    if (!startSDL()) {
        error = SDL_GetError();
    }
}

ch8::Interface::~Interface() {
    Stop();
    stopSDL();
}

ch8::Interface::Interface(Interface&& other) {
    this->operator=(std::move(other));
}

ch8::Interface& ch8::Interface::operator=(Interface&& other) {
    window = other.window;
    other.window = nullptr;

    renderer = other.renderer;
    other.renderer = nullptr;

    error = std::move(other.error);
    other.error.clear();

    return *this;
}

bool ch8::Interface::Start(const char* title, i32 width, i32 height) noexcept {
    if (!isSDLRunning()) {
        if (!startSDL()) {
            error = SDL_GetError();
            return false;
        }
    }

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