#   E3.9-1.rb
x=3
case x
  when 1...2
    print "x=",x,";在  1...2中"
  when 4..9,0
    print "x=",x,";在  4..9,0中,或是0"
  else
    print  "x=",x,";其它可能"
end
