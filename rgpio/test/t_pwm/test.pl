#! /usr/bin/perl -w
# 2019-12-07  William A. Hudson

# Testing:  rgpio-pwm  subcommand.
#    10-19  pwm basic options --help
#    20-29  Set registers
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
## pwm basic options --help
#---------------------------------------------------------------------------

run_test( "10", "bad platform RPi5",
    "rgpio --dev=f --rpi5  pwm",
    1,
    Stderr => q(
	Error:  RPi platform:  rgPwm:  require RPi4 (soc_BCM2711) or earlier
    ),
    Stdout => q(
    ),
);


run_test( "11", "pwm no args",
    "rgpio --dev=f --rpi4  pwm",
    0,
    Stderr => q(),
);

run_test( "12", "pwm help",
    "rgpio --dev=f --rpi4  pwm --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi4  pwm --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "extra argument",
    "rgpio --dev=f --rpi4  pwm 2",
    1,
    Stderr => q(
	Error:  extra arguments:  2
    ),
    Stdout => q(),
);

run_test( "15", "bad option",
    "rgpio --dev=f --rpi4  pwm -0",
    1,
    Stderr => q(
	Error:  unknown option:  -0
    ),
    Stdout => q(),
);

run_test( "16", "--Busy_1 not an option",
    "rgpio --dev=f --rpi4  pwm --Busy_1=0",
    1,
    Stderr => q(
	Error:  unknown option:  --Busy_1=0
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Set registers
#---------------------------------------------------------------------------

run_test( "20", "no --Fifo option",
    "rgpio --dev=f --rpi4  pwm --Fifo=99",
    1,
    Stderr => q(
	Error:  unknown option:  --Fifo=99
    ),
    Stdout => q(),
);

run_test( "21", "modify registers",
    "rgpio --dev=f --rpi4  pwm --Cntl=0xffffffff --Stat=0xffffffff --DmaConf=0xffffffff" .
    " --Ch2Range=0xffffffff --Ch2Data=0xffffffff" .
    " --Ch1Range=0xffffffff --Ch1Data=0xffffffff",
    0,
    Stderr => q(),
);

run_test( "22", "modify registers",
    "rgpio --dev=f --rpi4  pwm --Cntl=0x11111111 --Stat=0x22222222 --DmaConf=0x33333333" .
    " --Ch2Range=0x44444444 --Ch2Data=0x55555555" .
    " --Ch1Range=0x66666666 --Ch1Data=0x77777777",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
## Modify field options
#---------------------------------------------------------------------------

run_test( "30", "modify field",
    "rgpio --dev=f --rpi4  pwm -v --ClearFifo_1=1 --Ch2_GapErr_1=1 --DmaEnable_1=1",
    0,
    Stderr => q(),
);

run_test( "31", "apply reg then field",
    "rgpio --dev=f --rpi4  pwm -v --Cntl=0xffffffff --ClearFifo_1=0 --Stat=0xffffffff --Ch1_GapErr_1=0",
    0,
    Stderr => q(),
);

run_test( "32", "bad option range",
    "rgpio --dev=f --rpi4  pwm --Ch2_MsEnable_1=2 --DmaPanicLev_8=0x100",
    1,
    Stderr => q(
	Error:  require --Ch2_MsEnable_1={0,1}:  2
	Error:  require --DmaPanicLev_8={0..255}:  256
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Write Fifo
#---------------------------------------------------------------------------

run_test( "40", "write fifo",
    "rgpio --dev=f --rpi4  pwm -v --Cntl=0xffffffff --Ch2_MsEnable_1=0 --tx 0x111 0x222 0x333",
    0,
    Stderr => q(),
);

run_test( "41", "error --tx is non-numeric",
    "rgpio --dev=f --rpi4  pwm -v --Ch2_MsEnable_1=1 --tx  0x111 077 0bad",
    1,
    Stderr => q(
	Error:  --tx value is non-numeric:  0bad
    ),
    Stdout => q(
	PWM:
	+ Grab regs
	+ Modify regs
	+ Write Fifo
	   write Fifo:  0x00000111
	   write Fifo:  0x0000003f
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

