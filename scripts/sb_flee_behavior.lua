SbFleeBehavior = {}

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
  
  local delta = (entity.position - self.target)
  local desiredVelocity = delta:normal() * entity.max_speed
  
  return desiredVelocity - entity.velocity
end
