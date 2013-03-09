#ifndef ENTITY_H
#define ENTITY_H

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <string>
#include <memory>

#include "scriptedstatemachine.h"
#include "vector2d.h"

class Application;
class RenderSystem;
class SteeringBehaviors;

class Entity
{
public:
  //static Entity* Create();

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
  //void SetPosition(double x, double y);
  void SetPosition(const Vector2d& position);
  
  Vector2d GetVelocity() const;
  void SetVelocity(const Vector2d& velocity);
  
  Vector2d GetHeading() const;
  void SetHeading(const Vector2d& heading);
  
  double GetMass() const;
  void SetMass(double mass);

  double GetMaxSpeed() const;
  void SetMaxSpeed(double maxSpeed);

  double GetMaxForce() const;
  void SetMaxForce(double maxForce);

  double GetMaxTurnRate() const;
  void SetMaxTurnRate(double maxTurnRate);

  bool IsCollidable() const;
  void SetIsCollidadable(bool isCollidable);
  
  double GetBoundingRadius();
  void SetBoundingRadius(double boundingRadius);
  
  bool IsActive() const;
  void SetIsActive(bool isActive);
  
  void Update(double deltaTime);
  
  void Render(RenderSystem* renderSystem);

  bool IsVisible() const;
  void SetIsVisible(bool isVisible);

  bool HasCollidedWith(Entity* other);
  void HandleCollisionWith(Entity* other);
  
  void ChangeGlobalState(const luabind::object& state);  
  void SetGlobalState(const luabind::object& state);
  
  void ChangeCurrentState(const luabind::object& state);  
  void SetCurrentState(const luabind::object& state);
  
  luabind::object& GetScript();
  
  void Initialize(const luabind::object& script);
  void Reset();

  static void RegisterWithLua(lua_State* L);
  
private:
  static int _nextId;
  
  Application* _application;
  std::unique_ptr<SteeringBehaviors> _steeringBehaviors;

  int _id;
  std::string _name;
  std::string _type;
  bool _isActive;
  
  std::unique_ptr<ScriptedStateMachine<Entity>> _stateMachine;
  
  Vector2d _position;
  Vector2d _velocity;
  Vector2d _heading;
  double _mass;
  double _maxSpeed;
  double _maxForce;
  double _maxTurnRate;
  bool _isCollidable;
  double _boundingRadius;
  
  bool _isVisible;

  luabind::object _script;
  
  Entity(const Entity& other);

  void ReportCallFunctionException(const std::string& functionName, const std::exception& e);

};

#endif // ENTITY_H
