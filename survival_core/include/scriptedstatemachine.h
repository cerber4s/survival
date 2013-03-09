#ifndef SCRIPTEDSTATEMACHINE_H
#define SCRIPTEDSTATEMACHINE_H

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>

#include <iostream>
#include <exception>

template <class context_type>
class ScriptedStateMachine
{
public:
  ScriptedStateMachine(context_type* context);
  virtual ~ScriptedStateMachine();
  
  const luabind::object& GetGlobalState() const;
  void SetGlobalState(const luabind::object& state);
  
  const luabind::object& GetCurrentState() const;
  void SetCurrentState(const luabind::object& state);
  
  void ChangeGlobalState(const luabind::object& state);
  void ChangeCurrentState(const luabind::object& state);
  
  void Execute();
  
private:
  luabind::object _globalState;
  luabind::object _currentState;
  context_type* _context;
  
  void SafeCall(const std::string& stateName, const luabind::object& stateObject, const std::string& functionName);

};

template <class context_type>
ScriptedStateMachine<context_type>::ScriptedStateMachine(context_type* context) 
: _context(context) 
{ 
}

template <class context_type>
ScriptedStateMachine<context_type>::~ScriptedStateMachine()
{
  auto x = GetGlobalState();
  ChangeGlobalState(x);
  _globalState.swap(luabind::object());
  _currentState.swap(luabind::object());
  //ChangeGlobalState();
  //ChangeCurrentState(luabind::object());
}

template <class context_type>
const luabind::object& ScriptedStateMachine<context_type>::GetGlobalState() const
{
  return _globalState;
}

template <class context_type>
void ScriptedStateMachine<context_type>::SetGlobalState(const luabind::object& state)
{
  _globalState = state;
}


template <class context_type>
const luabind::object& ScriptedStateMachine<context_type>::GetCurrentState() const
{
  return _currentState;
}

template <class context_type>
void ScriptedStateMachine<context_type>::SetCurrentState(const luabind::object& state)
{
  _currentState = state;
}

template <class context_type>
void ScriptedStateMachine<context_type>::ChangeGlobalState(const luabind::object& state)
{
  SafeCall("global", _globalState, "exit");
  
  _globalState = state;
  
  SafeCall("global", _globalState, "enter");
}

template <class context_type>
void ScriptedStateMachine<context_type>::ChangeCurrentState(const luabind::object& state)
{
  SafeCall("current", _currentState, "exit");
  
  _currentState = state;
  
  SafeCall("current", _currentState, "enter");
}

template <class context_type>
void ScriptedStateMachine<context_type>::Execute()
{
  SafeCall("global", _globalState, "execute");
  SafeCall("current", _currentState, "execute");
}

template <class context_type>
void ScriptedStateMachine<context_type>::SafeCall(const std::string& stateName, const luabind::object& stateObject, const std::string& functionName)
{
  try
  {
    if (stateObject.is_valid())
    {
      luabind::call_function<void>(stateObject[functionName], stateObject, _context);
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "error in function call to 'scriptedstatemachine:" << functionName.c_str() << "' (" << stateName.c_str() << "): " << e.what() << std::endl;
    std::cerr << "entity id:" << _context->GetId() << std::endl;
    std::cerr << "entity type:" << _context->GetType() << std::endl;
    std::cerr << "entity name:" << _context->GetName() << std::endl;
    throw;
  }
}

#endif //SCRIPTEDSTATEMACHINE_H