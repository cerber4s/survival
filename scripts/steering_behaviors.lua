require "scripts/sb_seek_behavior"
require "scripts/sb_flee_behavior"
require "scripts/sb_arrive_behavior"

SteeringBehaviors = {}

function SteeringBehaviors:new(o)
  o = o or {
    seek = SbSeekBehavior:new {},
    flee = SbFleeBehavior:new {},
    arrive = SbArriveBehavior:new {},
  }
  
  setmetatable(o, self)
  self.__index = self
  return o
end

function SteeringBehaviors:calculate(entity)
  local force = v2()
  
  force = force + self.seek:calculate(entity)
  force = force + self.flee:calculate(entity)
  force = force + self.arrive:calculate(entity)
  
  return force:truncate(entity.max_speed)
end
