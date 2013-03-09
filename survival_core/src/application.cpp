#include "application.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath>
#include <ctime>
#include <exception>

#include <allegro5/allegro.h>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <luabind/function.hpp>
#include <luabind/iterator_policy.hpp>

#include "entity.h"
#include "entitymanager.h"
#include "luastate.h"
#include "random.h"
#include "rendersystem.h"

Application::Application() :
  _eventQueue(nullptr)
{
  _luaState = std::unique_ptr<LuaState>(new LuaState());
  _luaState->RunScript("scripts/application.lua");
  
  _script = luabind::globals(_luaState->GetLuaState());
  if (!_script.is_valid())
  {
    throw std::runtime_error("failed to acquire script globals");
  }
  
  _entityManager = std::unique_ptr<EntityManager>(new EntityManager(this, 1024 * 16));
  _collisionManager = std::unique_ptr<CollisionManager>(new CollisionManager(-16 * 1024, -16 * 1024, 16 * 1024, 16 * 1024, 32));
  
  CalculateViewportTransformations();
  
  InitializeButtonMap();
  InitializeKeyStates();
}

Application::~Application()
{
  if (_entityManager)
  {
    _entityManager->ActivateSpawnedEntities();
    
    std::set<Entity*> entities;
    _entityManager->GetActiveEntities(entities);

    BOOST_FOREACH(Entity* entity, entities)
    {
      _entityManager->Free(entity);
    }

    delete _entityManager.release();
  }

  _applicationScript.swap(luabind::object());
  _script.swap(luabind::object());

  if (_luaState)
  {
    delete _luaState.release();
  }
}

bool Application::Initialize()
{ 
  Random::Initialize();
  
  if(!al_init()) 
  {
    std::cout << "failed to initialize allegro" << std::endl;
    return false;
  }

  _renderSystem = std::unique_ptr<RenderSystem>(new RenderSystem(this));
  if (!_renderSystem->Initialize())
  {
    std::cout << "failed to initialize graphics context" << std::endl;
    return false;
  }
  
  _renderSystem->SetWindowTitle("survival");
  
  if (!al_install_keyboard())
  {
    std::cout << "failed to install keyboard" << std::endl;
    return false;
  }
 
  if (!al_install_mouse())
  {
    std::cout << "failed to install mouse" << std::endl;
    return false;
  }
 
  al_hide_mouse_cursor(_renderSystem->GetDisplay());
 
  al_get_mouse_state(&_mouseState);
 
  _eventQueue = al_create_event_queue();
  if (!_eventQueue)
  {
    std::cout << "failed to create event queue" << std::endl;
    return false;
  }
 
  al_register_event_source(_eventQueue, al_get_keyboard_event_source());
  al_register_event_source(_eventQueue, al_get_mouse_event_source());
  al_register_event_source(_eventQueue, al_get_display_event_source(_renderSystem->GetDisplay()));
 
  _applicationScript = _script["application"];
  if (!_applicationScript.is_valid())
  {
    std::cout << "application script is invalid" << std::endl;
    return false;
  }

  try
  {
    luabind::call_function<void>(_applicationScript["initialize"], _applicationScript, this);
  }
  catch (const std::exception& e)
  {
    std::cout << "error in function call to 'application:initialize': " << e.what() << std::endl;
    throw;
  }

  return true;
}

void Application::UnInitialize()
{
  if (_eventQueue)
  {
    //al_unregister_event_source(_eventQueue, al_get_keyboard_event_source());
    //al_unregister_event_source(_eventQueue, al_get_mouse_event_source());
    //al_unregister_event_source(_eventQueue, al_get_display_event_source(_renderSystem->GetDisplay()));

    al_destroy_event_queue(_eventQueue);
    _eventQueue = NULL;
  }

  if (_renderSystem)
  {
    _renderSystem->UnInitialize();
  }
}

void Application::Execute()
{
  int refresh_rate = 60;
  float fixed_dt = 1.0f / refresh_rate;
  float old_time = al_get_time();
  float game_time = al_get_time();
  
  bool done = false;
  while (!done)
  {
    double dt = al_current_time() - old_time;
    al_rest(fixed_dt - dt);
    dt = al_current_time() - old_time;
    old_time = al_current_time();
    
    if (old_time - game_time > dt)
    {
      game_time += fixed_dt * floor((old_time - game_time) / fixed_dt);
    }
    
    double start_time = al_current_time();
    while(!done && old_time - game_time >= 0)
    {
      game_time += fixed_dt;

      if (!Update())
      {
        done = true;
      }
      
      if(al_current_time() - start_time > fixed_dt * 0.5)
      {
        //break if we start taking too long
        break;
      }
    }
    
    if (!done)
    {
      Render();
    }
  }
}

