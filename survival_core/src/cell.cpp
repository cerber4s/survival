#include "cell.h"

#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include "entity.h"

//#define DEBUG_COUT

#define CELL_MIN_SIZE 256

Cell::Cell(int left, int top, int right, int bottom, int maxEntities) :
  _maxEntities(maxEntities),
  _bounds(Vector2d(left, top), Vector2d(right, bottom)),
  _subCells(nullptr)
{
}

Cell::Cell(const Vector2d& topLeft, const Vector2d& bottomRight, int maxEntities) :
  _maxEntities(maxEntities),
  _bounds(topLeft, bottomRight),
  _subCells(nullptr)
{
}

Cell::Cell(const BoundingBox& bounds, int maxEntities) :
  _maxEntities(maxEntities),
  _bounds(bounds),
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
#ifdef DEBUG_COUT
  std::cout << (boost::format("adding entity [%gx%g, %gx%g], entity ['%s', %gx%g] ...") % _bounds.GetLeft() % _bounds.GetTop() % _bounds.GetRight() % _bounds.GetBottom() % entity->GetType() % entity->GetPosition().x % entity->GetPosition().y).str().c_str() << std::endl;
#endif
  if (!_bounds.Contains(entity->GetPosition()))
  {
    auto message = (boost::format("unable to add entity, position is out of bounds (pos: %gx%g, type: '%s', bounds: %gx%g, %gx%g)") 
      % entity->GetPosition().x
      % entity->GetPosition().y
      % entity->GetType()
      % _bounds.GetLeft()
      % _bounds.GetTop()
      % _bounds.GetRight()
      % _bounds.GetBottom());

    throw std::runtime_error(message.str());
  }

  if (_subCells != nullptr)
  {
    AddToSubCell(entity);
    return;
  }
  
  if (_entities.size() < _maxEntities || _bounds.GetWidth() < CELL_MIN_SIZE || _bounds.GetHeight() < CELL_MIN_SIZE)
  {
    _entities.insert(entity);
  }
  else
  {
    if (_subCells == nullptr)
    {
#ifdef DEBUG_COUT
      std::cout << (boost::format("splitting cell [%gx%g, %gx%g] ...") % _bounds.GetLeft() % _bounds.GetTop() % _bounds.GetRight() % _bounds.GetBottom()).str().c_str() << std::endl;
#endif
      BOOST_FOREACH(Entity* e, _entities)
      {
        if (!_bounds.Contains(e->GetPosition()))
        {
          auto message = (boost::format("unable to split cell [%gx%g, %gx%g], contained entity ['%s', %gx%g] is out of bounds") 
            % _bounds.GetLeft()
            % _bounds.GetTop()
            % _bounds.GetRight()
            % _bounds.GetBottom()
            % e->GetType()
            % e->GetPosition().x
            % e->GetPosition().y);

          throw std::runtime_error(message.str());
        }
      }

      auto topleft = _bounds.GetTopLeft();
      auto size = _bounds.GetSize();
      auto halfsize = size * 0.5;
      auto center = _bounds.GetCenter();

      _subCells = new Cell*[4];
      _subCells[0] = new Cell(BoundingBox(topleft, topleft + halfsize), _maxEntities);
      
      _subCells[1] = new Cell(topleft + Vector2d(halfsize.x, 0), topleft + Vector2d(halfsize.x, 0) + halfsize, _maxEntities);
      _subCells[2] = new Cell(topleft + Vector2d(0, halfsize.y), topleft + Vector2d(0, halfsize.y) + halfsize, _maxEntities);
      _subCells[3] = new Cell(topleft + halfsize, topleft + size, _maxEntities);


#ifdef DEBUG_COUT
      std::cout << (boost::format("inserting entities ...")).str().c_str() << std::endl;
#endif
      BOOST_FOREACH(Entity* e, _entities)
      {
        AddToSubCell(e);
      }
      _entities.clear();

#ifdef DEBUG_COUT
      std::cout << (boost::format("cell splitted")).str().c_str() << std::endl;
#endif
    }

    AddToSubCell(entity);
  }
}

void Cell::Remove(Entity* entity, const Vector2d& previousPosition)
{
  _oversizedEntities.erase(entity);

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
  Remove(entity, previousPosition);
  Add(entity);
}

