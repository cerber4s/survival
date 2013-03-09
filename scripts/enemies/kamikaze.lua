require "scripts/entity_state_global"
require "scripts/entity_state_destroy"

require "scripts/enemies/kamikaze_states/kamikaze_state_charge"
require "scripts/enemies/kamikaze_states/kamikaze_state_destroy"
require "scripts/enemies/kamikaze_states/kamikaze_state_idle"
require "scripts/enemies/kamikaze_states/kamikaze_state_move"

require "scripts/steering_behaviors"

local activate_distance = 600
local view_distance = 900

Kamikaze = Entity:new { 
  type = "kamikaze",
  
  max_speed = 0.0,
  
  is_collidable = true,
  bounding_radius = 10,
  
  script = {
    is_destroyed = false,
    
    steering_behaviors = SteeringBehaviors:new {
      arrive = SbArriveBehavior:new {},
    },
    
    activate_distance = activate_distance,
    activate_distance_sqr = activate_distance * activate_distance,
    
    view_distance = view_distance,
    view_distance_sqr = view_distance * view_distance,
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = kamikaze_state_idle,
}

function Kamikaze:handle_collision_with (kamikaze, other)
  if (other:is_of_type(Player)) then
    kamikaze:change_current_state(kamikaze_state_destroy) --kamikaze.is_active = false
    
    other.script.current_ttl = math.max(0, other.script.current_ttl - (60.0 * 15))
    return
  end
  
  if (other:is_of_type(Bullet)) then
    
	
	if (other.script.player_flag) then
    kamikaze:change_current_state(kamikaze_state_destroy)
    
    local player = kamikaze.application:get_entity_by_name("player")
	  player.script.current_ttl = math.min(player.script.current_ttl + (60.0 * 15), player.script.max_ttl)    
	end
  
    --kamikaze:change_current_state(entity_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
