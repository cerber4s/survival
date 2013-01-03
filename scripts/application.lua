require "scripts/entity"
require "scripts/player"
require "scripts/bullet"
require "scripts/crosshair"
require "scripts/radar"

require "scripts/starfield"

function create_application()
  local application = {}
  
  local player = nil
  local starfield = Starfield:new()
  
  function application:initialize (context)
    local crosshair = context:spawn_entity(Crosshair:new {}) 
    
    local radar = context:spawn_entity(Radar:new {})
    radar.position = v2(800 - 100, 600 - 100)
    
    player = context:spawn_entity(Player:new { 
      crosshair_id = crosshair.id
    })
    
    local entity_spawn_radius = 1200
    
    for i = 1, 12 do
      entity = context:spawn_entity(Entity:new {})
      entity.position = v2(
        -entity_spawn_radius + math.random() * (entity_spawn_radius * 2), 
        -entity_spawn_radius + math.random() * (entity_spawn_radius * 2))
      
      local x = -1 + (math.random() * 2)
      local y = -1 + (math.random() * 2)
      entity.velocity = v2(x, y)
      entity.heading = v2(0, -1)
      entity.bounding_radius = 10 + math.random(10)
    end
    
    starfield:initialize()    
  end

  
  
  function application:update(context, delta_time)
    starfield:update(context, delta_time)
  end
  
  function application:render (context, gfx)
    starfield:render(context, gfx)
    
    if (not (player == nil)) then
      for i = 1, 24 do
        local p = v2(math.random(800), math.random(600))
        local f = 1.0 / 3.0 * player.velocity:length();
        local delta = (player.velocity:normal() * (2.0 + (22.0 * f)))
        gfx:draw_line(p, p + delta, color(64, 64, 96, (0.2 * f)), 2.0 * f)
      end
    end
    
    for entity in context.entities do
      entity:render(gfx)
    end
  end
  
  return application
end

application = create_application()
