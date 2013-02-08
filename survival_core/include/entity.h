#ifndef ENTITY_H
#define ENTITY_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <string>

#include "scriptedstatemachine.h"
#include "vector2d.h"

class Application;
class RenderSystem;
class SteeringBehaviors;

class Entity
{
public:
  Entity(Application* application);
  virtual ~Entity();
  
  Application* GetApplication() const;
  SteeringBehaviors* GetSteeringBehaviors() const;
  
  int GetId() const;

  std::string GetName() const;
  void SetName(const std::string& name);
  
  std::string GetType() const;
  void SetType(const std::string& type);
  
  bool IsOfType(const luabind::object& script) const;
  
  Vector2d GetPosition() const;
  void SetPosition(const Vector2d& position);
  
  Vector2d GetVelocity() const;
  void SetVelocity(const Vector2d& velocity);
  
  Vector2d GetHeading() const;
  void SetHeading(const Vector2d& heading);
  
  double GetMass() const;
  void SetMass(double mass);

  double GetMaxSpeed() const;
  void SetMaxSpeed(double maxSpeed);
  
  bool IsCollidable() const;
  void SetIsCollidadable(bool isCollidable);
  
  double GetBoundingRadius();
  void SetBoundingRadius(double boundingRadius);
  
  bool IsActive() const;
  void SetIsActive(bool isActive);
  
  void Update(double deltaTime);
  
  void Render(RenderSystem* renderSystem);
  
  bool HasCollidedWith(Entity* other);
  void HandleCollisionWith(Entity* other);
  
  void ChangeGlobalState(const luabind::object& state);  
  void SetGlobalState(const luabind::object& state);
  
  void ChangeCurrentState(const luabind::object& state);  
  void SetCurrentState(const luabind::object& state);
  
  luabind::object& GetScript();
  void SetScript(const luabind::object& script);
  
  void Initialize();
  
  static void RegisterWithLua(lua_State* L);
  
private:
  static int _nextId;
  
  Application* _application;
  SteeringBehaviors* _steeringBehaviors;

  int _id;
  std::string _name;
  std::string _type;
  bool _isActive;
  
  ScriptedStateMachine<Entity>* _stateMachine;
  
  Vector2d _position;
  Vector2d _velocity;
  Vector2d _heading;
  double _mass;
  double _maxSpeed;
  bool _isCollidable;
  double _boundingRadius;
  
  luabind::object _script;
  
  Entity(const Entity& other);

};

#endif // ENTITY_H
