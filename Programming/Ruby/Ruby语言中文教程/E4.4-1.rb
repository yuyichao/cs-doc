#E4.4-1.rb  

class Person
  
  def initialize( name,age=18 )
    @name = name
    @age = age
    @motherland = "China"
  end
  
  def talk
    puts "my name is "+@name+", age is "+@age.to_s
    if  @motherland == "China"
      puts "I am a Chinese."
    else
      puts "I am a foreigner."
    end
  end
  
 attr_writer :motherland
 
end



class Student < Person
  
  def talk
    puts "I am a student. my name is "+@name+", age is "+@age.to_s
  end  

end

p3=Student.new("kaichuan",25);p3.talk

p4=Student.new("Ben");p4.talk