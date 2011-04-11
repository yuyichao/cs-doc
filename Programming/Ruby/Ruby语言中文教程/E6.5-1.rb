#E6.5-1.rb  

class Person    
  def talk
    puts "    public :talk,   将调用speak"
    speak
  end  
  
  def speak
    puts "protected:speak,将调用laugh"
    laugh
  end  
  
  def laugh
    puts "   private:laugh"   
  end 
  
  protected :speak
  private     :laugh
end

p1=Person.new
p1.talk
#p1.speak
#p1.laugh
