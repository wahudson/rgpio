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
	rgClk.h \
	rgFselPin.h \
	rgHeaderPin.h \
	rgIic.h \
	rgIoPins.h \
	rgPads.h \
	rgPudPin.h \
	rgPullPin.h \
	rgPwm.h \
	rgRegister.h \
	rgRpiRev.h \
	rgSpi0.h \
	rgSysTimer.h \
	rgUniSpi.h \
	rgsFuncName.h \
	rgsIoBank.h \
	rgsIoCon.h \
	rgsIoPads.h \
	rgsRegAtom.h \
	rgsRio.h \
	rgVersion.h

		# objects not including main(), not rgVersion.o
OBJS       = \
	$(OJ)/rgAddrMap.o \
	$(OJ)/rgAltFuncName.o \
	$(OJ)/rgClk.o \
	$(OJ)/rgFselPin.o \
	$(OJ)/rgHeaderPin.o \
	$(OJ)/rgIic.o \
	$(OJ)/rgIoPins.o \
	$(OJ)/rgPads.o \
	$(OJ)/rgPudPin.o \
	$(OJ)/rgPullPin.o \
	$(OJ)/rgPwm.o \
	$(OJ)/rgRegister.o \
	$(OJ)/rgRpiRev.o \
	$(OJ)/rgSpi0.o \
	$(OJ)/rgSysTimer.o \
	$(OJ)/rgUniSpi.o \
	$(OJ)/rgsFuncName.o \
	$(OJ)/rgsIoCon.o \
	$(OJ)/rgsIoPads.o \
	$(OJ)/rgsRio.o

		# link libraries, required by rgAddrMap
LIBFLAGS   = -lcap

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11


compile:  $(OJ)  $(LIBDIR)  $(LIBDIR)/librgpio.a

mkdirs:   $(OJ)  $(LIBDIR)

man:	$(MAN)/man3 \
	$(MAN)/man3/rgAddrMap.3 \
	$(MAN)/man3/rgAltFuncName.3 \
	$(MAN)/man3/rgClk.3 \
	$(MAN)/man3/rgFselPin.3 \
	$(MAN)/man3/rgHeaderPin.3 \
	$(MAN)/man3/rgIoPins.3 \
	$(MAN)/man3/rgPudPin.3 \
	$(MAN)/man3/rgRpiRev.3 \
	$(MAN)/man3/rgUniSpi.3 \
	$(MAN)/man7 \
	$(MAN)/man7/rgClk.7 \
	$(MAN)/man7/rgIic.7 \
	$(MAN)/man7/rgIoPins.7 \
	$(MAN)/man7/rgPads.7 \
	$(MAN)/man7/rgPudPin.7 \
	$(MAN)/man7/rgPwm.7 \
	$(MAN)/man7/rgSpi0.7 \
	$(MAN)/man7/rgUniSpi.7

