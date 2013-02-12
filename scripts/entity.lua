require "scripts/entity_state_global"
require "scripts/entity_state_idle"
require "scripts/entity_state_destroy"

function create_entity() 
  local Entity = {
    ctor_args = {},
  }
  
  local Set = function(values)
    local set = {}
    for _, l in ipairs(values) do set[l] = true end
    return set
  end
  
  local supported_arg_keys = Set {
    "type",
    "name",
    "position",
    "velocity",
    "heading",
    "mass",
    "max_speed",
    "max_force",
    "max_turn_rate",
    "is_collidable",
    "bounding_radius",
    "is_active",
    "script",
    "initial_global_state",
    "initial_current_state",
  }
  
  local debug_print_table = function(caption, t)
    if (t == nil) then
      print( caption .. "empty table")
      print()
      
      return
    end
  
    print(caption)
    for key, value in pairs(t) do
      
      if (type(value) == "string") then
        print( key .. ": " .. value )
      elseif (type(value) == "boolean") then
        --
        if (value) then
          print( key .. ": true")
        else
          print( key .. ": false")
        end
      else
        print( key .. ": " .. type(value) )
      end
    end  
    print()
  end
  
  function Entity:new (args)
    args = args or {}
    
    --debug_print_table("entity:new called with: ", args)
    
    local ctor_args = {}
    for k, v in pairs(self.ctor_args) do
      ctor_args[k] = v
    end
    
    for k, v in pairs(args) do
      if (supported_arg_keys[k] == nil) then
        error("value for key '" .. k .. "' is not supported", 2)
      end
      
      if not (k == "script") then
        ctor_args[k] = v
      end
    end
    
    if not (args.script == nil) then
      ctor_args.script = ctor_args.script or {}
      for k, v in pairs(args.script) do
        ctor_args.script[k] = v
      end
    end
    
    local o = {
      type = ctor_args.type,
      ctor_args = ctor_args,
    }
    
    setmetatable(o, self)
    self.__index = self
    
    if (o.type == nil) then
      error("entity type is not specified", 2)
    end
    
    if not (type(o.type) == "string") then
      error("type is not a valid string", 2)
    end
    
    return o
  end

  function Entity:initialize (entity, args)
    args = args or {}
    
    --debug_print_table("entity:initialize called with args:", args)
    --debug_print_table("entity:initialize self.ctor_args:", self.ctor_args)
    
    local default_args_values = {
      type = self.type,
      name = string.format("%s-%d", self.type, entity.id),
      
      position = v2(),
      velocity = v2(),
      heading = v2(),
      
      max_speed = 1.0,
      max_force = 1.0,
      max_turn_rate = (math.pi * 2),
      
      is_collidable = true,
      bounding_radius = 10,
      
      is_active = true,
    }
    
    for key, value in pairs(default_args_values) do      
      --print( "setting key '" .. key .. "' for entity type '" .. self.type .. "'") 
      entity[key] = self.ctor_args[key] or args[key] or value          
    end
    
    for key, value in pairs(args.script or {}) do
      entity.script[key] = value
    end
    
    for key, value in pairs(self.ctor_args.script or {}) do
      entity.script[key] = value
    end
    
    entity:change_global_state(self.ctor_args.initial_global_state or args.initial_global_state or entity_state_global)
    entity:change_current_state(self.ctor_args.initial_current_state or args.initial_current_state or entity_state_idle)
    
    self.ctor_args = nil
  end

  function Entity:render (entity, gfx)
    local p = entity.application.viewport_transformation * entity.position
    
    if (gfx:is_in_viewport(p)) then
    
      if (entity.script.is_destroyed) then
        local alpha = (1.0 / entity.script.initial_ttl) * entity.script.ttl
     
        for index, point in pairs(entity.script.points) do
          local p2 = p + point.position
          local spread = (entity.bounding_radius * 0.3) + math.exp(alpha);
          for i = 1, 1 do
            local d = v2(-1 + math.random() * 2, -1 + math.random() * 2):normal()
            local n = (d * spread)
            local t = 0.5 + (1.5 * alpha)
            local c1 = color(255 * point.dr, (64 - (64 * alpha)) * point.dg, (128 - (128 * alpha)) * point.db, alpha)
            local c2 = color(192 * point.dr, (32 - (32 * alpha)) * point.dg, (64 - (64 * alpha)) * point.db, 0.5 * alpha)
            
            gfx:draw_line(p2, p2 + n, c1, t)
            gfx:draw_line(p2 + n, p2 + (d * (spread + 3)), c2, t)
            gfx:draw_filled_circle(p2, spread * 0.5, c2)
          end
        end
        return
      end
      
      gfx:draw_circle(p, entity.bounding_radius, color(192, 0, 0), 1)
      gfx:draw_text_centre(p + v2(0, -8), color(192, 192, 192), string.format("%d", entity.id))
    end
  end

  function Entity:handle_collision_with (entity, other)
    --[[
    if (other:is_of_type(Bullet)) then
      --entity.is_active = false
      entity:change_current_state(entity_state_destroy)
      other:change_current_state(bullet_state_destroy)
    end
    ]]
  end
  
  return Entity
end

Entity = create_entity()
