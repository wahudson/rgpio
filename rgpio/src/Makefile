# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(BINDIR)/ and $(OJ)/
# Dependency on only librgpio.a, assuming it is up-to-date with its headers.
# Note linking with -lrgpio; could have just linked with LIBS instead.

SHELL      = /bin/sh
BINDIR     = ../bin
OJ         = ../obj
MAN        = ../man

		# the librgpio library
INCDIR     = ../librgpio/src
LIBDIR     = ../librgpio/lib
LIBS       = $(LIBDIR)/librgpio.a

		# common include dependencies
INCS       = \
	Error.h \
	yOpVal.h \
	yOption.h \
	yUtil.h \
	$(LIBS)

		# include files of sub-commands, rgpio dependencies
YINCS       = \
	y_clk.h \
	y_fsel.h \
	y_fsel5.h \
	y_header.h \
	y_iic.h \
	y_info.h \
	y_io.h \
	y_man.h \
	y_pads.h \
	y_pud.h \
	y_pull.h \
	y_pwm.h \
	y_rcon.h \
	y_rio.h \
	y_rpad.h \
	y_rstat.h \
	y_spi0.h \
	y_timer.h \
	y_uspi.h \
	yVersion.h \
	version.rgpio

		# objects not including main()
OBJS       = \
	$(OJ)/Error.o \
	$(OJ)/y_clk.o \
	$(OJ)/y_fsel.o \
	$(OJ)/y_fsel5.o \
	$(OJ)/y_header.o \
	$(OJ)/y_iic.o \
	$(OJ)/y_info.o \
	$(OJ)/y_io.o \
	$(OJ)/y_man.o \
	$(OJ)/y_pads.o \
	$(OJ)/y_pud.o \
	$(OJ)/y_pull.o \
	$(OJ)/y_pwm.o \
	$(OJ)/y_rcon.o \
	$(OJ)/y_rio.o \
	$(OJ)/y_rpad.o \
	$(OJ)/y_rstat.o \
	$(OJ)/y_spi0.o \
	$(OJ)/y_timer.o \
	$(OJ)/y_uspi.o \
	$(OJ)/yOpVal.o \
	$(OJ)/yOption.o \
	$(OJ)/yUtil.o

		# link libraries, order matters
LIBFLAGS   = -lrgpio -lcap

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11  -I $(INCDIR)


run:	compile
	$(BINDIR)/rgpio --TESTOP

compile:  $(OJ)  $(BINDIR)  $(BINDIR)/rgpio

mkdirs:   $(OJ)  $(BINDIR)  $(MAN)/man1

cap:
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  $(BINDIR)/rgpio

man:	$(MAN)/man1 \
	$(MAN)/man1/rgpio.1 \
	$(MAN)/man1/rgpio-clk.1 \
	$(MAN)/man1/rgpio-fsel.1 \
	$(MAN)/man1/rgpio-fsel5.1 \
	$(MAN)/man1/rgpio-header.1 \
	$(MAN)/man1/rgpio-io.1 \
	$(MAN)/man1/rgpio-pads.1

