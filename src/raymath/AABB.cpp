#include <iostream>
#include "AABB.hpp"

AABB::AABB() : Min(Vector3()), Max(Vector3()) {}

AABB::AABB(Vector3 min, Vector3 max) : Min(min), Max(max) {}

AABB::~AABB() {}

AABB &AABB::operator=(AABB const &box)
{
    Min = box.Min;
    Max = box.Max;
    return *this;
}

void AABB::subsume(AABB const &other)
{
    Min.x = std::min(Min.x, other.Min.x);
    Min.y = std::min(Min.y, other.Min.y);
    Min.z = std::min(Min.z, other.Min.z);

    Max.x = std::max(Max.x, other.Max.x);
    Max.y = std::max(Max.y, other.Max.y);
    Max.z = std::max(Max.z, other.Max.z);
}

bool AABB::intersects(const Ray &r) const
{
    /**
     * Optimised implementation of ray-AABB intersection, taken from: https://tavianator.com/2011/ray_box.html
     */

    Vector3 o = r.GetPosition();
    Vector3 dInv = r.GetDirection().inverse();

    double tx1 = (Min.x - o.x) * dInv.x;
    double tx2 = (Max.x - o.x) * dInv.x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (Min.y - o.y) * dInv.y;
    double ty2 = (Max.y - o.y) * dInv.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (Min.z - o.z) * dInv.z;
    double tz2 = (Max.z - o.z) * dInv.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin && tmax > 0;
}

std::ostream &operator<<(std::ostream &_stream, AABB const &box)
{
    return _stream << "Min(" << box.Min << ")-Max(" << box.Max << ")";
}