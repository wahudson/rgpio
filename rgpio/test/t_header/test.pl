#! /usr/bin/perl -w
# 2023-02-11  William A. Hudson

# Testing:  rgpio-header  subcommand.
#    10-19  header basic options, --help
#    20-29  --row, --signal, --power
#    30-39  --mode
#    40-49  --show
#    50-59  --gpio

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
## header basic options, --help
#---------------------------------------------------------------------------

run_test( "10", "header no args RPi5",
    "rgpio --dev=f --rpi5  header",
    0,
    Stderr => q(),
);

run_test( "11", "header no args",
    "rgpio --dev=f --rpi4  header",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	  1    --      --    +3.3V
	    2  --      --    +5V
	  3    Gpio2   In    input
	    4  --      --    +5V
	  5    Gpio3   In    input
	    6  --      --    GND
	  7    Gpio4   In    input
	    8  Gpio14  In    input
	  9    --      --    GND
	   10  Gpio15  In    input
	 11    Gpio17  In    input
	   12  Gpio18  In    input
	 13    Gpio27  In    input
	   14  --      --    GND
	 15    Gpio22  In    input
	   16  Gpio23  In    input
	 17    --      --    +3.3V
	   18  Gpio24  In    input
	 19    Gpio10  In    input
	   20  --      --    GND
	 21    Gpio9   In    input
	   22  Gpio25  In    input
	 23    Gpio11  In    input
	   24  Gpio8   In    input
	 25    --      --    GND
	   26  Gpio7   In    input
	 27    Gpio0   In    input
	   28  Gpio1   In    input
	 29    Gpio5   In    input
	   30  --      --    GND
	 31    Gpio6   In    input
	   32  Gpio12  In    input
	 33    Gpio13  In    input
	   34  --      --    GND
	 35    Gpio19  In    input
	   36  Gpio16  In    input
	 37    Gpio26  In    input
	   38  Gpio20  In    input
	 39    --      --    GND
	   40  Gpio21  In    input
    ),
);

run_test( "12", "header help",
    "rgpio --dev=f --rpi4  header --help",
    0,
    Stderr => q(),
);

run_test( "13", "bad option",
    "rgpio --dev=f --rpi4  header --dev=xx",
    1,
    Stderr => q(
	Error:  unknown option:  --dev=xx
    ),
    Stdout => q(
    ),
);

run_test( "14", "valid header pins",
    "rgpio --dev=f --rpi4  header 1 40",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	  1    --      --    +3.3V
	   40  Gpio21  In    input
    ),
);

run_test( "15", "invalid header pins",
    "rgpio --dev=f --rpi4  header 41 0",
    1,
    Stderr => q(
	Error:  pin arg out-of-range {1..40}:  41
	Error:  pin arg out-of-range {1..40}:  0
    ),
    Stdout => q(),
);

