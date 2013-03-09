require "scripts/weapons/weapon"
require "scripts/weapons/pistol"
require "scripts/weapons/shotgun"
require "scripts/weapons/apocalyps"

function create_player_state_default()
  local player_state_default = entity_state_base:new('player_state_default')
  
  local weapons = {
    Pistol:new {},
    Shotgun:new {},
    Apocalyps:new {},
  }
  
  local weapon_index = 1
  local allow_weapon_switch = true
  
  function player_state_default:render(player, state_data, gfx)
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
    
    weapons[weapon_index]:render(player, gfx)
    
    gfx:draw_text(10, 10, color(64 + (128 * (1.0 - weapons[weapon_index]:get_charge())), 192 * weapons[weapon_index]:get_charge(), 0), weapons[weapon_index].name)
    --[[
    if (weapons[weapon_index]:get_charge() < 1) then
      gfx:draw_text(10, 54, color(128, 128 * weapons[weapon_index]:get_charge(), 0, weapons[weapon_index]:get_charge()), string.format("%.0d%%", 100 * weapons[weapon_index]:get_charge()))
    end
    --]]
    local width = 100
    local height = 3
    local offset = v2(10, 28)
    
    gfx:draw_filled_rectangle(offset, offset + v2(width * weapons[weapon_index]:get_charge(), height), color(192, 192, 192))
    gfx:draw_rectangle(offset, offset + v2(width, height), color(128, 128, 128), 1)
    
  end

  function player_state_default:execute_impl (player, state_data)
    local acceleration = 0.05
    
    for _, weapon in ipairs(weapons) do
      weapon:update()
    end
    
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
    
    if (allow_weapon_switch and player.application.is_button_secondary_pressed) then
      weapon_index = (weapon_index % (#weapons)) + 1
      allow_weapon_switch = false
    elseif (not allow_weapon_switch) and (not player.application.is_button_secondary_pressed) then
      allow_weapon_switch = true
    end
    
    local fire = (player.application.is_button_primary_pressed or player.application.is_left_mouse_button_pressed)
    if (fire) then
      weapons[weapon_index]:fire(player)
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