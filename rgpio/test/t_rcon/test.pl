#! /usr/bin/perl -w
# 2024-10-17  William A. Hudson

# Testing:  rgpio-rcon  subcommand.
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

run_test( "10", "rcon bad platform RPi4",
    "rgpio --dev=f --rpi4  rcon",
    1,
    Stderr => q(
	Error:  RPi platform:  rgsIoCon::  require RPi5 (soc_BCM2712)
    ),
    Stdout => q(),
);

run_test( "11", "rcon no args, default Table format",
    "rgpio --dev=f --rpi5  rcon  6 7",		# Gpio bits have no effect (OK)
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "12", "rcon help",
    "rgpio --dev=f --rpi5  rcon --help",
    0,
    Stderr => q(),
);

run_test( "13", "unknown option",
    "rgpio --dev=f --rpi5  rcon --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(),
);

run_test( "14", "rcon list format bit numbers",
    "rgpio --dev=f --rpi5  rcon --debug --list 8 9 1 0",
    0,
    Stderr => q(),
    Stdout => q(
	+ FeatureAddr  = 0x400d0000
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl( 1)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl( 0)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
## Errors
#---------------------------------------------------------------------------

run_test( "20", "error --norm= requires Gpio bits",
    "rgpio --dev=f --rpi5  rcon --norm=0x00ff00ff",
    1,
    Stderr => q(
	Error:  modify requires Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "21", "error --InOver_2= range",
    "rgpio --dev=f --rpi5  rcon --InOver_2=4  2 3",
    1,
    Stderr => q(
	Error:  require --InOver_2={0..3}:  4
    ),
    Stdout => q(),
);

run_test( "22", "error multiple atomic reg write options",
    "rgpio --dev=f --rpi5  rcon --norm=0x77 --flip=0x99  2 3",
    1,
    Stderr => q(
	Error:  require only one write atomic address --norm= ..
    ),
    Stdout => q(),
);

run_test( "23", "error --gpio=Mask and Gpio bit numbers",
    "rgpio --dev=f --rpi5  rcon --gpio=0x00f  2 3",
    1,
    Stderr => q(
	Error:  --gpio= not valid with Gpio bit numbers
    ),
    Stdout => q(),
);

run_test( "24", "error field and register modification",
    "rgpio --dev=f --rpi5  rcon --InOver_2=1 --norm=0x77  2 3",
    1,
    Stderr => q(
	Error:  field modification not valid with write atomic
    ),
    Stdout => q(),
);

run_test( "25", "error --gpio= range",
    "rgpio --dev=f --rpi5  rcon --gpio=0x10000000",
    1,
    Stderr => q(
	Error:  require --gpio=mask <= 0x0fffffff
    ),
    Stdout => q(),
);

run_test( "26", "error non-numeric Gpio bit number",
    "rgpio --dev=f --rpi5  rcon  --norm=0x77  2 3cpo 4",
    1,
    Stderr => q(
	Error:  bit arg non-numeric:  3cpo
    ),
    Stdout => q(),
);

run_test( "29", "error bad Gpio bit number",
    "rgpio --dev=f --rpi5  rcon  28",
    1,
    Stderr => q(
	Error:  bit arg out-of-range:  28
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## Read
#---------------------------------------------------------------------------

run_test( "31", "read IoCntl field table and Gpio bit registers",
    "rgpio --dev=f --rpi5  rcon --peek --norm  5 4",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl( 5)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek IoCntl( 5)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl( 4)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek IoCntl( 4)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "32", "read, show atomic registers only",
    "rgpio --dev=f --rpi5  rcon --list --gpio=0x0f00",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl(10)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  norm IoCntl(11)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "33", "read IoStat all 4 atomic registers",
    "rgpio --dev=f --rpi5  rcon --all  27",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set  IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  clr  IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "34", "read all 4 atomic reg, list only",
    "rgpio --dev=f --rpi5  rcon --all --list  27",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set  IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  clr  IoCntl(27)  0000 0000 0000 0000 0000 0000 0000 0000
    ),
);

#---------------------------------------------------------------------------
## Modify Fields IoCntl
#---------------------------------------------------------------------------

run_test( "41", "modify fields, default to IoCntl output",
    "rgpio --dev=f --rpi5  rcon --FuncSel_5=30 --FilterM_7=127  4 5",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0077 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 00ff 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0011 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 00ee 0000
    ),
);

run_test( "42", "modify fields",
    "rgpio --dev=f --rpi5  rcon  --OutOver_2=3 --OutEnOver_2=2 --InOver_2=3 --EdgeReset_1=1 --IrqOver_2=3  6 7 8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0033 3300 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0011 1100 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0033 3300 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0022 2200 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0033 3300 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "43", "modify fields",
    "rgpio --dev=f --rpi5  rcon --ImaskFall_1=1 --ImaskRise_1=1 --ImaskLow_1=1 --ImaskHigh_1=1 --ImaskFiltFall_1=1 --ImaskFiltRise_1=1 --ImaskFiltLow_1=1 --ImaskFiltHigh_1=1  --gpio=0x0c000f03",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskFiltLow_1  [26]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskFiltRise_1 [25]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskFiltFall_1 [24]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskHigh_1     [23]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskLow_1      [22]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskRise_1     [21]    ---- 1100 0000 0000 0000 1111 0000 0011
	   ImaskFall_1     [20]    ---- 1100 0000 0000 0000 1111 0000 0011
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
    ),
);

run_test( "45", "modify field, show atomic with --norm",
    "rgpio --dev=f --rpi5  rcon -v --FilterM_7=0x5a --norm  2 3 24 25",
    0,
    Stderr => q(),
    Stdout => q(
	+ Modify registers field
	+ Read registers
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0055 0000 0000 0000 0000 0000 5500
	   FilterM_7       [8:5]   ---- 00aa 0000 0000 0000 0000 0000 aa00
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	+ Read registers
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000b40  norm IoCntl( 2)  0000 0000 0000 0000 0000 1011 0100 0000
	   0x00000b40  norm IoCntl( 3)  0000 0000 0000 0000 0000 1011 0100 0000
	   0x00000b40  norm IoCntl(24)  0000 0000 0000 0000 0000 1011 0100 0000
	   0x00000b40  norm IoCntl(25)  0000 0000 0000 0000 0000 1011 0100 0000
    ),
);

#---------------------------------------------------------------------------
## Modify Atomic registers
#---------------------------------------------------------------------------

run_test( "51", "modify reg, table and list",
    "rgpio --dev=f --rpi5  rcon --norm=0x00f00037  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0011 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0011 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0011 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0011 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0011 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0011 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0077 0000 0000
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00f00037  norm IoCntl( 8)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm IoCntl( 9)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "52", "flip reg, list only",
    "rgpio --dev=f --rpi5  rcon --list --flip=0x00f00037  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00f00037  peek IoCntl( 8)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  peek IoCntl( 9)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "53", "set reg, view additional reg",
    "rgpio --dev=f --rpi5  rcon --list --set=0xf0000037 --norm  8 9",
    0,
    Stderr => q(),
    Stdout => q(
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl( 8)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  set  IoCntl( 8)  1111 0000 0000 0000 0000 0000 0011 0111
	   0x00000000  norm IoCntl( 9)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  set  IoCntl( 9)  1111 0000 0000 0000 0000 0000 0011 0111
    ),
);

run_test( "54", "clear reg, view all reg",
    "rgpio --dev=f --rpi5  rcon -v --list --clr=0xf0000037 --all  21",
    0,
    Stderr => q(),
    Stdout => q(
	+ Write atomic registers
	+ Read registers
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00000000  norm IoCntl(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  peek IoCntl(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0x00000000  set  IoCntl(21)  0000 0000 0000 0000 0000 0000 0000 0000
	   0xf0000037  clr  IoCntl(21)  1111 0000 0000 0000 0000 0000 0011 0111
    ),
);

run_test( "55", "modify reg by --gpio=, table and list",
    "rgpio --dev=f --rpi5  rcon --norm=0x00f00037 --gpio=0x0f000001",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 1111 0000 0000 0000 0000 0000 0001
	   ImaskLow_1      [22]    ---- 1111 0000 0000 0000 0000 0000 0001
	   ImaskRise_1     [21]    ---- 1111 0000 0000 0000 0000 0000 0001
	   ImaskFall_1     [20]    ---- 1111 0000 0000 0000 0000 0000 0001
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 1111 0000 0000 0000 0000 0000 0001
	   FuncSel_5      [4]      ---- 1111 0000 0000 0000 0000 0000 0001
	   FuncSel_5       [3:0]   ---- 7777 0000 0000 0000 0000 0000 0007
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00f00037  norm IoCntl( 0)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm IoCntl(24)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm IoCntl(25)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm IoCntl(26)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  norm IoCntl(27)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

run_test( "56", "see modify set reg, norm table",
    "rgpio --dev=f --rpi5  rcon --set=0x00f00037 --gpio=0x0f000001",
    0,
    Stderr => q(),
    Stdout => q(
	 IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0
	   IrqOver_2       [31:30] ---- 0000 0000 0000 0000 0000 0000 0000
	   EdgeReset_1     [28]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltHigh_1 [27]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltLow_1  [26]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltRise_1 [25]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFiltFall_1 [24]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskHigh_1     [23]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskLow_1      [22]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskRise_1     [21]    ---- 0000 0000 0000 0000 0000 0000 0000
	   ImaskFall_1     [20]    ---- 0000 0000 0000 0000 0000 0000 0000
	   InOver_2        [17:16] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutEnOver_2     [15:14] ---- 0000 0000 0000 0000 0000 0000 0000
	   OutOver_2       [13:12] ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7      [11:9]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FilterM_7       [8:5]   ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5      [4]      ---- 0000 0000 0000 0000 0000 0000 0000
	   FuncSel_5       [3:0]   ---- 0000 0000 0000 0000 0000 0000 0000
	 Read Atomic register bit:        28   24   20   16   12    8    4    0
	   0x00f00037  set  IoCntl( 0)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set  IoCntl(24)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set  IoCntl(25)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set  IoCntl(26)  0000 0000 1111 0000 0000 0000 0011 0111
	   0x00f00037  set  IoCntl(27)  0000 0000 1111 0000 0000 0000 0011 0111
    ),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

