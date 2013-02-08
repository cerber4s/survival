#ifndef SCRIPTEDSTATEMACHINE_H
#define SCRIPTEDSTATEMACHINE_H

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>

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
  
};

template <class context_type>
ScriptedStateMachine<context_type>::ScriptedStateMachine(context_type* context) 
: _context(context) 
{ 
}

template <class context_type>
ScriptedStateMachine<context_type>::~ScriptedStateMachine()
{
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
  if (_globalState.is_valid())
  {
    luabind::call_function<void>(_globalState["exit"], _globalState, _context);
  }
  
  _globalState = state;
  
  if (_globalState.is_valid())
  {
    luabind::call_function<void>(_globalState["enter"], _globalState, _context);
  }
}

template <class context_type>
void ScriptedStateMachine<context_type>::ChangeCurrentState(const luabind::object& state)
{
  if (_currentState.is_valid())
  {
    luabind::call_function<void>(_currentState["exit"], _currentState, _context);
  }
  
  _currentState = state;
  
  if (_currentState.is_valid())
  {
    luabind::call_function<void>(_currentState["enter"], _currentState, _context);
  }
}

template <class context_type>
void ScriptedStateMachine<context_type>::Execute()
{
  if (_globalState.is_valid())
  {
    luabind::call_function<void>(_globalState["exit"], _globalState, _context);
  }
  
  if (_currentState.is_valid())
  {
    luabind::call_function<void>(_currentState["execute"], _currentState, _context);
  }
}

#endif //SCRIPTEDSTATEMACHINE_H