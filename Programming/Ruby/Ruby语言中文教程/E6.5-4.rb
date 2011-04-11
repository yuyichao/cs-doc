#E6.5-4.rb  

class Person    
  private    #后面的方法设定为private
  def talk
    puts " already talk "
  end     
end

p1=Person.new
#p1.talk      private方法不能访问

class Person      
  public :talk
end

p1.talk
