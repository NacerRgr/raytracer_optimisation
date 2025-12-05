#pragma once

#include "BSPNode.hpp"
#include "SceneObject.hpp"
#include "../raymath/Ray.hpp"
#include <vector>

/**
 * BSPTree - Binary Space Partition Tree for spatial optimization
 *
 * Organizes scene objects in a tree structure for fast ray intersection queries.
 * Use this to reduce O(n) linear search to O(log n) tree traversal.
 */
class BSPTree
{
private:
    BSPNode *root;
    bool built;

public:
    BSPTree();
    ~BSPTree();

    /**
     * Build the BSP tree from a list of objects
     * Call this once after all objects are added and positioned
     * @param objects All scene objects
     */
    void build(std::vector<SceneObject *> &objects);

    /**
     * Find objects that ray might intersect
     * @param r Ray to test
     * @param results Output vector of candidate objects
     * @return true if any objects found
     */
    bool findIntersections(Ray &r, std::vector<SceneObject *> &results);

    /**
     * Check if tree has been built
     */
    bool isBuilt() const { return built; }
};