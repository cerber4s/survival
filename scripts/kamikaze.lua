require "scripts/entity_state_global"
require "scripts/kamikaze_state_idle"
require "scripts/entity_state_destroy"

require "scripts/steering_behaviors"

Kamikaze = Entity:new { 
  type = "kamikaze",
  is_destroyed = false,
}

function Kamikaze:initialize (kamikaze)
  Entity:initialize(kamikaze)
  
  kamikaze.name = string.format("kamikaze%d", kamikaze.id)
  kamikaze.type = self.type
  kamikaze.is_active = true
  
  kamikaze.max_speed = 1.0
  
  kamikaze.is_collidable = true
  kamikaze.bounding_radius = 10
  
  kamikaze.steering_behaviors.script = SteeringBehaviors:new()
  --local temp = kamikaze.steering_behaviors.script.seek:calculate(kamikaze)
  
  local activate_distance = 150
  kamikaze.script.activate_distance = activate_distance
  kamikaze.script.activate_distance_sqr = activate_distance * activate_distance
  
  local view_distance = 300
  kamikaze.script.view_distance = view_distance
  kamikaze.script.view_distance_sqr = view_distance * view_distance
  
  kamikaze:change_global_state(entity_state_global)
  kamikaze:change_current_state(kamikaze_state_idle)
end

function Kamikaze:handle_collision_with (kamikaze, other)
  if (other:is_of_type(Player)) then
    kamikaze.is_active = false
    
    other.script.current_ttl = math.max(0, other.script.current_ttl - (60.0 * 15))
    return
  end
  
  if (other:is_of_type(Bullet)) then
    kamikaze.is_active = false
    
    local player = kamikaze.application:get_entity_by_name("player")
    player.script.current_ttl = math.min(player.script.current_ttl + (60.0 * 15), player.script.max_ttl)
    
    --kamikaze:change_current_state(entity_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
