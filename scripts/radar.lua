require "scripts/entity_state_global"
require "scripts/radar_state_default"
require "scripts/entity"

Radar = Entity:new {
  type = "radar",
}

function Radar:initialize (radar)
  Entity:initialize(radar)
  
  radar.name = "radar"
  radar.type = self.type
  radar.is_active = true
  
  radar:change_global_state(entity_state_global)
  radar:change_current_state(radar_state_default)
end

function Radar:render (radar, gfx)
  local radar_line_radius = 87
  local radar_outer_radius = 75
  local radar_inner_radius = 40
  
  local c = color(32, 192, 32, 0.1)
  
  gfx:draw_circle(radar.position, radar_outer_radius, c, 1)
  gfx:draw_circle(radar.position, radar_inner_radius, c, 1)

  gfx:draw_line(radar.position + v2(-radar_line_radius, 0), radar.position + v2(radar_line_radius, 0), c, 1)
  gfx:draw_line(radar.position + v2(0, -radar_line_radius), radar.position + v2(0, radar_line_radius), c, 1)
  
  local player = radar.application:get_entity_by_name("player")
  
  local max_distance = 800 --temp
  local tower_inner_size = 2
  local tower_outer_size = 3
  local tower_alpha = 0.4
  local kamikaze_inner_size = 1
  local kamikaze_outer_size = 3
  local kamikaze_alpha = 0.4
  local threat_indicator_size = 5.0
  local threat_indicator_alpha = 0.10
  local threat_indicator_min_thickness = 2
  local threat_indicator_max_thickness = 12
  local threat_indicator_max_distance = 2800 --temp

  for entity in radar.application:get_entities_by_type("tower") do
    local delta = (entity.position - player.position)
    local distance_to = delta:length()
    if (distance_to > max_distance) then
      local f = (1.0 / threat_indicator_max_distance) * math.min(threat_indicator_max_distance, distance_to)
      local thickness = threat_indicator_min_thickness + ((threat_indicator_max_thickness - threat_indicator_min_thickness) * (1.0 - f))
      local alpha = threat_indicator_alpha + ((1.0 - threat_indicator_alpha) * f)
      
      gfx:draw_arc(radar.position, radar_outer_radius + 1.0 + (thickness / 2), delta:angle_in_degrees() - (threat_indicator_size / 2), threat_indicator_size, color(255, 0, 0, alpha), thickness)
    else
      local delta_p = delta:normal() * ((radar_outer_radius / max_distance) * distance_to)
      gfx:draw_filled_circle(radar.position + delta_p, tower_outer_size, color(255, 0, 0, tower_alpha * 0.5))
      gfx:draw_circle(radar.position + delta_p, tower_inner_size, color(255, 0, 0, tower_alpha), 1)
    end
  end

  for entity in radar.application:get_entities_by_type("kamikaze") do
    local delta = (entity.position - player.position)
    local distance_to = delta:length()
    if (distance_to > max_distance) then
      local f = (1.0 / threat_indicator_max_distance) * math.min(threat_indicator_max_distance, distance_to)
      local thickness = threat_indicator_min_thickness + ((threat_indicator_max_thickness - threat_indicator_min_thickness) * (1.0 - f))
      local alpha = threat_indicator_alpha + ((1.0 - threat_indicator_alpha) * f)
      
      gfx:draw_arc(radar.position, radar_outer_radius + 1.0 + (thickness / 2), delta:angle_in_degrees() - (threat_indicator_size / 2), threat_indicator_size, color(255, 0, 0, alpha), thickness)
    else
      local delta_p = delta:normal() * ((radar_outer_radius / max_distance) * distance_to)
      gfx:draw_filled_circle(radar.position + delta_p, kamikaze_outer_size, color(255, 0, 0, kamikaze_alpha * 0.5))
      gfx:draw_circle(radar.position + delta_p, kamikaze_inner_size, color(255, 0, 0, kamikaze_alpha), 1)
    end
  end

  for entity in radar.application:get_entities_by_type("bullet") do
    local delta = (entity.position - player.position)
    local distance_to = delta:length()
    if (distance_to <= max_distance) then
      local delta_p = delta:normal() * ((radar_outer_radius / max_distance) * distance_to)
      gfx:draw_circle(radar.position + delta_p, 1, color(255, 255, 255, 0.1), 1)
    end
  end
end

function Radar:handle_collision_with (crosshair, other)
end
