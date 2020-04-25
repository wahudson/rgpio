// 2018-01-05  William A. Hudson
//
// rGPIO Alternate Function Names - documentation
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.102-103  6.2  Alternative Function Assignments
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "rgAddrMap.h"
#include "rgIoPins.h"
#include "rgFselPin.h"

#include "rgAltFuncName.h"

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

rgAltFuncName::rgAltFuncName()
{
}


//--------------------------------------------------------------------------
// Class Data
//--------------------------------------------------------------------------

/*
* Alternate function table.
*     These names are adapted from the BCM documentation, modified to help
*     see what feature it belongs to.
*
* AltFunc[altN][bit]
*     altN  = Alternate function number 0..5 (for Alt0..Alt5).
*     bit   = bit number 0..53
* Names:
*     --    = reserved
*     **    = do NOT USE, may have unexpected results
*/
const char*		rgAltFuncName::AltFunc[][6] = {
 // Alt0          Alt1     Alt2   Alt3        Alt4          Alt5           bit

  { "iic0_SDA",   "sA5",   "--",  "**",       "**",         "**"       }, // 0
  { "iic0_SCL",   "sA4",   "--",  "**",       "**",         "**"       }, // 1
  { "iic1_SDA",   "sA3",   "--",  "**",       "**",         "**"       }, // 2
  { "iic1_SCL",   "sA2",   "--",  "**",       "**",         "**"       }, // 3
  { "gp_CLK0",    "sA1",   "--",  "**",       "**",         "arm_TDI"  }, // 4
  { "gp_CLK1",    "sA0",   "--",  "**",       "**",         "arm_TDO"  }, // 5
  { "gp_CLK2",    "sOEn/sE",  "--",  "**",    "**",         "arm_RTCK" }, // 6
  { "spi0_CE1_n", "sWEn/RWn", "--",  "**",    "**",         "**"       }, // 7
  { "spi0_CE0_n", "sD0",   "--",  "**",       "**",         "**"       }, // 8
  { "spi0_MISO",  "sD1",   "--",  "**",       "**",         "**"       }, // 9
  { "spi0_MOSI",  "sD2",   "--",  "**",       "**",         "**"      }, // 10
  { "spi0_SCLK",  "sD3",   "--",  "**",       "**",         "**"      }, // 11
  { "PWM0",       "sD4",   "--",  "**",       "**",         "arm_TMS" }, // 12
  { "PWM1",       "sD5",   "--",  "**",       "**",         "arm_TCK" }, // 13
  { "u0_TXD",     "sD6",   "--",  "**",       "**",         "u1_TXD"  }, // 14
  { "u0_RXD",     "sD7",   "--",  "**",       "**",         "u1_RXD"  }, // 15
  { "--",         "sD8",   "--",  "u0_CTS",   "spi1_CE2_n", "u1_CTS"  }, // 16
  { "--",         "sD9",   "--",  "u0_RTS",   "spi1_CE1_n", "u1_RTS"  }, // 17
  { "pcm_CLK",   "sD10",   "--",  "bscMOSI",  "spi1_CE0_n", "PWM0"    }, // 18
  { "pcm_FS",    "sD11",   "--",  "bscSCLK",  "spi1_MISO",  "PWM1"    }, // 19
  { "pcm_DIN",   "sD12",   "--",  "bscMISO",  "spi1_MOSI",  "gp_CLK0" }, // 20
  { "pcm_DOUT",  "sD13",   "--",  "bscCE_n",  "spi1_SCLK",  "gp_CLK1" }, // 21
  { "--",        "sD14",   "--",  "sd1_CLK",  "arm_TRST",   "**"      }, // 22
  { "--",        "sD15",   "--",  "sd1_CMD",  "arm_RTCK",   "**"      }, // 23
  { "--",        "sD16",   "--",  "sd1_DAT0", "arm_TDO",    "**"      }, // 24
  { "--",        "sD17",   "--",  "sd1_DAT1", "arm_TCK",    "**"      }, // 25
  { "--",         "--",    "--",  "sd1_DAT2", "arm_TDI",    "**"      }, // 26
  { "--",         "--",    "--",  "sd1_DAT3", "arm_TMS",    "**"      }, // 27

  { "iic0_SDA",   "sA5",   "pcm_CLK",  "--",     "**",      "**"      }, // 28
  { "iic0_SCL",   "sA4",   "pcm_FS",   "--",     "**",      "**"      }, // 29
  { "--",         "sA3",   "pcm_DIN",  "u0_CTS", "**",      "u1_CTS"  }, // 30
  { "--",         "sA2",   "pcm_DOUT", "u0_RTS", "**",      "u1_RTS"  }, // 31
  { "gp_CLK0",    "sA1",   "--",       "u0_TXD", "**",      "u1_TXD"  }, // 32
  { "--" ,        "sA0",   "--",       "u0_RXD", "**",      "u1_RXD"  }, // 33
  { "gp_CLK0",    "sOEn/sE",  "--",    "--",     "**",      "**"      }, // 34
  { "spi0_CE1_n", "sWEn/RWn", "**",    "--",     "**",      "**"      }, // 35
  { "spi0_CE0_n", "sD0",   "u0_TXD",   "--",     "**",      "**"      }, // 36
  { "spi0_MISO",  "sD1",   "u0_RXD",   "--",     "**",      "**"      }, // 37
  { "spi0_MOSI",  "sD2",   "u0_RTS",   "--",     "**",      "**"      }, // 38
  { "spi0_SCLK",  "sD3",   "u0_CTS",   "--",     "**",      "**"      }, // 39
  { "PWM0",       "sD4",   "**",       "--",  "spi2_MISO",  "u1_TXD"  }, // 40
  { "PWM1",       "sD5",   "--",       "--",  "spi2_MOSI",  "u1_RXD"  }, // 41
  { "gp_CLK1",    "sD6",   "--",       "--",  "spi2_SCLK",  "u1_RTS"  }, // 42
  { "gp_CLK2",    "sD7",   "--",       "--",  "spi2_CE0_n", "u1_CTS"  }, // 43
  { "gp_CLK1", "iic0_SDA", "iic1_SDA", "--",  "spi2_CE1_n", "**"      }, // 44
  { "PWM1",    "iic0_SCL", "iic1_SCL", "--",  "spi2_CE2_n", "**"      }, // 45

  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 46
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 47
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 48
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 49
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 50
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 51
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 52
  { "--",         "**",    "**",       "**",     "**",      "**"      }, // 53
};


