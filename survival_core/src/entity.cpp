#include "entity.h"

#include <cmath>
#include <string>

#include <luabind/function.hpp>

#include "application.h"
#include "color.h"
#include "rendersystem.h"
#include "steeringbehaviors.h"

int Entity::_nextId = 0;

Entity::Entity(Application* application) :
  _application(application),
  _steeringBehaviors(new SteeringBehaviors(this)),
  _id(_nextId++),
  _isActive(false),
  _stateMachine(new ScriptedStateMachine<Entity>(this)),
  _mass(1.0),
  _maxSpeed(1.0),
  _isCollidable(false)
{
}

Entity::~Entity()
{
  delete _stateMachine;
  _stateMachine = nullptr;
  
  delete _steeringBehaviors;
  _steeringBehaviors = nullptr;
}

Application* Entity::GetApplication() const
{
  return _application;
}

SteeringBehaviors* Entity::GetSteeringBehaviors() const
{
  return _steeringBehaviors;
}

int Entity::GetId() const
{
  return _id;
}

std::string Entity::GetName() const
{
  return _name;
}

void Entity::SetName(const std::string& name)
{
  _name = name;
}

std::string Entity::GetType() const
{
  return _type;
}

void Entity::SetType(const std::string& type)
{
  _type = type;
}

bool Entity::IsOfType(const luabind::object& script) const
{
  return GetType() == luabind::object_cast<std::string>(script["type"]);
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

double Entity::GetMass() const
{
  return _mass;
}

void Entity::SetMass(double mass)
{
  _mass = mass;
}

double Entity::GetMaxSpeed() const
{
  return _maxSpeed;
}

void Entity::SetMaxSpeed(double maxSpeed)
{
  _maxSpeed = maxSpeed;
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
    try
    {
      luabind::call_function<void>(_script["render"], _script, this, renderSystem);
    }
    catch (const luabind::error& e)
    {
      std::cout << "error in Entity::Render: " << e.what() << std::endl;
      throw;
    } 
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

luabind::object& Entity::GetScript()
{
  return _script;
}

void Entity::SetScript(const luabind::object& script)
{
  _script = script;
}

void Entity::Initialize()
{
  if (_script.is_valid())
  {
    luabind::call_function<void>(_script["initialize"], _script, this);
  }
}

void Entity::RegisterWithLua(lua_State* L)
{
  luabind::module(L, "survival")
  [
    luabind::class_<Entity>("entity")
      .def("change_global_state", &Entity::ChangeGlobalState)
      .def("change_current_state", &Entity::ChangeCurrentState)
      .def("handle_collision_with", &Entity::HandleCollisionWith)
      .def("initialize", &Entity::Initialize)
      .def("is_of_type", &Entity::IsOfType)
      .def("render", &Entity::Render)
      .property("application", &Entity::GetApplication)
      .property("steering_behaviors", &Entity::GetSteeringBehaviors)
      .property("id", &Entity::GetId)
      .property("name", &Entity::GetName, &Entity::SetName)
      .property("type", &Entity::GetType, &Entity::SetType)
      .property("is_active", &Entity::IsActive, &Entity::SetIsActive)
      .property("position", &Entity::GetPosition, &Entity::SetPosition)
      .property("velocity", &Entity::GetVelocity, &Entity::SetVelocity)
      .property("heading", &Entity::GetHeading, &Entity::SetHeading)
      .property("mass", &Entity::GetMass, &Entity::SetMass)
      .property("max_speed", &Entity::GetMaxSpeed, &Entity::SetMaxSpeed)
      .property("is_collidable", &Entity::IsCollidable, &Entity::SetIsCollidadable)
      .property("bounding_radius", &Entity::GetBoundingRadius, &Entity::SetBoundingRadius)
      .property("script", &Entity::GetScript, &Entity::SetScript)
  ];
}
