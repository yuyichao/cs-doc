#E8.3-3.rb

strdoc=<<DOC_EOF
This is windows2000 or windows98 system.
Windows system is BEST?
Windows2000 running in 12-31-2006,бнбн
DOC_EOF

re = /[w|W]indows(?:98|2000) /
strdoc.gsub!(re, "Windows XP ") 
re = /[1-9][0-9]\-[1-9][0-9]\-\d\d\d\d/
time = Time.now.strftime("%m-%d-%Y")
strdoc.gsub!(re, time) 
puts strdoc
