#pragma once

#include <vector>
#include "../raymath/AABB.hpp"
#include "../raymath/Ray.hpp"
#include "SceneObject.hpp"

/**
 * BSPNode - Node in Binary Space Partition tree
 *
 * Each node represents a spatial region with an AABB.
 * Internal nodes have left/right children.
 * Leaf nodes contain actual scene objects.
 */
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

    /**
     * Build this node by recursively subdividing space
     * @param objs Objects to partition
     * @param depth Current tree depth (for limiting recursion)
     */
    void build(std::vector<SceneObject *> &objs, int depth);

    /**
     * Find all objects that ray might intersect by traversing tree
     * @param r Ray to test
     * @param results Output vector of potentially intersecting objects
     */
    void findIntersections(Ray &r, std::vector<SceneObject *> &results);

    /**
     * Check if this is a leaf node
     */
    bool isLeaf() const { return leaf; }

    /**
     * Get bounding box for this node
     */
    AABB getBoundingBox() const { return boundingBox; }

private:
    /**
     * Calculate bounding box that contains all objects
     */
    void calculateBoundingBox(std::vector<SceneObject *> &objs);

    /**
     * Choose best axis (X=0, Y=1, Z=2) to split on
     */
    int chooseSplitAxis(std::vector<SceneObject *> &objs);

    /**
     * Partition objects into left and right based on split plane
     */
    void partition(std::vector<SceneObject *> &objs, int axis,
                   std::vector<SceneObject *> &leftObjs,
                   std::vector<SceneObject *> &rightObjs);
};