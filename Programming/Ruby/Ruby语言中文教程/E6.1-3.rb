#  E6.1-3.rb 


def talk(a)
    puts "This is talk version 1."    
end
  
def talk(a,b=1)
    puts "This is talk version 2."    
end
  
talk(2)
talk(2,7)
