#  E3.10-7.rb     ÑÝÊ¾times, upto, downto, each, step

3.times { print  "Hi!" }   
puts
1.upto(9) {|i| print  i  if  i<7 }   
puts
9.downto(1){|i| print  i  if  i<7 }  
puts
(1..9).each {|i| print  i  if  i<7}    
puts
0.step(11,3) {|i| print  i  } 
