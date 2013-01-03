#include "entity.h"

#include <cmath>

#include <luabind/function.hpp>

#include "application.h"
#include "color.h"
#include "rendersystem.h"

int Entity::_nextId = 0;

Entity::Entity(Application* application) :
  _application(application),
  _id(_nextId++),
  _isActive(false),
  _stateMachine(new ScriptedStateMachine<Entity>(this)),
  _isCollidable(false)
{
}

Entity::~Entity()
{
  delete _stateMachine;
  _stateMachine = nullptr;
}

Application* Entity::GetApplication() const
{
  return _application;
}

int Entity::GetId() const
{
  return _id;
}

int Entity::GetEntityType() const
{
  return _entityType;
}

void Entity::SetEntityType(int entityType)
{
  _entityType = entityType;
}

bool Entity::IsOfType(const luabind::object& script) const
{
  return _entityType == luabind::object_cast<int>(script["initial_entity_type"]);
}

bool Entity::IsActive() const
{
  return _isActive;
}

void Entity::SetIsActive(bool isActive)
{
  _isActive = isActive;
}

Vector2d Entity::GetPosition() const
{
    return _position;
}

void Entity::SetPosition(const Vector2d& position)
{
  _position = position;
}

Vector2d Entity::GetVelocity() const
{
  return _velocity;
}

void Entity::SetVelocity(const Vector2d& velocity)
{
  _velocity = velocity;
}

Vector2d Entity::GetHeading() const
{
  return _heading;
}

void Entity::SetHeading(const Vector2d& heading)
{
  _heading = heading;
}

bool Entity::IsCollidable() const
{
  return _isCollidable;
}

void Entity::SetIsCollidadable(bool isCollidable)
{
  _isCollidable = isCollidable;
}

double Entity::GetBoundingRadius()
{
  return _boundingRadius;
}

void Entity::SetBoundingRadius(double boundingRadius)
{
  _boundingRadius = boundingRadius;
}

void Entity::Update(double deltaTime)
{
  _stateMachine->Execute();
}

void Entity::Render(RenderSystem* renderSystem)
{
  if (_script.is_valid())
  {
    luabind::call_function<void>(_script["render"], _script, this, renderSystem);
  }
}

bool Entity::HasCollidedWith(Entity* other)
{
  double distanceSqr = GetPosition().DistanceToSqr(other->GetPosition());
  
  double totalRadius = GetBoundingRadius() + other->GetBoundingRadius();
  
  return distanceSqr < (totalRadius * totalRadius);
}

void Entity::HandleCollisionWith(Entity* other)
{
  if (_script.is_valid())
  {
    luabind::call_function<void>(_script["handle_collision_with"], _script, this, other);
  }
}

void Entity::ChangeGlobalState(const luabind::object& state)
{
  _stateMachine->ChangeGlobalState(state);
}

void Entity::SetGlobalState(const luabind::object& state)
{
  _stateMachine->SetGlobalState(state);
}

void Entity::ChangeCurrentState(const luabind::object& state)
{
  _stateMachine->ChangeCurrentState(state);
}

void Entity::SetCurrentState(const luabind::object& state)
{
  _stateMachine->SetCurrentState(state);
}

void Entity::SetScript(const luabind::object& script)
{
  _script = script;
  
  if (_script.is_valid())
  {
    luabind::call_function<void>(_script["initialize"], _script, this);
  }
}

luabind::object& Entity::GetScript()
{
  return _script;
}

void Entity::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<Entity>("entity")
      .def("change_global_state", &Entity::ChangeGlobalState)
      .def("change_current_state", &Entity::ChangeCurrentState)
      .def("handle_collision_with", &Entity::HandleCollisionWith)
      .def("is_of_type", &Entity::IsOfType)
      .def("render", &Entity::Render)
      .property("application", &Entity::GetApplication)
      .property("id", &Entity::GetId)
      .property("entity_type", &Entity::GetEntityType, &Entity::SetEntityType)
      .property("is_active", &Entity::IsActive, &Entity::SetIsActive)
      .property("position", &Entity::GetPosition, &Entity::SetPosition)
      .property("velocity", &Entity::GetVelocity, &Entity::SetVelocity)
      .property("heading", &Entity::GetHeading, &Entity::SetHeading)
      .property("is_collidable", &Entity::IsCollidable, &Entity::SetIsCollidadable)
      .property("bounding_radius", &Entity::GetBoundingRadius, &Entity::SetBoundingRadius)
      .property("script", &Entity::GetScript, &Entity::SetScript)
  ];
}
