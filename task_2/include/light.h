#pragma once

#include "math.h"
#include <vector>

namespace rt {
struct HitInfo;
struct Light {
    Vector3df position;
    Vector3df color;
    float     intensity;
};
}  // namespace rt