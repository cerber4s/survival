function create_sb_seek_behavior()
  local SbSeekBehavior = {}
  
  function SbSeekBehavior:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
  end

  function SbSeekBehavior:set_target(target)
    self.target = target
  end

  function SbSeekBehavior:calculate(entity)
    if (self.target == nil) then
      return v2()
    end
    
    local desiredVelocity = (self.target.position - entity.position):normal() * entity.max_speed
    return desiredVelocity - entity.velocity
  end
  
  return SbSeekBehavior
end

SbSeekBehavior = create_sb_seek_behavior()
