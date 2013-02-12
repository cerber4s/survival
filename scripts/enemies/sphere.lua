require "scripts/entity_state_global"

require "scripts/enemies/sphere_states/sphere_state_attack"
require "scripts/enemies/sphere_states/sphere_state_rotate"

require "scripts/steering_behaviors"

Sphere = Entity:new { 
  type = "sphere",
  
  max_speed = 0.0,
  
  is_collidable = true,
  bounding_radius = 20, 
  
  script = {
    is_destroyed = false,
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = sphere_state_attack,    
}

function Sphere:handle_collision_with (sphere, other)  
  if (other:is_of_type(Bullet)) then    
    other:change_current_state(bullet_state_destroy)
  end
end
