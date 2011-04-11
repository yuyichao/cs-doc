#E7.4-4.rb  
load "E7.4-1.rb"

class Student  
end

aStudent=Student.new
aStudent.extend(Me)
puts aStudent.sqrt(100.1, 12)
