Starfield = {}

function Starfield:new (o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function Starfield:initialize()
  self.stars = {}
  
  local star_count = 512
  local star_spawn_radius = 1200
  for i = 1, star_count do
    
    local alpha = math.random()
    local depth = 0.9 + (alpha * 0.1)
    local max_pulse = 120 + math.random(720)
    
    self.stars[i] = { 
      position = v2(
        -star_spawn_radius + (math.random() * (star_spawn_radius * 2)), 
        -star_spawn_radius + (math.random() * (star_spawn_radius * 2))),
      alpha = 0.05 + (math.random() * 0.05) + (alpha * 0.05),
      size = 0.2 + (depth * (2 + (math.random() * 2))),
      depth = depth,
      max_pulse = max_pulse,
      current_pulse = math.random(max_pulse),
    }
  end
end

function Starfield:update(context, delta_time)
  for index, star in pairs(self.stars) do
    star.current_pulse = (star.current_pulse + 1) % star.max_pulse
  end
end

function Starfield:render(context, gfx)
  for index, star in pairs(self.stars) do
    local t = m3.create_translation(context.viewport_translate * -2.0 * (1.0 - star.depth))
    local p = t * star.position
    
    if (gfx:is_in_viewport(p)) then
      local alpha = (1.0 / star.max_pulse) * star.current_pulse
      local y = math.sin(math.pi * alpha)
      
      local c_inner = 192 - ((64 + (64 * alpha)) * y)
      local c_outer = 124 - ((32 + (64 * alpha)) * y)
      
      gfx:draw_filled_circle(p, star.size, color(c_inner, c_inner, c_inner, star.alpha))
      gfx:draw_circle(p, star.size, color(c_outer, c_outer, c_outer, star.alpha), 1)
    end
  end
end
