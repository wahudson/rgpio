#! /usr/bin/perl -w
# 2021-02-11  William A. Hudson

# Testing:  rgpio-pull  subcommand.
#    10-19  pull basic options --help
#    20-29  Modify Bits
#    30-39  Modify Words
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
## pull basic options --help
#---------------------------------------------------------------------------

run_test( "11", "pull no args",
    "rgpio --dev=f  pull",
    0,
    Stderr => q(),
);

run_test( "12", "pull help",
    "rgpio --dev=f  pull --PullSel0=0xaaaa5555 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f  pull --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

#---------------------------------------
run_test( "14a", "error multiple directions",
    "rgpio --dev=f  pull --up --down 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "14b", "error multiple directions",
    "rgpio --dev=f  pull --up --off 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "14c", "error multiple directions",
    "rgpio --dev=f  pull --off --down 21",
    1,
    Stderr => q(
	Error:  require only one of:  --up --down --off
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "15a", "error missing direction",
    "rgpio --dev=f  pull  7",
    1,
    Stderr => q(
	Error:  modify requires one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "15b", "error missing direction",
    "rgpio --dev=f  pull --w0=0x00000000",
    1,
    Stderr => q(
	Error:  modify requires one of:  --up --down --off
    ),
    Stdout => q(),
);

run_test( "15c", "error missing direction",
    "rgpio --dev=f  pull --w1=0x00000000",
    1,
    Stderr => q(
	Error:  modify requires one of:  --up --down --off
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "16a", "error missing bits",
    "rgpio --dev=f  pull --up",
    1,
    Stderr => q(
	Error:  modify requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

run_test( "16b", "error missing bits",
    "rgpio --dev=f  pull --down",
    1,
    Stderr => q(
	Error:  modify requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

run_test( "16c", "error missing bits",
    "rgpio --dev=f  pull --off",
    1,
    Stderr => q(
	Error:  modify requires bit arguments or:  --w0=V --w1=V
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Modify Bits
#---------------------------------------------------------------------------

run_test( "21", "error bit too large",
    "rgpio --dev=f  pull --up  31 2 58 7",
    1,
    Stderr => q(
	Error:  bit arg out-of-range:  58
    ),
    Stdout => q(),
);

run_test( "22", "error too many bits",
    "rgpio --dev=f  pull --up 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4",
    1,
    Stderr => q(
	Error:  max bit args:  64
    ),
    Stdout => q(),
);

run_test( "23", "good bit numbers, duplicate",
    "rgpio --dev=f  pull -v --up  31 0 57 0 32 12 24 5 6 7",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
## Modify Words
#---------------------------------------------------------------------------

run_test( "31", "modify w0",
    "rgpio --dev=f  pull -v --down --w0=0xffff0307 --w1=0xffff0000",
    0,
    Stderr => q(),
);

run_test( "32", "modify registers",
    "rgpio --dev=f  pull -v --PullSel0=0x55aa00ff --PullSel1=0x5a0f5a0f --PullSel2=0xaaaa5555 --PullSel3=0x5555aaaa",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

