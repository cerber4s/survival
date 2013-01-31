function create_kamikaze_state_move()
  local kamikaze_state_move = {}

  function kamikaze_state_move:enter(kamikaze)
    kamikaze.script.render = function (self, kamikaze, gfx)
      local p = kamikaze.application.viewport_transformation * kamikaze.position
      
      if (gfx:is_in_viewport(p)) then
        
        gfx:draw_filled_circle(p, kamikaze.script.view_distance, color(128, 128, 128, 0.05))
        gfx:draw_filled_circle(p, kamikaze.script.view_distance - 2, color(32, 32, 32, 0.05))
        
        gfx:draw_filled_circle(p, kamikaze.bounding_radius, color(192, 0, 0))
        gfx:draw_filled_circle(p, kamikaze.bounding_radius - 2, color(96, 0, 0))
        
        gfx:draw_line(p, p + kamikaze.heading * 15, color(64, 192, 64), 2)
      end
    end
  end

  function kamikaze_state_move:exit(kamikaze)
    kamikaze.script.render = nil
  end

  function kamikaze_state_move:execute(kamikaze)
    local player = kamikaze.application:get_entity_by_name("player")    
    
    kamikaze.steering_behaviors.script.arrive:set_target(player)
    
    local force = kamikaze.steering_behaviors:calculate()

    kamikaze.heading = force:normal()
    kamikaze.velocity = force:normal() * 0.3
    kamikaze.position = kamikaze.position + kamikaze.velocity
    
    local delta = (player.position - kamikaze.position)
    local distance_to = delta:length_sqr()
    
    if (distance_to >= kamikaze.script.view_distance_sqr) then
      kamikaze:change_current_state(kamikaze_state_idle)
    end
  end

  return kamikaze_state_move
end
kamikaze_state_move = create_kamikaze_state_move()