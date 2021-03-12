#! /usr/bin/perl -w
# 2021-02-11  William A. Hudson

# Testing:  rgpio-pads  subcommand.
#    10-19  basic options --help
#    20-29  Errors
#    30-39  Modify
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
## basic options --help
#---------------------------------------------------------------------------

run_test( "11", "pads no args",
    "rgpio --dev=f  pads",
    0,
    Stderr => q(),
);

run_test( "12", "pads help",
    "rgpio --dev=f  pads --DriveStr_3=5 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f  pads --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(),
);

run_test( "14", "pads one register",
    "rgpio --dev=f  pads -v PadsA_27s0",
    0,
    Stderr => q(),
    Stdout => q(
	Pads Control register:            28   24   20   16   12    8    4    0
	+ Grab register
	0x00000000  PadsA_27s0          0000 0000 0000 0000 0000 0000 0000 0000
	   Passwd_8    = 0x00
	   SlewMax_1   = 0
	   HystEn_1    = 0
	   DriveStr_3  = 0
    ),
);

#---------------------------------------------------------------------------
## Errors
#---------------------------------------------------------------------------

run_test( "20", "error modify requires register",
    "rgpio --dev=f  pads --DriveStr_3=7",
    1,
    Stderr => q(
	Error:  modify requires register argument
    ),
    Stdout => q(),
);

run_test( "21", "error bad register",
    "rgpio --dev=f  pads --DriveStr_3=7 PadsA_27s0  PadsC",
    1,
    Stderr => q(
	Error:  unknown register:  PadsC
    ),
    Stdout => q(),
);

run_test( "22", "error field size",
    "rgpio --dev=f  pads --Passwd_8=0x100 --SlewMax_1=2 --HystEn_1=2 --DriveStr_3=8",
    1,
    Stderr => q(
	Error:  require --Passwd_8<=0xff:  0x100
	Error:  require --SlewMax_1={0,1}:  2
	Error:  require --HystEn_1={0,1}:  2
	Error:  require --DriveStr_3={0..7}:  8
	Error:  modify requires register argument
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Modify
#---------------------------------------------------------------------------

run_test( "30", "modify, default Passwd",
    "rgpio --dev=f  pads -v --DriveStr_3=6  PadsA_27s0",
    0,
    Stderr => q(),
    Stdout => q(
	Pads Control register:            28   24   20   16   12    8    4    0
	+ Grab register
	+ Modify register
	+ Grab register
	0x5a000006  PadsA_27s0          0101 1010 0000 0000 0000 0000 0000 0110
	   Passwd_8    = 0x5a
	   SlewMax_1   = 0
	   HystEn_1    = 0
	   DriveStr_3  = 6
    ),
);

run_test( "31", "modify word value",
    "rgpio --dev=f  pads -v --value=0xcc3333ff --DriveStr_3=2  PadsA_27s0",
    0,
    Stderr => q(),
    Stdout => q(
	Pads Control register:            28   24   20   16   12    8    4    0
	+ Grab register
	+ Modify register
	+ Grab register
	0xcc3333fa  PadsA_27s0          1100 1100 0011 0011 0011 0011 1111 1010
	   Passwd_8    = 0xcc
	   SlewMax_1   = 1
	   HystEn_1    = 1
	   DriveStr_3  = 2
    ),
);

run_test( "32", "modify fields, explicit Passwd",
    "rgpio --dev=f  pads --Passwd_8=0x33 --SlewMax_1=1 --HystEn_1=1 --DriveStr_3=3  PadsA_27s0",
    0,
    Stderr => q(),
    Stdout => q(
	Pads Control register:            28   24   20   16   12    8    4    0
	0x3300001b  PadsA_27s0          0011 0011 0000 0000 0000 0000 0001 1011
	   Passwd_8    = 0x33
	   SlewMax_1   = 1
	   HystEn_1    = 1
	   DriveStr_3  = 3
    ),
);

run_test( "33", "modify all registers",
    "rgpio --dev=f  pads -v --DriveStr_3=3  PadsA_27s0  PadsB_45s28  PadsC_53s46",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

