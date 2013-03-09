require "scripts/entity_state_global"

require "scripts/enemies/creator_states/creator_state_default"

Creator = Entity:new { 
  type = "creator",
  
  max_speed = 0.0,
  
  is_collidable = true,
  bounding_radius = 10,
  
  script = {  
    spawn_distance = 100,
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = creator_state_default,  
}

function Creator:handle_collision_with (creator, other)
  if (other:is_of_type(Bullet)) then
    other:change_current_state(bullet_state_destroy)	
  end
end
