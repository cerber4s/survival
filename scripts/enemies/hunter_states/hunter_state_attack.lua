function create_hunter_state_attack()  
  local hunter_state_attack = entity_state_base:new('hunter_state_attack')
  
  function hunter_state_attack:render(hunter, state_data, gfx)
    local p = hunter.application.viewport_transformation * hunter.position
    
    if (gfx:is_in_viewport(p)) then      
      local t = m3.create_rotation(((math.pi * 2.0) / 8.0) + (hunter.heading:angle() * -1))
      local points_ex = { 
          p + t * v2(-hunter.bounding_radius, -hunter.bounding_radius),  
          p + t * v2( hunter.bounding_radius, -hunter.bounding_radius),  
          p + t * v2( hunter.bounding_radius,  hunter.bounding_radius),  
          p + t * v2(-hunter.bounding_radius,  hunter.bounding_radius),  
      }
      
      local previous_index = table.getn(points_ex)
      for current_index, point in ipairs(points_ex) do
          gfx:draw_line(points_ex[previous_index], point, color(192, 0, 0), 1)
          previous_index = current_index            
      end
      
      gfx:draw_circle(p, hunter.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_circle(p, hunter.bounding_radius - 3, color(192, 0, 0), 1)
    end
  end    
  
  function hunter_state_attack:enter_impl(hunter, state_data)
    local shots_per_second = 60
    
    hunter.script.shot_count = hunter.script.shot_count or 0
    hunter.script.rotate_direction = hunter.script.rotate_direction or 1
    
    hunter.script.shot_count = hunter.script.shot_count + 1
    if (hunter.script.shot_count == 4) then
      hunter.script.rotate_direction = hunter.script.rotate_direction * -1
      hunter.script.shot_count = 0
    end
        
    state_data.current_charge = 60.0 * 0.25
    state_data.fire_delay = 0
    state_data.fire_charge_time =  60.0 / shots_per_second
    state_data.ticks_per_rotation = 60.0 * 5.0
  end

  function hunter_state_attack:execute_impl(hunter, state_data)
    if (state_data.current_charge > 0) then
      state_data.current_charge = state_data.current_charge - 1
    else
      hunter:change_current_state(hunter_state_charge)
      return
    end
  
    local t = m3.create_rotation(((math.pi * 2.0) / state_data.ticks_per_rotation) * hunter.script.rotate_direction)
    hunter.heading = t * hunter.heading
    
    if (state_data.fire_delay > 0) then
      state_data.fire_delay = state_data.fire_delay - 1
    else
      local bullet_range = 500.0
      local bullet_initial_ttl = 60.0 * 1.5
      local bullet_spawn_distance = hunter.bounding_radius + 16
      
      for index = 0, 3 do
        local current_heading = m3.create_rotation((math.pi * 2) / 4 * index) * hunter.heading
      
        local bullet = hunter.application:spawn_entity(Bullet:new())
        bullet.position = hunter.position + (current_heading * bullet_spawn_distance)
        bullet.velocity = current_heading:normal() * (1.0 / bullet_initial_ttl * (bullet_range - bullet_spawn_distance))
        bullet.heading = current_heading
      
        bullet.script.initial_ttl = bullet_initial_ttl
        bullet.script.ttl = bullet_initial_ttl
      end
      
      state_data.fire_delay = state_data.fire_charge_time
    end 
    
    local player = hunter.application:get_entity_by_name("player")
    hunter.script.steering_behaviors.arrive:set_target(player)
    
    local force = hunter.script.steering_behaviors:calculate(hunter)
    
    hunter.velocity = (hunter.velocity + force):truncate(hunter.max_speed)
    hunter.position = hunter.position + hunter.velocity    
    
  end

  return hunter_state_attack
end

hunter_state_attack = create_hunter_state_attack()