#include <iostream>
#include <cmath>
#include "Scene.hpp"
#include "Intersection.hpp"

Scene::Scene()
{
}

Scene::~Scene()
{
  for (int i = 0; i < objects.size(); ++i)
  {
    delete objects[i];
  }

  for (int i = 0; i < lights.size(); ++i)
  {
    delete lights[i];
  }
}

void Scene::add(SceneObject *object)
{
  objects.push_back(object);
}

void Scene::addLight(Light *light)
{
  lights.push_back(light);
}

void Scene::prepare()
{
  // apply transformations
  for (int i = 0; i < objects.size(); ++i)
  {
    objects[i]->applyTransform();
  }
  // calculate bounding boxes after transforms
  for (int i = 0; i < objects.size(); ++i)
  {
    objects[i]->calculateBoundingBox();
  }
}
// optimization : return reference to avoid copy
const std::vector<Light *> &Scene::getLights()
{
  return lights;
}
// std::vector<Light *> Scene::getLights()
// {
//   return lights;
// }

bool Scene::closestIntersection(Ray &r, Intersection &closest, CullingType culling)
{
  Intersection intersection;

  double closestDistanceSquared = -1;
  Intersection closestInter;
  // optmization: precompute size
  const size_t objectCount = objects.size();

  for (size_t i = 0; i < objectCount; ++i)
  {

    // Test bounding box beofre testing object
    if (!objects[i]->boundingBox.intersects(r))
    {
      continue; // Ray misses bounding box, skip this object!
    }

    // if bounding box was hit, test actual object
    if (objects[i]->intersects(r, intersection, culling))
    {

      double distanceSquared = (intersection.Position - r.GetPosition()).lengthSquared();

      if (closestDistanceSquared < 0 || distanceSquared < closestDistanceSquared)
      {
        closestDistanceSquared = distanceSquared;
        intersection.Distance = std::sqrt(distanceSquared);
        closestInter = intersection;

        //  OPTIMISATION : Early exit
        if (closestDistanceSquared < 0.0001)
        {
          break;
        }
      }
    }
  }
  closest = closestInter;
  return (closestDistanceSquared > -1);
}

Color Scene::raycast(Ray &r, Ray &camera, int castCount, int maxCastCount)
{

  Color pixel;

  Intersection intersection;

  if (closestIntersection(r, intersection, CULLING_FRONT))
  {
    // Add the view-ray for convenience (the direction is normalised in the constructor)
    intersection.View = (camera.GetPosition() - intersection.Position).normalize();

    if (intersection.Mat != NULL)
    {
      pixel = pixel + (intersection.Mat)->render(r, camera, &intersection, this);

      // Reflect
      if (castCount < maxCastCount & intersection.Mat->cReflection > 0)
      {
        Vector3 reflectDir = r.GetDirection().reflect(intersection.Normal);
        Vector3 origin = intersection.Position + (reflectDir * COMPARE_ERROR_CONSTANT);
        Ray reflectRay(origin, reflectDir);

        pixel = pixel + raycast(reflectRay, camera, castCount + 1, maxCastCount) * intersection.Mat->cReflection;
      }
    }
  }

  return pixel;
}