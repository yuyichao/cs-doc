#  E3.10-5.rb   演示break, next, redo, retry

puts "演示break"
c='a'
  for i in 1..4
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      break
    end
    print i,c," "
  end
 puts "\n\n"
 
puts "演示next" 
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      next
    end
    print i,c," "
  end
  puts "\n\n"
  
puts "演示redo" 
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      redo
    end
    print i,c," "
  end
  puts "\n\n"
  
  
puts "演示retry"
c='a'
  for i in 1..4    
    if i == 2 and c =='a'
      c = 'b'
      print "\n"
      retry
    end
    print i,c," "
  end
 puts "\n\n"
