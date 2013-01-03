#ifndef ENTITY_H
#define ENTITY_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "scriptedstatemachine.h"
#include "vector2d.h"

class Application;
class RenderSystem;

class Entity
{
public:
  Entity(Application* application);
  virtual ~Entity();
  
  Application* GetApplication() const;
  int GetId() const;
  
  int GetEntityType() const;
  void SetEntityType(int entityType);
  
  bool IsOfType(const luabind::object& script) const;
  
  Vector2d GetPosition() const;
  void SetPosition(const Vector2d& position);
  
  Vector2d GetVelocity() const;
  void SetVelocity(const Vector2d& velocity);
  
  Vector2d GetHeading() const;
  void SetHeading(const Vector2d& heading);
  
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
  
  void SetScript(const luabind::object& script);
  luabind::object& GetScript();
  
  static void RegisterWithLua(lua_State* L);
  
private:
  static int _nextId;
  
  Application* _application;
  
  int _id;
  int _entityType;
  bool _isActive;
  
  ScriptedStateMachine<Entity>* _stateMachine;
  
  Vector2d _position;
  Vector2d _velocity;
  Vector2d _heading;
  bool _isCollidable;
  double _boundingRadius;
  
  luabind::object _script;
  
};

#endif // ENTITY_H
