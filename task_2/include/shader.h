#pragma once

#include "math.h"
#include "geometry.h"
#include <SDL2/SDL.h>

namespace rt::shader {
// Die rekursive raytracing-Methode. Am besten ab einer bestimmten Rekursionstiefe (z.B. als Parameter übergeben) abbrechen.
const Uint32 vecToPixel(const Vector3df& c);

struct Material {
    // Basic material properties
    Vector3df ambient{0.1f, 0.1f, 0.1f};
    Vector3df diffuse{0.7f, 0.7f, 0.7f};
    Vector3df specular{1.0f, 1.0f, 1.0f};
    float     shininess    = 32.0f;
    float     reflectivity = 0.0f;

    // Return the base color for this materia
    Vector3df getColor() const noexcept {
        return diffuse;
    }
};

};  // namespace rt::shader