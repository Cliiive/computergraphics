#pragma once

#include <SDL2/SDL.h>
#include "math.h"
namespace rt::win {

constexpr int         WINDOW_WIDTH  = 500;
constexpr int         WINDOW_HEIGTH = 500;
constexpr const char* WINDOW_TITLE  = "Raytracer";

struct WindowPos {
    int x;
    int y;
};

class Window {
  public:
    Window(const char* title, int w, int h);

    ~Window();

    inline SDL_Window* handle() const {
        return _handle;
    }
    inline SDL_Surface* surface() const {
        return _surface;
    }

  private:
    SDL_Window*  _handle{};
    SDL_Surface* _surface{};
};

void setPixelColor(Window& window, WindowPos& pos, Uint32 color);

void waitForExit();

}  // namespace rt::win