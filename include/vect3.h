#pragma once

#include "util.h"
#include <cmath>
#include <iostream>

class Vect3 {
public:
  double d[3];
  
  constexpr Vect3(): d{0, 0, 0} {};
  constexpr Vect3(double d0, double d1, double d2) : d{d0, d1, d2} {};

  constexpr double x() const { return d[0]; };
  constexpr double y() const { return d[1]; };
  constexpr double z() const { return d[2]; };

  constexpr Vect3 operator-() const { return { -d[0], -d[1], -d[2] }; };
  constexpr double operator[](int i) const { return d[i]; };
  constexpr double& operator[](int i) { return d[i]; };

  constexpr Vect3& operator+=(const Vect3& v) {
    d[0] += v.d[0];
    d[1] += v.d[1];
    d[2] += v.d[2];
    return *this;
  };

  constexpr Vect3& operator*=(double t) {
    d[0] *= t;
    d[1] *= t;
    d[2] *= t;
    return *this;
  };

  constexpr Vect3& operator/=(double t) {
    return *this *= 1/t;
  };

  double length() const {
    return std::sqrt(this->length_squared());
  };

  constexpr double length_squared() const {
    return d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
  };

  bool near_zero() const {
    double s = 1e-8;
    return (std::fabs(this->d[0]) < s) && (std::fabs(this->d[1]) < s) && (std::fabs(this->d[2]) < s);
  }

  static Vect3 random() {
    return { 
      Utility::random_double(),
      Utility::random_double(),
      Utility::random_double()
    };
  };

  static Vect3 random(double min, double max) {
    return { 
      Utility::random_double(min, max),
      Utility::random_double(min, max),
      Utility::random_double(min, max) 
    };
  };
};

inline std::ostream& operator<<(std::ostream& out, const Vect3& v) {
  return out << v.d[0] << ' ' << v.d[1] << ' ' << v.d[2];
}

constexpr inline Vect3 operator+(const Vect3& u, const Vect3& v) {
  return { u.d[0] + v.d[0], u.d[1] + v.d[1], u.d[2] + v.d[2] };
}

constexpr inline Vect3 operator-(const Vect3& u, const Vect3& v) {
  return { u.d[0] - v.d[0], u.d[1] - v.d[1], u.d[2] - v.d[2] };
}

constexpr inline Vect3 operator*(const Vect3& u, const Vect3& v) {
  return { u.d[0] * v.d[0], u.d[1] * v.d[1], u.d[2] * v.d[2] };
}

constexpr inline Vect3 operator*(const Vect3& u, double t) {
  return { u.d[0] * t, u.d[1] * t, u.d[2] * t };
}

constexpr inline Vect3 operator*(double t, const Vect3& u) {
  return u * t;
}

constexpr inline Vect3 operator/(const Vect3& u, double t) {
  return u * (1 / t);
}

constexpr inline double dot(const Vect3&u, const Vect3& v) {
  return u.d[0] * v.d[0]
       + u.d[1] * v.d[1]
       + u.d[2] * v.d[2];
}

constexpr inline Vect3 cross( const Vect3& u, const Vect3&v) {
  return {
    u.d[1] * v.d[2] - u.d[2] * v.d[1],
    u.d[2] * v.d[0] - u.d[0] * v.d[2],
    u.d[0] * v.d[1] - u.d[1] * v.d[0],
  };
}

constexpr inline Vect3 unit_vector(const Vect3& u) {
  return u / u.length();
}

inline Vect3 refract(const Vect3& uv, const Vect3& normal, const double etai_over_etat) {
  const double cos_theta = std::fmin(dot(-uv, normal), 1.0);
  const Vect3 ray_out_perpendicular = etai_over_etat * (uv + cos_theta * normal);
  const Vect3 ray_out_parallel = -std::sqrt(std::fabs(1.0 - ray_out_perpendicular.length_squared())) * normal;
  return ray_out_perpendicular + ray_out_parallel;
}

constexpr inline Vect3 reflect(const Vect3& u, const Vect3& normal) {
  return u - 2 * dot(u, normal) * normal;
}

inline Vect3 random_unit_vector() {
  while (true) {
    Vect3 p = Vect3::random(-1, 1);
    double lensq = p.length_squared();
    if (1e-160 < lensq) { // && lensq <= 1) {
      return p / std::sqrt(lensq);
    }
    //std::clog << lensq << "Failed. Random unit retry\n";
  }
}

inline Vect3 random_on_hemisphere(const Vect3& normal) {
  Vect3 on_unit_sphere = random_unit_vector();
  if (dot(on_unit_sphere, normal) > 0.0) {
    return on_unit_sphere;
  }
  return -on_unit_sphere;
}

inline Vect3 random_in_unit_disk() {
  while (true) {
    Vect3 p { Utility::random_double(-1, 1), Utility::random_double(-1, 1), 0 };
    if (p.length_squared() < 1) {
      return p;
    }
  }
}
