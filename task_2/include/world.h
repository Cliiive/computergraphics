#pragma once

#include <concepts>
#include <memory>
#include <variant>
#include <optional>

#include "geometry.h"

namespace rt::world {

struct Material {
    // Basic material properties
    Vector3df ambient{0.1f, 0.1f, 0.1f};
    Vector3df diffuse{0.7f, 0.7f, 0.7f};
    Vector3df specular{1.0f, 1.0f, 1.0f};
    float     shininess    = 32.0f;
    float     reflectivity = 0.0f;

    // Return the base color for this materia
    Vector3df getColor() const noexcept {
        return diffuse;
    }
};

template <typename T>
concept Intersectable =
    requires(const T& t, const Ray3df& ray, Intersection_Context<float, 3> context) {
        { t.intersects(ray, context) } -> std::same_as<bool>;
    };

template <Intersectable T> class GeometricObject {
  private:
    T        geoObject;
    Material mat;

  public:
    GeometricObject(const Vector3df& center, float radius, const Material& material)
        : geoObject(center, radius), mat(material) {};
    GeometricObject(const Vector3df& a, const Vector3df& b, const Vector3df& c,
                    const Material& material)
        : geoObject(a, b, c), mat(material) {};

    bool intersect(const Ray3df& ray, float& tHit, Vector3df& normal) const {
        Intersection_Context<float, 3> context;
        bool                           hit = geoObject.intersects(ray, context);
        if (hit) {
            tHit   = context.t;
            normal = context.normal;
        }
        return hit;
    }

    const Material& material() const {
        return mat;
    }
};

// Conveniece typedefs
typedef GeometricObject<Sphere3df>   SphereObject;
typedef GeometricObject<Triangle3df> TriangleObject;

// Type erasure concept
template <typename T>
concept HittableObject = requires(const T& t, const Ray3df& r, float& tHit, Vector3df& n) {
    { t.material() } -> std::same_as<const Material&>;
    { t.intersect(r, tHit, n) } -> std::same_as<bool>;
};

// Type-erased wrapper class
class Hittable {
  private:
    struct ConceptBase {
        virtual const Material& material() const                                        = 0;
        virtual bool intersect(const Ray3df& ray, float& tHit, Vector3df& normal) const = 0;
        virtual ~ConceptBase()                                                          = default;
    };

    template <HittableObject T> struct Model : ConceptBase {
        T object;
        Model(T obj) : object(std::move(obj)) {}

        const Material& material() const override {
            return object.material();
        }

        bool intersect(const Ray3df& ray, float& tHit, Vector3df& normal) const override {
            return object.intersect(ray, tHit, normal);
        }
    };

    std::unique_ptr<ConceptBase> ptr;

  public:
    template <HittableObject T> Hittable(T obj) : ptr(std::make_unique<Model<T>>(std::move(obj))) {}

    const Material& material() const {
        return ptr->material();
    }

    bool intersect(const Ray3df& ray, float& tHit, Vector3df& normal) const {
        return ptr->intersect(ray, tHit, normal);
    }
};

// Helper function to create a world with various objects
inline std::vector<Hittable> createScene() {
    std::vector<Hittable> objects;

    // Materials
    Material redMaterial;
    redMaterial.diffuse = Vector3df{0.9f, 0.1f, 0.1f};

    Material greenMaterial;
    greenMaterial.diffuse = Vector3df{0.1f, 0.9f, 0.1f};

    Material whiteMaterial;
    whiteMaterial.diffuse = Vector3df{0.9f, 0.9f, 0.9f};

    Material wallMaterial;
    wallMaterial.diffuse = Vector3df{0.8f, 0.8f, 0.8f};

    // Box coordinates (open front at z = 0; box extends into negative z)
    const float xMin = -1.0f, xMax = 1.0f;
    const float yMin = -1.0f, yMax = 1.0f;
    const float zNear = -10.0f;  // front (open)
    const float zFar  = -30.0f;  // back

    // Left wall
    Vector3df leftA{xMin, yMin, zNear}, leftB{xMin, yMax, zNear}, leftC{xMin, yMin, zFar},
        leftD{xMin, yMax, zFar};
    objects.emplace_back(TriangleObject(leftA, leftB, leftC, greenMaterial));
    objects.emplace_back(TriangleObject(leftC, leftB, leftD, greenMaterial));
    //objects.emplace_back(TriangleObject(leftC, leftD, leftA, greenMaterial));

    // Right wall
    Vector3df rightA{xMax, yMin, zNear}, rightB{xMax, yMax, zNear}, rightC{xMax, yMax, zFar},
        rightD{xMax, yMin, zFar};
    objects.emplace_back(TriangleObject(rightA, rightB, rightD, redMaterial));
    objects.emplace_back(TriangleObject(rightB, rightC, rightD, redMaterial));

    // Top wall
    Vector3df topA{xMin, yMax, zNear}, topB{xMax, yMax, zNear}, topC{xMin, yMax, zFar},
        topD{xMax, yMax, zFar};
    objects.emplace_back(TriangleObject(topA, topB, topC, wallMaterial));
    objects.emplace_back(TriangleObject(topC, topB, topD, wallMaterial));

    // Bottom wall
    Vector3df bottomA{xMin, yMin, zNear}, bottomB{xMax, yMin, zNear}, bottomC{xMin, yMin, zFar},
        bottomD{xMax, yMin, zFar};
    objects.emplace_back(TriangleObject(bottomA, bottomB, bottomC, wallMaterial));
    objects.emplace_back(TriangleObject(bottomC, bottomB, bottomD, wallMaterial));

    // Back wall
    Vector3df backA{xMin, yMax, zFar}, backB{xMax, yMax, zFar}, backC{xMin, yMin, zFar},
        backD{xMax, yMin, zFar};
    objects.emplace_back(TriangleObject(backA, backB, backC, wallMaterial));
    objects.emplace_back(TriangleObject(backC, backB, backD, wallMaterial));

    objects.emplace_back(SphereObject(Vector3df{-0.1f, -0.5f, -8.0f}, 0.3f, whiteMaterial));

    return objects;
};

inline std::optional<std::reference_wrapper<const Hittable>>
findVisibleObject(const Ray3df& ray, const std::vector<Hittable>& scene) {
    std::optional<std::reference_wrapper<const Hittable>> visibleObject = std::nullopt;
    float minT = std::numeric_limits<float>::infinity();

    for (const auto& object : scene) {
        float     t;
        Vector3df normal;
        if (object.intersect(ray, t, normal) && t > 0 && t < minT) {
            visibleObject = std::cref(object);
            minT          = t;
        }
    }

    return visibleObject;
}

}  // namespace rt::world