Entity* Application::SpawnEntity(const luabind::object& script)
{
  Entity* entity = nullptr;
  if (!_entityManager->TrySpawn(&entity, script))
  {
    throw std::runtime_error("unable to spawn entity, no entities available (max. entity count reached)");
  }
  
  _collisionManager->Add(entity);

  return entity;
}

const std::set<Entity*>& Application::GetEntities() const
{
  return _entityManager->GetActiveEntities();
}

Entity* Application::GetEntityById(int entityId) const
{
  return _entityManager->GetEntityById(entityId);
}

Entity* Application::GetEntityByName(const std::string& name) const
{
  return _entityManager->GetEntityByName(name);
}

const std::set<Entity*>& Application::GetEntitiesByType(const std::string& type) const
{
  return _entityManager->GetEntitiesByType(type);
}

EntitySetWrapper Application::GetEntitiesInRange(const Vector2d& position, double radius) const
{
  return EntitySetWrapper(_collisionManager->GetEntitiesInRange(position, radius));
}

void Application::CalculateViewportTransformations()
{
  _viewportTransformation = Matrix3::CreateTranslation(_viewportTranslate * -1);
  _viewportInverseTransformation = Matrix3::CreateTranslation(_viewportTranslate);
}

bool Application::Update()
{
  al_get_mouse_state(&_mouseState);

  if (!ProcessAllegroEvents())
  {
    return false;
  }

  _entityManager->ActivateSpawnedEntities();
  
  try
  {
    luabind::call_function<void>(_applicationScript["update"], _applicationScript, this, 1.0);
  }
  catch (const std::exception& e)
  {
    std::cout << "error in function call to 'application:update': " << e.what() << std::endl;
    throw;
  }

  BOOST_FOREACH(Entity* entity, _entityManager->GetActiveEntities())
  {
    Vector2d previousPosition = entity->GetPosition();

    entity->Update(1.0);
    _collisionManager->Update(entity, previousPosition);
  }

  ClearInactiveEntities();
  HandleCollisions();
  
  for(u32 i = 0; i < ALLEGRO_KEY_MAX; i++)
  {
    _previousKeyStates[i] = _keyStates[i];
  }
  
  return true;
}

void Application::Render()
{
  al_wait_for_vsync();
  al_clear_to_color(al_map_rgb(0,0,0));
 
  al_set_blender(ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA);
 
  try
  {
    luabind::call_function<void>(_applicationScript["render"], _applicationScript, this, _renderSystem.get());  
  }
  catch (const std::exception& e)
  {
    std::cout << "error in function call to 'application:render': " << e.what() << std::endl;
    throw;
  }
  
  al_flip_display();
}
  
bool Application::ProcessAllegroEvents()
{
  ALLEGRO_EVENT event;
    
  while (al_get_next_event(_eventQueue, &event))
  {
    switch (event.type)
    {
      case ALLEGRO_EVENT_KEY_DOWN:
      {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
          return false;	
        }
        
        _keyStates[event.keyboard.keycode] = true;
        break;
      }
      case ALLEGRO_EVENT_KEY_UP:
      {
        _keyStates[event.keyboard.keycode] = false;
        break;
      }
      case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      {
        if (event.mouse.button == 1)
        {
        }
        break;
      }
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
      {
        return false;	
      }
      default:
        break;
    }
  }
  
  return true;
}

void Application::InitializeButtonMap()
{
  _buttonMap[ButtonUp]        = ButtonInfo(ALLEGRO_KEY_UP,    ALLEGRO_KEY_W);
  _buttonMap[ButtonDown]      = ButtonInfo(ALLEGRO_KEY_DOWN,  ALLEGRO_KEY_S);
  _buttonMap[ButtonLeft]      = ButtonInfo(ALLEGRO_KEY_LEFT,  ALLEGRO_KEY_A);
  _buttonMap[ButtonRight]     = ButtonInfo(ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_D);
  _buttonMap[ButtonPrimary]   = ButtonInfo(ALLEGRO_KEY_Z,     -1);
  _buttonMap[ButtonSecondary] = ButtonInfo(ALLEGRO_KEY_X,     -1);
  
  std::map<int, ButtonInfo>::const_iterator i = _buttonMap.begin();
  while (i != _buttonMap.end())
  {
    if ((*i).second.primary != -1)
    {
      _primaryButtonStates[(*i).first] = (*i).second.primary;
    }
    if ((*i).second.secondary != -1)
    {
      _secondaryButtonStates[(*i).first] = (*i).second.secondary;
    }
    
    i++;
  }
}

