function create_player_state_default()
  local player_state_default = {}

  local player_map = {}
  
  function player_state_default:enter (player)
    player_map[player.id] = {}
    player_map[player.id].fire_delay = 0
  end

  function player_state_default:exit (player)
    player_map[player.id] = nil
  end

  function player_state_default:execute (player)
    max_speed = 3.0
    bullet_speed = 6.0 + (-0.05 + math.random() * 0.1)
    acceleration = 0.05
    
    if (player.application.is_button_up_pressed) then
      player.velocity = player.velocity + v2(0, -acceleration)
    end
    if (player.application.is_button_down_pressed) then
      player.velocity = player.velocity + v2(0, acceleration)
    end
    if (player.application.is_button_left_pressed) then
      player.velocity = player.velocity + v2(-acceleration, 0)
    end
    if (entity.application.is_button_right_pressed) then
      player.velocity = player.velocity + v2(acceleration, 0)
    end
    
    if (player_map[player.id].fire_delay > 0) then
      player_map[player.id].fire_delay = player_map[player.id].fire_delay - 1
    end
    
    local fire = (player.application.is_button_primary_pressed or player.application.is_left_mouse_button_pressed)
    if (player_map[player.id].fire_delay == 0 and fire) then
      bullet = player.application:spawn_entity(Bullet:new())
      bullet.position = player.position + player.heading * 15
      bullet.velocity = player.heading * bullet_speed
      bullet.heading = player.heading
      
      player_map[player.id].fire_delay = 2
    end
    
    player.velocity = player.velocity:truncate(max_speed)
    
    player.position = player.position + player.velocity 
    
    -- set viewport
    player.application.viewport_translate = player.position - v2(400, 300)
    
    local crosshair = player.application:get_entity_by_id(player.script.crosshair_id)
    player.heading = (crosshair.position - player.position):normal()
  end
  
  return player_state_default
end

player_state_default = create_player_state_default()