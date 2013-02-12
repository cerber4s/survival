function create_hunter_state_charge()
  local hunter_state_charge = entity_state_base:new('hunter_state_charge')
  
  function hunter_state_charge:render(hunter, state_data, gfx)
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
          gfx:draw_line(points_ex[previous_index], point, color(0, 192, 0), 1)
          previous_index = current_index            
      end
      
      gfx:draw_circle(p, hunter.bounding_radius, color(0, 192, 0), 1)
      gfx:draw_circle(p, hunter.bounding_radius - 3, color(192, 0, 0), 1)
    end
  end    
  
  function hunter_state_charge:enter_impl(hunter, state_data)
    state_data.current_charge = 0
    state_data.max_charge = 60.0 * 0.5
  end

  function hunter_state_charge:execute_impl(hunter, state_data)
    if (state_data.current_charge >= state_data.max_charge) then
      hunter:change_current_state(hunter_state_attack)
      return
    end
    
    state_data.current_charge = state_data.current_charge + 1    
    
    local player = hunter.application:get_entity_by_name("player")
    hunter.script.steering_behaviors.arrive:set_target(player)
    
    local force = hunter.script.steering_behaviors:calculate(hunter)
    
    hunter.velocity = (hunter.velocity + force):truncate(hunter.max_speed)
    hunter.position = hunter.position + hunter.velocity
  end

  return hunter_state_charge
end

hunter_state_charge = create_hunter_state_charge()