#E4.3-1.rb  
class Person
  
  def initialize( name,age=18 )
    @name = name
    @age = age
    @motherland = "China"
  end
  
  def talk
    puts "my name is "+@name+", age is "+@age.to_s
    if  @motherland == "China"
      puts "I\'m Chinese."
    else
      puts "I\'m foreigner."
    end
  end
  
  attr_writer :motherland

  
end

p1=Person.new("kaichuan",20)
p1.talk

p2=Person.new("Ben")
p2.motherland="ABC"
p2.talk