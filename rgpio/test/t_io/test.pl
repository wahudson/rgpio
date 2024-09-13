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

run_test( "10", "bad platform RPi5",
    "rgpio --dev=f --rpi5  io",
    1,
    Stderr => q(
	Error:  RPi platform:  rgIoPins:  require RPi4 (soc_BCM2711) or earlier
    ),
    Stdout => q(
    ),
);

run_test( "11", "io no args",
    "rgpio --dev=f --rpi4  io",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  PinLevel_w0         0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  EventStatus_w0      0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectRise_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectFall_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectHigh_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectLow_w0        0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectAsyncRise_w0  0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectAsyncFall_w0  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "12", "io help",
    "rgpio --dev=f --rpi4  io --help",
    0,
    Stderr => q(),
);

run_test( "13", "bad option",
    "rgpio --dev=f --rpi4  io --dev=xx",
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

run_test( "20a", "io --set",
    "rgpio --dev=f --rpi4  io -v --set=0xfff  DetectLow_w0",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000fff    --set=            0000 0000 0000 0000 0000 1111 1111 1111
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000fff  DetectLow_w0        0000 0000 0000 0000 0000 1111 1111 1111
    ),
);

run_test( "20b", "io --clr",
    "rgpio --dev=f --rpi4  io --hex --clr=0x033  DetectLow_w0",
    0,
    Stderr => q(),
    Stdout => q(
	0x00000000  DetectLow_w0
    ),
);

run_test( "20c", "io --mask",
    "rgpio --dev=f --rpi4  io --mask=0x0000ffff --value=0xffff3cc3  DetectLow_w0",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00003cc3  DetectLow_w0        0000 0000 0000 0000 0011 1100 1100 0011
    ),
);

#---------------------------------------
run_test( "21", "io --set",
    "rgpio --dev=f --rpi4  io --set=0xf --clr=0x3  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --set invalid with --clr or --mask and --value
	Error:  --clr invalid with --set or --mask and --value
    ),
    Stdout => q(),
);

run_test( "22", "io --set",
    "rgpio --dev=f --rpi4  io --set=0xf --mask=0x3 --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --set invalid with --clr or --mask and --value
    ),
    Stdout => q(),
);

run_test( "23", "io --clr",
    "rgpio --dev=f --rpi4  io --clr=0xf --mask=0x3 --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --clr invalid with --set or --mask and --value
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "24a", "io --mask",
    "rgpio --dev=f --rpi4  io  --mask=0x3  DetectLow_w0",
    1,
    Stderr => q(
	Error:  modify requires --mask --value
    ),
    Stdout => q(),
);

run_test( "24b", "io --value",
    "rgpio --dev=f --rpi4  io  --value=0x0  DetectLow_w0",
    1,
    Stderr => q(
	Error:  modify requires --mask --value
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "25a", "io modify invalid with reg groups",
    "rgpio --dev=f --rpi4  io --clr=0x33 --all  PinLevel_w0",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
    ),
    Stdout => q(),
);

run_test( "25b", "io modify invalid with reg groups",
    "rgpio --dev=f --rpi4  io --clr=0x33 --all --raw  PinLevel_w0",
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
    "rgpio --dev=f --rpi4  io --set=0x33 --w0  PinLevel_w0",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
	Error:  clear only:  EventStatus_w0
    ),
    Stdout => q(),
);

run_test( "25d", "io modify invalid with reg groups",
    "rgpio --dev=f --rpi4  io --clr=0x33 --pud  PinLevel_w0",
    1,
    Stderr => q(
	Error:  modification invalid with --w0 --w1 --fsel --pud --all
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "26", "io --set requires register",	#!! error on default registers
    "rgpio --dev=f --rpi4  io --set=0x33",
    1,
    Stderr => q(
	Error:  modification requires register argument
	Error:  clear only:  EventStatus_w0
    ),
    Stdout => q(),
);

run_test( "27", "io unknown register",
    "rgpio --dev=f --rpi4  io --set=0x33  DetectLow_w0 FooFoo_w0",
    1,
    Stderr => q(
	Error:  unknown register:  FooFoo_w0
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "28a", "io --set",
    "rgpio --dev=f --rpi4  io --hex --set=zz --clr=3cpo  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --set non-numeric value:  zz
	Error:  --clr non-numeric value:  3cpo
	Error:  --set invalid with --clr or --mask and --value
	Error:  --clr invalid with --set or --mask and --value
    ),
    Stdout => q(),
);

run_test( "28b", "io non-numeric error",
    "rgpio --dev=f --rpi4  io --mask=0x3zz --value=3ff  DetectLow_w0",
    1,
    Stderr => q(
	Error:  --mask non-numeric value:  0x3zz
	Error:  --value non-numeric value:  3ff
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Outputs
#---------------------------------------------------------------------------

run_test( "30", "io --raw instead of virtual registers",
    "rgpio --dev=f --rpi4  io --raw --hex",
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
    ),
);

run_test( "31", "io -0 -1 alias",
    "rgpio --dev=f --rpi4  io -0 -1 --hex",
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

run_test( "32", "io --w1",
    "rgpio --dev=f --rpi4  io --w1 --hex",
    0,
    Stderr => q(),
    Stdout => q(
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

run_test( "33", "io --fsel",
    "rgpio --dev=f --rpi4  io --fsel",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  Fsel0               0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  Fsel1               0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  Fsel2               0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  Fsel3               0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  Fsel4               0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  Fsel5               0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------
run_test( "34a", "io --pud",
    "rgpio --dev=f --rpi3  io --pud",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  PudProgMode         0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w0       0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PudProgClk_w1       0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "34b", "io --pud",
    "rgpio --dev=f --rpi4  io --pud",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  PullSel0            0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PullSel1            0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PullSel2            0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  PullSel3            0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------
run_test( "35", "io --all binary output",
    "rgpio --dev=f --rpi3  io --all",
    0,
    Stderr => q(),
);

run_test( "36", "io  register args",
    "rgpio --dev=f --rpi4  io  PinLevel_w0 DetectHigh_w1",
    0,
    Stderr => q(),
    Stdout => q(
	IO Pin Registers:                 28   24   20   16   12    8    4    0
	0x00000000  PinLevel_w0         0000 0000 0000 0000 0000 0000 0000 0000
	0x00000000  DetectHigh_w1       0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "37", "io  register args",
    "rgpio --dev=f --rpi4  io -v --set=0x00ff5533  PinLevel_w0 DetectHigh_w1",
    0,
    Stderr => q(),
    Stdout => q(
	0x00ff5533    --set=            0000 0000 1111 1111 0101 0101 0011 0011
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

