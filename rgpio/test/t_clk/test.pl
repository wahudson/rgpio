#! /usr/bin/perl -w
# 2019-12-17  William A. Hudson

# Testing:  rgpio-clk  subcommand.
#    10-19  clock basic options --help
#    20-29  Modify with Clock number
#    30-39  Modify field options
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
## clock basic options --help
#---------------------------------------------------------------------------

run_test( "10", "bad platform RPi5",
    "rgpio --dev=f --rpi5  clk",
    1,
    Stderr => q(
	Error:  RPi platform:  rgClk:  require RPi4 (soc_BCM2711) or earlier
    ),
    Stdout => q(
    ),
);

run_test( "11", "clock no args",
    "rgpio --dev=f --rpi4  clk",
    0,
    Stderr => q(),
    Stdout => q(
	Clk0:
	   0.Cntl    = 0x00000000
	   0.Divr    = 0x00000000
	   0.Mash_2    = 0
	   0.Flip_1    = 0
	   0.Busy_1    = 0
	   0.Kill_1    = 0
	   0.Enable_1  = 0
	   0.Source_4  = 0
	   0.DivI_12   = 0
	   0.DivF_12   = 0
    ),
);

run_test( "12", "clock help",
    "rgpio --dev=f --rpi4  clk -0 --Enable_1=1 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi4  clk --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio --dev=f --rpi4  clk 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

run_test( "15", "bad clock name options",
    "rgpio --dev=f --rpi4  clk -3 -pwm -pcm",
    1,
    Stderr => q(
	Error:  unknown option:  -3
	Error:  unknown option:  -pwm
	Error:  unknown option:  -pcm
    ),
    Stdout => q(),
);

run_test( "16", "good clock numbers",
    "rgpio --dev=f --rpi4  clk -0 -1 -2 --pcm --pwm",
    0,
    Stderr => q(),
);

run_test( "17", "--Busy_1 is an option",
    "rgpio --dev=f --rpi4  clk --Busy_1=1",
    0,
    Stderr => q(),
);

#---------------------------------------
run_test( "18a", "clock verbose",
    "rgpio --dev=f --rpi4  clk -1 -v",
    0,
    Stderr => q(),
    Stdout => q(
	Clk1:
	+ Grab regs
	   1.Cntl    = 0x00000000
	   1.Divr    = 0x00000000
	   1.Mash_2    = 0
	   1.Flip_1    = 0
	   1.Busy_1    = 0
	   1.Kill_1    = 0
	   1.Enable_1  = 0
	   1.Source_4  = 0
	   1.DivI_12   = 0
	   1.DivF_12   = 0
    ),
);

run_test( "18b", "clock verbose with modify",
    "rgpio --dev=f --rpi4  clk -1 --verbose --Source_4=6",
    0,
    Stderr => q(),
    Stdout => q(
	Clk1:
	+ Grab regs
	+ Modify regs
	+ BusyCount= 0
	+ Grab regs
	   1.Cntl    = 0x5a000006
	   1.Divr    = 0x5a000000
	   1.Mash_2    = 0
	   1.Flip_1    = 0
	   1.Busy_1    = 0
	   1.Kill_1    = 0
	   1.Enable_1  = 0
	   1.Source_4  = 6
	   1.DivI_12   = 0
	   1.DivF_12   = 0
    ),
);


#---------------------------------------------------------------------------
## Modify with Clock number
#---------------------------------------------------------------------------

run_test( "20", "modify default clock",
    "rgpio --dev=f --rpi4  clk --Mash_2=3",
    0,
    Stderr => q(),
    Stdout => q(
	Clk0:
	   0.Cntl    = 0x5a000600
	   0.Divr    = 0x5a000000
	   0.Mash_2    = 3
	   0.Flip_1    = 0
	   0.Busy_1    = 0
	   0.Kill_1    = 0
	   0.Enable_1  = 0
	   0.Source_4  = 0
	   0.DivI_12   = 0
	   0.DivF_12   = 0
    ),
);

run_test( "21", "modify all clocks",
    "rgpio --dev=f --rpi4  clk -2 -1 -0 --pwm --pcm --Enable_1=1",
    0,
    Stderr => q(),
);

run_test( "22", "modify everything",
    "rgpio --dev=f --rpi4  clk --pwm -2 --Mash_2=3 --Flip_1=1 --Kill_1=1 --Enable_1=1 --Source_4=0xf --DivI_12=4095 --DivF_12=4095",
    0,
    Stderr => q(),
);

run_test( "23", "all modify errors",
    "rgpio --dev=f --rpi4  clk --pwm -2 --Mash_2=4 --Flip_1=2 --Kill_1=2 --Enable_1=2 --Source_4=0x1f --DivI_12=4096 --DivF_12=4096",
    1,
    Stderr => q(
	Error:  require --Mash_2={0..3}:  4
	Error:  require --Flip_1={0,1}:  2
	Error:  require --Kill_1={0,1}:  2
	Error:  require --Enable_1={0,1}:  2
	Error:  require --Source_4={0..15}:  31
	Error:  require --DivI_12={0..4095}:  4096
	Error:  require --DivF_12={0..4095}:  4096
    ),
    Stdout => q(),
);


#---------------------------------------------------------------------------
## Modify field options
#---------------------------------------------------------------------------

#---------------------------------------
run_test( "31b", "bad --Mash_2",
    "rgpio --dev=f --rpi4  clk -0 --Mash_2=4",
    1,
    Stderr => q(
	Error:  require --Mash_2={0..3}:  4
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "36a", "max --Source_4",
    "rgpio --dev=f --rpi4  clk -0 --Source_4=0xf",
    0,
    Stderr => q(),
);

#---------------------------------------
run_test( "37a", "max --DivI_12",
    "rgpio --dev=f --rpi4  clk -0 --DivI_12=4095",
    0,
    Stderr => q(),
);

run_test( "37b", "bad --DivI_12",
    "rgpio --dev=f --rpi4  clk -0 --DivI_12=4096",
    1,
    Stderr => q(
	Error:  require --DivI_12={0..4095}:  4096
    ),
    Stdout => q(),
);


#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

