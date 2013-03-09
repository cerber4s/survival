#ifndef APPLICATION_H
#define APPLICATION_H

#include "buttons.h"
#include "collisionmanager.h"
#include "entitysetwrapper.h"
#include "matrix3.h"
#include "types.h"
#include "vector2d.h"

#include <allegro5/allegro.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <list>
#include <memory>
#include <set>
#include <vector>

class Entity;
class EntityManager;
class LuaState;
class RenderSystem;

class Application
{
public:
  Application();
  virtual ~Application();
  
  bool Initialize();
  void UnInitialize();
  
  void Execute();
  
  Entity* SpawnEntity(const luabind::object& script);

  const std::set<Entity*>& GetEntities() const;

  Entity* GetEntityById(int entityId) const;
  Entity* GetEntityByName(const std::string& name) const;
  
  const std::set<Entity*>& GetEntitiesByType(const std::string& type) const;
  
  EntitySetWrapper GetEntitiesInRange(const Vector2d& position, double radius) const;

  bool IsButtonUpPressed() const { return IsButtonPressed(ButtonUp); }
  bool IsButtonDownPressed() const { return IsButtonPressed(ButtonDown); }
  bool IsButtonLeftPressed() const { return IsButtonPressed(ButtonLeft); }
  bool IsButtonRightPressed() const { return IsButtonPressed(ButtonRight); }
  bool IsButtonPrimaryPressed() const { return IsButtonPressed(ButtonPrimary); }
  bool IsButtonSecondaryPressed() const { return IsButtonPressed(ButtonSecondary); }
  
  bool IsButtonPressed(const Buttons& button) const;
  
  int GetMouseX() const;
  int GetMouseY() const;
  bool IsLeftMouseButtonPressed() const;
  bool IsRightMouseButtonPressed() const;
  
  Vector2d GetViewportTranslate() const;
  void SetViewportTranslate(const Vector2d& viewportTranslate);
  
  Matrix3 GetViewportTransformation() const;
  Matrix3 GetViewportInverseTransformation() const;
  
  RenderSystem* GetRenderSystem() const;

  static void RegisterWithLua(lua_State* L);
  
private:
  std::unique_ptr<RenderSystem> _renderSystem;
  ALLEGRO_EVENT_QUEUE *_eventQueue;
  ALLEGRO_MOUSE_STATE _mouseState;
  
  std::unique_ptr<EntityManager> _entityManager;
    
  std::unique_ptr<LuaState> _luaState;
  luabind::object _script;  
  luabind::object _applicationScript;
  
  std::unique_ptr<CollisionManager> _collisionManager;

  Vector2d _viewportTranslate;
  Matrix3 _viewportTransformation;
  Matrix3 _viewportInverseTransformation;
  
  typedef struct ButtonInfo
  {
    int primary;
    int secondary;
    
    ButtonInfo() {
      this->primary = -1;
      this->secondary = -1;
    }
    
    ButtonInfo(int primary, int secondary) {
      this->primary = primary;
      this->secondary = secondary;
    }
  } ButtonInfo;
  
  std::map<int, ButtonInfo> _buttonMap;
  std::map<int, int> _primaryButtonStates;
  std::map<int, int> _secondaryButtonStates;
  
  bool _keyStates[ALLEGRO_KEY_MAX];
  bool _previousKeyStates[ALLEGRO_KEY_MAX];
    
  Application(const Application& other);

  void CalculateViewportTransformations();

  bool Update();
  void Render();
  
  bool ProcessAllegroEvents();

  void InitializeButtonMap();
  void InitializeKeyStates();
  
  void ClearInactiveEntities();
  
  void HandleCollisions();
  
  void EnsureEntityNameIsNotRegistered(const std::string& name);

};

#endif // APPLICATION_H
