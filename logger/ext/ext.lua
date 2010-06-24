----------------------------------------------------------
--REQUIRE
----------------------------------------------------------
json = require('json')
require('mongo')
require('search_engines')
require('mailer')
----------------------------------------------------------

----------------------------------------------------------
--DEFINE CONSTANTS
----------------------------------------------------------
--SEC_DATABASE = 'fenix.events.sec'
local DATABASE = 'fenix'
local TOTAL_DATABASE = 'fenix.events.total'
local RAW_DATABASE = 'fenix.events.raw'

local SEC_DATABASE = 'fenix.events.sec'
local MIN_DATABASE = 'fenix.events.min'
local HOUR_DATABASE = 'fenix.events.hour'
local HOUR_DATABASE_SEG = 'fenix.events.hour.segmented'
local DAY_DATABASE = 'fenix.events.day'
local DAY_DATABASE_SEG = 'fenix.events.day.segmented'

local DIRECT_REF = "(direct)"
local UNKNOWN = "(unknown)"

---------------------------------------------------------
--EVENTS
---------------------------------------------------------
local EVENT_HIT = 1--"page hit"--1
local EVENT_NEW_VISITOR = 2--"new visitor"--2
local EVENT_RETURNING_VISITOR = 3--"returning visitor"--3
local EVENT_REPEATING_VISITOR = 4--"repeating visitor"--4
--local EVENT_CLICK
--local EVENT_CUSTOM

local NEW_VISIT_EVENTS = {EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR, EVENT_REPEATING_VISITOR}

local function event_name(e)
	local h = {EVENT_HIT = "page hit", EVENT_NEW_VISITOR = "new visitor",
		EVENT_RETURNING_VISITOR = "returning visitor", EVENT_REPEATING_VISITOR = "repeating visitor"}
	return h[e]
end
--------------------------------------------------------

--MAX_SECS_KEEP = 64
local MAX_RAW_KEEP = 8--seconds

local MAX_SECS_KEEP = 64--seconds
local MAX_MINS_KEEP = 64--minutes
local MAX_HOURS_KEEP = 32--hours
local MAX_DAYS_KEEP = 1024--days

local INC = "{'$inc': {_count: 1}}"

local DB_HOST = "localhost"
--END CONSTANTS
------------------------------------------------------------

local function table_print (tt, indent, done)
done = done or {}
indent = indent or 0
if type(tt) == "table" then

 local sb = {}
 for key, value in pairs (tt) do
   table.insert(sb, string.rep (" ", indent)) -- indent it

   if type (value) == "table" and not done [value] then
     done [value] = true

     table.insert(sb, "{\n");
     table.insert(sb, table_print (value, indent + 2, done))
     table.insert(sb, string.rep (" ", indent)) -- indent it
     table.insert(sb, "}\n");
   elseif "number" == type(key) then

     table.insert(sb, string.format("\"%s\"\n", tostring(value)))
   else
     table.insert(sb, string.format("%s = \"%s\"\n", tostring (key), tostring(value)))
    end

 end
 return table.concat(sb)
else
 return tt .. "\n"
end

end

local function to_string( tbl )
 if  "nil"       == type( tbl ) then
     return tostring(nil)
 elseif  "string" == type( tbl ) then
     return tbl
 elseif  "table" == type( tbl ) then
     return table_print(tbl)
 else
     tostring(tbl)
 end
end

local function scoped_db(func)  
  local db = mongo.Connection.New(DB_HOST)
  local ret = func(db)
  db:done()
	db = nil
	return ret
end

-----------------------------------------------------------------------
-- PURGE FUNCTIONS
-----------------------------------------------------------------------
local function _purge_min(ts)
	if ts then
		local second = tonumber(ts.ts_s)
	  local minute = tonumber(ts.ts_m)
		
		if second and minute then
			scoped_db(function(db)
				db:remove(RAW_DATABASE, {_ts = {['$lt'] = second - MAX_SECS_KEEP}}, false)
				db:remove(SEC_DATABASE, {_ts = {['$lt'] = second - MAX_SECS_KEEP}}, false)
				db:remove(MIN_DATABASE, {_ts = {['$lt'] = minute - MAX_MINS_KEEP}}, false)
			end)
		end
	end
end

