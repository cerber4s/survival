require "scripts/entity_state_global"
require "scripts/crosshair_state_default"
require "scripts/entity"

Crosshair = Entity:new {
  type = "crosshair",
}

function Crosshair:initialize (crosshair)
  Entity.initialize(self, crosshair, {
    name = "crosshair",
    
    is_collidable = false,
    bounding_radius = 0,
    
    initial_global_state = entity_state_global,
    initial_current_state = crosshair_state_default,
  })
end

function Crosshair:handle_collision_with (crosshair, other)
end
