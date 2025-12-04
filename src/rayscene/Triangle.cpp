#include <iostream>
#include <cmath>
#include <algorithm>
#include "Triangle.hpp"
#include "../raymath/Vector3.hpp"

Triangle::Triangle(Vector3 a, Vector3 b, Vector3 c) : SceneObject(), A(a), B(b), C(c)
{
}

Triangle::~Triangle()
{
}

void Triangle::applyTransform()
{
  tA = this->transform.apply(A);
  tB = this->transform.apply(B);
  tC = this->transform.apply(C);
}

void Triangle::calculateBoundingBox()
{
  double minX = std::min({tA.x, tB.x, tC.x});
  double minY = std::min({tA.y, tB.y, tC.y});
  double minZ = std::min({tA.z, tB.z, tC.z});

  double maxX = std::max({tA.x, tB.x, tC.x});
  double maxY = std::max({tA.y, tB.y, tC.y});
  double maxZ = std::max({tA.z, tB.z, tC.z});

  boundingBox = AABB(Vector3(minX, minY, minZ), Vector3(maxX, maxY, maxZ));
}

bool Triangle::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  Vector3 BA = tB - tA;
  Vector3 CA = tC - tA;
  Vector3 normal = BA.cross(CA).normalize();

  // Ray plane intersection
  float denom = r.GetDirection().dot(normal);

  //
  // If denom == 0 - it is parallel to the plane
  // If denom > 0, it means plane is behind the ray
  if (culling == CULLING_FRONT && denom > -0.000001)
  {
    return false;
  }
  if (culling == CULLING_BACK && denom < 0.000001)
  {
    return false;
  }

  float numer = (tA - r.GetPosition()).dot(normal);
  float t = numer / denom;

  // Behind the ray
  if (t <= 0)
  {
    return false;
  }

  // Point on plane
  Vector3 Q = r.GetPosition() + (r.GetDirection() * t);

  // Point contained in triangle
  Vector3 QA = Q - tA;
  Vector3 BAxQA = BA.cross(QA);
  if (BAxQA.dot(normal) < 0)
  {
    return false;
  }

  Vector3 CB = tC - tB;
  Vector3 QB = Q - tB;
  Vector3 CBxQB = CB.cross(QB);
  if (CBxQB.dot(normal) < 0)
  {
    return false;
  }

  Vector3 AC = tA - tC;
  Vector3 QC = Q - tC;
  Vector3 ACxQC = AC.cross(QC);
  if (ACxQC.dot(normal) < 0)
  {
    return false;
  }

  intersection.Position = Q;
  intersection.Mat = this->material;
  intersection.Normal = normal;

  return true;
}