/*
* Map rgFsel_enum into altN index number.
*     altN_enum[index] = altN number 0..5
*     index            = rgFsel_enum
* Mapping is needed because rgFsel_enum and altN are not in the same order,
* and we want the function name table to be specified in increasing altN order.
*/
const int		rgAltFuncName::altN_enum[] = {
    //     rgFsel_enum index:
    6,	// f_In,  extra
    7,	// f_Out, extra
    5,	// f_Alt5
    4,	// f_Alt4
    0,	// f_Alt0
    1,	// f_Alt1
    2,	// f_Alt2
    3,	// f_Alt3
};


/*
* Lookup Alternate Function name string.
*    Is additional information about the alternate function on a pin.
*    Intended for human use.
* call:  (class or object)
*    str_altfunc_bit( mode, bit )
*    mode = rgFsel_enum, alternate function Fsel mode
*    bit  = bit number 0..53
* return:
*    () = string identifying the alternate function.
*         Max string length is 12 char (currently 10 char).
*/
const char*
rgAltFuncName::str_altfunc_bit(
    rgFselPin::rgFsel_enum	mode,
    int				bit
)
{
    const char			*func;	// return string
    int				nalt;	// altN 0..5

    if ( (bit < 0) || (bit > 53) ) {
	std::ostringstream      css;
	css << "str_altfunc_bit():  bit out-of-range:" << bit;
	throw std::range_error ( css.str() );
    }

    nalt = altN_enum[mode];

    if      ( nalt == 6 ) {
	func = "input";
    }
    else if ( nalt == 7 ) {
	func = "output";
    }
    else {
    	func = AltFunc[bit][nalt];
    }

    return  func;
}

