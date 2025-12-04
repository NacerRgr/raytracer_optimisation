#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"

class Sphere : public SceneObject
{
private:
  Vector3 center;
  double radius;

public:
  Sphere(double r);
  ~Sphere();

  virtual void applyTransform() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
  void countPrimes();
  virtual void calculateBoundingBox() override;
};
