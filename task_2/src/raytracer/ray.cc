#include "ray.h"

namespace rt::ray {
Ray::Ray(const Vector3df& origin, const Vector3df& direction) : _orig(origin), _dir(direction) {};

Vector3df Ray::at(double t) const {
    Vector3df newDirVec{};
    newDirVec *= t;
    return _orig + newDirVec;
}
}  // namespace rt::ray