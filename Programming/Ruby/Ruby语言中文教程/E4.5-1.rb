#E4.5-1.rb  

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

class Worker < Person  
  def talk
    puts "I am a worker. my name is "+@name+", age is "+@age.to_s
  end  
end

p5=Worker.new("kaichuan",30);p5.talk
p6=Worker.new("Ben");p6.talk