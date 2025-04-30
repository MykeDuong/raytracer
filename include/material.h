#pragma once

#include <optional>

#include "hittable.h"
#include "color.h"
#include "ray.h"
#include "vect3.h"

struct ScatterRecord {
  const Ray scattered;
  const Color attenuation;
};

class Material {
public:
  virtual ~Material() = default;

  virtual std::optional<ScatterRecord> scatter(const Ray& ray_in, const HitRecord& record) const {
    (void)ray_in;
    (void)record;
    return std::nullopt;
  }
};

class Lambertian: public Material {
public:
  Lambertian(const Color& albedo) :albedo(albedo) {};

  std::optional<ScatterRecord> scatter(const Ray& ray_in, const HitRecord& record) const override {
    (void)ray_in;
    Vect3 scatter_direction = record.normal + random_unit_vector();
    if (scatter_direction.near_zero()) {
      scatter_direction = record.normal;
    }
    return ScatterRecord {Ray(record.p, scatter_direction), this->albedo };
  }

private:
  const Color albedo;
};

class Metal: public Material {
public:
  Metal(const Color& albedo, double fuzz = 0) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

  std::optional<ScatterRecord> scatter(const Ray& ray_in, const HitRecord& record) const override {
    Vect3 reflected = reflect(ray_in.direction(), record.normal);
    reflected = unit_vector(reflected) + (this->fuzz * random_unit_vector());
    Ray scattered { record.p, reflected };
    if (dot(scattered.direction(), record.normal) <= 0) {
      return std::nullopt;
    }
    return ScatterRecord{scattered, this->albedo};
  }

private:
  const Color albedo;
  const double fuzz;
};

class Dielectric : public Material {
public:
  Dielectric(double refraction_index) : refraction_index(refraction_index) {}
  
  std::optional<ScatterRecord> scatter(const Ray& ray_in, const HitRecord& record) const override {
    // Assuming air eta is 1.0
    const double etai_over_etat = record.front_face ? (1.0 / this->refraction_index) : this->refraction_index;

    const Vect3 unit_direction = unit_vector(ray_in.direction());
    const double cos_theta = std::fmin(dot(-unit_direction, record.normal), 1.0);
    const double sin_theta = std::sqrt(1 - cos_theta * cos_theta);
    const bool cannot_refract = etai_over_etat * sin_theta > 1.0;
    const Vect3 direction = (cannot_refract || reflectance(cos_theta, etai_over_etat) > Utility::random_double()) ? 
      reflect(unit_direction, record.normal) : 
      refract(unit_direction, record.normal, etai_over_etat);

    return ScatterRecord { Ray(record.p, direction), Color {1.0, 1.0, 1.0} };
  }

private:
  const double refraction_index;

  static double reflectance(const double cosine, const double refraction_index) {
    // Schlick's approximation for reflectance
    double r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);

  }
};