local function _purge_hour(ts)
	if ts then
		local hour = tonumber(ts.ts_h)
		
		if hour then	
			scoped_db(function(db)
				db:remove(HOUR_DATABASE, {_ts = {['$lt'] = hour - MAX_HOURS_KEEP}}, false)
				db:remove(HOUR_DATABASE_SEG, {_ts = {['$lt'] = hour - MAX_HOURS_KEEP}}, false)
			end)
		end
	end
end

local function _purge_day(ts)
	if ts then
		local day = tonumber(ts.ts_d)
		
		if day then	
			scoped_db(function(db)
				db:remove(DAY_DATABASE, {_ts = {['$lt'] = day - MAX_DAYS_KEEP}}, false)
				db:remove(DAY_DATABASE_SEG, {_ts = {['$lt'] = day - MAX_DAYS_KEEP}}, false)
			end)
		end
	end
end
------------------------------------------------------------------------------
local function _send_mails()
	local mails = scoped_db(function(db)
		local ret = {}
		
		local j = 1
		local a = db:find_remove(DATABASE, MAIL_QUEUE_TABLE)
		while a and j < 100 do--max 100 in one bach; they could keep adding non-stop
			ret[j] = a 					--while we reading and this would never finish
			j = j + 1
			a = db:find_remove(DATABASE, MAIL_QUEUE_TABLE)
		end
		return ret
	end)
	
	--send all queued mails
	if mails then
		for i, mail in ipairs(mails) do
			_send_mail(MAIL_SENDER, MAIL_FROM, mail.to, mail.subject, mail.body)
		end
	end
end

--put mail on queue to be send later
function enqueue_mail(mail)
	if mail.to and mail.subject and mail.body then
		scoped_db(function(db)
			db:insert(DATABASE.."."..
			MAIL_QUEUE_TABLE, mail)
		end)
	end
end

function send_registration_mail(receiver)
	enqueue_mail({to = receiver, subject = "Welcome to kliknik.com",
		body = REGISTRATION_MAIL_TEXT})	
	enqueue_mail({to= ADMIN_ADDRESS, subject = "New registration", body = "New user registered: "..receiver})
end

function send_activation_mail(receiver, site_id)
	enqueue_mail({to = receiver, subject = "Start using kliknik.com",
		body = string.format(ACTIVATION_MAIL_TEXT, site_id)})
end
-----------------------------------------------------------------------
--CRON TABS
-----------------------------------------------------------------------
cron = {}
function cron._cron_minutely(ts)
	_log("In cron minutely.")
  _purge_min(ts)
	_send_mails()
end

function cron._cron_hourly(ts)
	_log("In cron hourly.")
	_purge_hour(ts)
end

function cron._cron_daily(ts)
	_log("In cron daily.")
	_purge_day(ts)
end
-----------------------------------------------------------------------

-----------------------------------------------------------------------
--INCREMENT FUNCTIONS
-----------------------------------------------------------------------
local function inc_total(db, obj)
	db:update(TOTAL_DATABASE, obj, INC, true, false)
end

local function insert_raw(db, obj)
	db:insert(RAW_DATABASE, obj)
end

local function inc_sec(db, obj)
	db:update(SEC_DATABASE, obj, INC, true, false)
end

local function inc_min(db, obj)
	db:update(MIN_DATABASE, obj, INC, true, false)
end

local function inc_hour(db, obj)
	db:update(HOUR_DATABASE, obj, INC, true, false)
end

local function inc_hour_seg(db, obj)
	db:update(HOUR_DATABASE_SEG, obj, INC, true, false)
end

local function inc_day(db, obj)
	db:update(DAY_DATABASE, obj, INC, true, false)
end

local function inc_day_seg(db, obj)
	db:update(DAY_DATABASE_SEG, obj, INC, true, false)
end
------------------------------------------------------------------------

