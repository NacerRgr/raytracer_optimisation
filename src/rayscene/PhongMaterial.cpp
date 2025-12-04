#include <iostream>
#include <cmath>
#include "PhongMaterial.hpp"
#include "Intersection.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Intersection.hpp"

PhongMaterial::PhongMaterial()
{
}

PhongMaterial::~PhongMaterial()
{
}

Color PhongMaterial::getAmbient(Intersection *intersection)
{
  return Ambient;
}

Color PhongMaterial::render(Ray &r, Ray &camera, Intersection *intersection, Scene *scene)
{

  Color color = getAmbient(intersection) * scene->globalAmbient;

  // std::vector<Light *> lights = scene->getLights();
  // optimization : get reference to avoid copy
  const std::vector<Light *> &lights = scene->getLights();
  // optimization: precompute size or cach size
  const size_t lightCount = lights.size();
  for (size_t i = 0; i < lightCount; ++i)
  {
    Light *light = lights[i];

    Vector3 lightDir = (light->GetPosition() - intersection->Position).normalize();

    Vector3 origin = intersection->Position + lightDir;
    Ray lightRay(origin, lightDir);
    Intersection shadowInter;
    if (!scene->closestIntersection(lightRay, shadowInter, CULLING_BACK))
    {

      float dotProdLN = lightDir.dot(intersection->Normal);
      if (dotProdLN > 0)
      {
        color = color + (light->Diffuse * Diffuse * dotProdLN);
      }

      Vector3 R = (lightDir * -1).reflect(intersection->Normal);
      float dotProdRV = R.dot(intersection->View);
      if (dotProdRV > 0)
      {
        color = color + (light->Specular * Specular * pow(dotProdRV, Shininess));
      }
    }
  }

  return color;
}