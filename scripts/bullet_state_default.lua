function create_bullet_state_default()
  local bullet_state_default = {}

  function bullet_state_default:enter (entity)
  end

  function bullet_state_default:exit (bullet)
  end

  function bullet_state_default:execute (bullet)
    bullet.script.ttl = bullet.script.ttl - 1
    if (bullet.script.ttl == 0) then
      bullet:change_current_state(bullet_state_destroy)
    end
  
    bullet.position = bullet.position + bullet.velocity
  end
  
  return bullet_state_default
end

bullet_state_default = create_bullet_state_default()