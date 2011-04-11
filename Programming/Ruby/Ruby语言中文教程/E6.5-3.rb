#E6.5-3.rb 

class Person    
  def speak
    "protected:speak  "
  end  
  
  def laugh
    "   private:laugh"   
  end 
  
  protected :speak
  private     :laugh
  
  def useLaugh(another)    
    puts another.laugh
  end  
  
  def useSpeak(another)   
    puts another.speak
  end  
  
end

p1=Person.new 
p2=Person.new

p2.useSpeak(p1)
#p2.useLaugh(p1) 
