SbInterceptBehavior = {}

function SbInterceptBehavior:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  return o
end

function SbInterceptBehavior:set_target_entity_id(target_entity_id)
  self.target_entity_id = target_entity_id
end

function SbInterceptBehavior:calculate(entity)
  --[[
  if (self.target_entity_id == nil) then 
    return v2()
  end
  
  local target = entity.application:get_entity_by_id(self.target_entity_id)
  if (target == nil) then
    return v2()
  end
  
  local delta = (target.position - entity.position)
  local relativeHeading = entity.heading:dot(target.heading)
  
  -- acos(0.95) = 18 degrees 
  if ((delta:dot(entity.heading) > 0) and relativeHeading < -0.95) then
    local seek = SbSeekBehavior:new()
    seek:set_target(target.position)
    
    return seek:calculate(entity)
  end
  
  local target_current_speed = target.velocity:length()
  local lookAheadTime = delta:length() / (entity.max_speed + target__current_speed)
  
  local seek = SbSeekBehavior:new()
  seek:set_target(target.position + (target.velocity * lookAheadTime))

  return seek:calculate(entity)
  ]]
  return v2()
end
