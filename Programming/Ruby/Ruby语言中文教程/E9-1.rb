#E9-1.rb

class MetaPerson 
  
  def MetaPerson.method_missing(methodName, *args)  
    name = methodName.to_s
    begin  
      class_eval(%Q[       
      def #{name}
        puts '#{name}, #{name}, #{name}...'
      end      
     ])       
    rescue
      super(methodName, *args)  
    end
  end
  
  def method_missing(methodName, *args)      
    MetaPerson.method_missing(methodName, *args) 
    send(methodName)
  end
  
  def MetaPerson.modify_method(methodName, methodBody)     
    class_eval(%Q[
      def #{methodName}
        #{methodBody}
      end      
     ])
  end

  def modify_method(methodName, methodBody)    
    MetaPerson.modify_method(methodName, methodBody)    
  end
  
end
