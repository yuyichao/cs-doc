#E6.2-1.rb

class Person  
   def talk(name)
    print "my name is #{name}."
  end 
end

class Student < Person  
  def talk(name)
    super
    print "and I'm a student.\n"
  end  
end

aPerson=Person.new
aPerson.talk("kaichuan")
print "\n\n"

aStudent=Student.new
aStudent.talk("kaichuan")
