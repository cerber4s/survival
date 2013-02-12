require "scripts/entity_state_global"
require "scripts/radar_state_default"
require "scripts/entity"

Radar = Entity:new {
  type = "radar",
}

function Radar:initialize (radar)
  Entity.initialize(self, radar, {
    name = "radar",
    
    is_collidable = false,
    bounding_radius = 0,
    
    initial_global_state = entity_state_global,
    initial_current_state = radar_state_default,
  })
end

function Radar:handle_collision_with (crosshair, other)
end
