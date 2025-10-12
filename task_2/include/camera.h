#pragma once

#include "math.h"
#include "window.h"
#include "geometry.h"
#include "viewport.h"

#include <memory>

namespace rt::camera {

class Camera {
  public:
    Camera(Vector3df position, Vector3df direction, rt::view::Viewport& viewport);

    Ray3df getRay(int x, int y);

  private:
    Vector3df           _position;
    Vector3df           _direction;
    rt::view::Viewport& _viewport;
};

}  // namespace rt::camera