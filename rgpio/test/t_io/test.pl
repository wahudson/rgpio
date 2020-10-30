#! /usr/bin/perl -w
# 2018-05-25  William A. Hudson

# Testing:  rgpio-io  subcommand.
#    10-19  io basic options --help
#    20-29  Modify
#    30-39  Outputs
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
## io basic options --help
#---------------------------------------------------------------------------

run_test( "11", "io no args",
    "rgpio --dev=f  io",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  PinLevel_w0
	0x00000000  EventStatus_w0
	0x00000000  DetectRise_w0
	0x00000000  DetectFall_w0
	0x00000000  DetectHigh_w0
	0x00000000  DetectLow_w0
	0x00000000  DetectAsyncRise_w0
	0x00000000  DetectAsyncFall_w0

	0x00000000  PinLevel_w1
	0x00000000  EventStatus_w1
	0x00000000  DetectRise_w1
	0x00000000  DetectFall_w1
	0x00000000  DetectHigh_w1
	0x00000000  DetectLow_w1
	0x00000000  DetectAsyncRise_w1
	0x00000000  DetectAsyncFall_w1
    ),
);

run_test( "12", "io help",
    "rgpio --dev=f  io --help",
    0,
    Stderr => q(),
);

run_test( "13", "bad option",
    "rgpio --dev=f  io --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);


#---------------------------------------------------------------------------
## Modify
#---------------------------------------------------------------------------

run_test( "20", "io --set",
    "rgpio --dev=f  io --set=0xfff  DetectLow_w0",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000fff  DetectLow_w0
    ),
);

run_test( "20b", "io --clr",
    "rgpio --dev=f  io --clr=0x033  DetectLow_w0",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  DetectLow_w0
    ),
);

run_test( "21", "io --set",
    "rgpio --dev=f  io --set=0xf --clr=0x3  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --set invalid with --clr or --mask and --value
	Error:  --clr invalid with --set or --mask and --value
    ),
    Stdout => q(),
);

run_test( "22", "io --set",
    "rgpio --dev=f  io --set=0xf --mask=0x3 --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --set invalid with --clr or --mask and --value
    ),
    Stdout => q(),
);

run_test( "23", "io --clr",
    "rgpio --dev=f  io --clr=0xf --mask=0x3 --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --clr invalid with --set or --mask and --value
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "24a", "io --mask",
    "rgpio --dev=f  io  --mask=0x3  DetectLow_w0",
    1,
    Stderr => q(
	Error:  modify requires --mask --value
    ),
    Stdout => q(),
);

run_test( "24b", "io --value",
    "rgpio --dev=f  io  --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  modify requires --mask --value
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "25a", "io modify invalid with reg groups",
    "rgpio --dev=f  io --clr=0x33 --all",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
    ),
    Stdout => q(),
);

run_test( "25b", "io modify invalid with reg groups",
    "rgpio --dev=f  io --clr=0x33 --all --raw",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
	Error:  set only:  PinSet_w0
	Error:  set only:  PinSet_w1
	Error:  read only:  PinRead_w0
	Error:  read only:  PinRead_w1
    ),
    Stdout => q(),
);

run_test( "25c", "io modify invalid with reg groups",
    "rgpio --dev=f  io --set=0x33 --w0",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
	Error:  clear only:  EventStatus_w0
    ),
    Stdout => q(),
);

run_test( "25d", "io modify invalid with reg groups",
    "rgpio --dev=f  io --clr=0x33 --pud",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Outputs
#---------------------------------------------------------------------------

run_test( "31", "io --raw instead of pseudo registers",
    "rgpio --dev=f  io --raw",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  PinSet_w0
	0x00000000  PinClr_w0
	0x00000000  PinRead_w0
	0x00000000  EventStatus_w0
	0x00000000  DetectRise_w0
	0x00000000  DetectFall_w0
	0x00000000  DetectHigh_w0
	0x00000000  DetectLow_w0
	0x00000000  DetectAsyncRise_w0
	0x00000000  DetectAsyncFall_w0

	0x00000000  PinSet_w1
	0x00000000  PinClr_w1
	0x00000000  PinRead_w1
	0x00000000  EventStatus_w1
	0x00000000  DetectRise_w1
	0x00000000  DetectFall_w1
	0x00000000  DetectHigh_w1
	0x00000000  DetectLow_w1
	0x00000000  DetectAsyncRise_w1
	0x00000000  DetectAsyncFall_w1
    ),
);

run_test( "32", "io --w0",
    "rgpio --dev=f  io --w0",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  PinLevel_w0
	0x00000000  EventStatus_w0
	0x00000000  DetectRise_w0
	0x00000000  DetectFall_w0
	0x00000000  DetectHigh_w0
	0x00000000  DetectLow_w0
	0x00000000  DetectAsyncRise_w0
	0x00000000  DetectAsyncFall_w0
    ),
);

run_test( "33", "io --fsel",
    "rgpio --dev=f  io --fsel",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  Fsel0
	0x00000000  Fsel1
	0x00000000  Fsel2
	0x00000000  Fsel3
	0x00000000  Fsel4
	0x00000000  Fsel5
    ),
);

run_test( "34", "io --pud",
    "rgpio --dev=f  io --pud",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  PudProgMode
	0x00000000  PudProgClk_w0
	0x00000000  PudProgClk_w1
    ),
);

run_test( "35", "io --all -v  binary output",
    "rgpio --dev=f  io --all -v",
    0,
    Stderr => q(),
);

run_test( "36", "io  register args",
    "rgpio --dev=f  io  PinLevel_w0 DetectHigh_w1",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  PinLevel_w0
	0x00000000  DetectHigh_w1
    ),
);

run_test( "37", "io  register args",
    "rgpio --dev=f  io -v --set=0x00ff5533  PinLevel_w0 DetectHigh_w1",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  PinLevel_w0         0000 0000 0000 0000 0000 0000 0000 0000
	0x00ff5533  DetectHigh_w1       0000 0000 1111 1111 0101 0101 0011 0011
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

