compiler = g++
includepath = 
olist =  bin/./carrots bin/./convert bin/./getinfo bin/./myfirst bin/./ourfunc bin/./sqrt 
arglist = -W

.SUFFIXES : .cpp .c
all : makedir $(olist)
makedir :
	mkdir -p ./bin/.
bin/% : %.c
	$(compiler) $(arglist) $(includepath)  $< -o $@
bin/% : %.cpp
	$(compiler) $(arglist) $(includepath)  $< -o $@
clean : cleano
	rm -rf bin 2> /dev/null
