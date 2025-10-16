#pragma once
#include "hittable.h"
#include "light.h"
#include <memory>
#include <vector>
#include <optional>

namespace rt {

std::vector<std::unique_ptr<Hittable>> createScene();

std::optional<const Hittable*>
findNearestObject(const Ray3df& ray, const std::vector<std::unique_ptr<Hittable>>& scene);

}  // namespace rt::scene
