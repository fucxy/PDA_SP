.SUFFIXES: .cc .o
SHELL=/bin/sh
CXX=g++
DEBUG= -g
OPT= -O2 -DNDEBUG
CXXFLAGS= -c $(DEBUG) $(OPT)
LDFLAGS=

###########################################################################

LIBS = -lstdc++
OBJS = fplan.o sa.o
B_OBJS  = sp.o main.o $(OBJS)
SRCS = ${OBJS:%.o=%.cc}
all:    SP{

btree: $(B_OBJS)
        $(CXX) -o SP $(B_OBJS) $(LIBS) $(LDFLAGS)

%.o : %.cc %.h fplan.h sp.h
        $(CXX) $*.cc $(CXXFLAGS)

%.o : %.cc  fplan.h sp.h
        $(CXX) $*.cc $(CXXFLAGS)

clean:
        rm -f *.o SP *~

compact : SP
        strip $?
