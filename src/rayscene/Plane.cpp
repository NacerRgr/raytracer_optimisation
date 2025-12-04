#include <iostream>
#include <cmath>
#include <limits>
#include "Plane.hpp"
#include "../raymath/Vector3.hpp"

Plane::Plane(Vector3 p, Vector3 n) : point(p), normal(n)
{
}

Plane::~Plane()
{
}

void Plane::calculateBoundingBox()
{
  double max_val = std::numeric_limits<double>::max();
  double min_val = std::numeric_limits<double>::lowest();
  boundingBox = AABB(Vector3(min_val, min_val, min_val),
                     Vector3(max_val, max_val, max_val));
}

bool Plane::intersects(Ray &r, Intersection &intersection, CullingType culling)
{

  float denom = r.GetDirection().dot(normal);

  // If denom == 0 - it is parallel to the plane
  // If denom > 0, it means plane is behind the ray
  if (denom > -0.000001)
  {
    return false;
  }

  float numer = (point - r.GetPosition()).dot(normal);
  float t = numer / denom;

  intersection.Position = r.GetPosition() + (r.GetDirection() * t);
  intersection.Normal = normal;
  intersection.Mat = this->material;

  return true;
}