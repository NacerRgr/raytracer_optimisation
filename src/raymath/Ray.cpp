#include <iostream>
#include "Ray.hpp"
#include "Vector3.hpp"

Ray::Ray() : position(Vector3()), direction(Vector3(0, 0, 1))
{
}

Ray::Ray(Vector3 pos, Vector3 dir) : position(pos)
{
  direction = dir.normalize();
}

Ray::~Ray()
{
}

Vector3 Ray::GetPosition() const
{
  return position;
}

void Ray::SetPosition(Vector3 &pos)
{
  position = pos;
}

Vector3 Ray::GetDirection() const
{
  return direction;
}

void Ray::SetDirection(Vector3 &dir)
{
  direction = dir.normalize();
}

std::ostream &operator<<(std::ostream &_stream, Ray &ray)
{
  return _stream << "Ray(" << ray.GetPosition() << ", " << ray.GetDirection() << ")";
}
