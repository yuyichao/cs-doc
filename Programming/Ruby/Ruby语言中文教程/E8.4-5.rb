#E8.4-5.rb

def method(n)
  return proc{|i|  n +=i }
end

oneProc=method(3)
puts oneProc.call(9)
puts oneProc.call(5)
