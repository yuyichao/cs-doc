#E8.4-6.rb

class Array  
  def  one_by_one 
    for i in 0...size  
      yield(self[i] )  
    end 
    puts    
  end  
end

arr = [1,3,5,7,9]
arr.one_by_one {|k| print  k   , ", "}
arr.one_by_one {|h| print h*h, ", "}
