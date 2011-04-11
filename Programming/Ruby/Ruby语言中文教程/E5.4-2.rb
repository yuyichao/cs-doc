#   E5.4-2.rb    
class Person    
  def talk
    puts "Today is Saturday. "    
  end  
end

p1=Person.new
p1.talk

class Person    
  undef :talk
end
#p1.talk