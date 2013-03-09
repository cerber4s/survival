require "scripts/entity_state_global"
require "scripts/bullet_state_default"
require "scripts/bullet_state_destroy"

Bullet = Entity:new {  
  type = "bullet",

  is_collidable = true,
  bounding_radius = 2,
  
  script = {
    initial_ttl = 60,
    ttl = 60,
    destroy_on_impact = true,
    player_flag = false,
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = bullet_state_default,
}

function Bullet:handle_collision_with (bullet, other)
  
  if (other:is_of_type(Bullet)) then
    --[[
    bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
    --]]
    return
  end
  
  other:handle_collision_with(bullet)
end

function Bullet:request_destroy_after_collision(bullet) -- h4ck...
  if (bullet.script.destroy_on_impact) then
    bullet:change_current_state(bullet_state_destroy)
  end
end
