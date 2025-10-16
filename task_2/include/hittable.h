#pragma once
#include "geometry.h"
#include "shader.h"
namespace rt {

struct HitInfo {
    bool            hit;
    float           tHit;
    Vector3df       normal;
    Vector3df       hitPoint;
    const Material* material;
};

class Hittable {
  public:
    virtual HitInfo         intersect(const Ray3df& ray) const = 0;
    virtual const Material& material() const                   = 0;
    virtual ~Hittable()                                        = default;
};
}  // namespace rt