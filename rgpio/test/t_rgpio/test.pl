#! /usr/bin/perl -w
# 2018-04-30  William A. Hudson

# Testing:  rgpio main program.
#    10-19  rgpio basic options --help
#    20-29  Override --rpi3 --rpi4 --rpi5
#    30-39  Simulation --sim
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

my $TEST_BaseAddr = qx( ../bin/rpi_BaseAddr );	# string e.g. "0x3f000000"
chomp( $TEST_BaseAddr );
print( "TEST_BaseAddr = $TEST_BaseAddr\n" );

my $TEST_isRPi = $TEST_BaseAddr ne "0x00000000";	# TRUE if on RPi

#---------------------------------------------------------------------------
# Configure working directory.
#---------------------------------------------------------------------------

chdir( "tmp" ) || die_Error( "cannot chdir ./tmp\n" );


#---------------------------------------------------------------------------
## rgpio basic options --help
#---------------------------------------------------------------------------

#---------------------------------------
run_test( "11a", "rgpio no args",
    "rgpio",
    0,
    Stderr => q(),
    Stdout => q(
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "11b", "rgpio --verbose",
    "rgpio --dev=f -v",
    0,
    Stderr => q(),
    Stdout => q(
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "11c", "rgpio --debug  rgAddrMap raise/drop cap",
    "( rgpio --debug > /dev/null )",
    0,
    Stderr => ($TEST_isRPi) ? q(
	rgAddrMap:  raise cap:  cap_dac_override,cap_sys_rawio=ep
	rgAddrMap:  drop  cap:  =
    ) : q(),
    Stdout => q(),
);
#!! capability output may vary with OS version

#---------------------------------------
run_test( "12", "rgpio help",
    "rgpio --help",
    0,
    Stderr => q(),
);

run_test( "13", "rgpio bad --dev",
    "rgpio --dev=mm",
    1,
    Stderr => q(
	Error:  require --dev=m|g|f
    ),
    Stdout => q(),
);

run_test( "14", "rgpio /dev/mem",
    "rgpio --dev=m",
    0,
    Stderr => q(),
    Stdout => q(
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "15", "rgpio /dev/gpiomem",
    "rgpio --dev=g",
    0,
    Stderr => q(),
    Stdout => q(
	Do nothing.  Try 'rgpio --help'
    ),
);
#!! RPi5 /dev/gpiomem does not exist.

run_test( "16", "rgpio fake memory",
    "rgpio --dev=f",
    0,
    Stderr => q(),
    Stdout => q(
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "17", "rgpio bad subcommand",
    "rgpio --dev=f  foo",
    1,
    Stderr => q(
	Error:  unknown feature:  foo
    ),
    Stdout => q(
    ),
);

#---------------------------------------------------------------------------
# Override --rpi3 --rpi4 --rpi5
#---------------------------------------------------------------------------

run_test( "20", "conflicting --rpi3 --rpi4",
    "rgpio --dev=f --rpi3 --rpi4",
    1,
    Stderr => q(
	Error:  require only one:  --rpi3 --rpi4 --rpi5
    ),
    Stdout => q(),
);

run_test( "21", "rgpio --rpi3",
    "rgpio --dev=f --rpi3 --debug --TESTMODE",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2837
	+ rgRpiRev::Global.BaseAddr = 0x3f000000
	+ AddrMap.config_BaseAddr() = 0x3f000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "22", "rgpio --rpi4",
    "rgpio --dev=f --rpi4 --debug --TESTMODE",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2711
	+ rgRpiRev::Global.BaseAddr = 0xfe000000
	+ AddrMap.config_BaseAddr() = 0xfe000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "23", "rgpio --rpi5",
    "rgpio --dev=f --rpi5 --debug --TESTMODE",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2712
	+ rgRpiRev::Global.BaseAddr = 0x1f00000000
	+ AddrMap.config_BaseAddr() = 0x1f00000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

#---------------------------------------------------------------------------
# Simulation --sim
#---------------------------------------------------------------------------

run_test( "30", "rgpio fake memory",
    "rgpio --dev=f --TESTMODE --debug",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2835
	+ rgRpiRev::Global.BaseAddr = 0x00000000
	+ AddrMap.config_BaseAddr() = 0x00000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
    # Note BaseAddr=0 suggesting it is unknown.
);

run_test( "31", "rgpio --sim",
    "rgpio --dev=f --TESTMODE --debug --sim",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2835
	+ rgRpiRev::Global.BaseAddr = 0x20000000
	+ AddrMap.config_BaseAddr() = 0x20000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "32", "rgpio -n",
    "rgpio --dev=f --TESTMODE --debug -n",
    0,
    Stderr => q(),
    Stdout => qq(
	+ rgRpiRev::Global.SocEnum  = soc_BCM2835
	+ rgRpiRev::Global.BaseAddr = 0x20000000
	+ AddrMap.config_BaseAddr() = 0x20000000
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

