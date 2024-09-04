#! /usr/bin/perl -w
# 2020-12-29  William A. Hudson

# Testing:  rgpio-info  subcommand.
#    10-19  info basic options --help
#    20-29  info --code=
#    30-39  info --file=
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

my $TEST_isRPi = qx( ../bin/rpi_BaseAddr ) ne "0x00000000\n";

#---------------------------------------------------------------------------
# Configure working directory.
#---------------------------------------------------------------------------

chdir( "tmp" ) || die_Error( "cannot chdir ./tmp\n" );

#---------------------------------------------------------------------------
## info basic options --help
#---------------------------------------------------------------------------

if ( ! $TEST_isRPi ) {
  run_test( "11", "info no args, show object Global",
    "rgpio --dev=f  info",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0x00000000    0000 0000 0000 0000 0000 0000 0000 0000
	    OverVoltageDis_1 = 0
	    OtpWriteDis_1    = 0
	    OtpReadDis_1     = 0
	    WarrantyVoid_1   = 0
	    NewStyle_1       = 0
	    MemSize_3        = 0
	    MfgNumber_4      = 0
	    ChipNumber_4     = 0
	    BoardType_8      = 0
	    BoardRev_4       = 0
	  SocEnum   = soc_BCM2835
	  BaseAddr  = 0x00000000
	  InFile    = /proc/cpuinfo
	NOT on RPi
    ),
  );
  # Testing on RPi - result will depend on the RPi being run.
}

run_test( "12", "info help",
    "rgpio --dev=f  info --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f  info --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "error option combination",
    "rgpio --dev=f  info --code=0 --file=/dev/null",
    1,
    Stderr => q(
	Error:  choose only one:  --file= or --code=
    ),
    Stdout => q(),
);

run_test( "15", "RPi4 info simulate",
    "rgpio --dev=f --rpi4  info",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0x00000000    0000 0000 0000 0000 0000 0000 0000 0000
	    OverVoltageDis_1 = 0
	    OtpWriteDis_1    = 0
	    OtpReadDis_1     = 0
	    WarrantyVoid_1   = 0
	    NewStyle_1       = 0
	    MemSize_3        = 0
	    MfgNumber_4      = 0
	    ChipNumber_4     = 0
	    BoardType_8      = 0
	    BoardRev_4       = 0
	  SocEnum   = soc_BCM2711
	  BaseAddr  = 0x00000000
	  InFile    = /proc/cpuinfo
	NOT on RPi
    ),
    #!! address is not simulated?  Flag non-RPI.
);

#---------------------------------------------------------------------------
# info --code=
#---------------------------------------------------------------------------

run_test( "21", "info --code ok",
    "rgpio --dev=f  info --code=0xffff2fff",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0xffff2fff    1111 1111 1111 1111 0010 1111 1111 1111
	    OverVoltageDis_1 = 1
	    OtpWriteDis_1    = 1
	    OtpReadDis_1     = 1
	    WarrantyVoid_1   = 1
	    NewStyle_1       = 1
	    MemSize_3        = 7
	    MfgNumber_4      = 15
	    ChipNumber_4     = 2
	    BoardType_8      = 255
	    BoardRev_4       = 15
	  SocEnum   = soc_BCM2837
	  BaseAddr  = 0x3f000000
    ),
);

run_test( "22", "info --code bad Soc_enum",
    "rgpio --dev=f  info --code=0xffffcfff",
    1,
    Stderr => q(
	Error:  SocEnum exception:  rgRpiRev_Soc::find() ChipNumber has no enum:  12
    ),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0xffffcfff    1111 1111 1111 1111 1100 1111 1111 1111
	    OverVoltageDis_1 = 1
	    OtpWriteDis_1    = 1
	    OtpReadDis_1     = 1
	    WarrantyVoid_1   = 1
	    NewStyle_1       = 1
	    MemSize_3        = 7
	    MfgNumber_4      = 15
	    ChipNumber_4     = 12
	    BoardType_8      = 255
	    BoardRev_4       = 15
	  SocEnum   = soc_unknown
	  BaseAddr  = 0x00000000
    ),
);

run_test( "23", "info --code RPi5",
    "rgpio --dev=f  info --code=0xffff4fff",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0xffff4fff    1111 1111 1111 1111 0100 1111 1111 1111
	    OverVoltageDis_1 = 1
	    OtpWriteDis_1    = 1
	    OtpReadDis_1     = 1
	    WarrantyVoid_1   = 1
	    NewStyle_1       = 1
	    MemSize_3        = 7
	    MfgNumber_4      = 15
	    ChipNumber_4     = 4
	    BoardType_8      = 255
	    BoardRev_4       = 15
	  SocEnum   = soc_BCM2712
	  BaseAddr  = 0x1f00000000
    ),
    # note 64-bit address
);

#---------------------------------------------------------------------------
# info --file=
#---------------------------------------------------------------------------

run_test( "31", "info --file null",
    "rgpio --dev=f  info --file=/dev/null",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0x00000000    0000 0000 0000 0000 0000 0000 0000 0000
	    OverVoltageDis_1 = 0
	    OtpWriteDis_1    = 0
	    OtpReadDis_1     = 0
	    WarrantyVoid_1   = 0
	    NewStyle_1       = 0
	    MemSize_3        = 0
	    MfgNumber_4      = 0
	    ChipNumber_4     = 0
	    BoardType_8      = 0
	    BoardRev_4       = 0
	  SocEnum   = soc_BCM2835
	  BaseAddr  = 0x00000000
	  InFile    = /dev/null
	NOT on RPi
    ),
);

run_test( "32", "info --file missing",
    "rgpio --dev=f  info --file=missing",
    1,
    Stderr => q(
	Error:  exception caught:  read_rev_code() cannot open file:  missing
    ),
    Stdout => q(),
);

run_test( "33", "info --file RPi3",
    "rgpio --dev=f  info --file=../ref/rpi3.in",
    0,
    Stderr => q(),
    Stdout => q(
	                    bit:      28   24   20   16   12    8    4    0
	  RevCode   = 0x00a22082    0000 0000 1010 0010 0010 0000 1000 0010
	    OverVoltageDis_1 = 0
	    OtpWriteDis_1    = 0
	    OtpReadDis_1     = 0
	    WarrantyVoid_1   = 0
	    NewStyle_1       = 1
	    MemSize_3        = 2
	    MfgNumber_4      = 2
	    ChipNumber_4     = 2
	    BoardType_8      = 8
	    BoardRev_4       = 2
	  SocEnum   = soc_BCM2837
	  BaseAddr  = 0x3f000000
	  InFile    = ../ref/rpi3.in
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

