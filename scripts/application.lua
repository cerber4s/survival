require "scripts/entity"
require "scripts/entity_state_base"

require "scripts/bullet"
require "scripts/crosshair"
require "scripts/player"
require "scripts/radar"

require "scripts/enemies/bug"
require "scripts/enemies/creator"
require "scripts/enemies/hunter"
require "scripts/enemies/kamikaze"
require "scripts/enemies/sphere"
require "scripts/enemies/tank"
require "scripts/enemies/tower"

require "scripts/starfield"

function create_application()
  local application = {}
  
  local starfield = Starfield:new {}
  
  local display_size = v2(1280, 800)
  
  function application:initialize (context)
    
    local crosshair = context:spawn_entity(Crosshair:new {}) 
    
    local radar = context:spawn_entity(Radar:new {})
    -- todo read from gfx system
    radar.position = v2(display_size.x - 100, display_size.y - 100)

    local player = context:spawn_entity(Player:new {})
    
    --[[
    local tower_grid_size = 2
    local tower_grid_distance = 800
    local start_x = -((tower_grid_size / 4) * tower_grid_distance)
    local start_y =	-((tower_grid_size / 4) * tower_grid_distance)
    
    for ix = 1, tower_grid_size do
      for iy = 1, tower_grid_size do
        local tower = context:spawn_entity(Tower:new {})
          tower.position = v2(start_x + (tower_grid_distance * (ix - 1)), start_y + (tower_grid_distance * (iy - 1)))
      end
    end
    --]]
    
    --[[
    local hunter = context:spawn_entity(Hunter:new {})
    hunter.position = v2(-300, 0)
    hunter.heading = (player.position - hunter.position):normal()
    --]]
    
    --[[
    local sphere = context:spawn_entity(Sphere:new {
      position = v2(300, 0),
      heading = v2(0, -1),
    })
    --]]
    
    --[[
    local kamikaze = context:spawn_entity(Kamikaze:new {
      position = v2(0, -500),
    })
    --]]
    
    --[[
    local tank = context:spawn_entity( Tank:new {
      position = v2(-300, -300),
      name = "kevin",
      --xyz = true,
    })
    --]]
    
    --[[
    local bug = context:spawn_entity(Bug:new {
      position = v2(-400, -400),
      script = {
        steering_behaviors = SteeringBehaviors:new {
          flee = SbFleeBehavior:new {},
        },
      },      
    })  
    --]]
    
    --[[
    local other_bug = context:spawn_entity(Bug:new {
      position = v2(400, -400),
      script = {
        color = color(0, 255, 0),
        steering_behaviors = SteeringBehaviors:new {
          seek = SbSeekBehavior:new {},
        },
      },
    })
    --]]
    
    --bug.script.steering_behaviors.flee:set_target(other_bug)  
    --other_bug.script.steering_behaviors.seek:set_target(player)    
    
    --[-[
    context:spawn_entity( Creator:new {
      position = v2(-400, -400),
    })
    context:spawn_entity( Creator:new {
      position = v2(400, -400),
    })
    context:spawn_entity( Creator:new {
      position = v2(-400, 400),
    })
    context:spawn_entity( Creator:new {
      position = v2(400, 400),
    })
    --]]
    
    starfield:initialize()
  end

  function application:update(context, delta_time)
    starfield:update(context, delta_time)
  end
  
  local draw_grid = function(context, gfx)
    local grid = {
      color = color(64, 64, 64, 0.05),
      thickness = 1.0,
      width = 8,
      height = 6,
    }
  
    local t = context.viewport_translate * -1
    local offset = v2(math.mod(t.x, display_size.x / grid.width), math.mod(t.y,  display_size.y / grid.height))
    
    for i = 0, grid.width - 1 do
      local x = offset.x + (( display_size.x / grid.width) * i)
      gfx:draw_line(x, 0, x,  display_size.y, grid.color, grid.thickness)
    end
    
    for i = 0, grid.height - 1 do
      local y = offset.y + (( display_size.y / grid.height) * i)
      gfx:draw_line(0, y,  display_size.x, y, grid.color, grid.thickness)
    end  
  end
  
  local draw_grid2 = function(context, gfx)
    local grid = {
      color = color(128, 64, 64, 0.6),
      thickness = 1.0,
      width = 4,
      height = 4,
    }
  
    local t = context.viewport_translate * -1
    local offset = v2(math.mod(t.x, display_size.x / grid.width), math.mod(t.y,  display_size.y / grid.height))
    
    for i = 0, 8 do
      local left = 1280 * 0.5 - 400
      gfx:draw_line(left + (i * 100), 0, left + (i * 100),  800, grid.color, grid.thickness)
    end
    
    for i = 0, 8 do
      local top = 800 * 0.5 - 400
      gfx:draw_line(0, top + (i * 100),  1280, top + (i * 100), grid.color, grid.thickness)
    end  
  end
  
  function application:render (context, gfx)
    draw_grid(context, gfx)
    --draw_grid2(context, gfx)
    
    starfield:render(context, gfx)
    
    local render_types = {
      "creator",
      "tower",
      "tank",
      "kamikaze",
      "hunter",
      "sphere",
      "bug",
    }
    
    for _, render_type in ipairs(render_types) do
      for entity in context:get_entities_by_type(render_type) do
        if (entity.is_visible) then
          entity:render(gfx)
        end
      end
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
      gfx:draw_text_centre(display_size.x * 0.5, display_size.y * 0.5, color(255, 32, 32), "game over")
    end
    
    --gfx:draw_text(10, 10, color(255, 32, 32), string.format("%d", count))
  end
  
  return application
end

application = create_application()
