function create_bug_state_wander()
  local bug_state_wander = entity_state_base:new('bug_state_wander')
  
  function bug_state_wander:render(bug, state_data, gfx)
    local p = bug.application.viewport_transformation * bug.position
    
    local rotation = m3.create_rotation(bug.heading:angle())
    
    --gfx:draw_line(p, p + (bug.heading * bug.bounding_radius), color(32, 32, 32), 2)
    
    local size = bug.bounding_radius
    local points = {
      rotation * v2(-size, -size),
      rotation * v2(size, 0),
      rotation * v2(-size, size),
    }

    local prev_i = 1
    for current_i = 2, #points do
      gfx:draw_line(p + points[prev_i], p + points[current_i], bug.script.color, 1)
      prev_i = current_i
    end

    --[[
    gfx:draw_text_centre(p + v2(0, 10), color(224, 224, 224), string.format("%.3g", bug.heading:angle()))
    gfx:draw_text_centre(p + v2(0, 30), color(224, 224, 224), string.format("%.3g", bug.heading:angle_in_degrees()))
    --]]
  end    
  
  function bug_state_wander:enter_impl(bug, state_data)
    state_data.neighbor_range = 40
  end

  function bug_state_wander:execute_impl(bug, state_data)    
    local player = bug.application:get_entity_by_name("player")
    
    local neighbors = {}    
    
    local entities_in_range = bug.application:get_entities_in_range(bug.position, state_data.neighbor_range)
    for neighbor in entities_in_range.entities do
      if (neighbor.type == "bug" and neighbor.id ~= bug.id) then
        table.insert(neighbors, neighbor)
      end
    end   
    
    bug.script.steering_behaviors.separation:set_neighbors(neighbors)
    
    local time_elapsed = 1.0 --/ 60.0 -- TODO...
    
    local steering_force = bug.script.steering_behaviors:calculate(bug)
    local acceleration = steering_force / bug.mass
      
    bug.velocity = bug.velocity + (acceleration * time_elapsed)
    bug.velocity = bug.velocity:truncate(bug.max_speed)
    
    bug.position = bug.position + (bug.velocity * time_elapsed)
    
    if (bug.velocity:length_sqr() > 0.00001) then
      bug.heading = bug.velocity:normal()
    end
  end

  return bug_state_wander
end

bug_state_wander = create_bug_state_wander()