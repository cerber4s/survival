function create_bullet_state_default()
  local bullet_state_default = {}

  function bullet_state_default:enter (bullet)
    
    bullet.script.render = function(self, bullet, gfx)
      local p = bullet.application.viewport_transformation * bullet.position
      if (gfx:is_in_viewport(p)) then
        local alpha = (1.0 / bullet.script.initial_ttl) * bullet.script.ttl

        local r = 224
        local g = 128
        local b = 0
        
        local br = 192
        local bg = 164
        local bb = 0
        
        if (bullet.script.player_flag) then
          r = 224
          g = 224
          b = 192

          br = 192
          bg = 192
          bb = 128
        end
        
        gfx:draw_line(p, p + (bullet.heading * -5), color(r, g, b, 0.25 + (0.75 * alpha)), 1)
        
        gfx:draw_filled_circle(p, 2.0, color(r, g, b, alpha))
        gfx:draw_circle(p, 2.0, color(192, 64, 0, 0.75 + (0.25 * alpha)), 1)

      end
    end 
  end

  function bullet_state_default:exit (bullet)
  end

  function bullet_state_default:execute (bullet)
    bullet.script.ttl = bullet.script.ttl - 1
    if (bullet.script.ttl <= 0) then
      bullet:change_current_state(bullet_state_destroy)
    end
  
    bullet.position = bullet.position + bullet.velocity
  end
  
  return bullet_state_default
end

bullet_state_default = create_bullet_state_default()