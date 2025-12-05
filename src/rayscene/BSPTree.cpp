#include "BSPTree.hpp"
#include <iostream>

BSPTree::BSPTree() : root(nullptr), built(false)
{
}

BSPTree::~BSPTree()
{
    if (root != nullptr)
    {
        delete root;
    }
}

void BSPTree::build(std::vector<SceneObject *> &objects)
{
    if (objects.empty())
    {
        std::cout << "Warning: Building BSP tree with no objects" << std::endl;
        built = false;
        return;
    }

    std::cout << "Building BSP tree with " << objects.size() << " objects..." << std::endl;

    // Delete old tree if it exists
    if (root != nullptr)
    {
        delete root;
    }

    // Build new tree
    root = new BSPNode(objects);
    built = true;

    std::cout << "BSP tree built successfully" << std::endl;
}

bool BSPTree::findIntersections(Ray &r, std::vector<SceneObject *> &results)
{
    if (!built || root == nullptr)
    {
        // Tree not built - return empty results
        return false;
    }

    // Clear results vector
    results.clear();

    // Traverse tree to find candidate objects
    root->findIntersections(r, results);

    return !results.empty();
}