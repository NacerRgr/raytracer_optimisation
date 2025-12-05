#include "BSPNode.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

// Maximum depth of tree (prevent infinite recursion)
const int MAX_DEPTH = 20;

// Maximum objects in leaf node (stop subdividing when reached)
const int MAX_LEAF_OBJECTS = 4;

BSPNode::BSPNode() : left(nullptr), right(nullptr), leaf(true)
{
}

BSPNode::BSPNode(std::vector<SceneObject *> &objs) : left(nullptr), right(nullptr), leaf(true)
{
    build(objs, 0);
}

BSPNode::~BSPNode()
{
    if (left != nullptr)
    {
        delete left;
    }
    if (right != nullptr)
    {
        delete right;
    }
}

void BSPNode::build(std::vector<SceneObject *> &objs, int depth)
{
    // Calculate bounding box for all objects in this node
    calculateBoundingBox(objs);

    // Base cases: make this a leaf node
    if (objs.size() <= MAX_LEAF_OBJECTS || depth >= MAX_DEPTH)
    {
        leaf = true;
        objects = objs;
        return;
    }

    // This will be an internal node
    leaf = false;

    // Choose axis to split on (0=X, 1=Y, 2=Z)
    int splitAxis = chooseSplitAxis(objs);

    // Partition objects into left and right
    std::vector<SceneObject *> leftObjs;
    std::vector<SceneObject *> rightObjs;
    partition(objs, splitAxis, leftObjs, rightObjs);

    // Handle degenerate case: all objects went to one side
    if (leftObjs.empty() || rightObjs.empty())
    {
        // Can't split further - make this a leaf
        leaf = true;
        objects = objs;
        return;
    }

    // Recursively build left and right children
    left = new BSPNode();
    left->build(leftObjs, depth + 1);

    right = new BSPNode();
    right->build(rightObjs, depth + 1);
}

void BSPNode::calculateBoundingBox(std::vector<SceneObject *> &objs)
{
    if (objs.empty())
    {
        // Empty node - use zero bounding box
        boundingBox = AABB(Vector3(0, 0, 0), Vector3(0, 0, 0));
        return;
    }

    // Start with first object's bounding box
    boundingBox = objs[0]->boundingBox;

    // Expand to include all other objects
    for (size_t i = 1; i < objs.size(); ++i)
    {
        boundingBox.subsume(objs[i]->boundingBox);
    }
}

int BSPNode::chooseSplitAxis(std::vector<SceneObject *> &objs)
{
    // Simple strategy: choose axis where bounding box is largest
    // This tends to create more balanced splits

    // Calculate bounding box extent on each axis
    Vector3 minPt(INFINITY, INFINITY, INFINITY);
    Vector3 maxPt(-INFINITY, -INFINITY, -INFINITY);

    for (auto *obj : objs)
    {
        // Get object's bounding box corners
        // (We need to access AABB min/max - for now use boundingBox as is)
        // Note: This is simplified - ideally AABB would expose min/max
    }

    // For simplicity, just cycle through axes
    // More sophisticated: choose axis with largest extent
    static int axisCounter = 0;
    int axis = axisCounter % 3;
    axisCounter++;

    return axis;
}

void BSPNode::partition(std::vector<SceneObject *> &objs, int axis,
                        std::vector<SceneObject *> &leftObjs,
                        std::vector<SceneObject *> &rightObjs)
{
    // Calculate split point (median of object centers)
    std::vector<double> centers;
    centers.reserve(objs.size());

    for (auto *obj : objs)
    {
        // Get center of object's bounding box on the split axis
        // Simplified: assume boundingBox has getCenter() method
        // For now, estimate from transformation
        Vector3 center = obj->transform.getPosition();

        if (axis == 0)
            centers.push_back(center.x);
        else if (axis == 1)
            centers.push_back(center.y);
        else
            centers.push_back(center.z);
    }

    // Find median
    std::sort(centers.begin(), centers.end());
    double splitValue = centers[centers.size() / 2];

    // Partition objects based on their center vs split value
    for (auto *obj : objs)
    {
        Vector3 center = obj->transform.getPosition();
        double centerValue;

        if (axis == 0)
            centerValue = center.x;
        else if (axis == 1)
            centerValue = center.y;
        else
            centerValue = center.z;

        if (centerValue < splitValue)
        {
            leftObjs.push_back(obj);
        }
        else
        {
            rightObjs.push_back(obj);
        }
    }
}

void BSPNode::findIntersections(Ray &r, std::vector<SceneObject *> &results)
{
    // Test if ray intersects this node's bounding box
    if (!boundingBox.intersects(r))
    {
        // Ray doesn't hit this region - early exit
        return;
    }

    if (leaf)
    {
        // Leaf node - add all objects to results
        for (auto *obj : objects)
        {
            results.push_back(obj);
        }
    }
    else
    {
        // Internal node - recursively check children
        if (left != nullptr)
        {
            left->findIntersections(r, results);
        }

        if (right != nullptr)
        {
            right->findIntersections(r, results);
        }
    }
}