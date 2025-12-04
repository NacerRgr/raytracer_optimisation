#pragma once

#include <iostream>
#include "Vector3.hpp"

class Ray
{
private:
  Vector3 position;
  Vector3 direction;

public:
  Ray();
  Ray(Vector3 pos, Vector3 dir);
  ~Ray();

  Vector3 GetPosition() const;
  void SetPosition(Vector3 &pos);

  Vector3 GetDirection() const;
  void SetDirection(Vector3 &pos);

  friend std::ostream &operator<<(std::ostream &_stream, Ray &vec);
};
