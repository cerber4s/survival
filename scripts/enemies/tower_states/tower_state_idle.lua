function create_tower_state_idle()
  local tower_state_idle = entity_state_base:new('tower_state_idle')

  function tower_state_idle:render(tower, state_data, gfx)
    local p = tower.application.viewport_transformation * tower.position
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, tower.bounding_radius, color(128, 128, 128))
      gfx:draw_filled_circle(p, tower.bounding_radius - 2, color(32, 32, 32))
    end
  end
  
  function tower_state_idle:execute_impl(tower, state_data)
    local player = tower.application:get_entity_by_name("player")
    local delta = player.position - tower.position
    
    if (delta:length_sqr() < tower.script.kamikaze_spawn_radius_sqr) then
      tower:change_current_state(tower_state_warning)
      return
    end
  end
  
  return tower_state_idle
end

tower_state_idle = create_tower_state_idle()