------------------------------------------------------------------------
--LOG FUNCTIONS
------------------------------------------------------------------------
log = {}
local function _log_page_view(ts, obj, db, landing)
	inc_total(db, {_key = obj.id, _event = EVENT_HIT})
	
	--<url> ::= <scheme>://<authority>/<path>;<params>?<query>#<fragment>
	local url = parse(obj.url)
	
	local domain = ""
	local path = ""
	
	if url.scheme and url.scheme ~= "" 
		and url.authority and url.authority ~= "" then
		domain = url.scheme.."://"..url.authority
	end
	
	if url.path and url.path ~= "" then
		path = url.path
		if url.query and url.query ~= "" then
			path = path.."?"..url.query
			
			if url.fragment and url.fragment ~= "" then
				path = path.."#"..url.fragment
			end
		end
	end
	
	inc_sec(db, {_key = obj.id, _event = EVENT_HIT, _ts = ts.ts_s})
	inc_min(db, {_key = obj.id, _event = EVENT_HIT, _ts = ts.ts_m })
			
	inc_hour(db, {_key = obj.id, _event = EVENT_HIT, _ts = ts.ts_h})
	inc_hour_seg(db, 
		{_key = obj.id, _event= EVENT_HIT, _ts = ts.ts_h, 
		_url = path, _title = obj.title})

	inc_day(db, {_key = obj.id, _event = EVENT_HIT, _ts = ts.ts_d})
	inc_day_seg(db, 
		{_key = obj.id, _event = EVENT_HIT, _ts = ts.ts_d,
		--_dom - domain of page accessed; eg. it could be on subdomain
		_dom = domain, _url = path, _title = obj.title, _land = landing})
end

local function _log_visitor(ts, obj, event_m, event_h)  
	--because session length
	local ev1 = event_m
	local ev2 
	if event_h then
		ev2 = event_h
	else
		ev2 = ev1
	end
	
	scoped_db(function(db)
		_log_page_view(ts, obj, db, true)
		
		inc_total(db, {_key = obj.id, _event = ev2})
		
		local ref = get_search_engine(obj.referrer)
		
		insert_raw(db, {_key = obj.id, _uid = ts.ts_micro, _event = ev2, 
			_country = obj.country, _long = obj.longitude, _lat = obj.latitude, _city = obj.city,
			_url = obj.url, _referrer = ref.referrer, _squery = ref.query, _engine = ref.engine, _ts = ts.ts_s})
	
		inc_sec(db, {_key = obj.id, _event = ev1, _ts = ts.ts_s})
		inc_min(db, {_key = obj.id, _event = ev1, _ts = ts.ts_m})
  
		inc_hour(db, {_key = obj.id, _event = ev2, _ts = ts.ts_h})
		inc_hour_seg(db, 
			{_key = obj.id, _event = ev2, _ts = ts.ts_h, 
				_referrer = ref.referrer, _squery = ref.query, _engine = ref.engine, 
				_country = obj.country, _new_visit = true})
  
		inc_day(db, {_key = obj.id, _event = ev2, _ts = ts.ts_d})
		inc_day_seg(db, 
			{_key = obj.id, _event = ev2, _ts = ts.ts_d, 
				_referrer = ref.referrer, _squery = ref.query, _engine = ref.engine, 
				_country = obj.country, _new_visit = true})
	end)
end

function log.log_page_view(ts, obj)
  scoped_db(function(db)
  	_log_page_view(ts, obj, db, false)	
	end)
end

function log.log_new_visitor(ts, obj)	
	_log_visitor(ts, obj, EVENT_NEW_VISITOR)
end

function log.log_returning_visitor(ts, obj)	
	_log_visitor(ts, obj, EVENT_RETURNING_VISITOR)
end

function log.log_repeating_visitor(ts, obj)	
	_log_visitor(ts, obj, EVENT_RETURNING_VISITOR, EVENT_REPEATING_VISITOR)
end
--------------------------------------------------------------------------
local function float(f)
	local ret = "0.00"
	if tonumber(f) then
		ret = string.format("%.2f", f)
	end
	return ret
end

local function def(a)
  if a then
    if a._count then
      return a._count
    end
  end
  return 0
end

local function hash_array(cursor, filter)
  local ret = {}
  
  if cursor then
    for r in cursor:results() do
			ret[r._ts] = r
    end
  end
  
  return ret
end

local function hash_filtered(cursor, filter)
	local ret = {}
	
  if cursor then
    for r in cursor:results() do
			if filter then
				if filter(r) then
					if ret[r._ts] then
						ret[r._ts] = ret[r._ts] + def(r)
					else
						ret[r._ts] = def(r)
					end
				end
			else
				if ret[r._ts] then
					ret[r._ts] = ret[r._ts] + def(r)
				else
					ret[r._ts] = def(r)
				end
			end
    end
  end
  
  return ret
end

