require "scripts/entity_state_global"
require "scripts/bosses/sphere_state_attack"

require "scripts/steering_behaviors"

Sphere = Entity:new { 
  type = "sphere",
  is_destroyed = false,
}

function Sphere:initialize (sphere)
  Entity:initialize(sphere)
  
  sphere.name = string.format("sphere%d", sphere.id)
  sphere.type = self.type
  sphere.is_active = true
  
  sphere.max_speed = 0.0
  
  sphere.is_collidable = true
  sphere.bounding_radius = 20  
  
  sphere:change_global_state(entity_state_global)
  sphere:change_current_state(sphere_state_attack)
end

function Sphere:handle_collision_with (sphere, other)  
  if (other:is_of_type(Bullet)) then
    --bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
