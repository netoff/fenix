#!/usr/bin/ruby
require 'net/http'
require 'uri'

SITE_ID = "x123"
TRACK_URI = 'http://hello.local/app/log/hit'

srand()

i = 0
while true do
	n = rand(20)
	(0..n).each do 
		response = Net::HTTP.get(URI.parse("#{TRACK_URI}?s=#{SITE_ID}"))
		puts response
	end
	sleep(0.8)
	i += 1
end
