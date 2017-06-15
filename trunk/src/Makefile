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
        rgIoPin.h \
        gGpClock.h
#        Error.h \
#        yOption.h \

		# objects not including main()
OBJS       = \
	$(OJ)/rgAddrMap.o \
	$(OJ)/rgIoPin.o \
	$(OJ)/gGpClock.o
#	$(OJ)/Error.o \
#	$(OJ)/yOption.o \

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11


run	: compile
	$(BINDIR)/rgpio --TESTOP

mkdirs	:
	mkdir -p $(BINDIR)
	mkdir -p $(OJ)

compile	: $(BINDIR)/rgpio  $(OBJS)

x1	: x1.cpp  rgAddrMap.cpp  $(INCS)
	g++ $(CXXFLAGS) -o $@  x1.cpp   rgAddrMap.cpp
	./x1

clean: 
	rm -f  $(OJ)/*

clobber:
	rm -f  $(BINDIR)/rgpio  $(OJ)/*

$(BINDIR)/main:	main.cpp Error.cpp Error.h
	g++ -o $@       main.cpp   Error.cpp

$(BINDIR)/rgpio:	$(OJ)/rgpio.o  $(OBJS)
	g++ -o $@       $(OJ)/rgpio.o  $(OBJS)


$(OJ)/rgpio.o:	rgpio.cpp  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c rgpio.cpp

$(OJ)/rgAddrMap.o:	rgAddrMap.cpp  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgAddrMap.cpp

$(OJ)/rgIoPin.o:	rgIoPin.cpp  rgIoPin.h
	g++ $(CXXFLAGS) -o $@  -c rgIoPin.cpp

$(OJ)/gGpClock.o:	gGpClock.cpp  gGpClock.h
	g++ $(CXXFLAGS) -o $@  -c gGpClock.cpp

$(OJ)/Error.o:		Error.cpp  Error.h
	g++ $(CXXFLAGS) -o $@  -c Error.cpp

$(OJ)/yOption.o:	yOption.cpp  yOption.h  Error.h
	g++ $(CXXFLAGS) -o $@  -c yOption.cpp

