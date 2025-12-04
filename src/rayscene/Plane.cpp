#include <iostream>
#include <cmath>
#include "Plane.hpp"
#include "../raymath/Vector3.hpp"

Plane::Plane(Vector3 p, Vector3 n) : point(p), normal(n)
{
}

Plane::~Plane()
{
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

void Plane::calculateBoundingBox()
{
  // Plane is infinite, use infinite bounding box
  boundingBox = AABB(
      Vector3(-INFINITY, -INFINITY, -INFINITY),
      Vector3(INFINITY, INFINITY, INFINITY));
}