void Application::InitializeKeyStates()
{
  for(u32 i = 0; i < ALLEGRO_KEY_MAX; i++)
  {
    _keyStates[i] = false;
    _previousKeyStates[i] = false;
  }
}

void Application::ClearInactiveEntities()
{
  std::set<Entity*> activeEntities;
  _entityManager->GetActiveEntities(activeEntities);

  std::set<Entity*>::iterator i = activeEntities.begin();
  while (i != activeEntities.end())
  {
    Entity* entity = *i;
    if (!entity->IsActive())
    {
      _entityManager->Free(entity);
      _collisionManager->Remove(entity);
    
      i = activeEntities.erase(i);
    }
    else
    {
      i++;
    }
  }
}

void Application::HandleCollisions()
{
  _collisionManager->HandleCollisions();
}

bool Application::IsButtonPressed(const Buttons& button) const
{
  auto primary = _primaryButtonStates.find(button);
  auto secondary = _secondaryButtonStates.find(button);
  
  return 
    (primary != _primaryButtonStates.end() || secondary != _secondaryButtonStates.end()) &&
    ((primary != _primaryButtonStates.end() && _keyStates[(*primary).second]) ||
     (secondary != _secondaryButtonStates.end() && _keyStates[(*secondary).second]));
}

int Application::GetMouseX() const
{
  return _mouseState.x;
}

int Application::GetMouseY() const
{
  return _mouseState.y;
}

bool Application::IsLeftMouseButtonPressed() const
{
  return (_mouseState.buttons & 1);
}

bool Application::IsRightMouseButtonPressed() const
{
  return (_mouseState.buttons & 2);
}

Vector2d Application::GetViewportTranslate() const
{
  return _viewportTranslate;
}

void Application::SetViewportTranslate(const Vector2d& viewportTranslate)
{
  _viewportTranslate = viewportTranslate;
  CalculateViewportTransformations();
}

Matrix3 Application::GetViewportTransformation() const
{
  return _viewportTransformation;
}

Matrix3 Application::GetViewportInverseTransformation() const
{
  return _viewportInverseTransformation;
}

void Application::EnsureEntityNameIsNotRegistered(const std::string& name)
{
  Entity* entity = GetEntityByName(name);
  if (entity != nullptr)
  {
    throw std::runtime_error((boost::format("entity name '%s' is already registered") % name).str());
  }
}

RenderSystem* Application::GetRenderSystem() const
{
  return _renderSystem.get();
}

void Application::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<Application>("application")
      .def("spawn_entity", &Application::SpawnEntity)
      .def("get_entity_by_id", &Application::GetEntityById)
      .def("get_entity_by_name", &Application::GetEntityByName)
      .def("get_entities_by_type", &Application::GetEntitiesByType, luabind::return_stl_iterator)
      .def("get_entities_in_range", &Application::GetEntitiesInRange)
      .property("is_button_up_pressed", &Application::IsButtonUpPressed)
      .property("is_button_down_pressed", &Application::IsButtonDownPressed)
      .property("is_button_left_pressed", &Application::IsButtonLeftPressed)
      .property("is_button_right_pressed", &Application::IsButtonRightPressed)
      .property("is_button_primary_pressed", &Application::IsButtonPrimaryPressed)
      .property("is_button_secondary_pressed", &Application::IsButtonSecondaryPressed)
      .property("is_left_mouse_button_pressed", &Application::IsLeftMouseButtonPressed)
      .property("is_right_mouse_button_pressed", &Application::IsRightMouseButtonPressed)
      .property("mouse_x", &Application::GetMouseX)
      .property("mouse_y", &Application::GetMouseY)
      .property("viewport_translate", &Application::GetViewportTranslate, &Application::SetViewportTranslate)
      .property("viewport_transformation", &Application::GetViewportTransformation)
      .property("viewport_inverse_transformation", &Application::GetViewportInverseTransformation)
      .property("entities", &Application::GetEntities, luabind::return_stl_iterator)
      .property("gfx", &Application::GetRenderSystem)
  ];
}