local function hash_array_filtered(array, filter)
	local ret = {}
	
  if array then
    for i, r in ipairs(array) do
			if filter then
				if filter(r) then
					if ret[r._ts] then
						ret[r._ts] = ret[r._ts] + def(r)
					else
						ret[r._ts] = def(r)
					end
				end
			else
				if ret[r._ts] then
					ret[r._ts] = ret[r._ts] + def(r)
				else
					ret[r._ts] = def(r)
				end
			end
    end
  end
  
  return ret
end

local function flatten_list(h, sort, max)
  local ret = {}
  
  local j = 1
  for key,val in pairs(h) do
    ret[j] = {key, val}
    j = j + 1
  end
  
  if sort then
    table.sort(ret, function(a, b) return a[2] > b[2] end)
  end
  
  if max then
    local n = tonumber(max)
    if n and n < #ret then
      local t = {}
      for i =1, n do
        t[i] = ret[i]
      end
      ret = t
    end
  end
  
  return ret
end

local function results_array(cursor, field, filter)
  local ret = {}
	local max = 0
	
	if cursor then
  	local j = 1
  
		for r in cursor:results() do
			if filter then
				if filter(r) then
					ret[j] = r
					j = j + 1
					
					if r[field] > max then
						max = r[field]
					end
				end
			else
				ret[j] = r
				j = j + 1
				
				if r[field] > max then
					max = r[field]
				end			
			end
		end
	end
  return max, ret
end

local function array(cursor)
	local ret = {}
	
	local j = 1
	if cursor then
		for r in cursor:results() do
			ret[j] = r
			j = j + 1
		end
	end
	
	return ret
end

local function single_res(cursor, field)
	local ret = 0
	
	if cursor then
		local res = cursor:next()
		if res then
			if field then
				ret = res[field]
			else
				ret = res._count
			end
		end
	end
	
	return ret
end

local function filter(array, field, val)
	local ret = {}
	local j = 1
	for i, a in ipairs(array) do
		if type(field) == "function" then
			if field(a) then
				ret[j] = a
				j = j + 1
			end
		else
			if a[field] == val then
				ret[j] = a
				j = j + 1						
			end
		end
	end
	return ret
end

local function sum(array, field)
	local ret = 0
	
	for i, a in ipairs(array) do
		if a[field] then
			ret = ret + a[field]
		end
	end
	
	return ret
end

local function def0(a)
	local ret = 0
		if a and tonumber(a) then
			ret = a
		end
	return ret
end

local function merge_arrays(arrays, n)	
	local ret = {}
	if arrays then
		local k = #arrays
		local i, j
		for i = 1,n do
			local t = {}
			for j = 1,k do
				t[j] = def0(arrays[j][i])	
			end
			ret[i] = t
		end
	end
	return ret
end

local function sum_arrays(arrays, n)
	local ret = {}
	if arrays then
		local i, j
		local k = #arrays
		for i = 1,n do
			local a = 0
			for j = 1,k do
				a = a + def0(arrays[j][i])
			end
			ret[i] = a
		end
	end
	return ret
end

local function sum_array(array)
	local ret = 0
		for i,v in ipairs(array) do
			ret = ret + v
		end
	return ret
end
----------------------------------------------------------------------
--FEED FUNCTIONS
----------------------------------------------------------------------
local function is_direct(r)
	return not r._referrer or r._referrer == ""
end
local function is_search(r)
	return r._engine and r._engine ~= "" and r._squery and r._squery ~= ""
end
local function is_referred(r)
	return r._referrer ~= "" and (not r._squery or r._squery == "") 
			and (not r._engine or r._engine == "")
end
local function is_geo(r)
	return r._country and r._country ~= ""
end

local function _get_stats_per(q, slot, max, events)
	local ret = scoped_db(function(db)
		local ret = {}
	
		local timestamp1 = tonumber(q.ts1)
		local timestamp2 = tonumber(q.ts2)
		local site_id = q.id
	
		if site_id and timestamp1 and timestamp2 then
			local diff = timestamp2 - timestamp1
		
			if diff <= max then
				local a = {}
				for n, ev in ipairs(events) do
					a[n] = hash_array(db:query(slot,
									{_key = site_id, _event = ev, 
										_ts = {['$gte'] = timestamp1, ['$lte'] = timestamp2}}))
				end
	
				local j = 1
				for i = timestamp1,timestamp2 do
					local t = {}
					for n, v in ipairs(a) do
						t[n] = def(v[i])
					end
					ret[j] = t
					j = j + 1
				end
			end
		end
		return ret
	end)
	return ret
