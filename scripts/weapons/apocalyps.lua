Apocalyps = Weapon:new {
  name = "apocalyps",
  fire_delay = 0,
  fire_timeout = 60 * 30,
}

function Apocalyps:fire_impl(owner)
  local spread_in_degrees = 360
  local bullet_count = 40
  local bullet_range = 450
  local bullet_speed = 6.0
  local bullet_spawn_distance = owner.bounding_radius + 8
  
  local spread = spread_in_degrees * 0.0174532925
  local spread_space = spread / (bullet_count - 1)
  
  local angle_start = spread * -0.5
  
  local ring_versions = {
    { 
      angle_mod = 0,
      speed_mod = -1.5,
      range_mod = -150,
      spawn_distance_mod = 20,
    },
    {
      angle_mod = (360 / bullet_count * 0.5) * 0.0174532925,
      speed_mod = -1.0,  
      range_mod = -100,
      spawn_distance_mod = 10,
    },
    {
      angle_mod = 0,
      speed_mod = -0.5,      
      range_mod = -50,
      spawn_distance_mod = 0,
    },    
  }
  
  for _, ring_version in ipairs(ring_versions) do  
    for i = 0, bullet_count - 1 do
      
      local range = bullet_range + ring_version.range_mod
      local speed = bullet_speed + ring_version.speed_mod
      local ttl = range / speed
    
      local direction = m3.create_rotation(angle_start + ring_version.angle_mod + (i * spread_space)) * owner.heading
      local bullet = owner.application:spawn_entity(Bullet:new {
        position = owner.position + (direction * (bullet_spawn_distance + ring_version.spawn_distance_mod)),
        velocity = direction * speed,
        heading = direction,
        
        bounding_radius = 0.5,
        
        script = {
          initial_ttl = ttl,
          ttl = ttl,
          destroy_on_impact = false,
          player_flag = true,
        },
      })
    end
  end
  
end

function Apocalyps:render(owner, gfx)
  local p = owner.application.viewport_transformation * owner.position
  
  gfx:draw_circle(p, owner.bounding_radius - 2, color(8, 8, 8), 1)
  gfx:draw_circle(p, owner.bounding_radius - 5, color(8, 8, 8), 1)
end