run_test( "16", "error non-numeric header pin number",
    "rgpio --dev=f --rpi4  header 3 7h",
    1,
    Stderr => q(
	Error:  pin arg non-numeric:  7h
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## --row, --signal, --power
#---------------------------------------------------------------------------

run_test( "21", "pins by row",
    "rgpio --dev=f --rpi4  header --row",
    0,
    Stderr => q(),
    Stdout => q(
 Pin   Gpio    Mode  Function
  1    --      --    +3.3V
  3    Gpio2   In    input
  5    Gpio3   In    input
  7    Gpio4   In    input
  9    --      --    GND
 11    Gpio17  In    input
 13    Gpio27  In    input
 15    Gpio22  In    input
 17    --      --    +3.3V
 19    Gpio10  In    input
 21    Gpio9   In    input
 23    Gpio11  In    input
 25    --      --    GND
 27    Gpio0   In    input
 29    Gpio5   In    input
 31    Gpio6   In    input
 33    Gpio13  In    input
 35    Gpio19  In    input
 37    Gpio26  In    input
 39    --      --    GND
    2  --      --    +5V
    4  --      --    +5V
    6  --      --    GND
    8  Gpio14  In    input
   10  Gpio15  In    input
   12  Gpio18  In    input
   14  --      --    GND
   16  Gpio23  In    input
   18  Gpio24  In    input
   20  --      --    GND
   22  Gpio25  In    input
   24  Gpio8   In    input
   26  Gpio7   In    input
   28  Gpio1   In    input
   30  --      --    GND
   32  Gpio12  In    input
   34  --      --    GND
   36  Gpio16  In    input
   38  Gpio20  In    input
   40  Gpio21  In    input
    ),
);

run_test( "22", "power pins",
    "rgpio --dev=f --rpi4  header --power",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	  1    --      --    +3.3V
	    2  --      --    +5V
	    4  --      --    +5V
	    6  --      --    GND
	  9    --      --    GND
	   14  --      --    GND
	 17    --      --    +3.3V
	   20  --      --    GND
	 25    --      --    GND
	   30  --      --    GND
	   34  --      --    GND
	 39    --      --    GND
    ),
);

run_test( "23", "gpio pins",
    "rgpio --dev=f --rpi4  header --signal",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	  3    Gpio2   In    input
	  5    Gpio3   In    input
	  7    Gpio4   In    input
	    8  Gpio14  In    input
	   10  Gpio15  In    input
	 11    Gpio17  In    input
	   12  Gpio18  In    input
	 13    Gpio27  In    input
	 15    Gpio22  In    input
	   16  Gpio23  In    input
	   18  Gpio24  In    input
	 19    Gpio10  In    input
	 21    Gpio9   In    input
	   22  Gpio25  In    input
	 23    Gpio11  In    input
	   24  Gpio8   In    input
	   26  Gpio7   In    input
	 27    Gpio0   In    input
	   28  Gpio1   In    input
	 29    Gpio5   In    input
	 31    Gpio6   In    input
	   32  Gpio12  In    input
	 33    Gpio13  In    input
	 35    Gpio19  In    input
	   36  Gpio16  In    input
	 37    Gpio26  In    input
	   38  Gpio20  In    input
	   40  Gpio21  In    input
    ),
);

#!!
run_test( "24", "invalid --row combination",
    "rgpio --dev=f --rpi4  header --signal --row",
    1,
    Stderr => q(
	Error:  --row not valid with --signal --power
    ),
    Stdout => q(),
);

run_test( "25", "invalid --row combination",
    "rgpio --dev=f --rpi4  header --row --power",
    1,
    Stderr => q(
	Error:  --row not valid with --signal --power
    ),
    Stdout => q(),
);

#---------------------------------------
run_test( "28a", "invalid --row with pin list",
    "rgpio --dev=f --rpi4  header --row 7",
    1,
    Stderr => q(
	Error:  argv not valid with --signal --power --row
    ),
    Stdout => q(),
);

run_test( "28b", "invalid --signal with pin list",
    "rgpio --dev=f --rpi4  header --signal 7",
    1,
    Stderr => q(
	Error:  argv not valid with --signal --power --row
    ),
    Stdout => q(),
);

