width = 8

function incr(key, value)
  local args = _split(value, "=")

  if args and #args == 2 then
    local timestamp = tonumber(args[1])
    
    if timestamp then
      local vals = _split(args[2], '|')

      for i, v in pairs(vals) do
        local k = _split(v, ':')
      
        if k and #k == 2 then
          a = k[1]
          b = k[2]

          local root_key = key .. ':' .. a .. '*'
          
          _putkeep(root_key, tostring(timestamp))
          local root_timestamp = tonumber(_get(root_key))

          --root_timestamp still could be not a number 
          --test diff again
          local diff = timestamp - root_timestamp
          if diff then
            local row_key = key .. ':' .. a .. ':' .. b
            
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
              --invalidate data in the buffer
              local last = tonumber(b)
              local delta = last - diff
               
              if delta < 0 then
                delta = width + delta
              end
             
              delta = math.mod(delta + 1, width) 
              while delta ~= last do
                local remove_key = key .. ':' .. a .. ':' .. tostring(delta)
                _out(remove_key)
                delta = math.mod(delta + 1, width)
              end
              _out(row_key)
            end
            return _addint(row_key, 1)
          else
            _log("Error in circular buffer diff calculation: " .. tostring(diff))
          end  
        end
      end
    end
  end
end

function get_stat(key, value)
  local timestamp = tonumber(value) 
  local root_key = key .. '*'
  local root_timestamp = tonumber(_get(root_key))

  if root_timestamp and timestamp then
    local diff = root_timestamp -  timestamp
    if diff >= 0 and diff < width then
      local t = math.mod(timestamp, width)
      key = key..':'..tostring(t)
      local ret = _unpack('i', _get(key))
      if ret and #ret == 1 then
        return ret[1]
      end
    end
  else
    _log("Error fetching stas: "..key..", value: "..value)
  end
  return 0
end
