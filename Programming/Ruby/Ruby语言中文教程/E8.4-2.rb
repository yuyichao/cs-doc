#E8.4-2.rb

def one_block
  for num in 1..3
    yield(num)
  end
end

one_block do |i|
  puts "This is  block #{i}. " 
end