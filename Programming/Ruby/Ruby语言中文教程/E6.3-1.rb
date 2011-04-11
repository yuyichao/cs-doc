#E6.3-1.rb  

class StudentClass 
  @@count=0
  def initialize( name )
    @name = name
    @@count+=1
  end  
  
  def talk
    puts "I am #@name, This class have #@@count students."
  end  

end

p1=StudentClass.new("Student 1 ")
p2=StudentClass.new("Student 2 ")
p3=StudentClass.new("Student 3 ")
p4=StudentClass.new("Student 4 ")
p3.talk
p4.talk
