require "scripts/entity_state_global"
require "scripts/bosses/hunter_state_attack"

require "scripts/steering_behaviors"

Hunter = Entity:new { 
  type = "hunter",
  is_destroyed = false,
}

function Hunter:initialize (hunter)
  Entity:initialize(hunter)
  
  hunter.name = string.format("hunter%d", hunter.id)
  hunter.type = self.type
  hunter.is_active = true
  
  hunter.max_speed = 0.2
  
  hunter.is_collidable = true
  hunter.bounding_radius = 20
  
  hunter.steering_behaviors.script = SteeringBehaviors:new()
  
  hunter:change_global_state(entity_state_global)
  hunter:change_current_state(hunter_state_attack)
end

function Hunter:handle_collision_with (hunter, other)
  if (other:is_of_type(Bullet)) then
    --bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