end

local function _get_stats_filtered(q, slot, max, events, filter)
	local ret = scoped_db(function(db)
		local ret = {}
		
		local diff = q.ts2 - q.ts1
		if diff <= max then
		
			local a = hash_filtered(db:query(slot, 
				{_key = q.id, _event = {['$in'] = events}, 
					_ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}}), filter)
					
			local j = 1
			
			for i = q.ts1, q.ts2 do
				ret[j] = def0(a[i])
				j = j + 1
			end			
		end
		
		return ret
	end)
	
	return ret
end

local function _stats_filter(q, array, filter)
	local ret = {}
	
	
	local a = hash_array_filtered(array, filter)
			
	local j = 1
	
	for i = q.ts1, q.ts2 do
		ret[j] = def0(a[i])
		j = j + 1
	end
	
	return ret	
end

dashboard = {
	get_stats_per_sec = function (q)
		local ret = _get_stats_per(q, SEC_DATABASE, 60, 
			{EVENT_HIT, EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR})
		
		return json.encode(ret)	
	end,
	
  get_stats_per_min = function (q)	
	 	local ret = _get_stats_per(q, MIN_DATABASE, 60,
			{EVENT_HIT, EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR})
		
		return json.encode(ret)
  end,
	
	get_stats_per_hour = function (q)
		local ret = _get_stats_per(q, HOUR_DATABASE, 24,
			{EVENT_HIT, EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR})
		
		return json.encode(ret)
	end,
  
  get_countries_list = function(q)
		local ret = scoped_db(function(db)
    	local t = {}
    
			local timestamp = tonumber(q.ts)
			local site_id = q.id
    
			if site_id and timestamp then
				local countries = db:query(HOUR_DATABASE_SEG, 
					{_key = site_id, _new_visit = true, 
						_ts= {['$gt'] = timestamp - 24, ['$lte'] = timestamp}})
  
				if countries then 
        	for c in countries:results() do
						if c._country and c._country ~= "" then
          		if t[c._country] then
								t[c._country] = t[c._country] + c._count 
							else
								t[c._country] = c._count
							end
						end
					end
				end
			end
  
			return flatten_list(t, true, 10)
		end)
		
		return ret
  end,
  
  get_pages_list = function(q)
		local ret = scoped_db(function(db)
    	local t = {}
    
			local timestamp = tonumber(q.ts)
			local site_id = q.id
    
			if site_id and timestamp then
      	local pages = db:query(HOUR_DATABASE_SEG, 
					{_key = site_id, _event = EVENT_HIT, 
						_ts = {['$gt'] = timestamp - 24, ['$lte'] = timestamp}})
    
				if pages then
        	for p in pages:results() do
						local url = p._url
						if url and url ~= "" then
          		if t[url] then
								t[url] = t[url] + p._count 
							else
								t[url] = p._count
							end
						end
					end
				end
			end
    
			return flatten_list(t, true, 10)
		end)
		return ret
  end,
  
  get_referrers_list = function(q)
		local ret = scoped_db(function(db)
    	local t = {}
    
			local timestamp = tonumber(q.ts)
			local site_id = q.id
    
			if site_id and timestamp then 
      	local referrers = db:query(HOUR_DATABASE_SEG, 
					{_key = site_id, _new_visit = true, 
						_ts = {['$gt'] = timestamp - 24, ['$lte'] = timestamp}})
    
				if referrers then 
        	for r in referrers:results() do
						if r._referrer then
							local ref
							if is_search(r) then
								ref = r._engine
							else
								if r._referrer == "" then
									ref = DIRECT_REF
								else
									ref = r._referrer
								end
							end
						
							if t[ref] then
								t[ref] = t[ref] + r._count
							else
								t[ref] = r._count
							end
						end
					end
				end
			end
    
			return flatten_list(t, true, 10)
		end)
		return ret
  end,
  
  get_queries_list = function(q)
		local ret = scoped_db(function(db)
    	local t = {}
    
			local timestamp = tonumber(q.ts)
			local site_id = q.id
    
			if site_id and timestamp then
      	local queries = db:query(HOUR_DATABASE_SEG, 
					{_key = site_id, _new_visit = true, 
						_ts = {['$gt'] = timestamp - 24, ['$lte'] = timestamp}})
      
				if queries then
        	for q in queries:results() do
						local query = q._squery
						if query and query ~= "" then
          		if t[query] then
								t[query] = t[query] + q._count
							else
								t[query] = q._count 
							end
						end
					end
				end
			end
    
			return flatten_list(t, true, 10)
		end)
		
		return ret
  end,
	
	get_counters = function(q)
		local ret = scoped_db(function(db)
			local ret = {}
			
			local site_id = q.id
			local timestamp = tonumber(q.ts_d)
			
			if site_id and timestamp then
				local new_visitors = single_res(db:query(DAY_DATABASE,
					{_key = site_id, _event = EVENT_NEW_VISITOR, _ts = timestamp}))
				local returning_visitors = single_res(db:query(DAY_DATABASE,
					{_key = site_id, _event = EVENT_RETURNING_VISITOR, _ts = timestamp}))
				local repeating_visitors = single_res(db:query(DAY_DATABASE,
					{_key = site_id, _event = EVENT_REPEATING_VISITOR, _ts = timestamp}))
				local total_hits = single_res(db:query(TOTAL_DATABASE,
					{_key = site_id, _event = EVENT_HIT}))
				local total_new_visitors = single_res(db:query(TOTAL_DATABASE, 
					{_key = site_id, _event = EVENT_NEW_VISITOR}))
				local total_returning_visitors	= single_res(db:query(TOTAL_DATABASE,
					{_key = site_id, _event = EVENT_RETURNING_VISITOR}))
				local total_repeating_visitors = single_res(db:query(TOTAL_DATABASE,
					{_key = site_id, _event = EVENT_REPEATING_VISITOR}))
				ret = {
							new_visitors + returning_visitors + repeating_visitors, 
							new_visitors, 
							new_visitors + returning_visitors,
							total_hits,
							total_new_visitors + total_returning_visitors + total_repeating_visitors,
							total_new_visitors
							}
			end
			
			return ret
		end)
		
		return ret
	end,
	
	get_visitors = function(q)
		local ret = scoped_db(function(db)
			local ret = {}
			local max = 0
			
			local site_id = q.id
			local timestamp = tonumber(q.ts_micro)
			if site_id and timestamp then
				max, ret = results_array(db:query(RAW_DATABASE, 
					{_key = site_id, _uid = {['$gt'] = timestamp}}), "_uid", is_geo)
				if max == 0 then
					max = timestamp
				end
			end
			
			return {string.format("%.0f", max), ret}
		end)
		return ret
	end,
	
	get_feed = function(q)
		local feed = {}
		
		feed.countries = dashboard.get_countries_list(q)
		feed.pages = dashboard.get_pages_list(q)
		feed.referrers = dashboard.get_referrers_list(q)
		feed.queries = dashboard.get_queries_list(q)
		feed.counters = dashboard.get_counters(q)
		feed.visitors = dashboard.get_visitors(q)
		
		return json.encode(feed)
	end
}
----------------------------------------------------------------------------

