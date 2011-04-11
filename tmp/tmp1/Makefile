########################################################
#
#Created By YYC Makefile Creator 0.7.9
#
########################################################
CXX = g++ -c -fPIC
CC = gcc -c -fPIC
OPT_FLAGS = -O2
LINK = g++
MAKER = make
outpath = bin
INCLUDE_FLAGS = 
ORG_LIB_FLAGS = 
LIB_FLAGS =  $(ORG_LIB_FLAGS)
LIB_PATH = -L/usr/lib
olist =   $(outpath)/./main.o     
sofilelistc = 
FLAGS = -W -Wall
SO_FLAGS = -shared -fPIC
DEBUG_FLAGS = -g -D_DEBUG_ -UQT_NO_DEBUG
debug_o_list =  $(outpath)/./main_debug.o
debug_so_list = 
target_debug = tmp1_debug
DEBUGER = gdb
DEBUGER_FLAGS = 
hfilelist = 
NEW_DIR =  $(outpath)/.
InstallDir = /usr/bin
target = tmp1
existo = `find -type f -name "*.o"`
TAR = tar --bzip2 -c
TAR_NAME = $(target).tar.bz2
UNTAR = tar --bzip2 -x
BROWSER = nautilus

.PHONY : all
all : $(NEW_DIR) $(outpath)/$(target) $(olist) $(moclist) $(sofilelistc)
$(outpath)/$(target) : $(NEW_DIR) $(olist) $(moclist) $(sofilelistc)
	$(LINK) $(FLAGS) $(OPT_FLAGS) $(LIB_PATH) -o $(outpath)/$(target) $(olist) $(LIB_FLAGS)
.PHONY : debug
debug : $(NEW_DIR) $(outpath)/$(target_debug) $(debug_o_list) $(moclist) $(debug_so_list)
$(outpath)/$(target_debug) : $(NEW_DIR) $(debug_o_list) $(moclist) $(debug_so_list)
	$(LINK) $(FLAGS) $(DEBUG_FLAGS) $(LIB_PATH) -o $(outpath)/$(target_debug) $(debug_o_list) $(debug_so_list) $(ORG_LIB_FLAGS)
$(outpath)/. :
	mkdir -p $(outpath)/.
$(outpath)/./main.o:  main.c
	$(CC) $(FLAGS) $(OPT_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./main.o ./main.c
$(outpath)/./main_debug.o:  main.c
	$(CC) $(FLAGS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -o $(outpath)/./main_debug.o ./main.c
.PHONY : clean cleano cleanmoc cleanso cleantarget distclean cleanold
cleanold :
	rm `find -name "*~"` 2> /dev/null || true
clean : cleanold cleano cleanmoc cleanso cleantarget cleandebug
	for pathname in `find -depth -type d` ; do if [ "`ls -A1 $${pathname} | wc -l`" = "0" ] ; then rmdir $${pathname} ; fi ; done
cleanmoc :
	rm $(moclist) 2> /dev/null || true
cleano :
	rm $(existo) 2> /dev/null || true
cleantarget :
	rm $(outpath)/$(target) 2> /dev/null || true
.PHONY : cleandebug
cleandebug :
	rm $(outpath)/$(target_debug) 2> /dev/null || true
cleanso :
distclean : clean
	rm Makefile
.PHONY : run rundebug
run : debug
	$(outpath)/$(target_debug)
rundebug : debug
	$(DEBUGER) $(DEBUGER_FLAGS) $(outpath)/$(target_debug)
.PHONY : install uninstall
install : all
	mkdir -p $(InstallDir)
	cp $(outpath)/$(target) $(InstallDir)/$(target)
uninstall :
	rm $(InstallDir)/$(target) &> /dev/null || true
	if [ "`ls -A1 $(InstallDir) | wc -l `" = "0" ] ; then rmdir $(InstallDir) ; fi
.PHONY : tar ctar untar
tar :
	$(TAR) * > $(TAR_NAME)
untar :
	cat $(TAR_NAME) | $(UNTAR)
ctar :
	rm $(TAR_NAME) 2> /dev/null || true
.PHONY : bro
bro :
	$(BROWSER) .
