#include "scene.h"
#include "light.h"
#include "geometric_object.h"

namespace rt {
std::vector<std::unique_ptr<Hittable>> createScene() {
    std::vector<std::unique_ptr<Hittable>> objects;

    Material red, green, white, wall;
    red.diffuse   = {0.9f, 0.1f, 0.1f};
    green.diffuse = {0.1f, 0.9f, 0.1f};
    white.diffuse = {0.9f, 0.9f, 0.9f};
    wall.diffuse  = {0.8f, 0.8f, 0.8f};

    const float xMin = -1.0f, xMax = 1.0f;
    const float yMin = -1.0f, yMax = 1.0f;
    const float zNear = -10.0f, zFar = -30.0f;

    // Left wall
    Vector3df leftA{xMin, yMin, zNear}, leftB{xMin, yMax, zNear}, leftC{xMin, yMin, zFar},
        leftD{xMin, yMax, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(green, leftA, leftB, leftC));
    objects.emplace_back(std::make_unique<TriangleObject>(green, leftC, leftB, leftD));

    // Right wall
    Vector3df rightA{xMax, yMin, zNear}, rightB{xMax, yMax, zNear}, rightC{xMax, yMax, zFar},
        rightD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(red, rightA, rightB, rightD));
    objects.emplace_back(std::make_unique<TriangleObject>(red, rightB, rightC, rightD));

    // Top wall
    Vector3df topA{xMin, yMax, zNear}, topB{xMax, yMax, zNear}, topC{xMin, yMax, zFar},
        topD{xMax, yMax, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, topA, topB, topC));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, topC, topB, topD));

    // Bottom wall
    Vector3df bottomA{xMin, yMin, zNear}, bottomB{xMax, yMin, zNear}, bottomC{xMin, yMin, zFar},
        bottomD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, bottomA, bottomB, bottomC));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, bottomC, bottomB, bottomD));

    // Back wall
    Vector3df backA{xMin, yMax, zFar}, backB{xMax, yMax, zFar}, backC{xMin, yMin, zFar},
        backD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, backA, backB, backC));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, backC, backB, backD));

    // Sphere
    objects.emplace_back(
        std::make_unique<SphereObject>(white, Vector3df{-0.1f, -0.5f, -8.0f}, 0.3f));

    return objects;
}

const std::vector<Light> lights = {
    Light{Vector3df{1.0f, 1.0f, 1.0f}, Vector3df{1.0f, 1.0f, 1.0f}, 1.0}};

std::optional<const Hittable*>
findNearestObject(const Ray3df& ray, const std::vector<std::unique_ptr<Hittable>>& scene) {
    const Hittable* closest = nullptr;
    float           minT    = std::numeric_limits<float>::infinity();

    for (const auto& obj : scene) {
        HitInfo info = obj->intersect(ray);
        if (info.hit && info.tHit > 0 && info.tHit < minT) {
            minT    = info.tHit;
            closest = obj.get();
        }
    }
    if (closest)
        return closest;
    return std::nullopt;
}
}  // namespace rt