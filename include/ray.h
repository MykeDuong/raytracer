#pragma once

#include "vect3.h"
#include "point3.h"

class Ray {
public:
  Ray() {};

  Ray(const Point3& origin, const Vect3& direction): orig(origin), dir(direction), tm(0) {};
  Ray(const Point3& origin, const Vect3& direction, double time): orig(origin), dir(direction), tm(time) {};

  const Point3& origin() const { return orig;};
  const Vect3& direction() const { return dir; };

  double time() const { return this->tm; };

  Point3 at(double t) const {
    return orig + t * dir;
  }
  
private:
  Point3 orig;
  Vect3 dir;
  double tm;
};
