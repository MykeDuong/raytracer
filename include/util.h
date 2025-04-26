#pragma once

#include "constant.h"
#include <random>

namespace Utility{
  constexpr inline double degrees_to_radians(double degrees) {
    return degrees * Constant::pi / 180.0;
  }

  inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
  }

  inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
  }
}
