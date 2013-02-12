SbArriveBehavior = {}

function SbArriveBehavior:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function SbArriveBehavior:set_target(target)
  self.target = target
end

function SbArriveBehavior:set_deceleration(deceleration)
  self.deceleration = deceleration
end

function SbArriveBehavior:calculate(entity)
  if (entity == nil) then
    error("entity is nil", 2)
  end

  if (self.target == nil) then 
    return v2()
  end
  
  self.deceleration = self.deceleration or 0.5
  
  local delta = (self.target.position - entity.position)
  local distance = delta:length()
  
  if (distance > 0) then
    local speed = math.min(distance / self.deceleration, entity.max_speed)
    local desiredVelocity = delta:normal() * entity.max_speed
  
    return desiredVelocity - entity.velocity
  end
  
  return v2()  
end
