function create_player_state_default()
  local player_state_default = entity_state_base:new('player_state_default')
  
  function player_state_default:enter (player)
    
    player.script.state_default = {
      fire_delay = 0,
      fire_timeout = 3,
    }
	  	  
    player.script.render = function (self, player, gfx)
      local p = player.application.viewport_transformation * player.position
      
      local border_color = color(0, 192, 0)

      local factor = (1.0 / player.script.max_ttl * math.min(player.script.current_ttl, player.script.max_ttl))
      
      local f1 = function (c1, c2, f)
        return c1 + ((c1 - c2) * f)
      end
      
      local f2 = function (c1, c2, f)
        return color(f1(c1.r, c2.r, f), f1(c1.g, c2.g, f), f1(c1.b, c2.b, f)) 
      end
      
      local f3 = function (f)
        if (f < 0.5) then
          return f2(color(255, 255, 0), color(255, 0, 0), f / 0.5)
        end
        
        return f2(border_color, color(255, 255, 0), (f - 0.5) / 0.5)        
      end
      
      local current_border_color = f3(factor)
      
      gfx:draw_filled_circle(p, player.bounding_radius, color(32, 192, 32))
      gfx:draw_filled_circle(p, player.bounding_radius - 2, color(32, 96, 32))
      
      gfx:draw_circle(p, player.bounding_radius - 4, border_color, 2)
      gfx:draw_arc(p, player.bounding_radius - 4, -90.0, (360.0 * (1.0 - factor)), current_border_color, 2)
      
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
    
    local bullet_range = 300.0
    local bullet_initial_ttl = 60.0 * 1.0
    local bullet_spawn_distance = 17
    
    local fire = (player.application.is_button_primary_pressed or player.application.is_left_mouse_button_pressed)
    if (player.script.state_default.fire_delay == 0 and fire) then
      local bullet = player.application:spawn_entity(Bullet:new {
        position = player.position + player.heading * bullet_spawn_distance,
        velocity = player.heading:normal() * (1.0 / bullet_initial_ttl * (bullet_range - bullet_spawn_distance)),
        heading = player.heading,
        
        script = {
          initial_ttl = bullet_initial_ttl,
          ttl = bullet_initial_ttl,
          player_flag = true,
        },
      })
      
      player.script.state_default.fire_delay = player.script.state_default.fire_timeout
    end

    player.velocity = player.velocity:truncate(player.max_speed)
    
    player.position = player.position + player.velocity 
    
	-- todo: read resolution 
	
    -- set viewport
    player.application.viewport_translate = player.position - v2(1280 * 0.5, 800 * 0.5)
    
    local crosshair = player.application:get_entity_by_name("crosshair")
    player.heading = (crosshair.position - player.position):normal()
    
    if (player.script.current_ttl > 0) then
      player.script.current_ttl = player.script.current_ttl - 1
    else
      for entity in player.application.entities do
        entity.is_active = false
      end
    end
  end
  
  return player_state_default
end

player_state_default = create_player_state_default()