#include "viewport.h"

namespace rt::view {

Viewport::Viewport(float width, float height, float focalLength, int pixelWidth, int pixelHeight)
    : _focalLength(focalLength), _pixelWidth(pixelWidth), _pixelHeight(pixelHeight) {

    // Set up the viewport vectors (horizontal and vertical)
    _u = Vector3df{width, 0.0f, 0.0f};
    _v = Vector3df{0.0f, height, 0.0f};

    // Calculate pixel delta vectors
    _pixelDelta_u = _u / static_cast<float>(_pixelWidth);
    _pixelDelta_v = _v / static_cast<float>(_pixelHeight);

    // Calculate the viewport upper left corner
    // Assuming the viewport is centered at the origin and the camera looks along -Z
    Vector3df viewportCenter = Vector3df{0.0f, 0.0f, -_focalLength};
    Vector3df halfWidth      = _u / 2.0f;
    Vector3df halfHeight     = _v / 2.0f;

    _upperLeft = viewportCenter - halfWidth + halfHeight;
}

Ray3df Viewport::generateRay(const Vector3df& cameraPosition, const Vector3df& cameraDirection,
                             int pixelX, int pixelY) {}

}  // namespace rt::view