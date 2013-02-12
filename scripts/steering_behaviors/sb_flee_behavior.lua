function create_sb_flee_behavior()
  local SbFleeBehavior = {}
  
  function SbFleeBehavior:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
  end

  function SbFleeBehavior:set_target(target)
    self.target = target
  end

  function SbFleeBehavior:calculate(entity)
    if (self.target == nil) then
      return v2()
    end
    
    local panic_distance = 300
    local panic_distance_sqr = panic_distance * panic_distance
    local to_target = entity.position - self.target.position
    if (to_target:length_sqr() > panic_distance_sqr) then
      return v2()
    end
    
    local desiredVelocity = to_target:normal() * entity.max_speed
    return desiredVelocity - entity.velocity
  end
  
  return SbFleeBehavior
end

SbFleeBehavior = create_sb_flee_behavior()
