# 2019-11-02  William A. Hudson
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
	$(IC)/rgAddrMap.h \
	$(IC)/rgUniSpi.h

		# objects not including main()
OBJS       = \
	../obj/utLib1.o \
	$(LB)/librgpio.a

LIBS       = -lcap

		# compiler flags
CXXFLAGS   = -Wall -std=c++11  -I ../src


test:	test.exe
	./test.exe

cap:
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  ./test.exe

clean:
	rm -f  test.exe

test.exe:	test.cpp  $(OBJS)  $(INCS)
	g++ $(CXXFLAGS) -I $(IC) -o $@  test.cpp  $(OBJS)  $(LIBS)
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  ./test.exe

