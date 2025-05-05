#pragma once

#include "hittable_list.h"
#include "util.h"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <optional>

// Bounding Volume Hierarchy Node
class BVHNode final : public Hittable {
public:
  // constructor: copy construct HittableList
  BVHNode(HittableList list) : BVHNode(list.objects, 0, list.objects.size(), 0) {}

  // Start inclusive, end exclusive
  BVHNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, int depth) {
    int axis = Utility::random_int(0, 2); // Choose a random axis

    Comparator comparator(axis);

    size_t object_span = end - start;
    assert(object_span > 0 && "[ERROR] Object Span out of range");

    if (object_span == 1) {
      this->left = this->right = objects[start];
    } else if (object_span == 2) {
      this->left = objects[start];
      this->right = objects[end - 1];
    } else {
      std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);
      int mid = start + object_span / 2;
      this->left = std::make_shared<BVHNode>(objects, start, mid, depth + 1);
      this->right = std::make_shared<BVHNode>(objects, mid, end, depth + 1);
    }

    this->bbox = BoundingBox(left->bounding_box(), right->bounding_box());
  }

  std::optional<HitRecord> hit(const Ray& ray, const Interval ray_t) const override {
    std::optional<Interval> hit_bbox = this->bbox.hit(ray, ray_t);
    if (!hit_bbox.has_value()) { return std::nullopt; }

    std::optional<HitRecord> hit_left = this->left->hit(ray, ray_t);

    // If it already hit left child, the upper bound of t will be that of left hit result
    // If ray hits right child, that means it hits right child before left child
    std::optional<HitRecord> hit_right = this->right->hit(
      ray, 
      Interval(ray_t.min, hit_left.has_value() ? hit_left->t : ray_t.max)
    );

    if (!hit_left.has_value() && !hit_right.has_value()) return std::nullopt;
    if (hit_right.has_value()) return hit_right;

    return hit_left;
  };

  BoundingBox bounding_box() const override {
    return this->bbox;
  };

private:
  std::shared_ptr<Hittable> left;
  std::shared_ptr<Hittable> right;
  BoundingBox bbox;

  struct Comparator {
    const int axis_index;
    Comparator(int axis_index) : axis_index(axis_index) {
      assert(0 <= axis_index && axis_index <= 2 && "[ERROR] Axis index out of range\n");
    }
    bool operator() (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
      const Interval a_axis_interval = a->bounding_box().axis_interval(this->axis_index);
      const Interval b_axis_interval = b->bounding_box().axis_interval(this->axis_index);
      return a_axis_interval.min < b_axis_interval.min;
    }
  };
};
