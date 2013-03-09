function create_entity_state_base()
  local entity_state_base = {}

  function entity_state_base:new (state_name)
    if not (type(state_name) == "string") then
      error("state_name is not a valid string", 2)
    end
    
    local o = { 
      state_name = state_name,
    }
    
    setmetatable(o, self)
    self.__index = self
    return o
  end
  
  function entity_state_base:enter(entity, o)
    local state_data = o or {}
    entity.script[self.state_name] = state_data
    
    if not (self.enter_impl == nil) then      
      self:enter_impl(entity, state_data)
    end
    
    local state_self = self
    entity.script.render = function(self, entity, gfx)
      if not (state_self.render == nil) then
        state_self:render(entity, state_data, gfx)
      end
    end
  end
  
  function entity_state_base:exit(entity)
    if not (self.exit_impl == nil) then
      self:exit_impl(entity, entity.script[self.state_name])
    end
  
    entity.script[self.state_name] = nil
    entity.script.render = nil
  end

  function entity_state_base:execute(entity)
    if not (self.execute_impl == nil) then
      self:execute_impl(entity, entity.script[self.state_name])
      
      if (entity.script.update_is_visible) then
        local old_value = entity.is_visible
        local application = entity.application
        --entity.is_visible = application.gfx:is_in_viewport(application.viewport_transformation * entity.position)
        
        --[[
        if not (old_value == entity.is_visible) then
          print (string.format("%s.is_visible (%s => %s)", entity.name, tostring(old_value), tostring(entity.is_visible)))
        end
        --]]
      end
    end
  end
  
  --[[
  function entity_state_base:render(entity, state_data, gfx)
    error("entity state '" .. self.state_name .. "' does not implement 'render'")
  end
  --]]
  
  return entity_state_base
end
entity_state_base = create_entity_state_base()