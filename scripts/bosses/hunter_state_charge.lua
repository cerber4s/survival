function create_hunter_state_charge()
  local hunter_state_charge = {}

  function hunter_state_charge:enter(hunter)    
    hunter.script.state_charge = {
        current_charge = 0,
        max_charge = 60.0 * 0.5,
    }
  
    hunter.script.render = function (self, hunter, gfx)
    
      local p = hunter.application.viewport_transformation * hunter.position
      if (gfx:is_in_viewport(p)) then
        
        --local t = m3.create_rotation((math.pi * 2.0) / 8.0)
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
  end

  function hunter_state_charge:exit(hunter)
    hunter.script.state_charge = nil
    hunter.script.render = nil
  end

  function hunter_state_charge:execute(hunter)
    if (hunter.script.state_charge.current_charge >= hunter.script.state_charge.max_charge) then
      hunter:change_current_state(hunter_state_attack)
      return
    end
    
    hunter.script.state_charge.current_charge = hunter.script.state_charge.current_charge + 1    
    
    local player = hunter.application:get_entity_by_name("player")
    hunter.steering_behaviors.script.arrive:set_target(player)
    
    local force = hunter.steering_behaviors:calculate()
    
    hunter.velocity = (hunter.velocity + force):truncate(hunter.max_speed)
    hunter.position = hunter.position + hunter.velocity
    
  end

  return hunter_state_charge
end

hunter_state_charge = create_hunter_state_charge()