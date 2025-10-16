#include "shader.h"

namespace rt::shader {

const Uint32 vecToPixel(const Vector3df& c) {
    auto to8 = [](double v) -> Uint32 {
        if (v < 0.0)
            v = 0.0;
        if (v > 1.0)
            v = 1.0;
        return static_cast<Uint32>(v * 255.0 + 0.5);
    };

    Uint32 r = to8(c.vector[0]);
    Uint32 g = to8(c.vector[1]);
    Uint32 b = to8(c.vector[2]);

    // Pack into 0xRRGGBB
    return (r << 16) | (g << 8) | b;
}

const Vector3df lambertian(const Vector3df& N, const Vector3df& albedo
                           //const std::vector<Light>& lights
);
}  // namespace rt::shader