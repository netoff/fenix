#!/usr/bin/ruby
require 'net/http'
require 'uri'

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

SITE_ID = "a10007SN"
TRACK_URI = "http://log.kliknik.my/lg/view?_e=hit"

COUNTRIES = ["US", "US", "US", "US", "GB", "GB", "RS", "RS", "UA", "TR", "RO", "NO", "ME", "IN", "HU"]
TITLES = ["Page One", "Page Two", "Page Three", "Page Three"]
URLS = ["mysite.com/page1", "mysite.com/page1?a=12&b=31", "mysite.com/page2/index", "mystie.com/page3", "mystie.com/page3"]
REFERRERS = ["", "www.techcrunch.com/url1/url2?a=34&b=76&c=2010", "topblogs.com", "blogaaa.com/2010-01-01/abcdefghijklmnopqrst"]
VISITS = [-1, 30, 30, 30, 30, 2000, 2000, 200000, 200000]

def aparam(a)
	return URI.escape(a[rand(a.size)].to_s())
end

srand()

i = 0
while true do
	n = rand(15)
	(0..n).each do 
		request_url = TRACK_URI + "&_id=" + SITE_ID + 
			"&_tm=" + aparam(VISITS) + "&_u=" + aparam(URLS) + "&_t=" + 
			aparam(TITLES) + "&_r=" + aparam(REFERRERS) + "&_c=" + aparam(COUNTRIES)
		
		begin
		  response = Net::HTTP.get(URI.parse(request_url))
		  puts "-> Log"
	  	rescue
		end
	end
	
	sleep(0.8)
	i += 1
end
