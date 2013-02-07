require "scripts/bosses/sphere_state_rotate"

function create_sphere_state_attack()
  local sphere_state_attack = {}

  function sphere_state_attack:enter(sphere)
    local shots_per_second = 60
    sphere.script.state_attack = {
        fire_delay = 0,
        fire_charge_time =  60.0 / shots_per_second,
        fire_count = 64,
    }
  
    sphere.script.render = function (self, sphere, gfx)
      local p = sphere.application.viewport_transformation * sphere.position      
      if (gfx:is_in_viewport(p)) then
        gfx:draw_circle(p, sphere.bounding_radius, color(192, 0, 0), 1)
        gfx:draw_circle(p, sphere.bounding_radius - 3, color(192, 0, 0), 1)
        
        for index = 0, 3 do
          gfx:draw_line(p, p + (m3.create_rotation((math.pi * 0.5) * index) * sphere.heading) * sphere.bounding_radius, color(192, 0, 0), 1)
        end
      end
    end
  end

  function sphere_state_attack:exit(sphere)
    sphere.script.state_attack = nil
    sphere.script.render = nil
  end

  function sphere_state_attack:execute(sphere)
    if (sphere.script.state_attack.fire_delay > 0) then
      sphere.script.state_attack.fire_delay = sphere.script.state_attack.fire_delay - 1
    else
      local bullet_range = 350.0
      local bullet_initial_ttl = 60.0 * 1.5
      local bullet_spawn_distance = sphere.bounding_radius + 5.0
      
      for index = 0, 3 do
        local current_heading = m3.create_rotation((math.pi * 0.5) * index) * sphere.heading
      
        local bullet = sphere.application:spawn_entity(Bullet:new())
        bullet.position = sphere.position + (current_heading * bullet_spawn_distance)
        bullet.velocity = current_heading:normal() * (1.0 / bullet_initial_ttl * (bullet_range - bullet_spawn_distance))
        bullet.heading = current_heading
      
        bullet.script.initial_ttl = bullet_initial_ttl
        bullet.script.ttl = bullet_initial_ttl
      end
      
      sphere.script.state_attack.fire_delay = sphere.script.state_attack.fire_charge_time
      
      if (sphere.script.state_attack.fire_count > 0) then
        sphere.script.state_attack.fire_count = sphere.script.state_attack.fire_count - 1
      else
        sphere:change_current_state(sphere_state_rotate)
        return
      end
    end
  end

  return sphere_state_attack
end

sphere_state_attack = create_sphere_state_attack()