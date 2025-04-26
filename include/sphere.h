#pragma once

#include "hittable.h"
#include <cmath>
#include <memory>
#include <optional>

class Sphere: public Hittable {
private:
  Point3 center;
  double radius;
  std::shared_ptr<Material> material;

public:
  Sphere(const Point3& center, double radius, std::shared_ptr<Material> material) : 
    center(center), radius(std::fmax(0, radius)), material(material) {};

  std::optional<HitRecord> hit(const Ray& ray, const Interval ray_t) const override {
    const Vect3 oc = center - ray.origin();
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
    const Vect3 outward_normal = (p - this->center) / this->radius;
    return std::make_optional<HitRecord>(t, p, ray, outward_normal, this->material);
  }
  
};
