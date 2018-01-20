# 2017-05-11  William A. Hudson
#
# Compile and run the main program.

# Outputs in $(BINDIR)/ and $(OJ)/

SHELL      = /bin/sh
BINDIR     = ../bin
OJ         = ../obj
IH         = ../src

		# all include files for main program dependency
INCS       = \
        $(IH)/rgAddrMap.h \
        $(IH)/rgClock.h \
        $(IH)/Error.h \
        $(IH)/yOption.h

		# objects not including main()
OBJS       = \
	$(OJ)/rgAddrMap.o \
	$(OJ)/rgClock.o \
	$(OJ)/Error.o \
	$(OJ)/yOption.o

		# compiler flags
	# -std=c++11  needed for std::stoi
CXXFLAGS   = -Wall  -std=c++11  -I $(IH)


run:	compile
	$(BINDIR)/clock_perf --TESTOP

mkdirs:   $(OJ)  $(BINDIR)

compile:  $(OJ)  $(BINDIR)  $(BINDIR)/clock_perf

clean: 
	rm -f  $(OJ)/*

clobber:
	rm -f  $(BINDIR)/clock_perf  $(OJ)/*

$(BINDIR):
	mkdir -p  $(BINDIR)

$(OJ):
	mkdir -p  $(OJ)


$(BINDIR)/clock_perf:	       clock_perf.cpp  $(OBJS) $(INCS)
	g++ $(CXXFLAGS) -o $@  clock_perf.cpp  $(OBJS)

