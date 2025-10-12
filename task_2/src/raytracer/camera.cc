#include "camera.h"

namespace rt::camera {
Camera::Camera(Vector3df position, Vector3df direction, rt::view::Viewport& viewport)
    : _position(position), _direction(direction), _viewport(viewport) {
    // Normalize direction vector if needed
    _direction.normalize();
}

Ray3df Camera::getRay(int x, int y) {
    // Generate ray from camera position through the pixel on the viewport
    return _viewport.generateRay(_position, _direction, x, y);
}
};  // namespace rt::camera