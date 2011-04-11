#E6.4-1.rb  

class Person  
  def talk
    puts "Hello! "   
  end  
end

p1=Person.new
p2=Person.new

  def p2.talk
    puts "Here is p2. "   
  end  
  
  def p2.laugh
    puts "ha,ha,ha... "   
  end  
  
p1.talk
p2.talk
p2.laugh
