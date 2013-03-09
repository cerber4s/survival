Pistol = Weapon:new {
  name = "pistol",
  fire_delay = 0,
  fire_timeout = 60 / 4,
}

function Pistol:fire_impl(owner)
  local bullet_range = 300.0
  local bullet_initial_ttl = 60.0 * 1.0
  local bullet_spawn_distance = 17
  
  local bullet = owner.application:spawn_entity(Bullet:new {
    position = owner.position + owner.heading * bullet_spawn_distance,
    velocity = owner.heading:normal() * (1.0 / bullet_initial_ttl * (bullet_range - bullet_spawn_distance)),
    heading = owner.heading,
    
    script = {
      initial_ttl = bullet_initial_ttl,
      ttl = bullet_initial_ttl,
      player_flag = true,
    },
  })
end

function Pistol:render(owner, gfx)
  local p = owner.application.viewport_transformation * owner.position
  
  local lines = {
    {
      from = v2(0, 0),
      to = v2(owner.bounding_radius, 0), 
    },
  }
  
  local rotation = m3.create_rotation(owner.heading:angle())
  for _, line in ipairs(lines) do    
    gfx:draw_line(p + (rotation * line.from), p + (rotation * line.to), color(8, 8, 8), 2)
  end
end