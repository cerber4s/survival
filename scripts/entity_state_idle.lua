require "scripts/entity_state_move"

entity_state_idle = {}

entity_info = {}

function entity_state_idle:enter (entity)
  --entity_info[entity.id] = {}
  --entity_info[entity.id].ttl = 100 + math.random(150)
  
  --[[
  entity.active = true
  entity.position = v2(math.random(800), math.random(600))
  entity.velocity = v2()
  entity.heading = v2(0, -1)
  entity.bounding_radius = 10 + math.random(10)
  ]]
end

function entity_state_idle:exit (entity)
  entity_info[entity.id] = nil
end

function entity_state_idle:execute (entity)
  --entity_info[entity.id].ttl = entity_info[entity.id].ttl - 1
  
  --if (entity_info[entity.id].ttl <= 0) then
  --  entity:change_current_state(entity_state_move)
  --end
  entity:change_current_state(entity_state_move)
end
