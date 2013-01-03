require "scripts/entity_state_global"
require "scripts/player_state_default"

Player = Entity:new {
  initial_entity_type = 0,
}

function Player:initialize (player)
  player.entity_type = self.initial_entity_type
  player.is_active = true
  player.position = v2()
  player.velocity = v2()
  player.heading = v2(1, 0)
  player.is_collidable = true
  player.bounding_radius = 10
  
  player:change_global_state(entity_state_global)
  player:change_current_state(player_state_default)
end

function Player:render (player, gfx)
  local p = player.application.viewport_transformation * player.position
  
  --[[
  for i = 1, 4 do
    gfx:draw_circle(p + (player.velocity:normal() * -1.0 * ((4 - i) * (10.0 / 3.0 * player.velocity:length()))), (player.bounding_radius - 4)  * (0.2 + (0.2 * i)) , color(64, 192, 64, 0.75), 2)
  end
  ]]
  
  gfx:draw_circle(p, player.bounding_radius - 1, color(32, 192, 32), 1)
  gfx:draw_circle(p, player.bounding_radius - 4, color(64, 192, 64, 0.75), 2)
  
  gfx:draw_line(p, p + player.heading * 15, color(64, 192, 64, 0.75), 2)
  
  --gfx:draw_text(p + v2(0, 8), color(224, 224, 224), string.format("%dx%d", p.x, p.y))
  --gfx:draw_text(p + v2(0, 20), color(128, 128, 224), string.format("%dx%d", player.position.x, player.position.y))
end

function Player:handle_collision_with (player, other)
  if (other:is_of_type(Bullet)) then
    -- todo: dmg player
    other:change_current_state(bullet_state_destroy)
  end
end
