#pragma once
#include "hittable.h"
#include "light.h"
#include <memory>
#include <vector>
#include <optional>

namespace rt {

std::vector<std::unique_ptr<Hittable>> createScene();
const std::vector<Light>               LIGHTS = {
    Light{Vector3df{0.1f, 0.3f, -8.0f}, Vector3df{1.0f, 1.0f, 1.0f}, 0.5f}};

std::optional<const HitInfo> findNearestObject(const Ray3df&                                 ray,
                                               const std::vector<std::unique_ptr<Hittable>>& scene);

std::vector<Light> findLightSources(const HitInfo&                          info,
                                    std::vector<std::unique_ptr<Hittable>>& scene);

}  // namespace rt
