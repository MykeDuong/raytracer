#pragma once

#include "constant.h"

class Interval {
public:
  const double min, max;

  constexpr Interval() : min(+Constant::infinity), max(-Constant::infinity) {} // Empty by default
  constexpr Interval(double min, double max) : min(min), max(max) {} // Empty by default
  
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
