#E8.4-4.rb

def method(pr)
    puts pr.call(7)  
end

oneProc=proc{|k|   k *=3 }
method(oneProc)