----------------------------------------------------------------------------
--VISITORS FEED
----------------------------------------------------------------------------
local _visitors = {}
function _visitors.get_points(q)
	local ret = {}
	
	if q.type == -1 then
		ret = _get_stats_per(q, DAY_DATABASE, 240, {EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR, EVENT_REPEATING_VISITOR})
	elseif q.type == 2 then
		ret = _get_stats_per(q, DAY_DATABASE, 240, {EVENT_NEW_VISITOR})
	elseif q.type == 3 then
		ret = _get_stats_per(q, DAY_DATABASE, 240, {EVENT_RETURNING_VISITOR})
	elseif q.type == 4 then
		ret = _get_stats_per(q, DAY_DATABASE, 240, {EVENT_REPEATING_VISITOR})
	end
	
	return ret
end

function _visitors.get_overview(q)
	local ret = {}
	local diff = q.ts2 - q.ts1 + 1
	ret = scoped_db(function(db)
		local ret = {}	
		local visits = array(db:query(DAY_DATABASE_SEG, 
			{_key = q.id, _event = {['$in'] = {EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR, EVENT_REPEATING_VISITOR}}, 
				_ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}}))
		
		local visits_sum = sum(visits, "_count")
		local visits_avg = visits_sum/diff
		local new_visitors_sum = sum(filter(visits, "_event", EVENT_NEW_VISITOR), "_count")
		local new_visitors_avg = new_visitors_sum/diff
		
		ret = {visits_sum, float(visits_avg), new_visitors_sum, float(new_visitors_avg)}
		return ret
	end)
	
	return ret
