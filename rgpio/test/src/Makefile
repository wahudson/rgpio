# 2017-05-20  William A. Hudson
#
# Compile unit test library and Test tools.

# Outputs in $(BINDIR)/ and $(OJ)/

SHELL      = /bin/sh
BINDIR     = ../bin
OJ         = ../obj

		# the librgpio library
INCDIR     = ../librgpio/src
LIBDIR     = ../librgpio/lib
LIBS       = $(LIBDIR)/librgpio.a

		# objects not including main()
OBJS       = \
	$(OJ)/utLib1.o

		# commands
CMDS       = \
	$(BINDIR)/rpi_BaseAddr

		# compiler flags
CXXFLAGS   = -Wall  -std=c++11


compile:  $(OJ)  $(OBJS)  $(CMDS)

mkdirs:   $(OJ)

$(OJ):
	mkdir -p  $(OJ)

clean: 
	rm -f  $(OJ)/*  $(CMDS)

clobber:
	rm -f  $(OJ)/*  $(CMDS)


# Unit Test Framework

$(OJ)/utLib1.o:	utLib1.cpp  utLib1.h
	g++ $(CXXFLAGS) -o $@  -c utLib1.cpp

$(BINDIR)/rpi_BaseAddr:	rpi_BaseAddr.cpp  $(LIBS)
	g++ $(CXXFLAGS) -I $(INCDIR) -o $@ -L $(LIBDIR) rpi_BaseAddr.cpp -lrgpio

