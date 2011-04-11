#E7.4-3.rb  
require "E7.4-1"
require "E7.4-2"

class Student < Person
  include Me
end

aStudent=Student.new
aStudent.talk
puts aStudent.sqrt(77,2)
