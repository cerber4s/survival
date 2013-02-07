function create_tower_state_warning()
  local tower_state_warning = {}

  function tower_state_warning:enter(tower)
    tower.script.state_warning = {
      tick = 0,
      max_tick = 120,
    }
    
    tower.script.render = function (self, tower, gfx)
      local p = tower.application.viewport_transformation * tower.position

      if (gfx:is_in_viewport(p)) then
        local alpha = (1.0 / tower.script.state_warning.max_tick) * tower.script.state_warning.tick
        
        gfx:draw_filled_circle(p, tower.bounding_radius, color(128 + (128 * alpha), 128 + (-128 * alpha), 128 + (-128 * alpha)))
        gfx:draw_filled_circle(p, tower.bounding_radius - 2, color(32 + (128 * alpha), 32 + (-24 * alpha), 32 + (-24 * alpha)))
      end
    end
  end
    
  function tower_state_warning:exit(tower)
    tower.script.state_warning = nil
    tower.script.render = nil
  end

  function tower_state_warning:execute(tower)
    local player = tower.application:get_entity_by_name("player")
    local delta = player.position - tower.position
    
    if (delta:length_sqr() > tower.script.kamikaze_spawn_radius_sqr) then
      tower.script.state_warning.tick = tower.script.state_warning.tick - 1
    else
      tower.script.state_warning.tick = tower.script.state_warning.tick + 1
    end
    
    if (tower.script.state_warning.tick <= 0) then      
      tower:change_current_state(tower_state_idle)
      return
    end
    
    if (tower.script.state_warning.tick > tower.script.state_warning.max_tick) then
      local kamikaze = tower.application:spawn_entity(Kamikaze:new {})

      local spawn_direction = v2(-1 + (math.random() * 2), -1 + (math.random() * 2)):normal()
      local spawn_position = (spawn_direction * math.random(tower.bounding_radius, tower.script.kamikaze_spawn_radius))
      kamikaze.position = tower.position + spawn_position
      kamikaze.velocity = v2()
      kamikaze.heading = v2()
      kamikaze.bounding_radius = 10
      
      tower.script.state_warning.tick = 0      
    end
  end
  
  return tower_state_warning
end

tower_state_warning = create_tower_state_warning()