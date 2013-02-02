require "scripts/entity_state_global"
require "scripts/bullet_state_default"
require "scripts/bullet_state_destroy"

Bullet = Entity:new {
  type = "bullet",
  initial_ttl = 60,
  ttl = 60,
  is_destroyed = false,
}

function Bullet:initialize (bullet)
  Entity:initialize(bullet)
  
  bullet.name = string.format("bullet-%d", bullet.id)
  bullet.type = self.type
  bullet.is_active = true
  bullet.is_collidable = true
  bullet.bounding_radius = 2
  
  bullet:change_global_state(entity_state_global)
  bullet:change_current_state(bullet_state_default)
end

function Bullet:render (bullet, gfx)
  local p = bullet.application.viewport_transformation * bullet.position
  if (gfx:is_in_viewport(p)) then
    local alpha = (1.0 / bullet.script.initial_ttl) * bullet.script.ttl
    
    if (bullet.script.is_destroyed) then
      local spread = 1 + math.exp(alpha);
      for i = 1, 4 do
        local d = v2(-1 + math.random() * 2, -1 + math.random() * 2):normal()
        local n = (d * spread)
        local t = 0.5 + (1.5 * alpha)
        gfx:draw_line(p, p + n, color(255, 0 + (32 * alpha), 92 - (64 * alpha), alpha), t)
        gfx:draw_line(p + n, p + (d * (spread + 3)), color(192, 0 + (32 * alpha), 92 - (64 * alpha), 0.5 * alpha), t)
      end
      return
    end
    
    gfx:draw_line(p, p + (bullet.heading * -5), color(224, 128, 0, 0.25 + (0.75 * alpha)), 1)
    
    gfx:draw_filled_circle(p, bullet.bounding_radius, color(224, 128, 0, alpha))
    gfx:draw_circle(p, bullet.bounding_radius, color(192, 64, 0, 0.75 + (0.25 * alpha)), 1)
  end
end

function Bullet:handle_collision_with (bullet, other)
  if (other:is_of_type(Player)) then
    other:handle_collision_with(bullet)
  end
  
  if (other:is_of_type(Entity)) then
    other:handle_collision_with(bullet)
  end
  
  if (other:is_of_type(Bullet)) then
    bullet:change_current_state(bullet_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end