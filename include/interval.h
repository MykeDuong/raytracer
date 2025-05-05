#pragma once

#include "constant.h"
#include <algorithm>
#include <ostream>

class Interval {
public:
  double min, max;

  constexpr Interval() : min(+Constant::infinity), max(-Constant::infinity) {} // Empty by default
  constexpr Interval(double min, double max) : min(min), max(max) {} // Empty by default
  
  constexpr Interval(const Interval& a, const Interval& b):
    min(std::min(a.min, b.min)), max(std::max(a.max, b.max)) {};
  
  constexpr Interval expand(double delta) const { 
    const double padding = delta / 2;
    return Interval{min - padding, max + padding};
  };
  
  constexpr double size() const { return max - min; };

  constexpr bool contains(double x) const { return min <= x && x <= max; };
  constexpr bool surrounds(double x) const { return min < x && x < max; };
  constexpr double clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
  };
};

constexpr const Interval universe_interval{-Constant::infinity,Constant::infinity};
constexpr const Interval empty_interval{Constant::infinity,-Constant::infinity};

inline std::ostream &operator<<(std::ostream &out, const Interval& interval) {
  return out << "(" << interval.min << ", " << interval.max << ")";
}