end
function _visitors.get_segments(q)
	local ret = {countries = {}}
	
	ret.countries = scoped_db(function(db)
		local ret = {}
		local countries = db:query(DAY_DATABASE_SEG, 
			{_key = q.id, _event = {['$in'] = {EVENT_NEW_VISITOR, EVENT_RETURNING_VISITOR, EVENT_REPEATING_VISITOR}}, 
				_country = {['$exists'] = true}, _ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}})
				
		if countries then
			local t = {}
			for r in countries:results() do
				if r._country then
					local country
					if r._country == "" then 
						country = UNKNOWN 
					else
						country = r._country
					end
					if t[country] then
						if r._event == EVENT_NEW_VISITOR then
							t[country]['new_visits'] = t[country]['new_visits'] + r._count
						end
						t[country]['visits'] = t[country]['visits'] + r._count
					else
						if r._event == EVENT_NEW_VISITOR then
							t[country] = {new_visits = r._count, visits = r._count}
						else
							t[country] = {new_visits = 0, visits = r._count}
						end					
					end
				end
			end
			local j = 1
			for k,v in pairs(t) do
				ret[j] = {name = k, visits = v.visits, new_visits = v.new_visits, 
					percent = float(v.new_visits*100/v.visits)}
				j = j + 1
			end
			table.sort(ret, function(a, b) return a.visits > b. visits end)
		end
		
		return ret
	end)
	
	return ret
end
visitors = {
	get_feed = function(q)
		local feed = {}
		
		if q.id and tonumber(q.ts1) and tonumber(q.ts2) 
					and q.ts1 < q.ts2 and tonumber(q.type) then
			
			feed.type = string.format("%.0f", q.type)
			feed.points = _visitors.get_points(q)
			feed.overview = _visitors.get_overview(q)
			feed.by = _visitors.get_segments(q)
		end
		
		return json.encode(feed)	
	end
}
local _pages = {}
function _pages.get_points(q)
	local ret = {}
	
	if q.type ==  -1 then
		ret = _get_stats_per(q, DAY_DATABASE, 240, {EVENT_HIT})
	end
	
	return ret 
end
function _pages.get_overview(q)
	local ret = {}
	local diff = q.ts2 - q.ts1 + 1
	
	ret = scoped_db(function(db)
		local ret = {}
		local hits = array(db:query(DAY_DATABASE, 
			{_key = q.id, _event = EVENT_HIT, _ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}}))
		local page_hits = sum(hits, "_count")
		local page_hits_avg = page_hits/diff
		
		ret = {page_hits, string.format("%.0f", page_hits_avg)}
		return ret	
	end)
	
	return ret
end
function _pages.get_segments(q)
	local ret = {pages = {}}
	ret.pages = scoped_db(function(db)
		local ret = {}
		local pages = db:query(DAY_DATABASE_SEG, 
			{_key = q.id, _event = EVENT_HIT, _url = {['$exists'] = true}, _ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}})
			
		if pages then
			local t = {}
			for r in pages:results() do
				if t[r._url] then
					t[r._url]['hits'] = t[r._url]['hits'] + r._count
				else
					t[r._url] = {hits = r._count}
				end
			end
			local j = 1
			for k, v in pairs(t) do
				ret[j] = {name = k, hits = v.hits}
				j = j + 1
			end
			table.sort(ret, function(a, b) return a.hits> b.hits end)
		end
		return ret
	end)
	return ret 
end
pages = {
	get_feed = function(q)
		local feed = {}
		if q.id and tonumber(q.ts1) and tonumber(q.ts2) 
			and q.ts1 < q.ts2 and tonumber(q.type) then
			
			feed.type = string.format("%.0f", q.type)
			feed.points = _pages.get_points(q)
			feed.overview = _pages.get_overview(q)
			feed.by = _pages.get_segments(q)
		end
		return json.encode(feed)
	end
}

