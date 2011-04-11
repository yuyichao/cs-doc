#E8.1-3.rb

arr=[4,5,6]     
print arr.join(", "),"\n"

arr[4] = "m"
print arr.join(", "),"\n"
print arr[3] ,"\n"

arr.delete_at(3)     
print arr.join(", "),"\n"

arr[2] = ["a","b","c"]  
print arr.join(", "),"\n"
print arr[2] ,"\n"

arr[0..1] = [7,"h","b"]  
print arr.join(", "),"\n"

arr.push("b" )    
print arr.join(", "),"\n"


arr.delete(["a","b","c"] )     
print arr.join(", "),"\n"
arr.delete("b")     
print arr.join(", "),"\n"

arr.insert(3,"d")
print arr.join(", "),"\n"

arr<<"f"<<2         
print arr.join(", "),"\n"
arr.pop          
print arr.join(", "),"\n"
arr.shift          
print arr.join(", "),"\n"

arr.clear
print arr.join(", "),"\n"