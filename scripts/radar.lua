require "scripts/entity_state_global"
require "scripts/radar_state_default"
require "scripts/entity"

Radar = Entity:new {
  initial_entity_type = 4,
}

function Radar:initialize (radar)
  radar.entity_type = self.initial_entity_type
  radar.is_active = true
  
  radar:change_global_state(entity_state_global)
  radar:change_current_state(radar_state_default)
end

function Radar:render (radar, gfx)
  local radar_line_radius = 87
  local radar_outer_radius = 75
  local radar_inner_radius = 40
  
  local c = color(192, 192, 192)
  
  gfx:draw_circle(radar.position, radar_outer_radius, c, 1)
  gfx:draw_circle(radar.position, radar_inner_radius, c, 1)

  gfx:draw_line(radar.position + v2(-radar_line_radius, 0), radar.position + v2(radar_line_radius, 0), c, 1)
  gfx:draw_line(radar.position + v2(0, -radar_line_radius), radar.position + v2(0, radar_line_radius), c, 1)
  
  local player = nil
  for entity in radar.application.entities do
    if (entity:is_of_type(Player)) then
      player = entity
    end
  end
  
  local max_distance = 800
  local enemy_size = 3
  local enemy_alpha = 0.5
  local threat_indicator_size = 5.0
  local threat_indicator_alpha = 0.10
  local threat_indicator_min_thickness = 2
  local threat_indicator_max_thickness = 12
  local threat_indicator_max_distance = 2400
  
  for entity in radar.application.entities do
    if (entity:is_of_type(Entity)) then
      
      local delta = (entity.position - player.position)
      local distance_to = delta:length()
      if (distance_to > max_distance) then
        local f = (1.0 / threat_indicator_max_distance) * math.min(threat_indicator_max_distance, distance_to)
        local thickness = threat_indicator_min_thickness + ((threat_indicator_max_thickness - threat_indicator_min_thickness) * (1.0 - f))
        local alpha = threat_indicator_alpha + ((1.0 - threat_indicator_alpha) * f)
        
        gfx:draw_arc(radar.position, radar_outer_radius + 1.0 + (thickness / 2), delta:angle_in_degrees() - (threat_indicator_size / 2), threat_indicator_size, color(255, 0, 0, alpha), thickness)
      else
        local delta_p = delta:normal() * ((radar_outer_radius / max_distance) * distance_to)
        gfx:draw_circle(radar.position + delta_p, enemy_size, color(255, 0, 0, enemy_alpha), 1)
      end
  
    elseif (entity:is_of_type(Bullet)) then
    
      local delta = (entity.position - player.position)
      local distance_to = delta:length()
      if (distance_to <= max_distance) then
        local delta_p = delta:normal() * ((radar_outer_radius / max_distance) * distance_to)
        gfx:draw_circle(radar.position + delta_p, 1, color(255, 255, 255, 0.1), 1)
      end
    end
  end
  
end

function Radar:handle_collision_with (crosshair, other)
end
