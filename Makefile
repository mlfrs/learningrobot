#
# GNU Makefile for project
#

#
# Define library and header veriables
#

LOCALINC = /usr/local/include/

COMPOPTS = -O0 -c -g -std=c++11
LINKOPTS = -O0 -g -lstdc++ -lm -std=c++11
LINKLIBS = -m64
COMPILER = g++ -m64

#
# Specify our compiler and linker settings
# 

COMPILE = $(COMPILER) $(COMPOPTS) $(INCLUDES)
LINKER = $(COMPILER) $(LINKOPTS) 

#
# Compile main
#

all : main.o someclass.o
	${LINKER} -o mlfrs main.o someclass.o

#
# Compile modules
#

someclass.o : someclass.cc someclass.h
	${COMPILE} someclass.cc

main.o : main.cc
	${COMPILE} main.cc

# clean
#	.PHONY: clean

clean: 
	rm *.o mlfrs

