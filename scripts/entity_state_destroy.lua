function create_entity_state_destroy()  
  local entity_state_destroy = {}
  
  function entity_state_destroy:enter (entity)
    entity.is_collidable = false
    entity.script.is_destroyed = true
    entity.script.initial_ttl = 40
    entity.script.ttl = 40
    
    entity.script.points = {}
    --[[
    for i = 1, 4 do
      entity.script.points[i] = 
      {
        position = v2(
          -entity.bounding_radius + (math.random() * (entity.bounding_radius * 2.0)), 
          -entity.bounding_radius + (math.random() * (entity.bounding_radius * 2.0))),
        dr = math.random() * 0.50,
        dg = math.random() * 0.25,
        db = math.random() * 0.75,
      }
    end
    ]]
  end

  function entity_state_destroy:exit (entity)
  end

  function entity_state_destroy:execute (entity)
    entity.script.ttl = entity.script.ttl - 1
    if (entity.script.ttl == 0) then
      entity.is_active = false
    end
  end
  
  return entity_state_destroy
end

entity_state_destroy = create_entity_state_destroy()
