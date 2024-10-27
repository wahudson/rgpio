#! /usr/bin/perl -w
# 2024-06-16  William A. Hudson

# Testing:  rgpio-rpad  subcommand.
#    10-19  basic options --help
#    20-29  Errors
#    30-39  Read
#    40-49  Modify Fields
#    50-59  Modify Atomic registers

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
## basic options --help
#---------------------------------------------------------------------------

run_test( "10", "rpad bad platform RPi4",
    "rgpio --dev=f --rpi4  rpad",
    1,
    Stderr => q(
	Error:  RPi platform:  rgsIoPads::  require RPi5 (soc_BCM2712)
    ),
    Stdout => q(),
);

run_test( "11", "rpad no args, default Table format",
    "rgpio --dev=f --rpi5  rpad  6 7",		# Gpio bits have no effect (OK)
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "12", "rpad help",
    "rgpio --dev=f --rpi5  rpad --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi5  rpad --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(),
);

run_test( "14", "rpad list format bit numbers",
    "rgpio --dev=f --rpi5  rpad --debug --list 8 9 4 5",
    0,
    Stderr => q(),
    Stdout => q(
	+ 0.FeatureAddr  = 0x400f0000
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad( 4)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad( 5)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
## Errors
#---------------------------------------------------------------------------

run_test( "20", "error --norm= requires Gpio bits",
    "rgpio --dev=f --rpi5  rpad --norm=0x00ff00ff",
    1,
    Stderr => q(
	Error:  modify requires Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "21", "error --DriveStr_2= range",
    "rgpio --dev=f --rpi5  rpad --DriveStr_2=4  2 3",
    1,
    Stderr => q(
	Error:  require --DriveStr_2={0..3}:  4
    ),
    Stdout => q(),
);

run_test( "22", "error multiple atomic reg write options",
    "rgpio --dev=f --rpi5  rpad --norm=0x77 --flip=0x99  2 3",
    1,
    Stderr => q(
	Error:  require only one write atomic address --norm= ..
    ),
    Stdout => q(),
);

run_test( "23", "error --gpio=Mask and Gpio bit numbers",
    "rgpio --dev=f --rpi5  rpad --gpio=0x00f  2 3",
    1,
    Stderr => q(
	Error:  --gpio= not valid with Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "24", "error field and register modification",
    "rgpio --dev=f --rpi5  rpad --InEnable_1=1 --norm=0x77  2 3",
    1,
    Stderr => q(
	Error:  field modification not valid with write atomic
    ),
    Stdout => q(),
);

run_test( "25", "error --gpio= range",
    "rgpio --dev=f --rpi5  rpad --gpio=0x10000000",
    1,
    Stderr => q(
	Error:  require --gpio=mask <= 0x0fffffff
    ),
    Stdout => q(),
);

run_test( "26", "error non-numeric Gpio bit number",
    "rgpio --dev=f --rpi5  rpad  --norm=0x77  2 3cpo 4",
    1,
    Stderr => q(
	Error:  bit arg non-numeric:  3cpo
    ),
    Stdout => q(),
);

run_test( "29", "error bad Gpio bit number",
    "rgpio --dev=f --rpi5  rpad  28",
    1,
    Stderr => q(
	Error:  bit arg out-of-range:  28
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Read
#---------------------------------------------------------------------------

run_test( "30", "read, Gpio bits have no effect (is OK)",
    "rgpio --dev=f --rpi5  rpad  4 3 2",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "31", "read field table and Gpio bit registers",
    "rgpio --dev=f --rpi5  rpad --peek --norm  4 3",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad( 4)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek  0.IoPad( 4)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad( 3)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek  0.IoPad( 3)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "32", "read, show atomic registers only",
    "rgpio --dev=f --rpi5  rpad --list --gpio=0x0f00",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad(10)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm  0.IoPad(11)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "33", "read all 4 atomic registers",
    "rgpio --dev=f --rpi5  rpad --all  16",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek  0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set   0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  clr   0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "34", "read all 4 atomic reg, list only",
    "rgpio --dev=f --rpi5  rpad --all --list  16",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek  0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set   0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  clr   0.IoPad(16)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
## Modify Fields
#---------------------------------------------------------------------------

run_test( "41", "modify fields",
    "rgpio --dev=f --rpi5  rpad --OutDisable_1=1 --InEnable_1=1  4 5",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0011 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0011 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "42", "modify fields",
    "rgpio --dev=f --rpi5  rpad --DriveStr_2=3 --PullUpEn_1=1  8 9 10 11",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 3333 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 1111 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "43", "modify fields",
    "rgpio --dev=f --rpi5  rpad --PullDnEn_1=1 --HystEn_1=1 --SlewFast_1=1 5 4",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0011 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0011 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0011 0000
    ),
);

run_test( "44", "modify with --gpio=mask",
    "rgpio --dev=f --rpi5  rpad --DriveStr_2=2 --HystEn_1=1  --gpio=0x0ff00f0c",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 2222 2222 0000 0000 2222 0000 2200
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 1111 1111 0000 0000 1111 0000 1100
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "45", "modify field, show atomic with --norm",
    "rgpio --dev=f --rpi5  rpad -v --DriveStr_2=3 --norm  2 3 24 25",
    0,
    Stderr => q(),
    Stdout => q(
	+ Modify registers field
	+ Read registers
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0033 0000 0000 0000 0000 0000 3300
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
	+ Read registers
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000030  norm  0.IoPad( 2)  0000 0000 0000 0000 0000 0000 0011 0000
	   0x00000030  norm  0.IoPad( 3)  0000 0000 0000 0000 0000 0000 0011 0000
	   0x00000030  norm  0.IoPad(24)  0000 0000 0000 0000 0000 0000 0011 0000
	   0x00000030  norm  0.IoPad(25)  0000 0000 0000 0000 0000 0000 0011 0000
    ),
);

#---------------------------------------------------------------------------
## Modify Atomic registers
#---------------------------------------------------------------------------

run_test( "51", "modify reg, table and list",
    "rgpio --dev=f --rpi5  rpad --norm=0x00f00037  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0033 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0011 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0011 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0011 0000 0000
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00f00037  norm  0.IoPad( 8)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm  0.IoPad( 9)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "52", "flip reg, list only",
    "rgpio --dev=f --rpi5  rpad --list --flip=0x00f00037  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00f00037  peek  0.IoPad( 8)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  peek  0.IoPad( 9)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "53", "set reg, view additional reg",
    "rgpio --dev=f --rpi5  rpad --list --set=0xf0000037 --norm  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  set   0.IoPad( 8)  1111 0000 0000 0000 0000 0000 0011 0111
	   0x00000000  norm  0.IoPad( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  set   0.IoPad( 9)  1111 0000 0000 0000 0000 0000 0011 0111
    ),
);

run_test( "54", "clear reg, view all reg",
    "rgpio --dev=f --rpi5  rpad -v --list --clr=0xf0000037 --all  21",
    0,
    Stderr => q(),
    Stdout => q(
	+ Write atomic registers
	+ Read registers
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00000000  norm  0.IoPad(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek  0.IoPad(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set   0.IoPad(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  clr   0.IoPad(21)  1111 0000 0000 0000 0000 0000 0011 0111
    ),
);

run_test( "55", "modify reg by --gpio=, table and list",
    "rgpio --dev=f --rpi5  rpad --norm=0x00f00037 --gpio=0x0f000001",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 3333 0000 0000 0000 0000 0000 0003
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 1111 0000 0000 0000 0000 0000 0001
	   0.HystEn_1        [1]     ---- 1111 0000 0000 0000 0000 0000 0001
	   0.SlewFast_1      [0]     ---- 1111 0000 0000 0000 0000 0000 0001
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00f00037  norm  0.IoPad( 0)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm  0.IoPad(24)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm  0.IoPad(25)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm  0.IoPad(26)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm  0.IoPad(27)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "56", "see modify set reg, norm table",
    "rgpio --dev=f --rpi5  rpad --set=0x00f00037 --gpio=0x0f000001",
    0,
    Stderr => q(),
    Stdout => q(
	 IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0
	   0.OutDisable_1    [7]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.InEnable_1      [6]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.DriveStr_2      [5:4]   ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullUpEn_1      [3]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.PullDnEn_1      [2]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.HystEn_1        [1]     ---- 0000 0000 0000 0000 0000 0000 0000
	   0.SlewFast_1      [0]     ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:          28   24   20   16   12    8    4    0
	   0x00f00037  set   0.IoPad( 0)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set   0.IoPad(24)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set   0.IoPad(25)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set   0.IoPad(26)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set   0.IoPad(27)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

