#! /usr/bin/perl -w
# 2020-09-20  William A. Hudson

# Testing:  rgpio-pud  subcommand.
#    10-19  pud basic options --help
#    20-29  Bit Programming
#    30-39  Mask Programming --w0=  --w1=
#    40-49  Register Modification
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
## pud basic options --help
#---------------------------------------------------------------------------

run_test( "10", "pud no args",
    "rgpio --dev=f --rpi3  pud",
    0,
    Stderr => q(),
    Stdout => q(
	Pud Pin Registers:                28   24   20   16   12    8    4    0
	0x00000000  PudProgMode         0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w1       0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "11a", "Error on RPi5",
    "rgpio --dev=f --rpi5  pud",
    1,
    Stderr => q(
	Error:  RPi platform:  rgPudPin:  require RPi3 (soc_BCM2837) or earlier
    ),
    Stdout => q(
    ),
);

run_test( "12", "pud help",
    "rgpio --dev=f --rpi3  pud --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi3  pud --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

#---------------------------------------
run_test( "14a", "error multiple modes",
    "rgpio --dev=f --rpi3  pud --up --down 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "14b", "error multiple modes",
    "rgpio --dev=f --rpi3  pud --up --off 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "14c", "error multiple modes",
    "rgpio --dev=f --rpi3  pud --off --down 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "15a", "error missing mode",
    "rgpio --dev=f --rpi3  pud  7",
    1,
    Stderr => q(
	Error:  program requires one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "15b", "error missing mode",
    "rgpio --dev=f --rpi3  pud --w0=0x00000000",
    1,
    Stderr => q(
	Error:  program requires one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "15c", "error missing mode",
    "rgpio --dev=f --rpi3  pud --w1=0x00000000",
    1,
    Stderr => q(
	Error:  program requires one of:  --up --down --off
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "16a", "error missing bits",
    "rgpio --dev=f --rpi3  pud --up",
    1,
    Stderr => q(
	Error:  program requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

run_test( "16b", "error missing bits",
    "rgpio --dev=f --rpi3  pud --down",
    1,
    Stderr => q(
	Error:  program requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

run_test( "16c", "error missing bits",
    "rgpio --dev=f --rpi3  pud --off",
    1,
    Stderr => q(
	Error:  program requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Bit Programming
#---------------------------------------------------------------------------

run_test( "20", "good bit numbers, duplicate",
    "rgpio --dev=f --rpi3  pud --up  31 0 53 0",
    0,
    Stderr => q(),
    Stdout => q(
	 program bits:  Up
	   31  OK
	    0  OK
	   53  OK
	    0  OK
    ),
);

run_test( "21", "error bit too large",
    "rgpio --dev=f --rpi3  pud --up  31 2 54 7",
    1,
    Stderr => q(
	Error:  bit arg out-of-range:  54
    ),
    Stdout => q(),
);

run_test( "22", "error too many bits",
    "rgpio --dev=f --rpi3  pud --up 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4",
    1,
    Stderr => q(
	Error:  max bit args:  64
    ),
    Stdout => q(),
);

run_test( "23", "error non-numeric Gpio bit number",
    "rgpio --dev=f --rpi3  pud --up  7 a12 2two",
    1,
    Stderr => q(
	Error:  bit arg non-numeric:  a12
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Mask Programming --w0=  --w1=
#---------------------------------------------------------------------------

run_test( "30", "good --w0",
    "rgpio --dev=f --rpi3  pud --up  --w0=0xffffffff",
    0,
    Stderr => q(),
    Stdout => q(
	 program words:  Up
	   w0= 0xffffffff  OK
    ),
);

run_test( "31", "good --w0 --w1",
    "rgpio --dev=f --rpi3  pud --down  --w0=0xff223344 --w1=0xabbafedc",
    0,
    Stderr => q(),
    Stdout => q(
	 program words:  Down
	   w0= 0xff223344  OK
	   w1= 0xabbafedc  OK
    ),
);

#---------------------------------------------------------------------------
## Register Modification
#---------------------------------------------------------------------------

run_test( "40", "view registers",
    "rgpio --dev=f --rpi3  pud -v",
    0,
    Stderr => q(),
    Stdout => q(
	+ Read registers
	Pud Pin Registers:                28   24   20   16   12    8    4    0
	0x00000000  PudProgMode         0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w1       0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "41", "modify registers",
    "rgpio --dev=f --rpi3  pud -v --PudProgMode=0xffffffff --PudProgClk_w0=0xfeedabba",
    0,
    Stderr => q(),
    Stdout => q(
	+ Apply register values
	+ Read registers
	Pud Pin Registers:                28   24   20   16   12    8    4    0
	0xffffffff  PudProgMode         1111 1111 1111 1111 1111 1111 1111 1111
	0xfeedabba  PudProgClk_w0       1111 1110 1110 1101 1010 1011 1011 1010
	0x00000000  PudProgClk_w1       0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

