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

    // Create materials
    Material redMaterial;
    redMaterial.diffuse = Vector3df{0.9f, 0.1f, 0.1f};

    Material blueMaterial;
    blueMaterial.diffuse      = Vector3df{0.1f, 0.1f, 0.9f};
    blueMaterial.reflectivity = 0.3f;

    // Add objects
    objects.emplace_back(SphereObject(Vector3df{0.0f, 0.0f, -5.0f}, 1.0f, redMaterial));
    objects.emplace_back(SphereObject(Vector3df{2.0f, 0.0f, -7.0f}, 1.0f, blueMaterial));
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