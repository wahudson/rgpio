# 2024-06-07  William A. Hudson
#
# Compile and run this test.
# Use OBJS, but not build them.  Outputs in ./

SHELL      = /bin/sh
OJ         = ../../obj
IC         = ../../src
LB         = ../../lib

		# all include files for test program dependency
INCS       = \
	../src/utLib1.h \
	$(IC)/rgRpiRev.h \
	$(IC)/rgAddrMap.h

		# objects not including main()
OBJS       = \
	../obj/utLib1.o \
	$(LB)/librgpio.a

LIBS       = -lcap

		# compiler flags
CXXFLAGS   = -Wall -std=c++11  -I ../src


test:	test.exe
	dpkg --print-architecture
	getconf LONG_BIT
	./test.exe

cpp:
	g++ -Wall -E test.cpp | less

clean:
	rm -f  test.exe

test.exe:	test.cpp  $(OBJS)  $(INCS)
	g++ $(CXXFLAGS) -I $(IC) -o $@  test.cpp  $(OBJS)  $(LIBS)

