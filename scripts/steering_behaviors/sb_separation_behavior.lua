function create_sb_separation_behavior()
  local SbSeparationBehavior = {}
  
  function SbSeparationBehavior:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
  end

  function SbSeparationBehavior:set_neighbors(neighbors)
    self.neighbors = neighbors
  end

  function SbSeparationBehavior:calculate(entity)
    if (self.neighbors == nil) then
      return v2()
    end
    
    local steering_force = v2()
    
    for i = 1, #self.neighbors do
      local neighbor = self.neighbors[i]
      
      local to_neighbor = entity.position - neighbor.position
      steering_force = steering_force + (to_neighbor:normal() * to_neighbor:length())      
    end
    
    return steering_force
  end
  
  return SbSeparationBehavior
end

SbSeparationBehavior = create_sb_separation_behavior()
