function create_entity_state_global()
  local entity_state_global = {}

  function entity_state_global:enter (entity)
  end

  function entity_state_global:exit (entity)
  end

  function entity_state_global:execute (entity)
  end
  
  return entity_state_global
end

entity_state_global = create_entity_state_global()
