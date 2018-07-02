# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(LIBDIR)/ and $(OJ)/

SHELL      = /bin/sh
LIBDIR     = ../lib
OJ         = ../obj
MAN        = ../man

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

man:	$(MAN)/man3 \
	$(MAN)/man3/rgAddrMap.3 \
	$(MAN)/man3/rgIoPin.3 \
	$(MAN)/man3/rgUniSpi.3 \
	$(MAN)/man7 \
	$(MAN)/man7/rgUniSpi.7

clean:
	rm -f  $(OJ)/*

clobber:
	rm -f  $(OJ)/*
	rm -f  $(LIBDIR)/librgpio.a
	rm -f  $(MAN)/man3/*
	rm -f  $(MAN)/man7/*

$(LIBDIR):
	mkdir -p  $(LIBDIR)

$(OJ):
	mkdir -p  $(OJ)

$(MAN)/man3:
	mkdir -p  $(MAN)/man3

$(MAN)/man7:
	mkdir -p  $(MAN)/man7


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


$(MAN)/man3/rgAddrMap.3:	rgAddrMap.pod
	pod2man -s 3 -n rgAddrMap  --center='librgpio Raspberry Pi GPIO' \
	    rgAddrMap.pod  $@

$(MAN)/man3/rgIoPin.3:		rgIoPin.pod
	pod2man -s 3 -n rgIoPin  --center='librgpio Raspberry Pi GPIO' \
	    rgIoPin.pod  $@

$(MAN)/man3/rgUniSpi.3:		rgUniSpi.pod
	pod2man -s 3 -n rgUniSpi  --center='librgpio Raspberry Pi GPIO' \
	    rgUniSpi.pod  $@

$(MAN)/man7/rgUniSpi.7:		man7/rgUniSpi.pod
	pod2man -s 7 -n rgUniSpi  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgUniSpi.pod  $@

