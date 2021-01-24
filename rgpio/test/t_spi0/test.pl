#! /usr/bin/perl -w
# 2020-01-19  William A. Hudson

# Testing:  rgpio-spi0  subcommand.
#    10-19  spi0 basic options --help
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
## spi0 basic options --help
#---------------------------------------------------------------------------

run_test( "11", "spi0 no args",
    "rgpio --dev=f  spi0",
    0,
    Stderr => q(),
);

run_test( "12", "spi0 help",
    "rgpio --dev=f  spi0 --RunActive_1=1 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f  spi0 --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio --dev=f  spi0 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "15a", "bad spi0 number options",
    "rgpio --dev=f  spi0 -1 -2 -7",
    1,
    Stderr => q(
	Error:  unknown option:  -1
	Error:  unknown option:  -2
	Error:  unknown option:  -7
    ),
    Stdout => q(),
);

run_test( "15b", "RPi3 bad spi0 number options",
    "rgpio --dev=f --rpi3  spi0 -3 -4 -5 -6",
    1,
    Stderr => q(
	Error:  require spi number {-0} for BCM2837
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "16", "RPi4 all spi0 options",
    "rgpio --dev=f --rpi4  spi0 -v -0 -3 -4 -5 -6",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
## Modify errors
#---------------------------------------------------------------------------

run_test( "21", "CntlStat modify errors",
    "rgpio --dev=f  spi0 --LossiWord_1=2 --LossiDmaEn_1=2 --CsPolarity_3=8 --LossiEnable_1=2 --ReadEnable_1=2 --DmaEndCs_1=2 --IrqRxHalf_1=2 --IrqTxEmpty_1=2",
    1,
    Stderr => q(
	Error:  require --LossiWord_1={0,1}:  2
	Error:  require --LossiDmaEn_1={0,1}:  2
	Error:  require --CsPolarity_3={0..7}:  8
	Error:  require --LossiEnable_1={0,1}:  2
	Error:  require --ReadEnable_1={0,1}:  2
	Error:  require --DmaEndCs_1={0,1}:  2
	Error:  require --IrqRxHalf_1={0,1}:  2
	Error:  require --IrqTxEmpty_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "22", "CntlStat modify errors",
    "rgpio --dev=f  spi0 --DmaEnable_1=2 --RunActive_1=2 --CsPolarity_1=2 --ClearRxTxFifo_2=4 --ClockPolarity_1=2 --ClockPhase_1=2 --ChipSelectN_2=2",
    1,
    Stderr => q(
	Error:  require --DmaEnable_1={0,1}:  2
	Error:  require --RunActive_1={0,1}:  2
	Error:  require --CsPolarity_1={0,1}:  2
	Error:  require --ClearRxTxFifo_2={0..3}:  4
	Error:  require --ClockPolarity_1={0,1}:  2
	Error:  require --ClockPhase_1={0,1}:  2
    ),
    Stdout => q(),
);

run_test( "23", "register bit field modify errors",
    "rgpio --dev=f  spi0 --ClockDiv_16=0x10000 --DmaDataLen_16=0x10000 --LossiHoldDly_4=0x10",
    1,
    Stderr => q(
	Error:  require --ClockDiv_16 <= 0xffff:  65536
	Error:  require --DmaDataLen_16 <= 0xffff:  65536
	Error:  require --LossiHoldDly_4 <= 0xf:  16
    ),
    Stdout => q(),
);

run_test( "24", "DmaReq modify errors",
    "rgpio --dev=f  spi0 --DmaRxPanicLev_8=0x100 --DmaRxReqLev_8=0x100 --DmaTxPanicLev_8=0x100 --DmaTxReqLev_8=0x100",
    1,
    Stderr => q(
	Error:  require --DmaRxPanicLev_8 <= 0xff:  256
	Error:  require --DmaRxReqLev_8 <= 0xff:  256
	Error:  require --DmaTxPanicLev_8 <= 0xff:  256
	Error:  require --DmaTxReqLev_8 <= 0xff:  256
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
# Tx, Rx
#---------------------------------------------------------------------------

run_test( "31", "Tx Rx",
    "rgpio --dev=f  spi0 -v --rx=3 --tx  0x5555abba 0x77773cc3",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

