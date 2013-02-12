function create_tank_state_seek()
  local tank_state_seek = entity_state_base:new('tank_state_seek')
    
  function tank_state_seek:render(tank, state_data, gfx)
    local p = tank.application.viewport_transformation * tank.position      
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, tank.bounding_radius, color(128, 128, 128))
      gfx:draw_filled_circle(p, tank.bounding_radius - 2, color(32, 32, 32))
      
      gfx:draw_text_centre(p, color(255, 255, 255), string.format("%s: seek [%d]", tank.name, state_data.current_tick + 1))
    end 
  end
  
  function tank_state_seek:enter_impl(tank, state_data)
    state_data.current_tick = 0.0
  end
  
  function tank_state_seek:exit_impl(tank, state_data)
  end
  
  function tank_state_seek:execute_impl(tank, state_data)
    state_data.current_tick = state_data.current_tick + (1.0 / 60)
    if (state_data.current_tick > 4) then
      tank:change_current_state(tank_state_pursuit)
      return
    end
  end
  
  return tank_state_seek
end

tank_state_seek = create_tank_state_seek()
