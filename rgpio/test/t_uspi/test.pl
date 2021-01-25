#! /usr/bin/perl -w
# 2021-01-24  William A. Hudson

# Testing:  rgpio-uspi  subcommand.
#    10-19  uspi basic options --help
#    20-29  Modify errors
#    30-39  Tx, Rx
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
## uspi basic options --help
#---------------------------------------------------------------------------

run_test( "11", "uspi no args",
    "rgpio --dev=f  uspi",
    0,
    Stderr => q(),
);

run_test( "12", "uspi help",
    "rgpio --dev=f  uspi --EnableSerial_1=1 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f  uspi --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio --dev=f  uspi 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

run_test( "15", "bad uspi number options",
    "rgpio --dev=f  uspi -0 -3",
    1,
    Stderr => q(
	Error:  unknown option:  -0
	Error:  unknown option:  -3
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Modify errors
#---------------------------------------------------------------------------

run_test( "21", "AuxEn modify errors",
    "rgpio --dev=f  uspi --SpiEnable_1=2",
    1,
    Stderr => q(
	Error:  require --SpiEnable_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "22a", "Cntl0 modify errors",
    "rgpio --dev=f  uspi --Speed_12=0x1000 --ChipSelects_3=8 --PostInMode_1=2 --VariableCs_1=2 --VariableWidth_1=2 --DoutHoldTime_2=4 --EnableSerial_1=2",
    1,
    Stderr => q(
	Error:  require --Speed_12={0..4095}:  4096
	Error:  require --ChipSelects_3={0..7}:  8
	Error:  require --PostInMode_1={0,1}:  2
	Error:  require --VariableCs_1={0,1}:  2
	Error:  require --VariableWidth_1={0,1}:  2
	Error:  require --DoutHoldTime_2={0,3}:  4
	Error:  require --EnableSerial_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "22b", "Cntl0 modify errors",
    "rgpio --dev=f  uspi --InRising_1=2 --ClearFifos_1=2 --OutRising_1=2 --InvertClk_1=2 --OutMsbFirst_1=2 --ShiftLength_6=64",
    1,
    Stderr => q(
	Error:  require --InRising_1={0,1}:  2
	Error:  require --ClearFifos_1={0,1}:  2
	Error:  require --OutRising_1={0,1}:  2
	Error:  require --InvertClk_1={0,1}:  2
	Error:  require --OutMsbFirst_1={0,1}:  2
	Error:  require --ShiftLength_6={0..63}:  64
    ),
    Stdout => q(),
);

run_test( "23", "Cntl1 modify errors",
    "rgpio --dev=f  uspi --CsHighTime_3=8 --TxEmptyIrq_1=2 --DoneIrq_1=2 --InMsbFirst_1=2 --KeepInput_1=2",
    1,
    Stderr => q(
	Error:  require --CsHighTime_3={0..7}:  8
	Error:  require --TxEmptyIrq_1={0..1}:  2
	Error:  require --DoneIrq_1={0..1}:  2
	Error:  require --InMsbFirst_1={0..1}:  2
	Error:  require --KeepInput_1={0..1}:  2
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
# Tx, Rx
#---------------------------------------------------------------------------

run_test( "30", "--tx needs arguments",
    "rgpio --dev=f  uspi -v --rx=3 --tx",
    1,
    Stderr => q(
	Error:  --tx requires arg values
    ),
    Stdout => q(),
);

run_test( "31", "Tx Rx",
    "rgpio --dev=f  uspi -v --rx=3 --tx  0x5555abba 0x77773cc3",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

