require "scripts/kamikaze_state_move"

function create_kamikaze_state_idle()
  local kamikaze_state_idle = {}

  function kamikaze_state_idle:enter(kamikaze)
    kamikaze.script.render = function (self, kamikaze, gfx)
      local p = kamikaze.application.viewport_transformation * kamikaze.position
      
      if (gfx:is_in_viewport(p)) then
        gfx:draw_filled_circle(p, kamikaze.bounding_radius, color(192, 0, 0))
        gfx:draw_filled_circle(p, kamikaze.bounding_radius - 2, color(96, 0, 0))  
      end
    end
  end

  function kamikaze_state_idle:exit(kamikaze)
    kamikaze.script.render = nil
  end

  function kamikaze_state_idle:execute(kamikaze)
    local player = kamikaze.application:get_entity_by_name("player")    
    local delta = (player.position - kamikaze.position)
    local distance_to = delta:length_sqr()
    
    if (distance_to < kamikaze.script.view_distance_sqr) then
      kamikaze.heading = delta:normal()
    else
      kamikaze.heading = v2()
    end
    
    if (distance_to < kamikaze.script.activate_distance_sqr) then
      kamikaze:change_current_state(kamikaze_state_move)
    end
  end

  return kamikaze_state_idle
end

kamikaze_state_idle = create_kamikaze_state_idle()