#E6.3-2.rb  

class StudentClass 
  @@count=0
  
  def initialize
    @@count+=1
  end  
  
  def StudentClass.student_count
    puts "This class have #@@count students."
  end  

end

p1=StudentClass.new
p2=StudentClass.new
StudentClass.student_count

p3=StudentClass.new
p4=StudentClass.new
StudentClass.student_count
