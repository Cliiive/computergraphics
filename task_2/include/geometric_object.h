#pragma once
#include "hittable.h"

namespace rt::scene {

template <typename GeometryT> class GeometricObject : public Hittable {
  private:
    GeometryT geo;
    Material  mat;

  public:
    template <typename... Args>
    GeometricObject(const Material& material, Args&&... args)
        : geo(std::forward<Args>(args)...), mat(material) {}

    HitInfo intersect(const Ray3df& ray) const override {
        Intersection_Context<float, 3> ctx{};
        bool                           hit = geo.intersects(ray, ctx);
        return {hit, ctx.t, ctx.normal};
    }

    const Material& material() const override {
        return mat;
    }
};

using SphereObject   = GeometricObject<Sphere3df>;
using TriangleObject = GeometricObject<Triangle3df>;

}  // namespace rt::scene
