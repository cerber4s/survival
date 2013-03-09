require "scripts/steering_behaviors/sb_arrive_behavior"
require "scripts/steering_behaviors/sb_flee_behavior"
require "scripts/steering_behaviors/sb_intercept_behavior"
require "scripts/steering_behaviors/sb_seek_behavior"
require "scripts/steering_behaviors/sb_separation_behavior"

SteeringBehaviors = {}

function SteeringBehaviors:new(o)
  o = o or {
    --arrive = SbArriveBehavior:new {},
    --flee = SbFleeBehavior:new {},
    --intercept = SbInterceptBehavior:new {},
    --seek = SbSeekBehavior:new {},
  }
  
  setmetatable(o, self)
  self.__index = self
  return o
end

function SteeringBehaviors:calculate(entity)
  if (entity == nil) then
    error("entity is nil", 2)
  end

  local force = v2()
  
  local supported_behaviors = {
    { 
      name = "seek",
      weight = 1.0,
    },
    { 
      name = "flee",
      weight = 1.0,
    },
    { 
      name = "arrive",
      weight = 1.0,
    },
    { 
      name = "pursuit",
      weight = 1.0,
    },
    { 
      name = "separation",
      weight = 0.3,
    },
  }
  
  for _, behavior_info in ipairs(supported_behaviors) do
    if not (self[behavior_info.name] == nil) then
      force = force + (self[behavior_info.name]:calculate(entity) * behavior_info.weight)
    end
  end
  
  --[[
  force = force + self.arrive:calculate(entity)
  force = force + self.flee:calculate(entity)
  force = force + self.intercept:calculate(entity)
  force = force + self.seek:calculate(entity)
  --]]
  
  return force:truncate(entity.max_force)
end
