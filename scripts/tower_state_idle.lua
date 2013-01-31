require "scripts/tower_state_warning"

function create_tower_state_idle()
  local tower_state_idle = {}

  function tower_state_idle:enter(tower)
    tower.script.render = function (self, tower, gfx)
      local p = tower.application.viewport_transformation * tower.position

      if (gfx:is_in_viewport(p)) then
        gfx:draw_filled_circle(p, tower.script.kamikaze_spawn_radius, color(128, 128, 128, 0.05))
        gfx:draw_filled_circle(p, tower.script.kamikaze_spawn_radius - 2, color(32, 32, 32, 0.05))
        
        gfx:draw_filled_circle(p, tower.bounding_radius, color(128, 128, 128))
        gfx:draw_filled_circle(p, tower.bounding_radius - 2, color(32, 32, 32))
      end
    end
  end
    
  function tower_state_idle:exit(tower)
    --tower.script.idle_state = nil
  end

  function tower_state_idle:execute(tower)
    local player = tower.application:get_entity_by_name("player")
    local delta = player.position - tower.position
    
    if (delta:length_sqr() < tower.script.kamikaze_spawn_radius_sqr) then
      tower:change_current_state(tower_state_warning)
    end
    
    --[[
    local trigger_distance_sqr = 200 * 200
    local distance_sqr = delta:length_sqr()
    if (distance_sqr << trigger_distance_sqr) then
      player.velocity = player.velocity + (delta:normal() * (0.0 + (0.3 * (math.max(1.0, trigger_distance_sqr / distance_sqr)))))
    end
    ]]
    
    --[[
    if (tower.script.idle_state.spawn_timeout > 0) then
      tower.script.idle_state.spawn_timeout = tower.script.idle_state.spawn_timeout - 1
    else
      
      if (delta:length_sqr() < tower.script.kamikaze_spawn_radius_sqr) then
        local kamikaze = tower.application:spawn_entity(Kamikaze:new {})
        kamikaze.position = tower.position + (v2(-1 + (math.random() * 2), -1 + (math.random() * 2)):normal() * (math.random() * tower.script.kamikaze_spawn_radius))
        
        kamikaze.velocity = v2()
        kamikaze.heading = v2()
        kamikaze.bounding_radius = 10
        
        tower.script.idle_state.spawn_timeout = 50
      end
    end
    ]]
  end
  
  return tower_state_idle
end

tower_state_idle = create_tower_state_idle()