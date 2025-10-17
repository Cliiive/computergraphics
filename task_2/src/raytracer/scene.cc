#include "scene.h"
#include "light.h"
#include "geometric_object.h"
#include <memory>
namespace rt {
std::vector<std::unique_ptr<Hittable>> createScene() {
    std::vector<std::unique_ptr<Hittable>> objects;

    Material red, green, white, wall;
    red.diffuse   = {0.5f, 0.0f, 0.0f};
    green.diffuse = {0.0f, 0.5f, 0.0f};
    white.diffuse = {0.9f, 0.9f, 0.9f};
    wall.diffuse  = {0.2f, 0.2f, 0.2f};

    const float xMin = -1.0f, xMax = 1.0f;
    const float yMin = -1.0f, yMax = 1.0f;
    const float zNear = -10.0f, zFar = -30.0f;

    // Left wall
    Vector3df leftA{xMin, yMin, zNear}, leftB{xMin, yMax, zNear}, leftC{xMin, yMin, zFar},
        leftD{xMin, yMax, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(green, leftA, leftC, leftB));
    objects.emplace_back(std::make_unique<TriangleObject>(green, leftC, leftD, leftB));

    // Right wall (flipped normals)
    Vector3df rightA{xMax, yMin, zNear}, rightB{xMax, yMax, zNear}, rightC{xMax, yMax, zFar},
        rightD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(red, rightA, rightB, rightD));
    objects.emplace_back(std::make_unique<TriangleObject>(red, rightB, rightC, rightD));

    // Top wall (flipped normals)
    Vector3df topA{xMin, yMax, zNear}, topB{xMax, yMax, zNear}, topC{xMin, yMax, zFar},
        topD{xMax, yMax, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, topA, topB, topC));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, topB, topD, topC));

    // Bottom wall
    Vector3df bottomA{xMin, yMin, zNear}, bottomB{xMax, yMin, zNear}, bottomC{xMin, yMin, zFar},
        bottomD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, bottomA, bottomC, bottomB));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, bottomC, bottomD, bottomB));

    // Back wall
    Vector3df backA{xMin, yMax, zFar}, backB{xMax, yMax, zFar}, backC{xMin, yMin, zFar},
        backD{xMax, yMin, zFar};
    objects.emplace_back(std::make_unique<TriangleObject>(wall, backA, backC, backB));
    objects.emplace_back(std::make_unique<TriangleObject>(wall, backC, backD, backB));

    // Sphere
    objects.emplace_back(
        std::make_unique<SphereObject>(white, Vector3df{-0.4f, -0.5f, -12.0f}, 0.3f));

    // Sphere
    objects.emplace_back(
        std::make_unique<SphereObject>(green, Vector3df{0.3f, -0.2f, -12.0f}, 0.3f));

    return objects;
}

std::optional<const HitInfo>
findNearestObject(const Ray3df& ray, const std::vector<std::unique_ptr<Hittable>>& scene) {
    const Hittable* closest = nullptr;
    HitInfo         finalInfo{};
    float           minT = std::numeric_limits<float>::infinity();

    for (const auto& obj : scene) {
        HitInfo info = obj->intersect(ray);
        if (info.hit && info.tHit > 0 && info.tHit < minT) {
            finalInfo = info;
            minT      = info.tHit;
            closest   = obj.get();
        }
    }

    if (closest)
        return finalInfo;
    return std::nullopt;
}

std::vector<Light> findLightSources(const HitInfo&                          info,
                                    std::vector<std::unique_ptr<Hittable>>& scene) {
    std::vector<Light> lightSources{};
    float              epsilon = 0.001f;
    for (const auto& light : LIGHTS) {
        Vector3df origin = info.hitPoint + epsilon * info.normal;
        Ray3df    shadowRay{origin, light.position - origin};
        auto      shadowHit = findNearestObject(shadowRay, scene);

        if (!shadowHit.has_value()) {
            lightSources.push_back(light);
        }
    }
    return lightSources;
}
}  // namespace rt