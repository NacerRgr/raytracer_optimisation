#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
}

Sphere::~Sphere()
{
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
}

void Sphere::countPrimes()
{
  int prime_counter = 0;
  for (int n = 2; n < 1000; n++)
  {
    int count = 0;
    for (int i = 2; i <= i / 2; i++)
    {
      if (n % i == 0)
      {
        count++;
      }
      if (count == 0)
      {
        prime_counter++;
      }
    }
  }
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // Vector from ray origin to center of sphere
  Vector3 OC = center - r.GetPosition();

  // Project OC onto the ray
  Vector3 OP = OC.projectOn(r.GetDirection());

  // If the OP vector is pointing in the opposite direction of the ray
  // ... then it is behind the ray origin, ignore the object
  if (OP.dot(r.GetDirection()) <= 0)
  {
    return false;
  }

  // P is the corner of the right-angle triangle formed by O-C-P
  Vector3 P = r.GetPosition() + OP;

  // Is the length of CP greater than the radius of the circle ? If yes, no intersection!
  Vector3 CP = P - center;
  // Optimization
  // double distance = CP.length();
  double distanceSquared = CP.lengthSquared();

  if (distanceSquared > radius * radius)
  {
    return false;
  }
  double distance = sqrt(distanceSquared);

  // Calculate the exact point of collision: P1
  double a = sqrt(radius * radius - distance * distance);
  double t = OP.length() - a;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  // Pre-calculate some useful values for rendering
  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();

  return true;
}

void Sphere::calculateBoundingBox()
{
  // Sphere bounding box: center ± radius in all directions
  Vector3 minPt = center - Vector3(radius, radius, radius);
  Vector3 maxPt = center + Vector3(radius, radius, radius);

  boundingBox = AABB(minPt, maxPt);
}
