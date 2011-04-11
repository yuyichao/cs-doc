#E7.3-2.rb  

module Me
  def sqrt(num, rx=1, e=1e-10)
    num*=1.0
	  (num - rx*rx).abs <e ?	rx :	sqrt(num, (num/rx + rx)/2, e)	
  end
end

class Student  
end

aStudent=Student.new
aStudent.extend(Me)
puts aStudent.sqrt(93.1, 25)
