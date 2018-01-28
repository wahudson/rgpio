# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(BINDIR)/ and $(OJ)/

SHELL      = /bin/sh
BINDIR     = ../bin
OJ         = ../obj

		# all include files for main program dependency
INCS       = \
        rgAddrMap.h \
        rgAltFuncName.h \
        rgClock.h \
        rgFselPin.h \
        rgIoPin.h \
        rgUniSpi.h \
        Error.h \
        y_clock.h \
        y_fsel.h \
        y_io.h \
        y_uspi.h \
        yOpVal.h \
        yOption.h

		# objects not including main()
OBJS       = \
	$(OJ)/rgAddrMap.o \
	$(OJ)/rgAltFuncName.o \
	$(OJ)/rgClock.o \
	$(OJ)/rgFselPin.o \
	$(OJ)/rgIoPin.o \
	$(OJ)/rgUniSpi.o \
	$(OJ)/Error.o \
	$(OJ)/y_clock.o \
	$(OJ)/y_fsel.o \
	$(OJ)/y_io.o \
	$(OJ)/y_uspi.o \
	$(OJ)/yOpVal.o \
	$(OJ)/yOption.o

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11


run:	compile
	$(BINDIR)/rgpio --TESTOP

mkdirs:   $(OJ)  $(BINDIR)

compile:  $(OJ)  $(BINDIR)  $(BINDIR)/rgpio

x1	: x1.cpp  rgAddrMap.cpp  $(INCS)
	g++ $(CXXFLAGS) -o $@  x1.cpp   rgAddrMap.cpp
	./x1

clean: 
	rm -f  $(OJ)/*

clobber:
	rm -f  $(BINDIR)/rgpio  $(OJ)/*

$(BINDIR):
	mkdir -p  $(BINDIR)

$(OJ):
	mkdir -p  $(OJ)


$(BINDIR)/main:	main.cpp Error.cpp Error.h
	g++ -o $@       main.cpp   Error.cpp

$(BINDIR)/rgpio:	$(OJ)/rgpio.o  $(OBJS)
	g++ -o $@       $(OJ)/rgpio.o  $(OBJS)


$(OJ)/rgpio.o:	rgpio.cpp  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c rgpio.cpp

$(OJ)/y_clock.o:	y_clock.cpp  y_clock.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_clock.cpp

$(OJ)/y_fsel.o:		y_fsel.cpp  y_fsel.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_fsel.cpp

$(OJ)/y_io.o:		y_io.cpp  y_io.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_io.cpp

$(OJ)/y_uspi.o:		y_uspi.cpp  y_uspi.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_uspi.cpp


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


$(OJ)/Error.o:		Error.cpp  Error.h
	g++ $(CXXFLAGS) -o $@  -c Error.cpp

$(OJ)/yOpVal.o:		yOpVal.cpp  yOpVal.h
	g++ $(CXXFLAGS) -o $@  -c yOpVal.cpp

$(OJ)/yOption.o:	yOption.cpp  yOption.h  Error.h
	g++ $(CXXFLAGS) -o $@  -c yOption.cpp

