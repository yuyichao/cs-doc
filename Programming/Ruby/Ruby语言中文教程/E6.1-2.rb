#  E6.1-2.rb 

def sum(*num)
	numSum = 0
	num.each { |i| numSum+=i }
	return numSum
end

puts sum()
puts sum(3,6)
puts sum(1,2,3,4,5,6,7,8,9)
