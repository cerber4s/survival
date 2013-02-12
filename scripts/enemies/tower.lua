require "scripts/entity_state_global"

require "scripts/enemies/tower_states/tower_state_idle"
require "scripts/enemies/tower_states/tower_state_warning"

Tower = Entity:new { 
  type = "tower",
  
  max_speed = 0.0,
  
  is_collidable = true,
  bounding_radius = 30,
  
  script = {
    kamikaze_spawn_radius = 600,
    kamikaze_spawn_radius_sqr = 600 * 600,
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = tower_state_idle,  
}

function Tower:handle_collision_with (tower, other)
  if (other:is_of_type(Bullet)) then
    other:change_current_state(bullet_state_destroy)	
  end
end
