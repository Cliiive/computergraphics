#pragma once

#include <SDL2/SDL.h>
#include "math.h"

namespace rt {
class Material {
  public:
    Vector3df ambient{0.1f, 0.1f, 0.1f};
    Vector3df diffuse{0.7f, 0.7f, 0.7f};
    Vector3df specular{1.0f, 1.0f, 1.0f};
    float     shininess    = 32.0f;
    float     reflectivity = 0.0f;

    // Add methods as needed
    Vector3df getColor() noexcept {
        return diffuse;
    }
};

// Color conversion function
const Uint32 vecToPixel(const Vector3df& c);
}  // namespace rt