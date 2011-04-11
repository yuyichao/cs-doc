#E8.4-1.rb

def one_block
  yield
  yield
  yield
end

one_block { puts "This is a block. " }