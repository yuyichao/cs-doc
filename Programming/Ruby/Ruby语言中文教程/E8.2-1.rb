#E8.2-1.rb

str1='this is str1'
str2="this is str2"
str3=%q[this is str3]
str4=%Q{this is str4}
str5=<<OK_str
  Here is string document, str5
     line one;
     line two;
     line three.
  OK
OK_str

puts str3
puts str4
puts str5