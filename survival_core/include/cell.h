#ifndef CELL_H
#define CELL_H

#include "boundingbox.h"
#include "vector2d.h"

#include <set>

class Entity;

class Cell
{
public:
  Cell(int maxEntities, const Vector2d& topLeft, const Vector2d& bottomRight);
  virtual ~Cell();

  void Add(Entity* entity);
  void Remove(Entity* entity, const Vector2d& previousPosition);
  void Update(Entity* entity, const Vector2d& previousPosition);

  void Clear();

  const std::set<Entity*>& GetEntities() const;

  void HandleCollisions();

//private:
  BoundingBox _boundingBox;
  int _maxEntities;
  std::set<Entity*> _entities;
  Cell** _subCells;

  void ClearSubCells();
  int GetCellIndex(const Vector2d& position);

};

#endif
