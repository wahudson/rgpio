# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(BINDIR)/ and Inputs in $(OJ)/

SHELL      = /bin/sh
BINDIR     = ../bin
OJ         = ../obj
IH         = ../src

		# the librgpio library
INCDIR     = ../../librgpio/src
LIBDIR     = ../../librgpio/lib
LIBS       = $(LIBDIR)/librgpio.a

		# all include files for main program dependency
INCS       = \
        $(IH)/Error.h \
        $(IH)/yOption.h

		# objects not including main()
OBJS       = \
	$(OJ)/Error.o \
	$(OJ)/yOption.o

		# link libraries, order matters
LIBFLAGS   = -lrgpio -lcap

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11  -I $(IH)  -I $(INCDIR)


run:	compile
	$(BINDIR)/clock_perf --TESTOP

compile:  $(OJ)  $(BINDIR) \
	$(BINDIR)/clock_perf \
	$(BINDIR)/uspi_fifo \
	$(BINDIR)/uspi_trace

cap:
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  $(BINDIR)/clock_perf
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  $(BINDIR)/uspi_fifo
	sudo  setcap 'CAP_DAC_OVERRIDE,CAP_SYS_RAWIO=p'  $(BINDIR)/uspi_trace

mkdirs:   $(OJ)  $(BINDIR)

clean: 
	# rm -f  $(OJ)/*

clobber:
	rm -f  $(BINDIR)/clock_perf
	rm -f  $(BINDIR)/uspi_fifo
	rm -f  $(BINDIR)/uspi_trace

$(BINDIR):
	mkdir -p  $(BINDIR)

$(OJ):
	mkdir -p  $(OJ)


$(BINDIR)/clock_perf:	 clock_perf.cpp  $(OBJS)  $(INCS)  $(LIBS)
	g++ $(CXXFLAGS) -o $@ -L $(LIBDIR) \
			 clock_perf.cpp  $(OBJS)  $(LIBFLAGS)

$(BINDIR)/uspi_fifo:	 uspi_fifo.cpp  $(OBJS)  $(INCS)  $(LIBS)
	g++ $(CXXFLAGS) -o $@ -L $(LIBDIR) \
			 uspi_fifo.cpp  $(OBJS)  $(LIBFLAGS)

$(BINDIR)/uspi_trace:	 uspi_trace.cpp  $(OBJS)  $(INCS)  $(LIBS)
	g++ $(CXXFLAGS) -o $@ -L $(LIBDIR) \
			 uspi_trace.cpp  $(OBJS)  $(LIBFLAGS)

