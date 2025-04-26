#pragma once

#include "vect3.h"
#include "point3.h"

class Ray {
public:
  Ray() {};
  Ray(const Point3& origin, const Vect3& direction): orig(origin), dir(direction) {};

  const Point3& origin() const { return orig;};
  const Vect3& direction() const { return dir; };

  Point3 at(double t) const {
    return orig + t * dir;
  }
  
private:
  Point3 orig;
  Vect3 dir;
};
