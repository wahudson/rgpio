#! /usr/bin/perl -w
# 2018-06-30  William A. Hudson

# Testing:  rgpio-fsel  subcommand.
#    10-19  fsel basic options --help
#    20-29  --mode
#    30-39  --show
#    40-49  .
#    50-59  .

# usage:  ./test.pl
# files:
#    ./tmp/	run directory, all files written here
#    ./ref/	reference input/output files
#
#---------------------------------------------------------------------------

use lib "../lib";
use lib "../..";

use TestLib2 ( '1.01',
    'Error',
    'die_Error',
#   'here_text',
    'End_test',
    'run_test',
);

use strict;

print( "PATH=$ENV{PATH}\n" );


#---------------------------------------------------------------------------
# Configure working directory.
#---------------------------------------------------------------------------

chdir( "tmp" ) || die_Error( "cannot chdir ./tmp\n" );


#---------------------------------------------------------------------------
## fsel basic options --help
#---------------------------------------------------------------------------

run_test( "11", "fsel no args",
    "rgpio --dev=f  fsel",
    0,
    Stderr => q(),
    Stdout => q(
	Bit  Mode  Function
	  0  In    input
	  1  In    input
	  2  In    input
	  3  In    input
	  4  In    input
	  5  In    input
	  6  In    input
	  7  In    input
	  8  In    input
	  9  In    input
	 10  In    input
	 11  In    input
	 12  In    input
	 13  In    input
	 14  In    input
	 15  In    input
	 16  In    input
	 17  In    input
	 18  In    input
	 19  In    input
	 20  In    input
	 21  In    input
	 22  In    input
	 23  In    input
	 24  In    input
	 25  In    input
	 26  In    input
	 27  In    input
	 28  In    input
	 29  In    input
	 30  In    input
	 31  In    input
    ),
);

run_test( "12", "fsel help",
    "rgpio --dev=f  fsel --help",
    0,
    Stderr => q(),
);

run_test( "13", "bad option",
    "rgpio --dev=f  fsel --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "fsel bit 53",
    "rgpio --dev=f  fsel 53",
    0,
    Stderr => q(),
    Stdout => q(
	Bit  Mode  Function
	 53  In    input
    ),
);

run_test( "15", "fsel bit too large",
    "rgpio --dev=f  fsel 54",
    1,
    Stderr => q(
	Error:   bit arg out-of-range:  54
    ),
    Stdout => q(),
);

run_test( "16", "fsel verbose",
    "rgpio --dev=f  fsel -v 0",
    0,
    Stderr => q(),
    Stdout => q(
	Pin Function Select:
	Bit  Mode  Function
	  0  In    input
    ),
);


#---------------------------------------------------------------------------
## --mode
#---------------------------------------------------------------------------

run_test( "20", "fsel --mode=Out no default",
    "rgpio --dev=f  fsel --mode=Out",
    1,
    Stderr => q(
	Error:  --mode requires bit numbers or -0 -1
    ),
    Stdout => q(),
);

run_test( "21", "fsel --mode=Out  4",
    "rgpio --dev=f  fsel --mode=Out  4",
    0,
    Stderr => q(),
    Stdout => q(
	Modify:
	Bit  Mode  Function
	  4  Out   output
    ),
);

run_test( "22", "fsel --mode=Out  4",
    "rgpio --dev=f  fsel --mode=Alt5  14 15 16 17",
    0,
    Stderr => q(),
    Stdout => q(
	Modify:
	Bit  Mode  Function
	 14  Alt5  u1_TXD
	 15  Alt5  u1_RXD
	 16  Alt5  u1_CTS
	 17  Alt5  u1_RTS
    ),
);

run_test( "23", "fsel reset all pins",
    "rgpio --dev=f  fsel --mode=In -0 -1",
    0,
    Stderr => q(),
);

run_test( "24", "fsel --mode=Baad",
    "rgpio --dev=f  fsel --mode=Baad  4",
    1,
    Stderr => q(
	Error:  unknown Fsel mode:  --mode=Baad
	    rgFselPin::find_rgFsel_enum() no enum for string:  Baad
    ),
    Stdout => q(),
);


#---------------------------------------------------------------------------
## --show
#---------------------------------------------------------------------------

run_test( "30", "fsel --show",
    "rgpio --dev=f --rpi3  fsel --show  4 5",
    0,
    Stderr => q(),
    Stdout => q(
Bit  Alt0        Alt1        Alt2        Alt3        Alt4        Alt5       
  4  gp_CLK0     sm_A1       dpi_D0      **          **          arm_TDI    
  5  gp_CLK1     sm_A0       dpi_D1      **          **          arm_TDO    
    ),
    # trailing space
);

run_test( "31", "RPi3 fsel --show",
    "rgpio --dev=f --rpi3  fsel --show -0 -1",
    0,
    Stderr => q(),
);

run_test( "32", "RPi4 fsel --show",
    "rgpio --dev=f --rpi4  fsel --show -0 -1",
    0,
    Stderr => q(),
);

run_test( "35", "fsel --show invalid with --mode",
    "rgpio --dev=f  fsel --show --mode=In  4",
    1,
    Stderr => q(
	Error:  --mode not valid with --show
    ),
    Stdout => q(),
);


#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

