#pragma once

#include "math.h"

namespace rt::ray {
class Ray {
    Ray(const Vector3df& origin, const Vector3df& direction);
    ~Ray() = default;

    inline const Vector3df& origin() const {
        return _orig;
    }
    inline const Vector3df& direction() const {
        return _dir;
    }

    Vector3df at(double t) const;

  private:
    Vector3df _orig;
    Vector3df _dir;
};
}  // namespace rt::ray