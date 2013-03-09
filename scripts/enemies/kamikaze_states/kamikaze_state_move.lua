function create_kamikaze_state_move()
  local kamikaze_state_move = entity_state_base:new('kamikaze_state_move')
  
  function kamikaze_state_move:render(kamikaze, state_data, gfx)
    local p = kamikaze.application.viewport_transformation * kamikaze.position
    
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, kamikaze.bounding_radius, color(192, 0, 0))
      gfx:draw_filled_circle(p, kamikaze.bounding_radius - 2, color(96, 0, 0))
      
      gfx:draw_line(p, p + kamikaze.heading * 15, color(64, 192, 64), 2)
    end
  end  
  
  function kamikaze_state_move:enter_impl(kamikaze, state_data)
    kamikaze.max_speed = 1.0
    
    state_data.current_tick = 0
    state_data.max_tick = 60 * 2.0
    
    local player = kamikaze.application:get_entity_by_name("player")
    --kamikaze.script.steering_behaviors.arrive:set_target(player)
  end

  function kamikaze_state_move:exit_impl(kamikaze, state_data)
	  --kamikaze.script.steering_behaviors.arrive:set_target(nil)
	end

  function kamikaze_state_move:execute_impl(kamikaze, state_data)
    --[[
    local force = kamikaze.script.steering_behaviors:calculate(kamikaze)
    kamikaze.velocity = (kamikaze.velocity + force):truncate(kamikaze.max_speed)   
    kamikaze.position = kamikaze.position + kamikaze.velocity
    kamikaze.heading = kamikaze.velocity:normal()
    
    local player = kamikaze.application:get_entity_by_name("player")   
    local delta = (player.position - kamikaze.position)
    local distance_to = delta:length_sqr()
    
    if (distance_to >= kamikaze.script.view_distance_sqr) then
      kamikaze:change_current_state(kamikaze_state_idle)
      return
    end
	
    state_data.current_tick = state_data.current_tick + 1
    if (state_data.current_tick > state_data.max_tick) then
      kamikaze:change_current_state(kamikaze_state_charge)
      return
    end
    ]]
  end

  return kamikaze_state_move
end
kamikaze_state_move = create_kamikaze_state_move()