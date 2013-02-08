#include "cell.h"

#include <boost/foreach.hpp>

#include "entity.h"

#define CELL_MIN_SIZE 256

Cell::Cell(int maxEntities, const Vector2d& topLeft, const Vector2d& bottomRight) :
  _maxEntities(maxEntities),
  _boundingBox(topLeft, bottomRight),
  _subCells(nullptr)
{
}

Cell::~Cell()
{
  _entities.clear();
  ClearSubCells();
}

void Cell::Add(Entity* entity)
{
  if (_subCells != nullptr)
  {
    _subCells[GetCellIndex(entity->GetPosition())]->Add(entity);
    return;
  }

  Vector2d size = _boundingBox.GetSize();
  if (_entities.size() < _maxEntities || size.x < CELL_MIN_SIZE || size.y < CELL_MIN_SIZE)
  {
    _entities.insert(entity);
  }
  else
  {
    if (_subCells == nullptr)
    {
      auto topleft = _boundingBox.GetTopLeft();
      auto size = _boundingBox.GetSize();
      auto halfsize = size * 0.5;
      auto center = _boundingBox.GetCenter();

      _subCells = new Cell*[4];
      _subCells[0] = new Cell(_maxEntities, topleft, topleft + halfsize);
      _subCells[1] = new Cell(_maxEntities, topleft + Vector2d(halfsize.x, 0), topleft + Vector2d(halfsize.x, 0) + halfsize);
      
      _subCells[2] = new Cell(_maxEntities, topleft + Vector2d(0, halfsize.y), topleft + Vector2d(0, halfsize.y) + halfsize);
      _subCells[3] = new Cell(_maxEntities, topleft + halfsize, topleft + size);
    }
    
    BOOST_FOREACH(Entity* e, _entities)
    {
      _subCells[GetCellIndex(e->GetPosition())]->Add(e);
    }
    _entities.clear();

    _subCells[GetCellIndex(entity->GetPosition())]->Add(entity);
  }
}

void Cell::Remove(Entity* entity, const Vector2d& previousPosition)
{
  if (_subCells == nullptr)
  {
    _entities.erase(entity);
  }
  else
  {
    _subCells[GetCellIndex(previousPosition)]->Remove(entity, previousPosition);
  }
}

void Cell::Update(Entity* entity, const Vector2d& previousPosition)
{
  if (_subCells == nullptr)
  {
    _entities.erase(entity);
  }
  else
  {
    _subCells[GetCellIndex(previousPosition)]->Remove(entity, previousPosition);
  }

  Add(entity);
}

void Cell::Clear()
{
  ClearSubCells();
  _entities.clear();
}

const std::set<Entity*>& Cell::GetEntities() const
{
  return _entities;
}

void Cell::HandleCollisions()
{
  if (_subCells != nullptr)
  {
    for(int i = 0; i < 4; i++)
    {
      _subCells[i]->HandleCollisions();
    }

    return;
  }

  std::set<Entity*>::iterator i = _entities.begin();
  while (i != _entities.end())
  {
    std::set<Entity*>::iterator j = std::set<Entity*>::iterator(i);
    j++;

    while (j != _entities.end())
    {
      Entity* left = (*i);
      Entity* right = (*j);
          
      if (left->IsCollidable() && right->IsCollidable() && left->HasCollidedWith(right))
      {
        left->HandleCollisionWith(right);
      }

      j++;
    }
    i++;
  }
}

void Cell::ClearSubCells()
{
  if (_subCells != nullptr)
  {
    for(int i = 0; i < 4; i++)
    {
      delete _subCells[i];
    }

    delete [] _subCells;
    _subCells = nullptr;
  }
}

int Cell::GetCellIndex(const Vector2d& position)
{
  Vector2d center = _boundingBox.GetCenter();

  if (position.x <= center.x)
  {
    if (position.y <= center.y)
      return 0;

    return 2;
  }

  if (position.y <= center.y)
    return 1;

  return 3;
}
