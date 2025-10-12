#include "math.h"
#include "geometry.h"

namespace rt::view {
class Viewport {
  public:
    Viewport(float width, float height, float focalLength, int pixelWidth, int pixelHeight);

    Ray3df generateRay(const Vector3df& cameraPosition, const Vector3df& cameraDirection,
                       int pixelX, int pixelY);

  private:
    Vector3df _u, _v;
    Vector3df _upperLeft;
    Vector3df _pixelDelta_u, _pixelDelta_v;
    float     _focalLength;
    int       _pixelWidth, _pixelHeight;
};
}  // namespace rt::view