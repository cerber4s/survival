function create_tank_state_pursuit()
  local tank_state_pursuit = entity_state_base:new('tank_state_pursuit')

  function tank_state_pursuit:render(tank, data, gfx)
    local p = tank.application.viewport_transformation * tank.position      
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, tank.bounding_radius, color(128, 32, 128))
      gfx:draw_filled_circle(p, tank.bounding_radius - 2, color(32, 128, 32))
      
      gfx:draw_text_centre(p, color(255, 255, 255), string.format("pursuit"))
    end  
  end
  
  function tank_state_pursuit:execute_impl(tank, data, gfx)    
  end
  
  return tank_state_pursuit
end

tank_state_pursuit = create_tank_state_pursuit()