run_test( "28c", "invalid --power with pin list",
    "rgpio --dev=f --rpi4  header --power 7",
    1,
    Stderr => q(
	Error:  argv not valid with --signal --power --row
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
## --mode
#---------------------------------------------------------------------------

run_test( "30", "modify all pins",
    "rgpio --dev=f --rpi4  header --mode=Alt0",
    0,
    Stderr => q(),
);

run_test( "31", "modify pin list",
    "rgpio --dev=f --rpi4  header --mode=Out  5 6 7",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	  5    Gpio3   Out   output
	    6  --      --    GND
	  7    Gpio4   Out   output
    ),
);

run_test( "32", "verbose modify pin list",
    "rgpio --dev=f --rpi3  header -v --mode=Alt4  10 11 12",
    0,
    Stderr => q(),
    Stdout => q(
	+ Modify pins
	 Pin   Gpio    Mode  Function
	   10  Gpio15  Alt4  **
	 11    Gpio17  Alt4  spi1_CE1_n
	   12  Gpio18  Alt4  spi1_CE0_n
    ),
);

run_test( "34", "bad mode name",
    "rgpio --dev=f --rpi4  header --mode=alt0  4",
    1,
    Stderr => q(
	Error:  unknown pin function select:  --mode=alt0
	    rgFselPin::find_rgFsel_enum() no enum for string:  alt0
    ),
    Stdout => q(),
);

run_test( "37", "invalid --mode combination",
    "rgpio --dev=f --rpi4  header --mode=In --row --power --show",
    1,
    Stderr => q(
	Error:  --row not valid with --signal --power
	Error:  --mode not valid with --power
	Error:  --mode not valid with --show
    ),
    Stdout => q(),
);

run_test( "38", "valid --mode with --signal",
    "rgpio --dev=f --rpi3  header -v --mode=Alt4 --signal",
    0,
    Stderr => q(),
);

run_test( "39", "valid --mode with --gpio",
    "rgpio --dev=f --rpi3  header --mode=Alt4 --gpio",
    0,
    Stderr => q(),
);

#---------------------------------------------------------------------------
## --show
#---------------------------------------------------------------------------

run_test( "40", "show pin list",
    "rgpio --dev=f --rpi3  header --show  7",
    0,
    Stderr => q(),
    Stdout => q(
 Pin  Gpio    Alt0       Alt1       Alt2       Alt3       Alt4       Alt5
  7   Gpio4   gp_CLK0    sm_A1      dpi_D0     **         **         arm_TDI   
    ),
    # trailing space
);

run_test( "41", "RPi3 show pins",
    "rgpio --dev=f --rpi3  header --show",
    0,
    Stderr => q(),
);

run_test( "42", "RPi4 show pins",
    "rgpio --dev=f --rpi4  header --show",
    0,
    Stderr => q(),
);

run_test( "43", "header --show invalid with --mode",
    "rgpio --dev=f --rpi4  header --show --mode=In  7",
    1,
    Stderr => q(
	Error:  --mode not valid with --show
    ),
    Stdout => q(),
);

run_test( "44", "show gpio bits",
    "rgpio --dev=f --rpi3  header --show --gpio  8 4",
    0,
    Stderr => q(),
    Stdout => q(
 Pin  Gpio    Alt0       Alt1       Alt2       Alt3       Alt4       Alt5
   24 Gpio8   spi0_CE0_n sm_D0      dpi_D4     **         **         **        
  7   Gpio4   gp_CLK0    sm_A1      dpi_D0     **         **         arm_TDI   
    ),
    # trailing space
);

#---------------------------------------------------------------------------
## --gpio
#---------------------------------------------------------------------------

run_test( "50", "gpio default",
    "rgpio --dev=f --rpi4  header --gpio",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	 27    Gpio0   In    input
	   28  Gpio1   In    input
	  3    Gpio2   In    input
	  5    Gpio3   In    input
	  7    Gpio4   In    input
	 29    Gpio5   In    input
	 31    Gpio6   In    input
	   26  Gpio7   In    input
	   24  Gpio8   In    input
	 21    Gpio9   In    input
	 19    Gpio10  In    input
	 23    Gpio11  In    input
	   32  Gpio12  In    input
	 33    Gpio13  In    input
	    8  Gpio14  In    input
	   10  Gpio15  In    input
	   36  Gpio16  In    input
	 11    Gpio17  In    input
	   12  Gpio18  In    input
	 35    Gpio19  In    input
	   38  Gpio20  In    input
	   40  Gpio21  In    input
	 15    Gpio22  In    input
	   16  Gpio23  In    input
	   18  Gpio24  In    input
	   22  Gpio25  In    input
	 37    Gpio26  In    input
	 13    Gpio27  In    input
    ),
);

run_test( "51", "gpio bits",
    "rgpio --dev=f --rpi4  header --gpio  8 2 7",
    0,
    Stderr => q(),
    Stdout => q(
	 Pin   Gpio    Mode  Function
	   24  Gpio8   In    input
	  3    Gpio2   In    input
	   26  Gpio7   In    input
    ),
);

run_test( "52", "valid --gpio with --power",
    "rgpio --dev=f --rpi4  header --gpio --power",
    0,
    Stderr => q(),
);

run_test( "53", "invalid --gpio combination",
    "rgpio --dev=f --rpi4  header --gpio --row --signal",
    1,
    Stderr => q(
	Error:  --gpio not valid with --signal --row
	Error:  --row not valid with --signal --power
    ),
    Stdout => q(),
);

run_test( "54", "error non-numeric Gpio bit number",
    "rgpio --dev=f --rpi4  header --gpio 5 7c",
    1,
    Stderr => q(
	Error:  bit arg non-numeric:  7c
    ),
    Stdout => q(),
);

#---------------------------------------------------------------------------
# Check that all tests ran.
#---------------------------------------------------------------------------

End_test();		# last test exit

__END__

