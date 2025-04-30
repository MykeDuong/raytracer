#pragma once

#include "point3.h"
#include "ray.h"
#include "vect3.h"
#include "interval.h"

#include <memory>
#include <optional>

class Material;

struct HitRecord {
  double t;
  Point3 p;
  Vect3 normal;
  std::shared_ptr<Material> material;
  bool front_face;

  HitRecord() = delete;
  // HitRecord(double t, const Point3& p, const Vect3& normal) : t(t), p(p), normal(normal), front_face(false) {}

  HitRecord(
    double t, const Point3& p, const Ray& ray, 
    const Vect3& outward_normal, 
    std::shared_ptr<Material> material
  ) : t(t), p(p), material(material) {
    this->set_face_normal(ray, outward_normal);
  }

  void set_face_normal(const Ray& ray, const Vect3& outward_normal) {
    // Sets the hit record normal
    // NOTE: Assumption: outward_normal has unit length
    this->front_face = dot(ray.direction(), outward_normal) < 0;
    this->normal = front_face ? outward_normal : -outward_normal;
  }

};

class Hittable {
public:
  virtual ~Hittable() = default;
  virtual std::optional<HitRecord> hit(const Ray& ray, const Interval ray_t) const = 0;
};
