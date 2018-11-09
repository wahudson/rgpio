#! /usr/bin/perl -w
# 2018-04-30  William A. Hudson

# Testing:  rgpio main program.
#    10-19  rgpio basic options --help
#    20-29  .
#    30-39  .
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

run_test( "11b", "rgpio no args",
    "rgpio -v",
    0,
    Stderr => q(),
    Stdout => q(
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "11c", "rgpio no args",
    "rgpio --debug",
    0,
    Stderr => q(),
    Stdout => q(
	Using Fake memory
	Do nothing.  Try 'rgpio --help'
    ),
);

#---------------------------------------
run_test( "12", "rgpio help",
    "rgpio --help",
    0,
    Stderr => q(),
);

run_test( "13", "rgpio bad --dev",
    "rgpio --dev=xx",
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

run_test( "16", "rgpio fake memory",
    "rgpio --dev=f",
    0,
    Stderr => q(),
    Stdout => q(
	Do nothing.  Try 'rgpio --help'
    ),
);

run_test( "17", "rgpio bad subcommand",
    "rgpio foo",
    1,
    Stderr => q(
	Error:  unknown feature:  foo
    ),
    Stdout => q(
    ),
);


#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

