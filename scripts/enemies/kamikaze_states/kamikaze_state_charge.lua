function create_kamikaze_state_charge()

local kamikaze_state_charge = entity_state_base:new('kamikaze_state_charge')
  
  function kamikaze_state_charge:render(kamikaze, state_data, gfx)
    local p = kamikaze.application.viewport_transformation * kamikaze.position
    
    if (gfx:is_in_viewport(p)) then
      gfx:draw_filled_circle(p, kamikaze.bounding_radius, color(0, 0, 192))
      gfx:draw_filled_circle(p, kamikaze.bounding_radius - 2, color(0, 0, 96))
      
      gfx:draw_line(p, p + kamikaze.heading * 15, color(64, 64, 192), 2)
    end
  end  
  
  function kamikaze_state_charge:enter_impl(kamikaze, state_data)
    kamikaze.max_speed = 4.0
	
    state_data.current_tick = 0
    state_data.max_tick = 60 * 1.0
    
    local player = kamikaze.application:get_entity_by_name("player")        
    kamikaze.script.steering_behaviors.seek:set_target(player)
  end

  function kamikaze_state_charge:exit_impl(kamikaze, state_data)
	  kamikaze.script.steering_behaviors.seek:set_target(nil)
	end

  function kamikaze_state_charge:execute_impl(kamikaze, state_data)
    local force = kamikaze.script.steering_behaviors:calculate(kamikaze)
    
    local xf = 1.0 / state_data.max_tick * state_data.current_tick	
    kamikaze.velocity = (kamikaze.velocity + force):truncate(kamikaze.max_speed - (2.0 * xf))   
    kamikaze.position = kamikaze.position + kamikaze.velocity
    kamikaze.heading = kamikaze.velocity:normal()
    
    state_data.current_tick = state_data.current_tick + 1
    if (state_data.current_tick > state_data.max_tick) then
      kamikaze:change_current_state(kamikaze_state_idle)
      return
    end	
  end
  
  --[[
  local state_name = 'state_charge'
  local f_enter = function(entity)	
	entity.script[state_name] = {}
  end
  
  local f_exit = function(entity)
	entity.script[state_name] = nil
  end
  
  local f = function(entity)
	return entity.script[state_name]
  end
  
  function kamikaze_state_charge:enter(kamikaze)

	
    kamikaze.script.render = function (self, kamikaze, gfx)

    end
  end

  function kamikaze_state_charge:exit(kamikaze)
	f_exit(kamikaze)
	
	kamikaze.steering_behaviors.script.seek:set_target(nil)
    
	kamikaze.script.render = nil
  end

  function kamikaze_state_charge:execute(kamikaze)

  end
  ]]
  return kamikaze_state_charge
end

kamikaze_state_charge = create_kamikaze_state_charge()
