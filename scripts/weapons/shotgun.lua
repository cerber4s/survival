Shotgun = Weapon:new {
  name = "shotgun",
  fire_delay = 0,
  fire_timeout = 60 * 0.75,
}

function Shotgun:fire_impl(owner)
  local spread_in_degrees = 30
  local bullet_count = 12
  local bullet_range = 175
  local bullet_range_clip_range = 20
  local bullet_speed = 8.0
  local bullet_speed_clip_range = 1.5
  local bullet_spawn_distance = owner.bounding_radius + 8
  
  local spread = spread_in_degrees * 0.0174532925
  local spread_space = spread / (bullet_count - 1)
  
  local angle_start = spread * -0.5
  
  for i = 0, bullet_count - 1 do
    
    local range = bullet_range - (math.random() * bullet_range_clip_range)    
    local speed = bullet_speed  - (math.random() * bullet_speed_clip_range)
    local ttl = range / speed
  
    local direction = m3.create_rotation(angle_start + (i * spread_space)) * owner.heading
    local bullet = owner.application:spawn_entity(Bullet:new {
      position = owner.position + (direction * bullet_spawn_distance),
      velocity = direction * speed,
      heading = direction,
      
      bounding_radius = 0.5,
      
      script = {
        initial_ttl = ttl,
        ttl = ttl,
        player_flag = true,
      },
    })
  end
end

function Shotgun:render(owner, gfx)
  local p = owner.application.viewport_transformation * owner.position
  
  local lines = {
    {
      from = v2(0, 0),
      to = v2(owner.bounding_radius, 0), 
    },    
    {
      from = v2(2, -2),
      to = v2(owner.bounding_radius - 4, -5), 
    },
    {
      from = v2(2, 2),
      to = v2(owner.bounding_radius - 4, 5),
    },
  }
  local rotation = m3.create_rotation(owner.heading:angle())
  for _, line in ipairs(lines) do    
    gfx:draw_line(p + (rotation * line.from), p + (rotation * line.to), color(8, 8, 8), 2)
  end  
end