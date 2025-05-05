#pragma once

#include "hittable.h"

#include <optional>
#include <memory>
#include <vector>
#include <optional>

class HittableList final : public Hittable {
public:
  std::vector<std::shared_ptr<Hittable>> objects;

  HittableList() {};
  HittableList(std::shared_ptr<Hittable> object) { this->add(object); };

  void clear() { objects.clear(); };

  void add(std::shared_ptr<Hittable> object) {
    objects.push_back(object);
    this->bbox = BoundingBox(this->bbox, object->bounding_box());
  }

  std::optional<HitRecord> hit(const Ray& ray, const Interval ray_t) const override {
    std::optional<HitRecord> result = std::nullopt;
    double closest_so_far = ray_t.max;

    for (const std::shared_ptr<Hittable>& object: objects) {
      std::optional<HitRecord> hitRecord = object->hit(ray, { ray_t.min, closest_so_far });
      if (hitRecord.has_value()) {
        closest_so_far = hitRecord->t;
        result = std::move(hitRecord);
      }
    }
    return result;
  }

  BoundingBox bounding_box() const override { return this->bbox; }
private:
  BoundingBox bbox;
};
