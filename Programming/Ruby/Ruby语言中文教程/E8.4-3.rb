#E8.4-3.rb

def do_something
   yield  
end

do_something do
  (1..9).each {|i| print  i  if  i<5}
  puts
end

do_something do
  3.times { print  "Hi!" } 
  puts
end
