require "scripts/entity_state_global"
require "scripts/entity_state_destroy"

require "scripts/enemies/bug_states/bug_state_wander"

require "scripts/steering_behaviors"

Bug = Entity:new {
  type = "bug",
  
  position = v2(),
  velocity = v2(),
  heading = v2(),
  
  mass = 1.0,
  
  max_turn_rate = (math.pi * 2.0),
  max_force = 0.05,
  max_speed = 1.0,
  
  bounding_radius = 10,
  
  script = {
    color = color(255, 0, 0),
    steering_behaviors = SteeringBehaviors:new {},
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = bug_state_wander,  
}

function Bug:handle_collision_with (bug, other)  
  if (other:is_of_type(Bullet)) then    
    other:change_current_state(bullet_state_destroy)
    return
  end
end
