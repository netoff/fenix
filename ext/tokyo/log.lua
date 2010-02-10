json = require('json')

-- slots' width 
-- if you simply change slots' width here
-- would old data become invalid??
local width = {
  sec = 64,
  min = 64,
  hour = 32,
  day = 2048 }
  
local function concat(array)
  return table.concat(array, "")
end
  
function make_id(key, value)
  local md5 = _hash("md5", value)  
  
  if _put(concat({key, ":", md5}), value) then
    return md5
  else
    return ""
  end
end

function log(key, value)
  --=====================================
  -- parse_slots function
  ---------------------------------------
  function parse_slots(query)
    
    function parse_slot(s)
      local slot = nil
      local row = _split(s, "=")
      
      if row and #row == 2 then
        local timestamp = tonumber(row[1])
	local keys = _split(row[2], "|")
	
	if timestamp and keys and #keys > 0 then
          slot = {}
	  slot.timestamp = timestamp 
	  slot.keys = keys
	end
      end
      
      return slot
    end
    
    local slots = {}
    
    local items = _split(query, "/")
    
    if items and #items > 0 then
      local n = #items
      
      slots.sec = parse_slot(items[1])
      if slots.sec then
        slots.sec.width = width.sec
      end
      -- it has minute slot
      if n > 1 then
        slots.min = parse_slot(items[2])
	if slots.min then
	  slots.min.width = width.min
	end
      end
      
      -- it has hour slot
      if n > 2 then
        slots.hour = parse_slot(items[3])
	if slots.hour then 
	  slots.hour.width = width.hour
	end
      end
      
      -- it has day slot
      if n > 3 then
        slots.day = parse_slot(items[4])
	if slots.day then
	  slots.day.width = width.day
	end
      end
    end
    
    return slots
  end------------------------------------
  -- END parse_slots function
  ---------------------------------------

  --=====================================
  -- inc(increment) function
  ---------------------------------------
  function inc(key, val, slot)
    local timestamp = slot.timestamp
    local width = slot.width
    
    if not timestamp or not width then
      return -1
    end
  
    -- a/b portion of the key
    -- a is key 'name', b is timestamp part of the key
    a = val
    b = math.mod(timestamp, width)
    
    local root_key = concat({key, ':', a, '*'})

    _putkeep(root_key, tostring(timestamp))
    local root_timestamp = tonumber(_get(root_key))
    
    --if it is still not number value
    if not root_timestamp then
      _put(root_key, tostring(timestamp))
      root_timestamp = tonumber(_get(root_key))
    end

    local diff = timestamp - root_timestamp
    if diff then
      local row_key = concat({key, ':', b, ':', a})
  
      if diff > 0 then
        _lock(root_key)
	root_timestamp = tonumber(_get(root_key))
	if not root_timestamp or root_timestamp < timestamp then
	
	_put(root_key, timestamp)
	end
	_unlock(root_key)

	if diff >= width then
          diff = width - 1
        end
				
	--invalidate data from buffer
	local last = tonumber(b)
	local delta = last - diff

	if delta < 0 then
	  delta = width + delta
	end

	delta = math.mod(delta + 1, width) 
	while delta ~= last do
	  local remove_key = concat({key, ':', tostring(delta), ':', a})
	  _out(remove_key)
	  delta = math.mod(delta + 1, width)
	end
	
	_out(row_key)
      end
			
      return _addint(row_key, 1)
    else
      _log("Error in circular buffer diff calculation: " .. tostring(diff))
    end  
  end----------------------------------
  -- END inc function
  -------------------------------------
  
  local slots = parse_slots(value)
  
  if slots.sec then
    local slot = slots.sec
    
    -- do second's slots
    for i, k in ipairs(slot.keys) do
      inc(key, k, slot)
    end
  end
  
  if slots.min then
    local slot = slots.min
    
    -- do minute's slots
    for i, k in ipairs(slot.keys) do
      inc(key, k, slot)
    end
  end
  
  if slots.hour then
    local slot = slots.hour
    
    -- do hour's slots
    for i, k in ipairs(slot.keys) do
      inc(key, k, slot)
    end
  end
  
  if slots.day then
    local slot = slots.day
    
    -- do day's slots
    for i, k in ipairs(slot.keys) do
      inc(key, k, slot)
    end
  end
end
  
local function get_value(key, k, timestamp, width)
  local root_key = concat({key, ':', k, '*'})
  local root_timestamp = tonumber(_get(root_key))

  if root_timestamp then
    local diff = root_timestamp -  timestamp
    
    if diff >= 0 and diff < width then
      local t = math.mod(timestamp, width)        
      local row_key = concat({key, ':', tostring(t), ':', k})
 
      local ret = _unpack('i', _get(row_key))
      
      if ret and #ret == 1 then    
        return ret[1]
      end
    end
  else
    _log("Error fetching stats : " .. key .. ", k: " .. k .. ", timestamp: " .. timestamp .. ", width: " .. width)
  end

  -- otherwise return 0, means no stats, nothing is counted
  return 0
end

local function get_values(key, params)
  local ret = {}
  
  local from = params.from
  local timestamp = params.timestamp

  for i = from, timestamp do
    local a = {}
    
    for j, k in ipairs(params.keys) do
      table.insert(a, get_value(key, k, i, width[params.slot]))
    end
    
    table.insert(ret, a)
  end
  
  return ret
end
  
local function get_value_a(key, k, timestamp, width)
  local ret = {}
   
  local t = math.mod(timestamp, width) 
  local pattern = concat({key, ':', tostring(t), ':', k , '_'})
  local keys = _fwmkeys(pattern)
    
  for i, x in ipairs(keys) do
    local id = _split(x, "_")[2]
    local val = get_value(key, concat({k, '_', id}), timestamp, width)
            
    ret[id] = val 
  end
     
  return ret
end

local function get_list(key, params)
  function combine_stats(s, max)
    local ret = {}
    local temp = {}
    local a = {}
    
    for i, r in ipairs(s) do
      for k, v in pairs(r) do
        if not a[k] then
	  a[k] = tonumber(v)
        else
	  a[k] = a[k] + tonumber(v)
        end
      end
    end
  
    for k, v in pairs(a) do
      table.insert(temp, {k, v})  
    end
  
    table.sort(temp, function(a, b) if a[2] > b[2] then return true end return false end)
  
    for i=1, max do
      if temp[i] then
        table.insert(ret, temp[i])
      end
    end
   
    return ret
  end
  
  function lookup_stats(s, key, tab)
    local ret = {}

    for i, v in ipairs(s) do
      local val = _get(concat({key, ":", tab, ":", v[1]}))
      table.insert(ret, {val, v[2]})
    end  
  
    return ret
  end
 
  local ret = {}
  
  local timestamp = params.timestamp
  
  for i = timestamp, timestamp - (params.count - 1), -1 do
    table.insert(ret, get_value_a(key, params.key, i, width[params.slot]))
  end
  
  local ret = combine_stats(ret, params.max)
  
  if params.lookup then
    ret = lookup_stats(ret, key, params.lookup)
  end
  
  return ret
end

function query(key, value)
  local ret = ""
  
  local query = json.decode(value)
  
  if query and #query == 2 then
    local foo = query[1]
    local params = query[2]
    local tmp = nil
    
    if foo == "get_list" then
      tmp = get_list(key, params)
    elseif foo == "get_values" then
      tmp = get_values(key, params)
    end
    
    if params.encode then
      ret = json.encode(tmp)
    elseif type(tmp) == 'string' then
      ret = tmp      
    end
  end
  
  return ret
end