clean:
	rm -f  $(OJ)/*

clobber:
	rm -f  $(OJ)/*
	rm -f  $(LIBDIR)/librgpio.a
	rm -f  $(MAN)/man3/*
	rm -f  $(MAN)/man7/*

version  version.inc:  $(OBJS)
	git describe --dirty | perl -pe' s/^/"/;  s/$$/"/ ' > version.inc
	cat version.inc

$(LIBDIR):
	mkdir -p  $(LIBDIR)

$(OJ):
	mkdir -p  $(OJ)

$(MAN)/man3:
	mkdir -p  $(MAN)/man3

$(MAN)/man7:
	mkdir -p  $(MAN)/man7


$(LIBDIR)/librgpio.a:	$(OBJS)  $(OJ)/rgVersion.o
	rm -f    $(LIBDIR)/librgpio.a
	ar -rcs  $(LIBDIR)/librgpio.a  $(OBJS)  $(OJ)/rgVersion.o


$(OJ)/rgAddrMap.o:	rgAddrMap.cpp  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgAddrMap.cpp

$(OJ)/rgAltFuncName.o:	rgAltFuncName.cpp  rgAltFuncName.h \
			rgAddrMap.h  rgIoPins.h  rgFselPin.h  rgRpiRev.h
	g++ $(CXXFLAGS) -o $@  -c rgAltFuncName.cpp

$(OJ)/rgClk.o:		rgClk.cpp  rgClk.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgClk.cpp

$(OJ)/rgFselPin.o:	rgFselPin.cpp  rgFselPin.h  rgAddrMap.h  rgIoPins.h
	g++ $(CXXFLAGS) -o $@  -c rgFselPin.cpp

$(OJ)/rgHeaderPin.o:	rgHeaderPin.cpp  rgHeaderPin.h
	g++ $(CXXFLAGS) -o $@  -c rgHeaderPin.cpp

$(OJ)/rgIic.o:		rgIic.cpp  rgIic.h  rgAddrMap.h  rgRegister.h  rgRpiRev.h
	g++ $(CXXFLAGS) -o $@  -c rgIic.cpp

$(OJ)/rgIoPins.o:	rgIoPins.cpp  rgIoPins.h  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgIoPins.cpp

$(OJ)/rgPads.o:	rgPads.cpp  rgPads.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgPads.cpp

$(OJ)/rgPudPin.o:	rgPudPin.cpp  rgPudPin.h  rgIoPins.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgPudPin.cpp

$(OJ)/rgPullPin.o:	rgPullPin.cpp  rgPullPin.h  rgIoPins.h  rgAddrMap.h
	g++ $(CXXFLAGS) -o $@  -c rgPullPin.cpp

$(OJ)/rgPwm.o:		rgPwm.cpp  rgPwm.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgPwm.cpp

$(OJ)/rgRegister.o:	rgRegister.cpp  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgRegister.cpp

$(OJ)/rgRpiRev.o:	rgRpiRev.cpp  rgRpiRev.h
	g++ $(CXXFLAGS) -o $@  -c rgRpiRev.cpp

$(OJ)/rgSpi0.o:		rgSpi0.cpp  rgSpi0.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgSpi0.cpp

$(OJ)/rgSysTimer.o:	rgSysTimer.cpp  rgSysTimer.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgSysTimer.cpp

$(OJ)/rgUniSpi.o:	rgUniSpi.cpp  rgUniSpi.h  rgAddrMap.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgUniSpi.cpp

$(OJ)/rgVersion.o:	rgVersion.cpp  rgVersion.h  version.inc
	g++ $(CXXFLAGS) -o $@  -c rgVersion.cpp

$(OJ)/rgsFuncName.o:	rgsFuncName.cpp  rgsFuncName.h
	g++ $(CXXFLAGS) -o $@  -c rgsFuncName.cpp

$(OJ)/rgsIoCon.o:	rgsIoCon.cpp  rgsIoCon.h rgsIoBank.h \
			rgAddrMap.h  rgsRegAtom.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgsIoCon.cpp

$(OJ)/rgsIoPads.o:	rgsIoPads.cpp  rgsIoPads.h \
			rgAddrMap.h  rgsRegAtom.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgsIoPads.cpp

$(OJ)/rgsRio.o:		rgsRio.cpp  rgsRio.h rgsIoBank.h \
			rgAddrMap.h  rgsRegAtom.h  rgRegister.h
	g++ $(CXXFLAGS) -o $@  -c rgsRio.cpp


$(MAN)/man3/rgAddrMap.3:	rgAddrMap.pod
	pod2man -s 3 -n rgAddrMap  --center='librgpio Raspberry Pi GPIO' \
	    rgAddrMap.pod  $@

$(MAN)/man3/rgAltFuncName.3:	rgAltFuncName.pod
	pod2man -s 3 -n rgAltFuncName  --center='librgpio Raspberry Pi GPIO' \
	    rgAltFuncName.pod  $@

$(MAN)/man3/rgClk.3:		rgClk.pod
	pod2man -s 3 -n rgClk    --center='librgpio Raspberry Pi GPIO' \
	    rgClk.pod  $@

$(MAN)/man3/rgFselPin.3:	rgFselPin.pod
	pod2man -s 3 -n rgFselPin    --center='librgpio Raspberry Pi GPIO' \
	    rgFselPin.pod  $@

$(MAN)/man3/rgHeaderPin.3:	rgHeaderPin.pod
	pod2man -s 3 -n rgHeaderPin    --center='librgpio Raspberry Pi GPIO' \
	    rgHeaderPin.pod  $@

$(MAN)/man3/rgIoPins.3:		rgIoPins.pod
	pod2man -s 3 -n rgIoPins  --center='librgpio Raspberry Pi GPIO' \
	    rgIoPins.pod  $@

$(MAN)/man3/rgPudPin.3:		rgPudPin.pod
	pod2man -s 3 -n rgPudPin  --center='librgpio Raspberry Pi GPIO' \
	    rgPudPin.pod  $@

$(MAN)/man3/rgRpiRev.3:		rgRpiRev.pod
	pod2man -s 3 -n rgRpiRev  --center='librgpio Raspberry Pi GPIO' \
	    rgRpiRev.pod  $@

$(MAN)/man3/rgUniSpi.3:		rgUniSpi.pod
	pod2man -s 3 -n rgUniSpi  --center='librgpio Raspberry Pi GPIO' \
	    rgUniSpi.pod  $@


$(MAN)/man7/rgClk.7:		man7/rgClk.pod
	pod2man -s 7 -n rgClk  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgClk.pod  $@

$(MAN)/man7/rgIic.7:		man7/rgIic.pod
	pod2man -s 7 -n rgIic  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgIic.pod  $@

$(MAN)/man7/rgIoPins.7:		man7/rgIoPins.pod
	pod2man -s 7 -n rgIoPins  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgIoPins.pod  $@

$(MAN)/man7/rgPads.7:		man7/rgPads.pod
	pod2man -s 7 -n rgPads  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgPads.pod  $@

$(MAN)/man7/rgPudPin.7:		man7/rgPudPin.pod
	pod2man -s 7 -n rgPudPin  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgPudPin.pod  $@

$(MAN)/man7/rgPwm.7:		man7/rgPwm.pod
	pod2man -s 7 -n rgPwm  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgPwm.pod  $@

$(MAN)/man7/rgSpi0.7:		man7/rgSpi0.pod
	pod2man -s 7 -n rgSpi0  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgSpi0.pod  $@

$(MAN)/man7/rgUniSpi.7:		man7/rgUniSpi.pod
	pod2man -s 7 -n rgUniSpi  --center='librgpio Raspberry Pi GPIO' \
	    man7/rgUniSpi.pod  $@

