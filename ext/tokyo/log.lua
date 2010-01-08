-- slots' width 
-- if you simply change slots' width here
-- would old data become invalid??
local width = {
  sec = 64,
  min = 64,
  hour = 32768,
  day = 2048 }

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
    
    local root_key = key .. ':' .. a .. '*'

    _putkeep(root_key, tostring(timestamp))
    local root_timestamp = tonumber(_get(root_key))
    
    --if it is still not number value
    if not root_timestamp then
      _put(root_key, tostring(timestamp))
      root_timestamp = tonumber(_get(root_key))
    end

    local diff = timestamp - root_timestamp
    if diff then
      local row_key = key .. ':' .. b .. ':' .. a
  
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
	  local remove_key = key .. ':' .. tostring(delta) .. ':' .. a
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

local read = {

  get_stats = function (key, value, width)
    local vals = _split(value, "=")

    if not vals or not (#vals == 2) then
      -- error in 'query', -1 means no stas/error
      _log("Error fetching stats 1: " .. key .. ", value: " .. value .. ", width: " .. width)
      return -1 
    end

    local timestamp = tonumber(vals[1])
    local k = vals[2]
 
    local root_key = key .. ':' .. k .. '*'
    local root_timestamp = tonumber(_get(root_key))

    if root_timestamp and timestamp then
      local diff = root_timestamp -  timestamp
      if diff >= 0 and diff < width then
        local t = math.mod(timestamp, width)
        
        row_key = key..':'..tostring(t) .. ':' .. k
 
        local ret = _unpack('i', _get(row_key))
        if ret and #ret == 1 then
          
          return ret[1]
        end
      end
    else
      _log("Error fetching stats 2: "..key..", value: "..value .. ", width: " .. width)
    end

    -- otherwise return 0, means no stats, nothing is counted
    return 0
  end }

function get_slot_sec(key, value)
  return read.get_stats(key, value, width.sec)
end

function get_slot_min(key, value)
  return read.get_stats(key, value, width.min)
end

function get_slot_hour(key, value)
  return read.get_stats(key, value, width.hour)
end

function get_slot_day(key, value)
  return read.get_stats(key, value, width.day)
end
