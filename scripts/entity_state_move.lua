entity_state_move = {}

function entity_state_move:enter (entity)
end

function entity_state_move:exit (entity)
end

function entity_state_move:execute (entity)
  --[[
  entity.position = entity.position + entity.velocity 
  
  entity.position.x = entity.position.x % 800
  entity.position.y = entity.position.y % 600  
  ]]
end