void Cell::Clear()
{
  ClearSubCells();

  _oversizedEntities.clear();
  _entities.clear();
}

const BoundingBox& Cell::GetBounds() const
{
  return _bounds;
}

const std::set<Entity*>& Cell::GetEntities() const
{
  return _entities;
}


std::set<Entity*> Cell::GetEntitiesInRange(double x, double y, double radius) const
{
  std::set<Entity*> entitiesInRange;
  if (radius > 0)
  {
    GetEntitiesInRange(Vector2d(x, y), radius, entitiesInRange);
  }

  return entitiesInRange;
}

std::set<Entity*> Cell::GetEntitiesInRange(const Vector2d& position, double radius) const
{
  std::set<Entity*> entitiesInRange;
  if (radius > 0)
  {
    GetEntitiesInRange(position, radius, entitiesInRange);
  }

  return entitiesInRange;
}

void Cell::HandleCollisions()
{
  static std::set<Entity*> empty;
  HandleCollisions(empty);
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
  Vector2d center = _bounds.GetCenter();

  if (position.x < center.x)
  {
    if (position.y < center.y)
      return 0;

    return 2;
  }

  if (position.y < center.y)
    return 1;

  return 3;
}

void Cell::AddToSubCell(Entity* entity)
{
#ifdef DEBUG_COUT
  std::cout << (boost::format("adding to subcel [%gx%g, %gx%g], entity ['%s', %gx%g] ...") % _bounds.GetLeft() % _bounds.GetTop() % _bounds.GetRight() % _bounds.GetBottom() % entity->GetType() % entity->GetPosition().x % entity->GetPosition().y).str().c_str() << std::endl;
#endif

  if (!_bounds.Contains(entity->GetPosition()))
  {
    throw std::runtime_error("invalid cel");
  }

  Cell* cell = _subCells[GetCellIndex(entity->GetPosition())];
  if (!cell->GetBounds().Contains(entity->GetPosition()))
  {
    throw std::runtime_error("invalid subcel");
  }

  cell->Add(entity);

  BoundingBox boundingBox(entity->GetPosition() + Vector2d(-entity->GetBoundingRadius(), -entity->GetBoundingRadius()), 
    entity->GetPosition() + Vector2d(entity->GetBoundingRadius(), entity->GetBoundingRadius()));

  if (!cell->GetBounds().Contains(boundingBox))
  {
    _oversizedEntities.insert(entity);
  }
}

void Cell::HandleCollisions(const std::set<Entity*>& oversizedEntities)
{
  std::set<Entity*> o(oversizedEntities);
  o.insert(_oversizedEntities.begin(), _oversizedEntities.end());

  if (_subCells != nullptr)
  {
    for(int i = 0; i < 4; i++)
    {
      _subCells[i]->HandleCollisions(o);
    }

    return;
  }

  o.insert(_entities.begin(), _entities.end());
  std::set<Entity*>::iterator i = o.begin();
  while (i != o.end())
  {
    std::set<Entity*>::iterator j = std::set<Entity*>::iterator(i);
    j++;

    while (j != o.end())
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

void Cell::GetEntitiesInRange(const Vector2d& position, double radius, std::set<Entity*>& entitiesInRange) const
{
  double radiusSqr = radius * radius;

  if (_subCells == nullptr)
  {
    BOOST_FOREACH(Entity* entity, _entities)
    {
      if (entity->GetPosition().DistanceToSqr(position) <= (radiusSqr)){
        entitiesInRange.insert(entity);
      }
    }
  }
  else
  {   
    BoundingBox boundingBox(position + Vector2d(-radius, -radius), position + Vector2d(radius, radius));
    for(int i = 0; i < 4; i++)
    {
      if (boundingBox.IsOverlappedWith(_subCells[i]->GetBounds()))
      {
        _subCells[i]->GetEntitiesInRange(position, radius, entitiesInRange);
      }
    }
  }

  BOOST_FOREACH(Entity* entity, _oversizedEntities)
  {
    if (entity->GetPosition().DistanceToSqr(position) <= radiusSqr){
      entitiesInRange.insert(entity);
    }
  }
}
