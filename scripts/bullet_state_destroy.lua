function create_bullet_state_destroy()
  local bullet_state_destroy = {}

  function bullet_state_destroy:enter (bullet)
    bullet.is_collidable = false
    bullet.script.is_destroyed = true
    bullet.script.initial_ttl = 20
    bullet.script.ttl = 20
	
	bullet.script.render = function(self, bullet, gfx)
	  local p = bullet.application.viewport_transformation * bullet.position
	  if (gfx:is_in_viewport(p)) then
		local alpha = (1.0 / bullet.script.initial_ttl) * bullet.script.ttl
		
		local spread = 5;
		for i = 1, 1 do
		  local d = v2(-1 + math.random() * 2, -1 + math.random() * 2):normal()
		  local n = (d * spread)
		  local t = 0.5 + (1.5 * alpha)
		  
		  gfx:draw_line(p, p + n, color(255, 0 + (32 * alpha), 92 - (64 * alpha), alpha), t)
		  gfx:draw_line(p + n, p + (d * (spread + 3)), color(192, 0 + (32 * alpha), 92 - (64 * alpha), 0.5 * alpha), t)
		end
	  end
	end 	
  end

  function bullet_state_destroy:exit (bullet)
  end

  function bullet_state_destroy:execute (bullet)
    bullet.script.ttl = bullet.script.ttl - 1
    if (bullet.script.ttl == 0) then
      bullet.is_active = false
    end
  end
  
  return bullet_state_destroy
end

bullet_state_destroy = create_bullet_state_destroy()