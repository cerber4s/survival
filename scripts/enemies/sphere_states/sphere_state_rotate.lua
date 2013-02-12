function create_sphere_state_rotate()
  local sphere_state_rotate = entity_state_base:new('sphere_state_rotate')
  
  function sphere_state_rotate:render(sphere, state_data, gfx)
    local p = sphere.application.viewport_transformation * sphere.position      
    if (gfx:is_in_viewport(p)) then
    
      gfx:draw_filled_circle(p, sphere.bounding_radius, color(32, 32, 32, 0.1))
      gfx:draw_circle(p, sphere.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_circle(p, sphere.bounding_radius - 3, color(192, 0, 0), 1)
      
      gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 0) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 1) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 2) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 3) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
    end
  end
  
  function sphere_state_rotate:enter_impl(sphere, state_data)
    state_data.ttl = 60.0 * 0.25
    state_data.angle_in_degrees = 45.0
  end
  
  function sphere_state_rotate:exit_impl(sphere, state_data)
  end
  
  function sphere_state_rotate:execute_impl(sphere, state_data)
    if (state_data.ttl > 0) then
      state_data.ttl = state_data.ttl - 1
    else
      sphere:change_current_state(sphere_state_attack)
      return
    end
  
    local f = 1.0 / 360.0 * (state_data.angle_in_degrees / 60.0)
    local t = m3.create_rotation((math.pi * 2.0) * f)
    sphere.heading = t * sphere.heading    
  end  
  
  return sphere_state_rotate
end

sphere_state_rotate = create_sphere_state_rotate()