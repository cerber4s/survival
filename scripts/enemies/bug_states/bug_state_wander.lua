function create_bug_state_wander()
  local bug_state_wander = entity_state_base:new('bug_state_wander')
  
  function bug_state_wander:render(bug, state_data, gfx)
    local p = bug.application.viewport_transformation * bug.position
    if (gfx:is_in_viewport(p)) then
      local rotation = m3.create_rotation(bug.heading:angle())
      
      gfx:draw_line(p, p + (bug.heading * bug.bounding_radius), color(32, 32, 32), 2)
      
      local size = bug.bounding_radius
      local points = {
        rotation * v2(-size, -size),
        rotation * v2(size, 0),
        rotation * v2(-size, size),
      }

      local prev_i = 1
      for current_i = 1, #points do
        gfx:draw_line(p + points[prev_i], p + points[current_i], bug.script.color, 1)
        prev_i = current_i
      end
      
      --[[
      gfx:draw_text_centre(p + v2(0, 10), color(224, 224, 224), string.format("%.3g", bug.heading:angle()))
      gfx:draw_text_centre(p + v2(0, 30), color(224, 224, 224), string.format("%.3g", bug.heading:angle_in_degrees()))
      --]]
    end
  end    
  
  function bug_state_wander:enter_impl(bug, state_data)
  end

  function bug_state_wander:execute_impl(bug, state_data)    
    local player = bug.application:get_entity_by_name("player")
    --bug.heading = (player.position - bug.position):normal()
    
    local time_elapsed = 1.0 --/ 60.0 -- TODO...
    
    local steering_force = bug.script.steering_behaviors:calculate(bug)
    local acceleration = steering_force / bug.mass
      
    bug.velocity = bug.velocity + (acceleration * time_elapsed)
    bug.velocity = bug.velocity * 0.98
    bug.velocity = bug.velocity:truncate(bug.max_speed)
    
    bug.position = bug.position + (bug.velocity * time_elapsed)
    
    if (bug.velocity:length_sqr() > 0.00001) then
      bug.heading = bug.velocity:normal()
    end
  end

  return bug_state_wander
end

bug_state_wander = create_bug_state_wander()