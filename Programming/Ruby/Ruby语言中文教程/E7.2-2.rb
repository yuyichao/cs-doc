#E7.2-2.rb 

module Me
  def sqrt(num, rx=1, e=1e-10)
    num*=1.0
	  (num - rx*rx).abs <e ?	rx :	sqrt(num, (num/rx + rx)/2, e)	
  end
end

module Me2
  def Me2.sqrt(*num)
    "This is text sqrt. "
  end
  PI=3.14
end

puts   Math.sqrt(1.23)
puts   Math::PI
puts    Me2.sqrt(55, 66, 77, 88, 99)
puts    Me2::PI

include Me
puts   sqrt(456, 7, 0.01)
