# 2021-02-25  William A. Hudson
#
# Compile and run this example.

SHELL      = /bin/sh
LB         = ../librgpio
LIBRGPIO   = $(LB)/lib/librgpio.a
IC         = $(LB)/src

		# all files for program dependency
INCS       = \
	$(LIBRGPIO) \
	$(IC)/rgAddrMap.h

LIBS       = $(LIBRGPIO) -lcap

		# compiler flags
CXXFLAGS   = -Wall -std=c++11  -I $(IC)


run:	demo.exe
	./demo.exe

cap:
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  ./demo.exe

clean:
	rm -f  demo.exe

demo.exe:	demo.cpp  $(OBJS)  $(INCS)
	g++ $(CXXFLAGS) -o $@  demo.cpp  $(LIBS)

