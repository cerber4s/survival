function create_crosshair_state_default()
  local crosshair_state_default = {}

  function crosshair_state_default:enter (entity)
  end

  function crosshair_state_default:exit (entity)
  end

  function crosshair_state_default:execute (entity)
    local delta = v2(entity.application.mouse_x, entity.application.mouse_y)
    entity.position = entity.application.viewport_inverse_transformation * delta
  end
  
  return crosshair_state_default
end

crosshair_state_default = create_crosshair_state_default()