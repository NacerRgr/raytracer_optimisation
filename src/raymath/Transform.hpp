#pragma once

#include <iostream>
#include "Matrix.hpp"

class Transform
{
private:
  Vector3 position;
  Vector3 rotation;
  Matrix matrix;

  void setMatrix();

public:
  Transform();
  ~Transform();

  Vector3 getPosition() const;
  void setPosition(Vector3 const &pos);
  void setRotation(Vector3 const &rot);

  Vector3 apply(Vector3 const &pos);
};
