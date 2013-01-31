require "scripts/entity_state_global"
require "scripts/tower_state_idle"

Tower = Entity:new { 
  type = "tower",
  is_destroyed = false,
}

function Tower:initialize (tower)
  Entity:initialize(tower)
  
  tower.name = string.format("tower-%d", tower.id)
  tower.type = self.type
  tower.is_active = true
  
  tower.max_speed = 0.0
  
  tower.is_collidable = true
  tower.bounding_radius = 30
  
  tower.script.kamikaze_spawn_radius = 300
  tower.script.kamikaze_spawn_radius_sqr = 300 * 300
  
  tower:change_global_state(entity_state_global)
  tower:change_current_state(tower_state_idle)
end

function Tower:handle_collision_with (tower, other)
  if (other:is_of_type(Bullet)) then
    --bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
