#pragma once
#include "../raymath/Vector3.hpp"
#include "../raymath/Ray.hpp"

class AABB
{
private:
  Vector3 Min;
  Vector3 Max;

public:
  AABB();
  AABB(Vector3 min, Vector3 max);
  ~AABB();
  AABB &operator=(AABB const &vec);

  /**
   * Grows the AABB to include the one passed as a parameter.
   */
  void subsume(AABB const &other);

  bool intersects(const Ray &r) const;

  friend std::ostream &operator<<(std::ostream &_stream, AABB const &box);
};
