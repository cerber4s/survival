SbSeekBehavior = {}

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
  
  local delta = (self.target - entity.position)
  local desiredVelocity = delta:normal() * entity.max_speed
  
  return desiredVelocity - entity.velocity
end
