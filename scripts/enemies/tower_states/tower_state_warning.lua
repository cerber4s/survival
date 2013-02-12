function create_tower_state_warning()
  local tower_state_warning = entity_state_base:new('tower_state_warning')
  
  function tower_state_warning:render(tower, state_data, gfx)
    local p = tower.application.viewport_transformation * tower.position
    if (gfx:is_in_viewport(p)) then
      local alpha = (1.0 / state_data.max_tick) * state_data.tick
      
      gfx:draw_filled_circle(p, tower.bounding_radius, color(128 + (128 * alpha), 128 + (-128 * alpha), 128 + (-128 * alpha)))
      gfx:draw_filled_circle(p, tower.bounding_radius - 2, color(32 + (128 * alpha), 32 + (-24 * alpha), 32 + (-24 * alpha)))
    end
  end
    
  function tower_state_warning:enter_impl(tower, state_data)
    state_data.tick = 0
    state_data.max_tick = 60
  end

  function tower_state_warning:execute_impl(tower, state_data)
    local player = tower.application:get_entity_by_name("player")
    local delta = player.position - tower.position
    
    if (delta:length_sqr() > tower.script.kamikaze_spawn_radius_sqr) then
      state_data.tick = state_data.tick - 1
    else
      state_data.tick = state_data.tick + 1
    end
    
    if (state_data.tick <= 0) then      
      tower:change_current_state(tower_state_idle)
      return
    end
    
    if (state_data.tick > state_data.max_tick) then
      local kamikaze = tower.application:spawn_entity(Kamikaze:new {})

      local spawn_direction = v2(-1 + (math.random() * 2), -1 + (math.random() * 2)):normal()
      local spawn_position = (spawn_direction * math.random(tower.bounding_radius, tower.script.kamikaze_spawn_radius))
      kamikaze.position = tower.position + spawn_position
      kamikaze.velocity = v2()
      kamikaze.heading = v2()
      kamikaze.bounding_radius = 10
      
      state_data.tick = 0      
    end
  end
  
  return tower_state_warning
end

tower_state_warning = create_tower_state_warning()