local _referrers = {}
function _referrers.get_points(q, array)
	local ret = {}
	
	local direct, search, referred
	
	if q.type == 2 or q.type == -1 then
		direct = _stats_filter(q, array, is_direct)
	end
	if q.type == 3 or q.type == -1 then		
		search = _stats_filter(q, array, is_search)
	end
	if q.type == 4 or q.type == -1 then
		referred = _stats_filter(q, array, is_referred)
	end
	
	local n = q.ts2 - q.ts1 + 1
	
	if q.type == -1 then
		--local sum = sum_arrays({direct, search, referred}, n)
		ret = merge_arrays({direct, search, referred}, n)
	elseif q.type == 2 then
		ret = merge_arrays({direct}, n)
	elseif q.type == 3 then
		ret = merge_arrays({search}, n)
	elseif q.type == 4 then
		ret = merge_arrays({referred}, n)
	end
	
	return ret
end
function _referrers.get_overview(q, array)
	
	local new_visits = 0
	local new_visits_avg = 0
	local visits = 0
	local visits_avg = 0
	local diff = q.ts2 - q.ts1 + 1
	
	if q.type == -1 then
		new_visits = sum_array(_stats_filter(q, filter(array, "_event", EVENT_NEW_VISITOR)))
		visits = sum_array(_stats_filter(q, array))		
	elseif q.type == 2 then
		new_visits = sum_array(_stats_filter(q, filter(array, "_event", EVENT_NEW_VISITOR),
			is_direct))
		visits = sum_array(_stats_filter(q, array, is_direct))
	elseif q.type == 3 then
		new_visits = sum_array(_stats_filter(q, filter(array, "_event", EVENT_NEW_VISITOR),
			is_search))
		visits = sum_array(_stats_filter(q, array, is_search))
	elseif q.type == 4 then
		new_visits = sum_array(_stats_filter(q, filter(array, "_event", EVENT_NEW_VISITOR), 
			is_referred))
		visits = sum_array(_stats_filter(q, array, is_referred))
	end
	
	if new_visits and visits then
		new_visits_avg = new_visits/diff
		visits_avg = visits/diff
	end
	
	return {visits, float(visits_avg), new_visits, float(new_visits_avg)}
end

function _referrers.get_segments(q, array)
	local ret = {referrers = {}}
	
	local a
	if q.type == -1 then
		a = array
	elseif q.type == 2 then
		a = filter(array, is_direct)
	elseif q.type == 3 then
		a = filter(array, is_search)
	elseif q.type == 4 then
		a = filter(array, is_referred)
	end
	
	local t = {}
	for i, r in ipairs(a) do
		local ref = r._referrer
		if ref then
			if ref == "" then ref = DIRECT_REF end
			if t[ref] then
				if r._event == EVENT_NEW_VISITOR then
					t[ref]['new_visits'] = t[ref]['new_visits'] + r._count
				end
				t[ref]['visits'] = t[ref]['visits'] + r._count
			else
				if r._event == EVENT_NEW_VISITOR then
					t[ref] = {new_visits = r._count, visits = r._count}
				else
					t[ref] = {new_visits = 0, visits = r._count}	
				end
			end
		end
	end
	local j = 1
	for k, v in pairs(t) do
		ret.referrers[j] = {name = k, visits = v.visits, new_visits = v.new_visits, 
			percent = float(v.new_visits*100/v.visits)}
		j = j + 1		
	end
	table.sort(ret, function(a, b) return a.visits > b.visits end)
	
	return ret
end
referrers = {
	get_feed = function(q)
		local feed = {}
		
		if q.id and tonumber(q.ts1) and tonumber(q.ts2)
			and q.ts1 < q.ts2 and tonumber(q.type) then
			
			feed.type = string.format("%.0f", q.type)
			
			local array = scoped_db(function(db)
				return array(db:query(DAY_DATABASE_SEG, 
				{_key = q.id, _event = {['$in'] = NEW_VISIT_EVENTS}, 
					_referrer = {['$exists'] = true}, _ts = {['$gte'] = q.ts1, ['$lte'] = q.ts2}}))
			end)
			
			feed.points = _referrers.get_points(q, array)
			feed.overview = _referrers.get_overview(q, array)
			feed.by = _referrers.get_segments(q, array)
		end
		return json.encode(feed)
	end
}
----------------------------------------------------------------------------
