require "scripts/entity_state_global"
require "scripts/crosshair_state_default"
require "scripts/entity"

Crosshair = Entity:new {
  initial_entity_type = 3,
}

function Crosshair:initialize (crosshair)
  crosshair.entity_type = self.initial_entity_type
  crosshair.is_active = true
  crosshair.is_collidable = false
  
  crosshair:change_global_state(entity_state_global)
  crosshair:change_current_state(crosshair_state_default)
end

function Crosshair:render (crosshair, gfx)
  local p = crosshair.application.viewport_transformation * crosshair.position
  local size = 15
  local c = color(192, 192, 192, 0.25)
  local c_dark = color(32, 32, 32, 0.75)
  
  gfx:draw_circle(p, 11, c, 3)
  gfx:draw_circle(p, 12, c_dark, 1)
  gfx:draw_circle(p, 9, c_dark, 1)
  
  gfx:draw_circle(p, 7, c, 1)
  
  gfx:draw_line(p + v2(-size, 0), p + v2(size, 0), c, 1)
  gfx:draw_line(p + v2(0, -size), p + v2(0, size), c, 1)
end

function Crosshair:handle_collision_with (crosshair, other)
end
