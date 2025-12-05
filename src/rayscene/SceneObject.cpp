#include <iostream>
#include <cmath>
#include "SceneObject.hpp"
#include "Intersection.hpp"

SceneObject::SceneObject() : material(NULL)
{
}

SceneObject::~SceneObject()
{
}

bool SceneObject::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  return false;
}

void SceneObject::applyTransform()
{
}

void SceneObject::calculateBoundingBox()
{
  // default implementation that This will be overridden by specific object types
  boundingBox = AABB(
      Vector3(-INFINITY, -INFINITY, -INFINITY),
      Vector3(INFINITY, INFINITY, INFINITY));
}
