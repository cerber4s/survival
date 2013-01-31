function create_player_state_default()
  local player_state_default = {}

  function player_state_default:enter (player)
    player.script.state_default = {
      fire_delay = 0,
    }
      
    player.script.render = function (self, player, gfx)
      local p = player.application.viewport_transformation * player.position
      
      gfx:draw_filled_circle(p, player.bounding_radius, color(32, 192, 32))
      gfx:draw_filled_circle(p, player.bounding_radius - 2, color(32, 96, 32))
      --gfx:draw_circle(p, player.bounding_radius - 1, color(32, 192, 32), 1)
      --gfx:draw_circle(p, player.bounding_radius - 1, color(32, 192, 32), 1)
      
      gfx:draw_circle(p, player.bounding_radius - 4, color(32, 192, 32), 2)
      
      gfx:draw_line(p, p + player.heading * 15, color(64, 192, 64), 2)
    end
  end

  function player_state_default:exit (player)
    player.script.state_default = nil
    player.script.render = nil
  end

  function player_state_default:execute (player)
    local acceleration = 0.05
    
    if (player.application.is_button_up_pressed) then
      player.velocity = player.velocity + v2(0, -acceleration)
    end
    if (player.application.is_button_down_pressed) then
      player.velocity = player.velocity + v2(0, acceleration)
    end
    if (player.application.is_button_left_pressed) then
      player.velocity = player.velocity + v2(-acceleration, 0)
    end
    if (player.application.is_button_right_pressed) then
      player.velocity = player.velocity + v2(acceleration, 0)
    end
    
    if (player.script.state_default.fire_delay > 0) then
      player.script.state_default.fire_delay = player.script.state_default.fire_delay - 1
    end
    
    local bullet_speed = 5.0
    
    local fire = (player.application.is_button_primary_pressed or player.application.is_left_mouse_button_pressed)
    if (player.script.state_default.fire_delay == 0 and fire) then
      bullet = player.application:spawn_entity(Bullet:new())
      bullet.position = player.position + player.heading * 15
      bullet.velocity = player.heading * bullet_speed
      bullet.heading = player.heading
      
      player.script.state_default.fire_delay = 4
    end

    player.velocity = player.velocity:truncate(player.max_speed)
    
    player.position = player.position + player.velocity 
    
    -- set viewport
    player.application.viewport_translate = player.position - v2(400, 300)
    
    local crosshair = player.application:get_entity_by_name("crosshair")
    player.heading = (crosshair.position - player.position):normal()
  end
  
  return player_state_default
end

player_state_default = create_player_state_default()