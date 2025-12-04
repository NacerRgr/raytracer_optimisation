#include <iostream>
#include "Mesh.hpp"
#include "../raymath/Vector3.hpp"
#include "../objloader/OBJ_Loader.h"

Mesh::Mesh() : SceneObject()
{
}

Mesh::~Mesh()
{
    for (int i = 0; i < triangles.size(); ++i)
    {
        delete triangles[i];
    }
}

void Mesh::loadFromObj(std::string path)
{

    objl::Loader *loader = new objl::Loader();
    bool loadout = loader->LoadFile(path);

    if (loadout)
    {
        for (int i = 0; i < loader->LoadedMeshes.size(); i++)
        {
            objl::Mesh curMesh = loader->LoadedMeshes[i];

            for (int j = 0; j < curMesh.Indices.size(); j += 3)
            {
                Vector3 v1(
                    curMesh.Vertices[curMesh.Indices[j]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                Vector3 v2(
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
                Vector3 v3(
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
                    curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);

                Triangle *triangle = new Triangle(
                    v1,
                    v2,
                    v3);
                triangle->name = "T:" + std::to_string(j);
                triangle->ID = j;
                triangles.push_back(triangle);
            }
        }
    }

    this->applyTransform();
    delete loader;
}

void Mesh::applyTransform()
{
    for (int i = 0; i < triangles.size(); ++i)
    {
        triangles[i]->material = this->material;
        triangles[i]->transform = transform;
        triangles[i]->applyTransform();
    }
}

bool Mesh::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
    Intersection tInter;

    /// double closestDistance = -1;
    double closestDistanceSquared = -1;
    Intersection closestInter;
    // optmization: precompute size
    const size_t triangleCount = triangles.size();

    for (size_t i = 0; i < triangleCount; ++i)
    {
        if (triangles[i]->intersects(r, tInter, culling))
        {
            // Optimisation : lenghth au lieu de lengthSquared
            double distanceSquared = (tInter.Position - r.GetPosition()).lengthSquared();
            // tInter.Distance = (tInter.Position - r.GetPosition()).length();
            if (closestDistanceSquared < 0 || distanceSquared < closestDistanceSquared)
            {
                closestDistanceSquared = distanceSquared;
                intersection.Distance = sqrt(distanceSquared);
                closestInter = tInter;

                // OPTIMISATION : Early exit
                if (closestDistanceSquared < 0.0001)
                {
                    break;
                }
            }
        }
    }

    if (closestDistanceSquared < 0)
    {
        return false;
    }

    intersection = closestInter;
    return true;
}