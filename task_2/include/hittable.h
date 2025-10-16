#pragma once
#include "geometry.h"
#include "shader.h"

namespace rt::world {

using namespace rt::shader;

struct HitInfo {
    bool      hit;
    float     tHit;
    Vector3df normal;
};

class Hittable {
  public:
    virtual HitInfo         intersect(const Ray3df& ray) const = 0;
    virtual const Material& material() const                   = 0;
    virtual ~Hittable()                                        = default;
};
}  // namespace rt::world