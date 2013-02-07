#ifndef COLLISIONCEL_H
#define COLLISIONCEL_H

#include "boundingbox.h"
#include "vector2d.h"

template <class entity_type>
class CollisionCell
{
public:
  CollisionCell(const Vector2d& topLeft, const Vector2d& bottomRight);

  const std::list<entity_type>& GetEntities() const;

private:
  BoundingBox _boundingBox;
  std::list<entity_type> _entities;

};


template <class entity_type>
CollisionCell::CollisionCell(const Vector2d& topLeft, const Vector2d& bottomRight) :
  _boundingBox(topLeft, bottomRight)
{
}

#endif
