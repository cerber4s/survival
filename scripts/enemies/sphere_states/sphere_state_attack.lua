function create_sphere_state_attack()  
  local sphere_state_attack = entity_state_base:new('sphere_state_attack')
  
  function sphere_state_attack:render(sphere, state_data, gfx)
    local p = sphere.application.viewport_transformation * sphere.position      
    if (gfx:is_in_viewport(p)) then
    
      gfx:draw_filled_circle(p, sphere.bounding_radius, color(255, 32, 32, 0.1))
      gfx:draw_circle(p, sphere.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_circle(p, sphere.bounding_radius - 3, color(192, 0, 0), 1)
      
      for index = 0, 3 do
        gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * index) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
      end
    end
  end
  
  function sphere_state_attack:enter_impl(sphere, state_data)
    local shots_per_second = 30
    
    sphere.script.test_flag = sphere.script.test_flag or false
    sphere.script.test_flag = not sphere.script.test_flag
    
    state_data.fire_delay = 0
    state_data.fire_charge_time =  60.0 / shots_per_second
    state_data.fire_count = 6
  end
  
  function sphere_state_attack:exit_impl(sphere, state_data)
  end
  
  function sphere_state_attack:execute_impl(sphere, state_data)
    if (state_data.fire_delay > 0) then
      state_data.fire_delay = state_data.fire_delay - 1
      return
    end
    
    local temp = 1.0
    if (sphere.script.test_flag) then
      temp = 0.5
    end
    
    local bullet_range = 200.0 * temp
    local bullet_initial_ttl = 60.0 * (0.5 * temp)
    local bullet_spawn_distance = sphere.bounding_radius + 15.0
    
    for index = 0, 7 do
      local current_heading = m3.create_rotation(((math.pi * 2) / 8) * index) * sphere.heading
    
      local bullet = sphere.application:spawn_entity(Bullet:new())
      bullet.position = sphere.position + (current_heading * bullet_spawn_distance)
      bullet.velocity = current_heading:normal() * (1.0 / bullet_initial_ttl * (bullet_range - bullet_spawn_distance))
      bullet.heading = current_heading
    
      bullet.script.initial_ttl = bullet_initial_ttl
      bullet.script.ttl = bullet_initial_ttl
    end
    
    state_data.fire_delay = state_data.fire_charge_time
    
    if (state_data.fire_count > 0) then
      state_data.fire_count = state_data.fire_count - 1
    else
      sphere:change_current_state(sphere_state_rotate)
      return
    end
  end

  return sphere_state_attack
end

sphere_state_attack = create_sphere_state_attack()