#include "application.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <ctime>
#include <exception>

#include <allegro5/allegro.h>

#include <boost/format.hpp>
#include <boost/foreach.hpp>

#include <luabind/function.hpp>
#include <luabind/iterator_policy.hpp>

#include "rendersystem.h"
#include "entity.h"
#include "random.h"

#include <sstream>
#include <iostream>

int add_file_and_line(lua_State* L)
{
  lua_Debug d;
  lua_getstack(L, 1, &d);
  lua_getinfo(L, "Sln", &d);
  std::string err = lua_tostring(L, -1);
  lua_pop(L, 1);
  std::stringstream msg;
  msg << d.short_src << ":" << d.currentline;

  if (d.name != 0)
  {
    msg << "(" << d.namewhat << " " << d.name << ")";
  }
  msg << " " << err;

  std::cout << msg.str() << std::endl;
  lua_pushstring(L, msg.str().c_str());
return 1;
}

Application::Application() :
  _renderSystem(nullptr),
  _eventQueue(nullptr),
  _luaState(new LuaState())
{
  luabind::set_pcall_callback(add_file_and_line);
  
  _luaState->RunScript("scripts/application.lua");
  
  _script = luabind::globals(_luaState->GetLuaState());
  if (!_script.is_valid())
  {
    throw std::runtime_error("failed to acquire script globals");
  }
  
  int entityPoolSize = 1024 * 16;
  _entityPool = std::vector<Entity*>(entityPoolSize);
  for (int i = (int)_entityPool.size() - 1; i >= 0; i--)
  {
    _entityPool[i] = new Entity(this);
  }
  
  CalculateViewportTransformations();
  
  InitializeButtonMap();
  InitializeKeyStates();
}

Application::~Application()
{
  _entityNamesMap.clear();
  
  BOOST_FOREACH(Entity* entity, _entityPool)
  {
    delete entity;
  }
  _entityPool.clear();
  
  BOOST_FOREACH(Entity* entity, _spawnedEntities)
  {
    delete entity;
  }
  _spawnedEntities.clear();
  
  BOOST_FOREACH(Entity* entity, _entities)
  {
    delete entity;
  }
  _entities.clear();
  _entityMap.clear();
}

