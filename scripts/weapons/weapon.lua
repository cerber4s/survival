Weapon = {
  name = "weapon",
  fire_delay = 0,
  fire_timeout = 0,
}

function Weapon:new(o)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  
  return o
end

function Weapon:update()
  if (self.fire_delay > 0) then
    self.fire_delay = self.fire_delay - 1
  end
end

function Weapon:fire(owner)
  if (self.fire_delay <= 0) then
    self:fire_impl(owner)
    self.fire_delay = self.fire_timeout
  end
end

function Weapon:get_charge()
  return 1.0 - ((1.0 / self.fire_timeout) * self.fire_delay)
end

function Weapon:fire_impl(owner)
  error("fire_impl is not implemented for weapon '" .. self.name .. "'", -1)
end
