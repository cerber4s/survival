function create_tank_state_attack()
  local tank_state_attack = entity_state_base:new('tank_state_attack')

  function tank_state_attack:render(tank, data, gfx)
    local p = tank.application.viewport_transformation * tank.position      
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, tank.bounding_radius, color(128, 128, 32))
      gfx:draw_filled_circle(p, tank.bounding_radius - 2, color(32, 32, 128))
      
      gfx:draw_text_centre(p, color(255, 255, 255), string.format("attack"))
    end
  end
  
  return tank_state_attack
end

tank_state_attack = create_tank_state_attack()
