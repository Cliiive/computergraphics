#include "shader.h"
#include "hittable.h"
#include "light.h"
#include <vector>
namespace rt {

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

const Vector3df lambertian(std::vector<Light> lights, const HitInfo& info) {
    // If we didn't hit anything, return black
    if (!info.hit) {
        return Vector3df{0.0f, 0.0f, 0.0f};
    }

    Vector3df result{0.0f, 0.0f, 0.0f};

    // For each light source, calculate diffuse lighting contribution
    for (const auto& light : lights) {
        // Calculate light direction (from hit point to light)
        Vector3df lightDir = light.position - info.hitPoint;
        float     distance = lightDir.length();
        lightDir.normalize();

        // Calculate the cosine of angle between normal and light direction
        float cosTheta = std::max(0.0f, info.normal * lightDir);

        Vector3df diffuse = cosTheta * info.material->diffuse;

        result = result + diffuse;
    }

    // Ensure values are in valid range
    for (int i = 0; i < 3; i++) {
        result.vector[i] = std::max(0.0f, std::min(1.0f, result.vector[i]));
    }

    return result;
}
}  // namespace rt::shader