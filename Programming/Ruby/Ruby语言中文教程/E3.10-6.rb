#  E3.10-6.rb          求50以内的素数
for i in 2..50      #50以内
  f=true            
  for p in 2...i
    if  i%p==0 
      f=!f
      break
    end
  end
  print i," " if f 
end
