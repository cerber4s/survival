function create_sphere_state_rotate()
  local sphere_state_rotate = {}

  function sphere_state_rotate:enter(sphere)
    sphere.script.state_rotate = {
        --ticks_per_rotation = 60.0 * 2.0, --* 5.0,
        ttl = 60.0 * 0.5,
        angle_in_degrees = 45.0 * 0.5,
    }
  
    sphere.script.render = function (self, sphere, gfx)
      local p = sphere.application.viewport_transformation * sphere.position      
      if (gfx:is_in_viewport(p)) then
        gfx:draw_circle(p, sphere.bounding_radius, color(192, 0, 0), 1)
        gfx:draw_circle(p, sphere.bounding_radius - 3, color(192, 0, 0), 1)
        
        gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 0) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
        gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 1) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
        gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 2) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
        gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * 3) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
      end
    end    
  end

  function sphere_state_rotate:exit(sphere)
    sphere.script.state_rotate = nil
    sphere.script.render = nil
  end

  function sphere_state_rotate:execute(sphere)
    if (sphere.script.state_rotate.ttl > 0) then
      sphere.script.state_rotate.ttl = sphere.script.state_rotate.ttl - 1
    else
      sphere:change_current_state(sphere_state_attack)
      return
    end
  
    local f = 1.0 / 360.0 * (sphere.script.state_rotate.angle_in_degrees / 60.0)
    local t = m3.create_rotation((math.pi * 2.0) * f)
    sphere.heading = t * sphere.heading
  end

  return sphere_state_rotate
end

sphere_state_rotate = create_sphere_state_rotate()