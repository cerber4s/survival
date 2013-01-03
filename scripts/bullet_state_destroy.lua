function create_bullet_state_destroy()
  local bullet_state_destroy = {}

  function bullet_state_destroy:enter (bullet)
    bullet.is_collidable = false
    bullet.script.is_destroyed = true
    bullet.script.initial_ttl = 20
    bullet.script.ttl = 20
  end

  function bullet_state_destroy:exit (bullet)
  end

  function bullet_state_destroy:execute (bullet)
    bullet.script.ttl = bullet.script.ttl - 1
    if (bullet.script.ttl == 0) then
      bullet.is_active = false
    end
  end
  
  return bullet_state_destroy
end

bullet_state_destroy = create_bullet_state_destroy()