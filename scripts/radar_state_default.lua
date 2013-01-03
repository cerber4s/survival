function create_radar_state_default()
  local radar_state_default = {}

  function radar_state_default:enter (radar)
  end

  function radar_state_default:exit (radar)
  end

  function radar_state_default:execute (radar)
  end
  
  return radar_state_default
end

radar_state_default = create_radar_state_default()