require "scripts/entity_state_global"
require "scripts/entity_state_destroy"

require "scripts/enemies/tank_states/tank_state_seek"
require "scripts/enemies/tank_states/tank_state_pursuit"
require "scripts/enemies/tank_states/tank_state_attack"

require "scripts/steering_behaviors"

Tank = Entity:new {
  type = "tank",
  max_speed = 0.0,
  
  bounding_radius = 10,
  
  script = {
    steering_behaviors = SteeringBehaviors:new {},
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = tank_state_seek,  
}

function Tank:handle_collision_with (tank, other)
  if (other:is_of_type(Bullet)) then    
    other:change_current_state(bullet_state_destroy)
    return
  end
end
