#pragma once

#include <vector>
#include "../raymath/AABB.hpp"
#include "../raymath/Ray.hpp"
#include "SceneObject.hpp"

class BSPNode
{
private:
    AABB boundingBox;                   // Bounding box for this spatial region
    BSPNode *left;                      // Left child (objects on left side of split)
    BSPNode *right;                     // Right child (objects on right side of split)
    std::vector<SceneObject *> objects; // Objects in this node (only for leaf nodes)
    bool leaf;                          // Is this a leaf node?

public:
    BSPNode();
    BSPNode(std::vector<SceneObject *> &objs);
    ~BSPNode();

    void build(std::vector<SceneObject *> &objs, int depth);

    void findIntersections(Ray &r, std::vector<SceneObject *> &results);

    bool isLeaf() const { return leaf; }

    AABB getBoundingBox() const { return boundingBox; }

private:
    void calculateBoundingBox(std::vector<SceneObject *> &objs);

    int chooseSplitAxis(std::vector<SceneObject *> &objs);

    void partition(std::vector<SceneObject *> &objs, int axis,
                   std::vector<SceneObject *> &leftObjs,
                   std::vector<SceneObject *> &rightObjs);
};