clean:
	rm -f  $(OJ)/*

clobber:
	rm -f  $(OJ)/*
	rm -f  $(BINDIR)/rgpio
	rm -f  $(MAN)/man1/*

version  version.rgpio:  $(OBJS)
	git describe --dirty | perl -pe' s/^/"/;  s/$$/"/ ' > version.rgpio
	cat version.rgpio

$(BINDIR):
	mkdir -p  $(BINDIR)

$(OJ):
	mkdir -p  $(OJ)

$(MAN)/man1:
	mkdir -p  $(MAN)/man1


$(BINDIR)/rgpio:	        $(OJ)/rgpio.o  $(OBJS)  $(LIBS)
	g++ -o $@ -L $(LIBDIR)  $(OJ)/rgpio.o  $(OBJS)  $(LIBFLAGS)
#!! or just link librgpio.a since it is needed for dependency anyway


$(OJ)/rgpio.o:	rgpio.cpp  $(INCS)  $(YINCS)
	g++ $(CXXFLAGS) -o $@  -c rgpio.cpp


$(OJ)/y_clk.o:		y_clk.cpp  y_clk.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_clk.cpp

$(OJ)/y_fsel.o:		y_fsel.cpp  y_fsel.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_fsel.cpp

$(OJ)/y_fsel5.o:	y_fsel5.cpp  y_fsel5.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_fsel5.cpp

$(OJ)/y_header.o:	y_header.cpp  y_header.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_header.cpp

$(OJ)/y_iic.o:		y_iic.cpp  y_iic.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_iic.cpp

$(OJ)/y_info.o:		y_info.cpp  y_info.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_info.cpp

$(OJ)/y_io.o:		y_io.cpp  y_io.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_io.cpp

$(OJ)/y_man.o:		y_man.cpp  y_man.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_man.cpp

$(OJ)/y_pads.o:		y_pads.cpp  y_pads.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_pads.cpp

$(OJ)/y_pud.o:		y_pud.cpp  y_pud.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_pud.cpp

$(OJ)/y_pull.o:		y_pull.cpp  y_pull.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_pull.cpp

$(OJ)/y_pwm.o:		y_pwm.cpp  y_pwm.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_pwm.cpp

$(OJ)/y_rcon.o:		y_rcon.cpp  y_rcon.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_rcon.cpp

$(OJ)/y_rio.o:		y_rio.cpp  y_rio.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_rio.cpp

$(OJ)/y_rpad.o:		y_rpad.cpp  y_rpad.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_rpad.cpp

$(OJ)/y_rstat.o:	y_rstat.cpp  y_rstat.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_rstat.cpp

$(OJ)/y_spi0.o:		y_spi0.cpp  y_spi0.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_spi0.cpp

$(OJ)/y_timer.o:	y_timer.cpp  y_timer.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_timer.cpp

$(OJ)/y_uspi.o:		y_uspi.cpp  y_uspi.h  $(INCS)
	g++ $(CXXFLAGS) -o $@  -c y_uspi.cpp


$(OJ)/Error.o:		Error.cpp  Error.h
	g++ $(CXXFLAGS) -o $@  -c Error.cpp

$(OJ)/yOpVal.o:		yOpVal.cpp  yOpVal.h
	g++ $(CXXFLAGS) -o $@  -c yOpVal.cpp

$(OJ)/yOption.o:	yOption.cpp  yOption.h  Error.h
	g++ $(CXXFLAGS) -o $@  -c yOption.cpp

$(OJ)/yUtil.o:		yUtil.cpp  yUtil.h
	g++ $(CXXFLAGS) -o $@  -c yUtil.cpp


$(MAN)/man1/rgpio.1:		rgpio.pod
	pod2man -s 1 -n rgpio --center='Raspberry Pi GPIO'  rgpio.pod  $@

$(MAN)/man1/rgpio-clk.1:	man1/rgpio-clk.pod
	pod2man -s 1 -n rgpio-clk --center='Raspberry Pi GPIO' \
		   man1/rgpio-clk.pod  $@

$(MAN)/man1/rgpio-fsel.1:	man1/rgpio-fsel.pod
	pod2man -s 1 -n rgpio-fsel --center='Raspberry Pi GPIO' \
		   man1/rgpio-fsel.pod  $@

$(MAN)/man1/rgpio-fsel5.1:	man1/rgpio-fsel5.pod
	pod2man -s 1 -n rgpio-fsel5 --center='Raspberry Pi GPIO' \
		   man1/rgpio-fsel5.pod  $@

$(MAN)/man1/rgpio-header.1:	man1/rgpio-header.pod
	pod2man -s 1 -n rgpio-header --center='Raspberry Pi GPIO' \
		   man1/rgpio-header.pod  $@

$(MAN)/man1/rgpio-io.1:		man1/rgpio-io.pod
	pod2man -s 1 -n rgpio-io --center='Raspberry Pi GPIO' \
		   man1/rgpio-io.pod  $@

$(MAN)/man1/rgpio-pads.1:	man1/rgpio-pads.pod
	pod2man -s 1 -n rgpio-pads --center='Raspberry Pi GPIO' \
		   man1/rgpio-pads.pod  $@

