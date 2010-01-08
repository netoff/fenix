#!/usr/bin/ruby
require 'net/http'
require 'uri'

SITE_ID = "nx3456"
TRACK_URI = "http://kliknik.com/app/lg?e=view"

srand()

i = 0
while true do
	n = rand(15)
	(0..n).each do 
		t = rand(100)
		
		last_view = nil
		
		case t % 8
		when 0
			#new visitor
			last_view = -1
		when 1..3
			#same visit
			last_view = 30
		when 4..7
			#repeating visit
			last_view = 123333
		end
		
		response = Net::HTTP.get(URI.parse("#{TRACK_URI}&id=#{SITE_ID}&tm=#{last_view}"))
		puts response
	end
	sleep(0.8)
	i += 1
end
