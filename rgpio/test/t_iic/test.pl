#! /usr/bin/perl -w
# 2021-01-06  William A. Hudson

# Testing:  rgpio-iic  subcommand.
#    10-19  iic basic options --help
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
## iic basic options --help
#---------------------------------------------------------------------------

run_test( "10", "bad platform RPi5",
    "rgpio --dev=f --rpi5  iic",
    1,
    Stderr => q(
	Error:  RPi platform:  rgIic:  require RPi4 (soc_BCM2711) or earlier
    ),
    Stdout => q(
    ),
);

run_test( "11", "iic no args",
    "rgpio --dev=f --rpi4  iic",
    0,
    Stderr => q(),
);

run_test( "12", "iic help",
    "rgpio --dev=f --rpi4  iic -1 --IicEnable_1=1 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi4  iic --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio --dev=f --rpi4  iic 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "15a", "RPi3 bad iic number options",
    "rgpio --dev=f --rpi3  iic -3",
    1,
    Stderr => q(
	Error:  require IIC number {-0, -1, -2} for BCM2837
    ),
    Stdout => q(),
);

run_test( "15b", "bad iic number options",
    "rgpio --dev=f --rpi4  iic -8",
    1,
    Stderr => q(
	Error:  unknown option:  -8
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "16a", "good iic numbers",
    "rgpio --dev=f --rpi3  iic -v -0 -1 -2",
    0,
    Stderr => q(),
);

run_test( "16b", "good iic numbers",
    "rgpio --dev=f --rpi4  iic -v -3 -7",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
## Modify errors
#---------------------------------------------------------------------------

run_test( "21", "Cntl modify errors",
    "rgpio --dev=f --rpi4  iic -v -1 --IicEnable_1=2 --IrqRxHalf_1=2 --IrqTxHalf_1=2 --IrqDone_1=2 --StartTrans_1=2 --ClearFifo_2=4 --ReadPacket_1=2",
    1,
    Stderr => q(
	Error:  require --IicEnable_1={0,1}:  2
	Error:  require --IrqRxHalf_1={0,1}:  2
	Error:  require --IrqTxHalf_1={0,1}:  2
	Error:  require --IrqDone_1={0,1}:  2
	Error:  require --StartTrans_1={0,1}:  2
	Error:  require --ClearFifo_2={0..3}:  4
	Error:  require --ReadPacket_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "22", "Stat modify errors",
    "rgpio --dev=f --rpi4  iic -v -1 --ClkTimeout_1=2 --AckErr_1=2 --TransDone_1=2",
    1,
    Stderr => q(
	Error:  require --ClkTimeout_1={0,1}:  2
	Error:  require --AckErr_1={0,1}:  2
	Error:  require --TransDone_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "23", "reg modify errors",
    "rgpio --dev=f --rpi4  iic -v -1 --DataLen_16=0x10000 --SlaveAddr_7=0x100 --ClkDiv_16=0x10000 --Fall2Out_16=0x10000 --Rise2In_16=0x10000 --TimeOut_16=0x10000",
    1,
    Stderr => q(
	Error:  require --DataLen_16={0..65535}:  65536
	Error:  require --SlaveAddr_7={0..127}:  256
	Error:  require --ClkDiv_16={0..65535}:  65536
	Error:  require --Fall2Out_16={0..65535}:  65536
	Error:  require --Rise2In_16={0..65535}:  65536
	Error:  require --TimeOut_16={0..65535}:  65536
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
# Tx, Rx
#---------------------------------------------------------------------------

run_test( "30", "--tx needs arguments",
    "rgpio --dev=f --rpi4  iic -v -1 --rx=3 --tx",
    1,
    Stderr => q(
	Error:  --tx requires arg values
    ),
    Stdout => q(),
);

run_test( "31", "Tx Rx",
    "rgpio --dev=f --rpi4  iic -v -1 --rx=3 --tx  0x5555abba 0x77773cc3",
    0,
    Stderr => q(),
);

run_test( "33", "error --tx is non-numeric",
    "rgpio --dev=f --rpi4  iic -v -1  --tx  0x5555abba  077  0bad",
    1,
    Stderr => q(
	Error:  --tx value is non-numeric:  0bad
    ),
    Stdout => q(
	Iic1:
	+ Grab regs
	+ Write Tx Fifo
	   1.write_Fifo:  0x5555abba
	   1.write_Fifo:  0x0000003f
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

