#pragma once

#include "hittable.h"
#include <cmath>
#include <memory>
#include <optional>

class Sphere: public Hittable {
private:
  Ray center;
  double radius;
  std::shared_ptr<Material> material;
  BoundingBox bbox;

public:
  // Stationary
  Sphere(const Point3& center, double radius, std::shared_ptr<Material> material) : 
    center(center, { 0, 0, 0 }), radius(std::fmax(0, radius)), material(material) {
    const Vect3 radius_vector { radius, radius, radius };
    this->bbox = BoundingBox(center - radius_vector, center + radius_vector);

  };

  // Moving
  Sphere(const Point3& center1, const Point3& center2, double radius, std::shared_ptr<Material> material) : 
    center(center1, center2 - center1), radius(std::fmax(0, radius)), material(material) {
    const Vect3 radius_vector { radius, radius, radius };
    BoundingBox box1(center.at(0) - radius_vector, center.at(0) + radius_vector);
    BoundingBox box2(center.at(1) - radius_vector, center.at(1) + radius_vector);
    this->bbox = BoundingBox(box1, box2);

  };

  std::optional<HitRecord> hit(const Ray& ray, const Interval ray_t) const override {
    const Point3 current_center = center.at(ray.time());
    const Vect3 oc = current_center - ray.origin();
    const double a = ray.direction().length_squared();
    // double b = -2 * dot(ray.direction(), oc);
    const double h = dot(ray.direction(), oc); // h = -2b
    const double c = oc.length_squared() - radius * radius;
    const double discriminant = h * h - a * c;
    
    if (discriminant < 0) {
      return std::nullopt;
    }
    const double discriminant_sqrt = std::sqrt(discriminant);

    // Find the nearest root that lies in acceptable range
    double root = (h - discriminant_sqrt) / a;
    if (root <= ray_t.min || ray_t.max <= root) {
      root = (h + discriminant_sqrt) / a;
      if (root <= ray_t.min || ray_t.max <= root) {
        return std::nullopt;
      }
    }

    const double t = root;
    const Point3 p = ray.at(t);
    const Vect3 outward_normal = (p - current_center) / this->radius;
    return std::make_optional<HitRecord>(t, p, ray, outward_normal, this->material);
  }

  BoundingBox bounding_box() const override {
    return bbox;
  };
};
