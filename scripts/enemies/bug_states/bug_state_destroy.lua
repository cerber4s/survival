function create_bug_state_destroy()
  local bug_state_destroy = entity_state_base:new('bug_state_destroy')
  
  function bug_state_destroy:enter_impl(bug, state_data)
    bug.is_active = false
  end
  
  return bug_state_destroy
end

bug_state_destroy = create_bug_state_destroy()