bool Application::Initialize()
{
  std::cout << "Initialize" << std::endl;
  
  Random::Initialize();

  if(!al_init()) 
  {
    std::cout << "failed to initialize allegro!" << std::endl;
    return false;
  }
  
  _renderSystem = new RenderSystem(this);
  if (!_renderSystem->Initialize())
  {
    std::cout << "failed to initialize graphics context" << std::endl;
    return false;
  }
  
  _renderSystem->SetWindowTitle("survival");
  
  if (!al_install_keyboard())
  {
    std::cout << "failed to install keyboard!" << std::endl;
    return false;
  }
 
  if (!al_install_mouse())
  {
    std::cout << "failed to install mouse!" << std::endl;
    return false;
  }
 
  al_hide_mouse_cursor(_renderSystem->GetDisplay());
 
  al_get_mouse_state(&_mouseState);
 
  _eventQueue = al_create_event_queue();
  if (!_eventQueue)
  {
    std::cout << "failed to create event queue!" << std::endl;
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

  luabind::call_function<void>(_applicationScript["initialize"], _applicationScript, this);
  
  return true;
}

void Application::UnInitialize()
{
  std::cout << "UnInitialize" << std::endl;
  
  luabind::object invalidState;
  BOOST_FOREACH(Entity* entity, _entities)
  {
    entity->SetCurrentState(invalidState);
  }
  
  if (_renderSystem)
  {
    _renderSystem->UnInitialize();
    
    delete _renderSystem;
    _renderSystem = NULL;
  }
  
  if (_eventQueue)
  {
    al_destroy_event_queue(_eventQueue);
    _eventQueue = NULL;
  }
}

void Application::Execute()
{
  std::cout << "Execute" << std::endl;  

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
  if (_entityPool.empty())
  {
    throw std::runtime_error("no more entities available");
  }
  
  Entity* entity = _entityPool.back();
  entity->SetScript(script);
  entity->Initialize();
  
  _entityPool.pop_back();
  _spawnedEntities.push_back(entity);
  
  std::map<std::string, Entity*>::const_iterator name_iter = _entityNamesMap.find(entity->GetName());
  if (name_iter != _entityNamesMap.end())
  {
    throw std::runtime_error((boost::format("entity name '%s' (type: '%s')is already registered") % entity->GetName() % entity->GetType()).str());
  }
  
  _entityNamesMap[entity->GetName()] = entity;

  std::map<std::string, std::set<Entity*>>::iterator i = _entityTypesMap.find(entity->GetType());
  if (i == _entityTypesMap.end())
  {
    _entityTypesMap[entity->GetType()] = std::set<Entity*>();
    _entityTypesMap[entity->GetType()].insert(entity);
    //.push_back(entity);
  }
  else
  {
    (*i).second.insert(entity);
  }
  
  return entity;
}

Entity* Application::GetEntityById(int entityId)
{
  std::map<int, Entity*>::const_iterator i = _entityMap.find(entityId);  
  
  Entity* entity = nullptr;
  if (i != _entityMap.end())
  {
    entity = (*i).second;
  }
 
  return entity;
}

Entity* Application::GetEntityByName(const std::string& name)
{
  std::map<std::string, Entity*>::const_iterator i = _entityNamesMap.find(name);  
  
  Entity* entity = nullptr;
  if (i != _entityNamesMap.end())
  {
    entity = (*i).second;
  }
  
  return entity;
}

const std::set<Entity*>& Application::GetEntitiesByType(const std::string& type)
{
  static std::set<Entity*> emptyResult;
  std::map<std::string, std::set<Entity*>>::const_iterator i = _entityTypesMap.find(type);
  if (i != _entityTypesMap.end())
  {
    return (*i).second;
  }
  return emptyResult;
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

  if (!_spawnedEntities.empty())
  {
    std::copy(_spawnedEntities.begin(), _spawnedEntities.end(), std::back_inserter(_entities));
    BOOST_FOREACH(Entity* entity, _spawnedEntities)
    {
      _entityMap[entity->GetId()] = entity;
    }
    
    _spawnedEntities.clear();
  }
  
  luabind::call_function<void>(_applicationScript["update"], _applicationScript, this, 1.0);
  
  BOOST_FOREACH(Entity* entity, _entities)
  {
    entity->Update(1.0);
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
 
  luabind::call_function<void>(_applicationScript["render"], _applicationScript, this, _renderSystem);
  
  /*
  BOOST_FOREACH(Entity* entity, _entities)
  {
    if (entity->IsActive())
    {
      entity->Render(_renderSystem);
    }
  }

  _renderSystem->DrawText(10, 10, Color(255, 32, 32), (boost::format("entity count: %d") % _entities.size()).str());

  for(int i = 0; i < ButtonMax; i++)
  {
    _renderSystem->DrawText(10, 22 + (i * 12), Color(255, 32, 32, 0.5f), (boost::format("%s") % (IsButtonPressed((Buttons)i) ? "x" : "-")).str());
  }
  */
  
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
  auto i = _entities.begin();
  while (i != _entities.end())
  {
    Entity* entity = *i; 
    if (!entity->IsActive())
    {
      _entityPool.push_back(entity);
      
      _entityMap.erase(entity->GetId());
      
      _entityNamesMap.erase(entity->GetName());
      
      std::map<std::string, std::set<Entity*>>::iterator i2 = _entityTypesMap.find(entity->GetType());
      if (i2 != _entityTypesMap.end())
      {
        //(*i2).second.erase(std::find((*i2).second.begin(), (*i2).second.end(), entity));
        (*i2).second.erase(entity);
      }    
      else
      {
        throw std::runtime_error((boost::format("type '%s' not found in entity types map") % entity->GetType()).str());
      }
      
      i = _entities.erase(i);
    }
    else
    { 
      i++;
    }
  }
}

void Application::HandleCollisions()
{
  for (size_t i = 0; i < _entities.size(); i++)
  {
    for (size_t j = i + 1; j < _entities.size(); j++)
    {
      Entity* left = _entities[i];
      Entity* right = _entities[j];
      
      if (left->IsCollidable() && right->IsCollidable() && left->HasCollidedWith(right))
      {
        left->HandleCollisionWith(right);
      }
    }
  }
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

void Application::RegisterWithLua(lua_State* L)
{
  luabind::module(L)
  [
    luabind::class_<Application>("application")
      .def("spawn_entity", &Application::SpawnEntity)
      .def("get_entity_by_id", &Application::GetEntityById)
      .def("get_entity_by_name", &Application::GetEntityByName)
      .def("get_entities_by_type", &Application::GetEntitiesByType, luabind::return_stl_iterator)
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
  ];    
}
