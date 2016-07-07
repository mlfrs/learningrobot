#
# GNU Makefile for project
#

#
# Define library and header veriables
#
LOCALINC = /usr/local/include/

#
# compiler options
#

COMPOPTS = -O2 -c -g -std=c++11
LINKOPTS = -O2 -g -lstdc++ -lm -std=c++11
COMPILER = g++ -m64

# 
# pkg-config flags
#

PROTOBUF = 'protobuf'
SIMBODY = 'simbody'

#
# Compiler flags
#

LD_FLAGS  = $(shell pkg-config --libs $(PROTOBUF) $(SIMBODY))
CXX_FLAGS  = $(shell pkg-config --cflags $(PROTOBUF) $(SIMBODY))

#
# Specify compiler and linker settings
# 

COMPILE = $(COMPILER) $(COMPOPTS) $(INCLUDES) $(CXX_FLAGS)
LINKER = $(COMPILER) $(LINKOPTS) $(LD_FLAGS)

#
# Compile main / mlfrs
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

clean : 
	rm *.o mlfrs

