#pragma once

#include <cmath>
#include <iostream>

#include "interval.h"
#include "vect3.h"

struct Color : public Vect3 {
  constexpr Color(): Vect3() {};
  constexpr Color(double d0, double d1, double d2) : Vect3(d0, d1, d2) {};
  constexpr Color(const Vect3& v) : Vect3(v) {};

  inline static double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
      return std::sqrt(linear_component);
    }

    return 0;
  }

  void write_color(std::ostream& out) const {
    double r = Color::linear_to_gamma(this->x());
    double g = Color::linear_to_gamma(this->y());
    double b = Color::linear_to_gamma(this->z());

    static const Interval intensity(0.000, 0.999);
    int rbyte = (int)(256 * intensity.clamp(r));
    int gbyte = (int)(256 * intensity.clamp(g));
    int bbyte = (int)(256 * intensity.clamp(b));

    out << rbyte << " " << gbyte << " " << bbyte << "\n";
  };
};
