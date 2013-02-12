require "scripts/entity_state_global"
require "scripts/player_state_default"

Player = Entity:new {
  type = "player",
  name = "player",
  
  position = v2(),
  velocity = v2(),
  heading = v2(1, 0),
  
  max_speed = 2.0,
  
  is_collidable = true,
  bounding_radius = 10,
  
  script = {
    max_ttl = 60.0 * (60.0 * 10.0),
    current_ttl = 60.0 * (60.0 * 10.0),
  },
  
  initial_global_state = entity_state_global,
  initial_current_state = player_state_default,  
}

function Player:handle_collision_with (player, other)
  if (other:is_of_type(Kamikaze)) then
    other:handle_collision_with(player)
    return
  end
  
  if (other:is_of_type(Bullet)) then
    player.script.current_ttl = math.max(0, player.script.current_ttl - (60.0 * 2.0))
    other:change_current_state(bullet_state_destroy)
    return
  end
end
