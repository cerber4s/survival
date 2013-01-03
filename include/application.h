#ifndef APPLICATION_H
#define APPLICATION_H

#include <allegro5/allegro.h>

#include <lua.hpp>
#include <luabind/luabind.hpp>

#include <list>
#include <memory>
#include <vector>

#include "luastate.h"

#include "buttons.h"
#include "matrix3.h"
#include "types.h"
#include "vector2d.h"

class Entity;
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
  
  Entity* GetEntityById(int entityId);
  
  const std::vector<Entity*>& GetEntities() const { return _entities; }
  
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
  
  static void RegisterWithLua(lua_State* L);
  
private:
  RenderSystem* _renderSystem;
  ALLEGRO_EVENT_QUEUE *_eventQueue;
  ALLEGRO_MOUSE_STATE _mouseState;
  
  std::vector<Entity*> _entityPool;

  std::vector<Entity*> _entities;
  std::map<int, Entity*> _entityMap;
  
  std::vector<Entity*> _spawnedEntities;
    
  std::unique_ptr<LuaState> _luaState;
  luabind::object _script;  
  luabind::object _application;  
  
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
  
  void CalculateViewportTransformations();

  bool Update();
  void Render();
  
  bool ProcessAllegroEvents();

  void InitializeButtonMap();
  void InitializeKeyStates();
  
  void ClearInactiveEntities();
  
  void HandleCollisions();
  
};

#endif // APPLICATION_H
