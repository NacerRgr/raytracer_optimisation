#include <iostream>
#include <cmath>
#include "Transform.hpp"

const double DEG_TO_RAD = M_PI / 180.0;

Transform::Transform()
{
  this->setMatrix();
}

Transform::~Transform()
{
}

Matrix getYaw(double degrees)
{
  double rad = degrees * DEG_TO_RAD;
  double posMat[4][4] = {
      {1, 0, 0, 0},
      {0, std::cos(rad), -std::sin(rad), 0},
      {0, std::sin(rad), std::cos(rad), 0},
      {0, 0, 0, 1}};
  Matrix m(&posMat);
  return m;
}
Matrix getPitch(double degrees)
{
  double rad = degrees * DEG_TO_RAD;
  double posMat[4][4] = {
      {std::cos(rad), 0, std::sin(rad), 0},
      {0, 1, 0, 0},
      {-std::sin(rad), 0, std::cos(rad), 0},
      {0, 0, 0, 1}};
  Matrix m(&posMat);
  return m;
}
Matrix getRoll(double degrees)
{
  double rad = degrees * DEG_TO_RAD;
  double posMat[4][4] = {
      {std::cos(rad), -std::sin(rad), 0, 0},
      {std::sin(rad), std::cos(rad), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};
  Matrix m(&posMat);
  return m;
}

void Transform::setMatrix()
{

  // Position
  double posMat[4][4] = {
      {1, 0, 0, position.x},
      {0, 1, 0, position.y},
      {0, 0, 1, position.z},
      {0, 0, 0, 1}};
  Matrix mpos(&posMat);

  // Rotation
  Matrix mrot = getRoll(rotation.z) * (getPitch(rotation.y) * getYaw(rotation.x));

  this->matrix = mpos * mrot;
}

Vector3 Transform::getPosition() const
{
  return this->position;
}

void Transform::setPosition(Vector3 const &pos)
{
  this->position = pos;
}

void Transform::setRotation(Vector3 const &rot)
{
  this->rotation = rot;
}

Vector3 Transform::apply(Vector3 const &pos)
{
  this->setMatrix();
  return this->matrix * pos;
}