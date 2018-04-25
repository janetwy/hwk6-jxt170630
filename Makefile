#
# CS 3377.502 
# Janet Tang - jxt170630@utdallas.edu
# homework 6 - makefile
#

CXX = g++
CXXFLAGS = -Wall -Wno-write-strings -g
CPPFLAGS = -I/scratch/perkins/include
LDFLAGS = -L/scratch/perkins/lib
LDLIBS = -lcdk -lcurses

EXECFILE = prog6

OBJS = prog6.o


all: $(EXECFILE)

clean:	
	rm -f $(OBJS) $(EXECFILE) *.P *~ \#*


$(EXECFILE): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS) $(LDLIBS)
