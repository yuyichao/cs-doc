#E9-2.rb

require "E9-1"

class Person < MetaPerson 
 
end

person1 = Person.new
person2 = Person.new

person1.sleep
person1.running

person1.modify_method("sleep", "puts 'ZZZ...'")
person1.sleep
person2.sleep