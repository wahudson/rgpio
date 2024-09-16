#! /usr/bin/perl -w
# 2024-05-29  William A. Hudson

# Testing:  rgpio-rio  subcommand.
#    10-19  basic options --help
#    20-29  Errors
#    30-39  Read
#    40-49  Write Atomic registers
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
## basic options --help
#---------------------------------------------------------------------------

run_test( "10", "rio bad platform RPi4",
    "rgpio --dev=f --rpi4  rio",
    1,
    Stderr => q(
	Error:  RPi platform:  rgsRio::  require RPi5 (soc_BCM2712)
    ),
    Stdout => q(),
);

run_test( "11", "rio no args",
    "rgpio --dev=f --rpi5  rio",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioOut     norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioOutEn   norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  norm   0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "12", "rio help",
    "rgpio --dev=f --rpi5  rio --RioOut --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi5  rio --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(),
);

run_test( "14", "rio register options",
    "rgpio --dev=f --rpi5  rio  --RioIn --RioOut",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioOut     norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      norm   0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
## Errors
#---------------------------------------------------------------------------

run_test( "20", "error bad register",
    "rgpio --dev=f --rpi5  rio --RioBad",
    1,
    Stderr => q(
	Error:  unknown option:  --RioBad
    ),
    Stdout => q(),
);

run_test( "21", "error modify requires register",
    "rgpio --dev=f --rpi5  rio --norm=0x00ff00ff",
    1,
    Stderr => q(
	Error:  modify requires register option
    ),
    Stdout => q(),
);

run_test( "22", "error modify multiple registers",
    "rgpio --dev=f --rpi5  rio --RioOut --RioOutEn --norm=0x00ff00ff",
    1,
    Stderr => q(
	Error:  modify requires only one register option
    ),
    Stdout => q(),
);

run_test( "23", "error extra arguments",
    "rgpio --dev=f --rpi5  rio  --RioOut  RioInSync",
    1,
    Stderr => q(
	Error:  extra arguments:  RioInSync
    ),
    Stdout => q(),
);

run_test( "28", "error non-numeric option",
    "rgpio --dev=f --rpi5  rio --norm=0bad",
    1,
    Stderr => q(
	Error:  invalid argument:  non-numeric option value:  0bad
    ),
    Stdout => q(),
);

run_test( "29", "error bank 1 not supported",
    "rgpio --dev=f --rpi5  rio -1",
    1,
    Stderr => q(
	Error:  only Bank0 is supported
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Read
#---------------------------------------------------------------------------

run_test( "30", "read selected registers",
    "rgpio --dev=f --rpi5  rio  --RioIn --RioInSync",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioIn      norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  norm   0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "31", "read selected registers, all atomic",
    "rgpio --dev=f --rpi5  rio  --RioIn --RioInSync --all",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioIn      norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      peek   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      set    0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      clr    0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  peek   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  set    0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  clr    0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "32", "read --peek",
    "rgpio --dev=f --rpi5  rio  --peek",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioOut     peek   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioOutEn   peek   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioIn      peek   0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioInSync  peek   0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "33", "read --hex",
    "rgpio --dev=f --rpi5  rio  --RioOutEn --set --clr --norm --hex",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  RioOutEn   norm
	0x00000000  RioOutEn   set 
	0x00000000  RioOutEn   clr 
    ),
    # <space> at end of line
);

#---------------------------------------------------------------------------
## Write Atomic registers
#---------------------------------------------------------------------------

run_test( "40", "write --norm",
    "rgpio --dev=f --rpi5  rio  --RioOut --norm=0x800000ff",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x800000ff  RioOut     norm   1000 0000 0000 0000 0000 0000 1111 1111
    ),
);

run_test( "41", "write --flip",
    "rgpio --dev=f --rpi5  rio  --RioOutEn --flip=0x4000abba",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x4000abba  RioOutEn   peek   0100 0000 0000 0000 1010 1011 1011 1010
    ),
);

run_test( "42", "write --set, see norm",
    "rgpio --dev=f --rpi5  rio  --RioIn --set=0x20cc3300 --norm",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioIn      norm   0000 0000 0000 0000 0000 0000 0000 0000
	0x20cc3300  RioIn      set    0010 0000 1100 1100 0011 0011 0000 0000
    ),
);

run_test( "43", "write --clr",
    "rgpio --dev=f --rpi5  rio  --RioInSync --clr=0x1000ffff",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x1000ffff  RioInSync  clr    0001 0000 0000 0000 1111 1111 1111 1111
    ),
);

run_test( "44", "write --flip, see all",
    "rgpio --dev=f --rpi5  rio  --RioOutEn --flip=0xff00f0f0 --all",
    0,
    Stderr => q(),
    Stdout => q(
	  Register bit:                 28   24   20   16   12    8    4    0
	0x00000000  RioOutEn   norm   0000 0000 0000 0000 0000 0000 0000 0000
	0xff00f0f0  RioOutEn   peek   1111 1111 0000 0000 1111 0000 1111 0000
	0x00000000  RioOutEn   set    0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  RioOutEn   clr    0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

