require "scripts/entity"
require "scripts/bullet"
require "scripts/crosshair"
require "scripts/kamikaze"
require "scripts/player"
require "scripts/radar"
require "scripts/tower"

require "scripts/bosses/hunter"
require "scripts/bosses/sphere"

require "scripts/starfield"

function create_application()
  local application = {}
  
  local starfield = Starfield:new {}
  
  function application:initialize (context)
    
    local crosshair = context:spawn_entity(Crosshair:new {}) 
    
    local radar = context:spawn_entity(Radar:new {})
    radar.position = v2(800 - 100, 600 - 100)
    
    local player = context:spawn_entity(Player:new {})
   
--[[   
    local entity_spawn_radius = 500    
    local kamikaze_count = 4
    
    for i = 1, kamikaze_count do
      local kamikaze = context:spawn_entity(Kamikaze:new {})
      kamikaze.position = v2(
        -entity_spawn_radius + math.random() * (entity_spawn_radius * 2), 
        -entity_spawn_radius + math.random() * (entity_spawn_radius * 2))
      
      kamikaze.velocity = v2(-1 + (math.random() * 2), -1 + (math.random() * 2))
      kamikaze.heading = v2(0, -1)
      kamikaze.bounding_radius = 10
    end
    ]]
    local tower_position = 400
    local tower_positions = {
      v2(-tower_position, -tower_position),
      v2( tower_position, -tower_position),
      v2(-tower_position,  tower_position),
      v2( tower_position,  tower_position),
    }
      
    for i, v in ipairs(tower_positions) do       
      local tower = context:spawn_entity(Tower:new {})
      tower.position = v
    end
    
    local hunter = context:spawn_entity(Hunter:new {})
    hunter.position = v2(-300, 0)
    hunter.heading = (player.position - hunter.position):normal()
        
    local sphere = context:spawn_entity(Sphere:new {})
    sphere.position = v2(300, 0)
    sphere.heading = v2(0, -1)
    
    starfield:initialize()    
  end

  function application:update(context, delta_time)
    starfield:update(context, delta_time)
  end
  
  function application:render (context, gfx)
    starfield:render(context, gfx)
    
    local t = context.viewport_translate * -1
    local offset = v2(math.mod(t.x, 800 / 8), math.mod(t.y, 600 / 6))
    for i = 0, 7 do
      local x = offset.x + ((800 / 8) * i)
      gfx:draw_line(x, 0, x, 600, color(64, 64, 64, 0.05), 1.0)
    end
    
    for i = 0, 5 do
      local y = offset.y + ((600 / 6) * i)
      gfx:draw_line(0, y, 800, y, color(64, 64, 64, 0.05), 1.0)
    end
    
    --[[
    if (not (player == nil)) then
      for i = 1, 24 do
        local p = v2(math.random(800), math.random(600))
        local f = 1.0 / 3.0 * player.velocity:length();
        local delta = (player.velocity:normal() * (2.0 + (22.0 * f)))
        gfx:draw_line(p, p + delta, color(64, 64, 96, (0.2 * f)), 2.0 * f)
      end
    end
    ]]
    
    for entity in context:get_entities_by_type("tower") do
      entity:render(gfx)
    end

    for entity in context:get_entities_by_type("kamikaze") do
      entity:render(gfx)
    end
    
    for entity in context:get_entities_by_type("hunter") do
      entity:render(gfx)
    end
    
    for entity in context:get_entities_by_type("sphere") do
      entity:render(gfx)
    end
    
    local player = context:get_entity_by_name("player")
    if not (player == nil) then
      player:render(gfx)
    end
    
    for entity in context:get_entities_by_type("bullet") do
      entity:render(gfx)
    end
    
    local radar = context:get_entity_by_name("radar")
    if not (radar == nil) then
      radar:render(gfx)
    end
    
    local crosshair = context:get_entity_by_name("crosshair")
    if not (crosshair == nil) then
      crosshair:render(gfx)
    end
    
    local count = 0
    for entity in context.entities do
      count = count + 1
    end
    
    if (count == 0) then
      gfx:draw_text(400, 300, color(255, 32, 32), "game over")
    end
    
	gfx:draw_text(10, 10, color(255, 32, 32), string.format("%d", count))
  end
  
  return application
end

application = create_application()
