function create_creator_state_default()
  local creator_state_default = entity_state_base:new('creator_state_default')

  function creator_state_default:render(creator, state_data, gfx)
    local p = creator.application.viewport_transformation * creator.position

    gfx:draw_filled_circle(p, creator.script.spawn_distance * 2, color(128, 128, 128, 0.2))
    gfx:draw_circle(p, creator.script.spawn_distance, color(128, 128, 128), 1)
    
    gfx:draw_filled_circle(p, creator.bounding_radius, color(128, 128, 128))
    gfx:draw_filled_circle(p, creator.bounding_radius - 2, color(32, 32, 32))
  end
  
  function creator_state_default:enter_impl(creator, state_data)
    state_data.spawn_count = 4
    state_data.spawn_timeout = 0
    state_data.charge_time = 60 * 1.0
    
    state_data.bug_count = 0
  end
  
  function creator_state_default:exit_impl(creator, state_data)   
  end
  
  function creator_state_default:execute_impl(creator, state_data)
    --[-[
    local player = creator.application:get_entity_by_name("player")
    local delta = player.position - creator.position
    
    state_data.bug_count = 0
    local bugs = creator.application:get_entities_by_type("bug")
    for bug in bugs do
      if (bug.script.owner_id == creator.id) then
        state_data.bug_count = state_data.bug_count + 1
      end
    end
    
    if (state_data.spawn_timeout > 0) then
      state_data.spawn_timeout = state_data.spawn_timeout - 1
    elseif (state_data.bug_count < state_data.spawn_count) then
      local bug = creator.application:spawn_entity(Bug:new {
        position = creator.position + delta:normal() * creator.script.spawn_distance,
        heading = delta:normal(),
        
        script = {
          steering_behaviors = SteeringBehaviors:new {
            seek = SbSeekBehavior:new {},
            separation = SbSeparationBehavior:new {},
          },
          owner_id = creator.id,
        },
      })
      bug.script.steering_behaviors.seek:set_target(player)
      
      state_data.spawn_timeout = state_data.charge_time
    end
  end
  
  return creator_state_default
end

creator_state_default = create_creator_state_default()
