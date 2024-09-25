#! /usr/bin/perl -w
# 2024-09-24  William A. Hudson

# Testing:  rgpio-fsel5  subcommand.
#    10-19  Basic options --help
#    20-29  Errors
#    30-39  Output formats
#    40-49  Modify
#    50-59  --show

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
## Basic options --help
#---------------------------------------------------------------------------

run_test( "10", "fsel5 bad platform RPi4",
    "rgpio --dev=f --rpi4  fsel5",
    1,
    Stderr => q(
	Error:  RPi platform:  rgsIoCon::  require RPi5 (soc_BCM2712)
    ),
    Stdout => q(),
);

run_test( "11", "fsel5 no args",
    "rgpio --dev=f --rpi5  fsel5",
    0,
    Stderr => q(),
);

run_test( "12", "fsel5 help",
    "rgpio --dev=f --rpi5  fsel5 --func=3 --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi5  fsel5 --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(),
);

run_test( "14a", "fsel5 Gpio bit numbers",
    "rgpio --dev=f --rpi5  fsel5  0 27 4",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Gpio  Mode  Function
	    0    a0  spi0_SIO3
	   27    a0  sd0_DAT3
	    4    a0  gp_CLK0
    ),
);

run_test( "14b", "fsel5 Gpio bit numbers",
    "rgpio --dev=f --rpi5  fsel5  --gpio=0x000003f0",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Gpio  Mode  Function
	    4    a0  gp_CLK0
	    5    a0  gp_CLK1
	    6    a0  gp_CLK2
	    7    a0  spi0_CSn1
	    8    a0  spi0_CSn0
	    9    a0  spi0_SIO1
    ),
);

run_test( "15", "Bank0 only",
    "rgpio --dev=f --rpi5  fsel5 -1",
    1,
    Stderr => q(
	Error:  only Bank0 is supported
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Errors
#---------------------------------------------------------------------------

run_test( "20", "error modify without Gpio bits",
    "rgpio --dev=f --rpi5  fsel5 --func=3",
    1,
    Stderr => q(
	Error:  modify requires Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "21", "error mixed Gpio spec methods",
    "rgpio --dev=f --rpi5  fsel5 --gpio=0x00000033  2 3 4",
    1,
    Stderr => q(
	Error:  --gpio= not valid with Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "22", "error non-numeric Gpio bit number",
    "rgpio --dev=f --rpi5  fsel5  1 7 3cpo",
    1,
    Stderr => q(
	Error:  bit arg non-numeric:  3cpo
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "23a", "error field ranges",
    "rgpio --dev=f --rpi5  fsel5  --FuncSel_5=32 --InOver_2=4 --OutOver_2=4  0",
    1,
    Stderr => q(
	Error:  require --InOver_2 <= 3:  4
	Error:  require --OutOver_2 <= 3:  4
	Error:  require --FuncSel_5={0..31}:  32
    ),
    Stdout => q(),
);

run_test( "23b", "error field ranges",
    "rgpio --dev=f --rpi5  fsel5  --func=32 --FilterM_7=128 --OutEnOver_2=4  0",
    1,
    Stderr => q(
	Error:  require --OutEnOver_2 <= 3:  4
	Error:  require --FilterM_7 <= 127:  128
	Error:  require --FuncSel_5={0..31}:  32
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Output formats
#---------------------------------------------------------------------------

run_test( "30a", "list only",
    "rgpio --dev=f --rpi5  fsel5  --list --func=31  3 4",
    0,
    Stderr => q(),
    Stdout => q(
	 Gpio  Mode  Function
	    3   a31  null
	    4   a31  null
    ),
);

run_test( "30b", "table only",
    "rgpio --dev=f --rpi5  fsel5  --table --func=31  3 4",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 0000 0001 1000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 000f f000
    ),
);

run_test( "30c", "table and list",
    "rgpio --dev=f --rpi5  fsel5  --table --list --func=31  3 4",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 0000 0001 1000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 000f f000
	 Gpio  Mode  Function
	    3   a31  null
	    4   a31  null
    ),
);

#---------------------------------------------------------------------------
## Modify
#---------------------------------------------------------------------------

run_test( "41", "modify ",
    "rgpio --dev=f --rpi5  fsel5  --InOver_2=3 --OutOver_2=3 --OutEnOver_2=2  4 5 6 7",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 3333 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 2222 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 3333 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Gpio  Mode  Function
	    4    a0  gp_CLK0
	    5    a0  gp_CLK1
	    6    a0  gp_CLK2
	    7    a0  spi0_CSn1
    ),
);

run_test( "42", "modify ",
    "rgpio --dev=f --rpi5  fsel5 --FilterM_7=127 --FuncSel_5=30 --gpio=0x0f00",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [11:9]  ---- 0000 0000 0000 0000 7777 0000 0000
	   FilterM_7       [ 8:5]  ---- 0000 0000 0000 0000 ffff 0000 0000
	   FuncSel_5       [4]     ---- 0000 0000 0000 0000 1111 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 eeee 0000 0000
	 Gpio  Mode  Function
	    8   a30  --
	    9   a30  --
	   10   a30  --
	   11   a30  --
    ),
);

#---------------------------------------------------------------------------
## --show
#---------------------------------------------------------------------------

run_test( "50", "error --show with modification",
    "rgpio --dev=f --rpi5  fsel5  --show --func=5  2 3 4",
    1,
    Stderr => q(
	Error:  modification not valid with --show
    ),
    Stdout => q(),
);

run_test( "51", "--show",
    "rgpio --dev=f --rpi5  fsel5 --show  7 4 0",
    0,
    Stderr => q(),
);
# <space> at end of lines

run_test( "52", "--show all",
    "rgpio --dev=f --rpi5  fsel5 --show",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

