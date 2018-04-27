# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(LIBDIR)/ and $(OJ)/

SHELL      = /bin/sh
LIBDIR     = ../lib
OJ         = ../obj

		# all include files for main program dependency
INCS       = \
        rgAddrMap.h \
        rgAltFuncName.h \
        rgClock.h \
        rgFselPin.h \
        rgIoPin.h \
        rgUniSpi.h

		# objects not including main()
OBJS       = \
	$(OJ)/rgAddrMap.o \
	$(OJ)/rgAltFuncName.o \
	$(OJ)/rgClock.o \
	$(OJ)/rgFselPin.o \
	$(OJ)/rgIoPin.o \
	$(OJ)/rgUniSpi.o

		# link libraries, required by rgAddrMap
LIBFLAGS   = -lcap

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11


compile:  $(OJ)  $(LIBDIR)  $(LIBDIR)/librgpio.a

mkdirs:   $(OJ)  $(LIBDIR)

x1	: x1.cpp  rgAddrMap.cpp  $(INCS)
	g++ $(CXXFLAGS) -o $@  x1.cpp   rgAddrMap.cpp  $(LIBFLAGS)
	./x1

clean: 
	rm -f  $(OJ)/*

clobber:
	rm -f  $(OJ)/*
	rm -f  $(LIBDIR)/librgpio.a

$(LIBDIR):
	mkdir -p  $(LIBDIR)

$(OJ):
	mkdir -p  $(OJ)


$(LIBDIR)/librgpio.a:	$(OBJS)
	rm -f    $(LIBDIR)/librgpio.a
	ar -rcs  $(LIBDIR)/librgpio.a  $(OBJS)


$(OJ)/rgAddrMap.o:	rgAddrMap.cpp  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgAddrMap.cpp

$(OJ)/rgAltFuncName.o:	rgAltFuncName.cpp  rgAltFuncName.h \
			rgAddrMap.h  rgIoPin.h  rgFselPin.h
	g++ $(CXXFLAGS) -o $@  -c rgAltFuncName.cpp

$(OJ)/rgClock.o:	rgClock.cpp  rgClock.h  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgClock.cpp

$(OJ)/rgFselPin.o:	rgFselPin.cpp  rgFselPin.h  rgAddrMap.h  rgIoPin.h
	g++ $(CXXFLAGS) -o $@  -c rgFselPin.cpp

$(OJ)/rgIoPin.o:	rgIoPin.cpp  rgIoPin.h  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgIoPin.cpp

$(OJ)/rgUniSpi.o:	rgUniSpi.cpp  rgUniSpi.h  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgUniSpi.cpp

