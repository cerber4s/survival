#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "cell.h"
#include "vector2d.h"

#include <memory>

class Entity;

class CollisionManager
{
public:
  CollisionManager(int maxEntitiesPerCel);
  
  void Add(Entity* entity);
  void Remove(Entity* entity);
  void Update(Entity* entity, const Vector2d& previousPosition);

  void Clear();

  void HandleCollisions();

//private:
  std::unique_ptr<Cell> _root;

};

#endif
