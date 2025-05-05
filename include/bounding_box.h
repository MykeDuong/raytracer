#pragma once

#include "interval.h"
#include "point3.h"
#include "ray.h"
#include <algorithm>
#include <optional>
#include <cassert>

// This implements an Axis-aligned Bounding Box (AABB)
class BoundingBox {
public:
  Interval x, y, z;

  BoundingBox() {}; // Empty by default, as default interval is empty
  
  BoundingBox(const Interval& x, const Interval& y, const Interval& z) : x(x), y(y), z(z) {}

  BoundingBox(const Point3& a, const Point3& b) : 
    x(std::min(a[0], b[0]), std::max(a[0], b[0])),
    y(std::min(a[1], b[1]), std::max(a[1], b[1])),
    z(std::min(a[2], b[2]), std::max(a[2], b[2])) {}

  BoundingBox(const BoundingBox& a, const BoundingBox& b) :
    x(Interval(a.x, b.x)),
    y(Interval(a.y, b.y)),
    z(Interval(a.z, b.z)) {}

  const Interval& axis_interval(int n) const {
    assert(0 <= n && n <= 2 && "[ERROR] Value of axis interval out of range");
    if (n == 1)
      return y;
    if (n == 2)
      return z;
    return x;
  };

  std::optional<Interval> hit(const Ray& ray, const Interval& ray_t) const {
    const Point3& ray_origin    = ray.origin();
    const Vect3&  ray_direction = ray.direction();

    Interval result = ray_t;

    for (int axis = 0; axis < 3; axis++) {
      // x = q + td
      // t = (x - q) / d
      const Interval& ax = this->axis_interval(axis);
      const double adinv = 1.0 / ray_direction[axis];

      const double t0 = (ax.min - ray_origin[axis]) * adinv;
      const double t1 = (ax.max - ray_origin[axis]) * adinv;

      const double lower_t = std::min(t0, t1);
      const double higher_t = std::max(t0, t1);

      if (lower_t > result.min) result.min = lower_t;
      if (higher_t < result.max) result.max = higher_t;

      if (result.min >= result.max) return std::nullopt;
    }
    return result;
  }
};

inline std::ostream &operator<<(std::ostream &out, const BoundingBox& bbox) {
  return out << "Bounding Box " << bbox.x << ", " << bbox.y << ", " << bbox.z;
}

