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

COMPOPTS = -O0 -c -g -std=c++11
LINKOPTS = -O0 -g -lstdc++ -lm -std=c++11
COMPILER = g++ -m64 -o $(BUILDDIR)$@
BUILDDIR = 'build/'

# 
# pkg-config flags
#

PKG_CONFIG_PATH = '/usr/lib/pkgconfig:/usr/local/lib/pkgconfig/'

PROTOBUF = 'protobuf'
SIMBODY = 'simbody'
JSONCPP = 'jsoncpp'
EIGEN = 'eigen3'

#
# Compiler flags
#

LD_FLAGS  = $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --libs $(PROTOBUF) $(SIMBODY) $(JSONCPP) $(EIGEN))
CXX_FLAGS  = $(shell PKG_CONFIG_PATH=$(PKG_CONFIG_PATH) pkg-config --cflags $(PROTOBUF) $(SIMBODY) $(JSONCPP) $(EIGEN))

#
# Specify compiler and linker settings
# 

COMPILE = $(COMPILER) $(COMPOPTS) $(INCLUDES) $(CXX_FLAGS)
LINKER = $(COMPILER) $(LINKOPTS) $(LD_FLAGS)

#
# Compile main / mlfrs
#

all : main.o someclass.o physics.o mlfrs_simbody.o model_parser.o object_manager.o
	${LINKER} -o mlfrs $(BUILDDIR)main.o $(BUILDDIR)someclass.o $(BUILDDIR)physics.o $(BUILDDIR)mlfrs_simbody.o $(BUILDDIR)model_parser.o $(BUILDDIR)object_manager.o

#
# Compile modules
#

someclass.o : someclass.cc someclass.h
	${COMPILE} someclass.cc

physics.o : physics/physics.cc physics/physics.h
	${COMPILE} physics/physics.cc

mlfrs_simbody.o : physics/mlfrs_simbody.cc physics/mlfrs_simbody.h
	${COMPILE} physics/mlfrs_simbody.cc

model_parser.o : model/mdl_parser.cc model/mdl_parser.h 
	${COMPILE} model/mdl_parser.cc

object_manager.o : object_manager.cc object_manager.h 
	${COMPILE} object_manager.cc

main.o : main.cc 
	${COMPILE} main.cc

clean : 
	-@rm build/*.o mlfrs 2>/dev/null || true

