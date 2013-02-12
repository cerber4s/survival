function create_kamikaze_state_destroy()
  local kamikaze_state_destroy = entity_state_base:new('kamikaze_state_destroy')

  function kamikaze_state_destroy:enter_impl(kamikaze, state_data)
    kamikaze.is_active = false
  end

  return kamikaze_state_destroy
end

kamikaze_state_destroy = create_kamikaze_state_destroy()