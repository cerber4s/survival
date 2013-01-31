require "scripts/entity_state_global"
require "scripts/entity_state_idle"
require "scripts/entity_state_destroy"

Entity = { 
  type = "entity",
  is_destroyed = false,
}

function Entity:new (o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function Entity:initialize (entity)
  entity.name = string.format("entity-%d", entity.id)
  entity.type = self.type
  entity.is_active = true
  entity.is_collidable = true
  
  entity:change_global_state(entity_state_global)
  entity:change_current_state(entity_state_idle)
end

function Entity:render (entity, gfx)
  local p = entity.application.viewport_transformation * entity.position
  
  if (gfx:is_in_viewport(p)) then
  
    if (entity.script.is_destroyed) then
      local alpha = (1.0 / entity.script.initial_ttl) * entity.script.ttl
   
      for index, point in pairs(entity.script.points) do
        local p2 = p + point.position
        local spread = (entity.bounding_radius * 0.3) + math.exp(alpha);
        for i = 1, 1 do
          local d = v2(-1 + math.random() * 2, -1 + math.random() * 2):normal()
          local n = (d * spread)
          local t = 0.5 + (1.5 * alpha)
          local c1 = color(255 * point.dr, (64 - (64 * alpha)) * point.dg, (128 - (128 * alpha)) * point.db, alpha)
          local c2 = color(192 * point.dr, (32 - (32 * alpha)) * point.dg, (64 - (64 * alpha)) * point.db, 0.5 * alpha)
          
          gfx:draw_line(p2, p2 + n, c1, t)
          gfx:draw_line(p2 + n, p2 + (d * (spread + 3)), c2, t)
          gfx:draw_filled_circle(p2, spread * 0.5, c2)
        end
      end
      return
    end
    
    gfx:draw_circle(p, entity.bounding_radius, color(192, 0, 0), 1)
    gfx:draw_text_centre(p + v2(0, -8), color(192, 192, 192), string.format("%d", entity.id))
  end
end

function Entity:handle_collision_with (entity, other)
  if (other:is_of_type(Bullet)) then
    --entity.is_active = false
    entity:change_current_state(entity_state_destroy)
    other:change_current_state(bullet_state_destroy)
  end
end
