#! /usr/bin/perl -w
# 2018-06-30  William A. Hudson

# Testing:  rgpio-clock  subcommand.
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

run_test( "11", "clock no args",
    "rgpio clock",
    0,
    Stderr => q(),
    Stdout => q(
	  0.CtlReg= 0x00000000
	  0.DivReg= 0x00000000
	  0.Mash_2=   0
	  0.Flip_1=   0
	  0.Busy_1=   0
	  0.Kill_1=   0
	  0.Enable_1= 0
	  0.Source_4= 0
	  0.DivI_12=  0
	  0.DivF_12=  0

    ),
);

run_test( "12", "clock help",
    "rgpio clock --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio clock --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio clock 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

run_test( "15", "bad clock number",
    "rgpio clock -3",
    1,
    Stderr => q(
	Error:  unknown option:  -3
    ),
    Stdout => q(),
);

run_test( "16", "good clock numbers",
    "rgpio clock -0 -1 -2",
    0,
    Stderr => q(),
);

run_test( "17", "--Busy_1 not an option",
    "rgpio clock --Busy_1=0",
    1,
    Stderr => q(
	Error:  unknown option:  --Busy_1=0
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "18a", "clock verbose",
    "rgpio clock -1 -v",
    0,
    Stderr => q(),
    Stdout => q(
	Clock Controls:
	  1.CtlReg= 0x00000000
	  1.DivReg= 0x00000000
	  1.Mash_2=   0
	  1.Flip_1=   0
	  1.Busy_1=   0
	  1.Kill_1=   0
	  1.Enable_1= 0
	  1.Source_4= 0
	  1.DivI_12=  0
	  1.DivF_12=  0

    ),
);

run_test( "18b", "clock verbose",
    "rgpio clock -1 --verbose",
    0,
    Stderr => q(),
    Stdout => q(
	Clock Controls:
	  1.CtlReg= 0x00000000
	  1.DivReg= 0x00000000
	  1.Mash_2=   0
	  1.Flip_1=   0
	  1.Busy_1=   0
	  1.Kill_1=   0
	  1.Enable_1= 0
	  1.Source_4= 0
	  1.DivI_12=  0
	  1.DivF_12=  0

    ),
);


#---------------------------------------------------------------------------
## Modify with Clock number
#---------------------------------------------------------------------------

run_test( "20", "modify default clock",
    "rgpio clock --Mash_2=3",
    0,
    Stderr => q(),
    Stdout => q(
	  0.CtlReg= 0x5a000600
	  0.DivReg= 0x5a000000
	  0.Mash_2=   3
	  0.Flip_1=   0
	  0.Busy_1=   0
	  0.Kill_1=   0
	  0.Enable_1= 0
	  0.Source_4= 0
	  0.DivI_12=  0
	  0.DivF_12=  0

    ),
);

run_test( "21", "modify all clocks",
    "rgpio clock -0 -1 -2 --Enable_1=1",
    0,
    Stderr => q(),
);

run_test( "22", "modify everything",
    "rgpio clock -0 -1 -2 --Mash_2=3 --Flip_1=1 --Kill_1=1 --Enable_1=1 --Source_4=0xf --DivI_12=4095 --DivF_12=4095",
    0,
    Stderr => q(),
);

run_test( "23", "all modify errors",
    "rgpio clock -0 -1 -2 --Mash_2=4 --Flip_1=2 --Kill_1=2 --Enable_1=2 --Source_4=0x1f --DivI_12=4096 --DivF_12=4096",
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
    "rgpio clock -0 --Mash_2=4",
    1,
    Stderr => q(
	Error:  require --Mash_2={0..3}:  4
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "35a", "max --Source_4",
    "rgpio clock -0 --Source_4=0xf",
    0,
    Stderr => q(),
);

#---------------------------------------
run_test( "37a", "max --DivF_12",
    "rgpio clock -0 --DivF_12=4095",
    0,
    Stderr => q(),
);

run_test( "37b", "bad --DivF_12",
    "rgpio clock -0 --DivF_12=4096",
    1,
    Stderr => q(
	Error:  require --DivF_12={0..4095}:  4096
    ),
    Stdout => q(),
);


#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

