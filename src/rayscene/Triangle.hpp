#pragma once
#include "SceneObject.hpp"
#include "../raymath/Vector3.hpp"
#include "../raymath/Color.hpp"
#include "../raymath/Ray.hpp"
#include "../raymath/Transform.hpp"

class Triangle : public SceneObject
{
private:
  Vector3 A;
  Vector3 B;
  Vector3 C;

  Vector3 tA;
  Vector3 tB;
  Vector3 tC;

public:
  Triangle(Vector3 a, Vector3 b, Vector3 c);
  ~Triangle();

  int ID;

  virtual void applyTransform() override;
  virtual void calculateBoundingBox() override;
  virtual bool intersects(Ray &r, Intersection &intersection, CullingType culling) override;
};
