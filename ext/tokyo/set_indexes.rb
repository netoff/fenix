#!/usr/bin/ruby

require 'rubygems'
require 'tokyo_tyrant'

HOST = "localhost"
PORT =1980

tt = TokyoTyrant::Table.new(HOST, PORT)

puts "\nSetting index on email column:"
puts tt.set_index('email', :lex)

puts "\nSetting index on type column:"
puts tt.set_index('type', :lex)

puts "\nSetting index on user_id column:"
puts tt.set_index('user_id', :dec)

puts "\nSetting index on key column:"
puts tt.set_index('key', :lex)
