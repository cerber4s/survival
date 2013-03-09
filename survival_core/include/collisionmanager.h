#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "boundingbox.h"
#include "cell.h"
#include "vector2d.h"

#include <memory>

class Entity;

class CollisionManager
{
public:
  CollisionManager(double left, double top, double right, double bottom, int maxEntitiesPerCel);
  CollisionManager(const Vector2d& topLeft, const Vector2d& bottomRight, int maxEntitiesPerCel);
  CollisionManager(const BoundingBox& size, int maxEntitiesPerCel);
  
  void Add(Entity* entity);
  void Remove(Entity* entity);
  void Update(Entity* entity, const Vector2d& previousPosition);

  void Clear();

  void HandleCollisions();

  std::set<Entity*> GetEntitiesInRange(double x, double y, double radius) const;
  std::set<Entity*> GetEntitiesInRange(const Vector2d& position, double radius) const;

private:
  std::unique_ptr<Cell> _root;

};

#endif
