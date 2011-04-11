#E6.5-2.rb  

class Person    
  def speak
    "protected:speak  "
  end  
  
  def laugh
    "   private:laugh"   
  end 
  
  protected :speak
  private     :laugh
end

class Student < Person  
  
  def useLaugh
    puts laugh
  end  
  
  def useSpeak
    puts speak
  end  
  
end
 
p2=Student.new
p2.useLaugh
p2.useSpeak
