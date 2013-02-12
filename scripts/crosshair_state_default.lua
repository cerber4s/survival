function create_crosshair_state_default()
  local crosshair_state_default = {}

  function crosshair_state_default:enter (crosshair)
    crosshair.script.render = function (self, crosshair, gfx)
      local p = crosshair.application.viewport_transformation * crosshair.position
      local size = 15
      local c = color(192, 192, 192, 0.25)
      local c_dark = color(32, 32, 32, 0.75)
      
      gfx:draw_circle(p, 11, c, 3)
      gfx:draw_circle(p, 12, c_dark, 1)
      gfx:draw_circle(p, 9, c_dark, 1)
      
      gfx:draw_circle(p, 7, c, 1)
      
      gfx:draw_line(p + v2(-size, 0), p + v2(size, 0), c, 1)
      gfx:draw_line(p + v2(0, -size), p + v2(0, size), c, 1)
    end
  end

  function crosshair_state_default:exit (crosshair)
  end

  function crosshair_state_default:execute (crosshair)
    local delta = v2(crosshair.application.mouse_x, crosshair.application.mouse_y)
    crosshair.position = crosshair.application.viewport_inverse_transformation * delta
  end
  
  return crosshair_state_default
end

crosshair_state_default = create_crosshair_state_default()