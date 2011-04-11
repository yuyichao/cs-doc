#   E3.8-1.rb      赋值和关系运算符
print "赋值\n" 
a = 1 ;b =  2 + 3           ;print "a=",a," b=",b,"\n"           
a ,b = b ,a                    ;print "a=",a," b=",b,"\n"   
a = b = 1 + 2 + 3          ;print "a=",a," b=",b,"\n"      
a = (b = 1 + 2) + 3       ;print "a=",a," b=",b,"\n"     
x = 0                           ;print "\n"   
a,b,c = x, (x+1), (x+2)  ;print "a=",a," b=",b," c=",c,"\n"   

print "关系运算符\n" 
a=1;  b=1.0;  puts a==b
a=1;  b=1.0;  puts a.eql?(b)
a=1.0;  b=1.0;  puts a.equal?(b)
a=1.0;  b=a ;  puts a.equal?(b)
puts "aab" <=> "acb"
puts [5] <=> [4,9]
puts  (0..9)=== 3.14
puts  ('a'..'f')=== 'c'
