require "scripts/entity_state_global"

require "scripts/enemies/hunter_states/hunter_state_attack"
require "scripts/enemies/hunter_states/hunter_state_charge"

require "scripts/steering_behaviors"

Hunter = Entity:new {
  type = "hunter",
  
  max_speed = 0.2,
  
  is_collidable = true,
  bounding_radius = 20,
  
  script = {
    is_destroyed = false,
    steering_behaviors = SteeringBehaviors:new(),
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = hunter_state_attack,
}

function Hunter:handle_collision_with (hunter, other)
  if (other:is_of_type(Bullet)) then
    --bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
    return
  end
end
