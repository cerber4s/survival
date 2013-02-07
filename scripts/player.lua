require "scripts/entity_state_global"
require "scripts/player_state_default"

Player = Entity:new {
  type = "player"
}

function Player:initialize (player)
  player.type = self.type
  player.name = "player"
  player.is_active = true
  
  player.position = v2()
  player.velocity = v2()
  player.heading = v2(1, 0)
  player.max_speed = 2.0
  
  player.is_collidable = true
  player.bounding_radius = 10
  
  player.script.max_ttl = 60.0 * (60.0 * 3.0)
  player.script.current_ttl = 360060.0 * (60.0 * 1.0)
  
  player:change_global_state(entity_state_global)
  player:change_current_state(player_state_default)
end

--[[
function Player:render (player, gfx)
  local p = player.application.viewport_transformation * player.position
  gfx:draw_circle(p, player.bounding_radius - 1, color(32, 192, 32), 1)
  gfx:draw_circle(p, player.bounding_radius - 4, color(64, 192, 64, 0.75), 2)
  
  gfx:draw_line(p, p + player.heading * 15, color(64, 192, 64, 0.75), 2)
end
]]

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
