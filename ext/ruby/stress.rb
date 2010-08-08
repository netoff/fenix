#!/usr/bin/ruby
require 'net/http'
require 'uri'
require 'cgi'
require 'benchmark'

#UNITED STATES	 US
#UNITED KINGDOM	 GB
#UKRAINE	 UA
#TURKEY	 TR
#SERBIA	 RS
#ROMANIA	 RO
#NORWAY	 NO
#MONTENEGRO	 ME
#INDIA	 IN
#HUNGARY	 HU

MAX_REQUESTS = 2
THREAD_SLEEP = 0.8

TRACK_SERVER = "log.kliknik.com"
SITE_ID = "4c18e3b26c8bdc2ac6c8bf67"

if ARGV[0]
	TRACK_SERVER = ARGV[0]
end

if ARGV[1]
	SITE_ID = ARGV[1]
end

TRACK_URI = "http://" + TRACK_SERVER + "/lg/js?_e=hit"

COUNTRIES = [
	["US", "New York", -73.9667, 40.7833], 
	["US", "Washington", -77.0333, 38.8833],
	["US", "San Francisco", -122.4333, 37.7833], 
	["US", "Austin", -97.7333, 30.2667], 
	["GB", "London", 0.0, 51.4833], 
	["GB", "Glasgow", -4.2833, 55.8667], 
	["RS", "Belgrade", 20.4667, 44.8], 
	["RS", "Belgrade", 20.4667, 44.8], 
	["UA", "", 0, 0], 
	["TR", "Istanbul", 28.8333, 40.9667], 
	["RO", "Bucharest", 26.1, 44.4167], 
	["NO", "Oslo", 10.7333, 59.9333], 
	["ME", "", 0, 0], 
	["IN", "New Delhi", 77.2, 28.5833], 
	["HU", "Budapest", 19.0333, 47.5167]]
	
TITLES = ["Page One", "Page Two", "Page Three", "Page Three"]
URLS = ["http://kliknik.com/", "http://kliknik.com/", "http://demo.kliknik.com/page1", "http://demo.kliknik.com/page1?a=12&b=31", "http://demo.kliknik.com/page2/index", "http://demo.kliknik.com/page3", "http://demo.kliknik.com/page3"]
REFERRERS = [
#direct
	"", "", "",
	#search engines
	"http://www.google.com/search?q=query1", "http://www.google.com/search?q=query2", "http://www.google.com/search?q=query2", 
	"http://search.yahoo.com/search?p=query1","http://search.yahoo.com/search?p=queryAB", 
	#referrers
	"http://www.techcrunch.com/url1/url2?a=34&b=76&c=2010", "http://topblogs.com", "http://blogaaa.com/2010-01-01/abcdefghijklmnopqrst"]
VISITS = [-1, 30, 30, 30, 30, 2000, 2000, 200000, 200000]
QUERIES = ["Query 1", "query AA", "query 2", "query 23", "Query 1", "query Query", "one two three", "one two three"]

def aparam(a)
	return CGI.escape(a[rand(a.size)].to_s())
end
def parm(a)
	return a[rand(a.size)]
end

srand()

i = 0
while true do
	threads = []
	
	n = rand(MAX_REQUESTS)
	
	(0..n).each do |j|
		threads << Thread.new(j) do
			while(true) do 
			
				geo = parm(COUNTRIES)
				
				request_url = TRACK_URI + "&_id=" + SITE_ID + 
					"&_tm=" + aparam(VISITS) + "&_u=" + aparam(URLS) + "&_t=" + 
					aparam(TITLES) + "&_r=" + aparam(REFERRERS) + "&_c=" + 
					CGI.escape(geo[0]) + "&_cty=" + CGI.escape(geo[1]) + 
					"&_lg=" + CGI.escape(geo[2].to_s()) + "&_lt="+ CGI.escape(geo[3].to_s()) +
					"&_demo=J8c4USez"#+ "&_q=" + aparam(QUERIES) 
			
				begin
					t = Benchmark.realtime() {
						response = Net::HTTP.get(URI.parse(request_url))
					}
					puts "Request: #{request_url}\nTime: (#{t})s"
				rescue
				end
				
				sleep(THREAD_SLEEP)
			end
	  end
	end
	
	threads.each{|thread| thread.join() }
	
	
	i += 1
end
