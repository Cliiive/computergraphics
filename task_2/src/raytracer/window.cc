#include "window.h"
#include <stdexcept>

namespace rt::win {

Window::Window(const char* title, int w, int h) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error(SDL_GetError());
    }

    _handle = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    if (!_handle)
        throw std::runtime_error(SDL_GetError());
    _surface = SDL_GetWindowSurface(_handle);
};

Window::~Window() {
    if (_handle)
        SDL_DestroyWindow(_handle);
}

void setPixelColor(Window& window, WindowPos& pos, Uint32 color) {
    Uint32* pixels                              = (Uint32*) window.surface()->pixels;
    pixels[pos.y * window.surface()->w + pos.x] = color;
    SDL_UpdateWindowSurface(window.handle());
}

void waitForExit() {
    bool      running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;  // exit loop
            }
        }
        // Optional: add delay to reduce CPU usage
        SDL_Delay(16);  // ~60 FPS
    }
}

}  // namespace rt::win