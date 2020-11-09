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
* Alternate function name table.
*     These names are adapted from the BCM documentation, modified to help
*     see what feature it belongs to.
*     Named set from BCM Doc "BCM2835 ARM Peripherals (06 February 2012)".
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

  { "iic0_SDA",   "sm_A5", "--",  "**",       "**",         "**"       }, // 0
  { "iic0_SCL",   "sm_A4", "--",  "**",       "**",         "**"       }, // 1
  { "iic1_SDA",   "sm_A3", "--",  "**",       "**",         "**"       }, // 2
  { "iic1_SCL",   "sm_A2", "--",  "**",       "**",         "**"       }, // 3
  { "gp_CLK0",    "sm_A1", "--",  "**",       "**",         "arm_TDI"  }, // 4
  { "gp_CLK1",    "sm_A0", "--",  "**",       "**",         "arm_TDO"  }, // 5
  { "gp_CLK2",    "sm_OE_n", "--", "**",      "**",         "arm_RTCK" }, // 6
  { "spi0_CE1_n", "sm_WE_n", "--", "**",      "**",         "**"       }, // 7
  { "spi0_CE0_n", "sm_D0",  "--", "**",       "**",         "**"       }, // 8
  { "spi0_MISO",  "sm_D1",  "--", "**",       "**",         "**"       }, // 9
  { "spi0_MOSI",  "sm_D2",  "--", "**",       "**",         "**"      }, // 10
  { "spi0_SCLK",  "sm_D3",  "--", "**",       "**",         "**"      }, // 11
  { "PWM0",       "sm_D4",  "--", "**",       "**",         "arm_TMS" }, // 12
  { "PWM1",       "sm_D5",  "--", "**",       "**",         "arm_TCK" }, // 13
  { "u0_TXD",     "sm_D6",  "--", "**",       "**",         "u1_TXD"  }, // 14
  { "u0_RXD",     "sm_D7",  "--", "**",       "**",         "u1_RXD"  }, // 15
  { "--",         "sm_D8",  "--", "u0_CTS",   "spi1_CE2_n", "u1_CTS"  }, // 16
  { "--",         "sm_D9",  "--", "u0_RTS",   "spi1_CE1_n", "u1_RTS"  }, // 17
  { "pcm_CLK",    "sm_D10", "--", "bsc_MOSI", "spi1_CE0_n", "PWM0"    }, // 18
  { "pcm_FS",     "sm_D11", "--", "bsc_SCLK", "spi1_MISO",  "PWM1"    }, // 19
  { "pcm_DIN",    "sm_D12", "--", "bsc_MISO", "spi1_MOSI",  "gp_CLK0" }, // 20
  { "pcm_DOUT",   "sm_D13", "--", "bsc_CE_n", "spi1_SCLK",  "gp_CLK1" }, // 21
  { "--",         "sm_D14", "--", "sd1_CLK",  "arm_TRST",   "**"      }, // 22
  { "--",         "sm_D15", "--", "sd1_CMD",  "arm_RTCK",   "**"      }, // 23
  { "--",         "sm_D16", "--", "sd1_DAT0", "arm_TDO",    "**"      }, // 24
  { "--",         "sm_D17", "--", "sd1_DAT1", "arm_TCK",    "**"      }, // 25
  { "--",         "--",    "--",  "sd1_DAT2", "arm_TDI",    "**"      }, // 26
  { "--",         "--",    "--",  "sd1_DAT3", "arm_TMS",    "**"      }, // 27

  { "iic0_SDA",   "sm_A5", "pcm_CLK",  "--",     "**",      "**"      }, // 28
  { "iic0_SCL",   "sm_A4", "pcm_FS",   "--",     "**",      "**"      }, // 29
  { "--",         "sm_A3", "pcm_DIN",  "u0_CTS", "**",      "u1_CTS"  }, // 30
  { "--",         "sm_A2", "pcm_DOUT", "u0_RTS", "**",      "u1_RTS"  }, // 31
  { "gp_CLK0",    "sm_A1", "--",       "u0_TXD", "**",      "u1_TXD"  }, // 32
  { "--" ,        "sm_A0", "--",       "u0_RXD", "**",      "u1_RXD"  }, // 33
  { "gp_CLK0",    "sm_OE_n", "--",     "--",     "**",      "**"      }, // 34
  { "spi0_CE1_n", "sm_WE_n", "**",     "--",     "**",      "**"      }, // 35
  { "spi0_CE0_n", "sm_D0", "u0_TXD",   "--",     "**",      "**"      }, // 36
  { "spi0_MISO",  "sm_D1", "u0_RXD",   "--",     "**",      "**"      }, // 37
  { "spi0_MOSI",  "sm_D2", "u0_RTS",   "--",     "**",      "**"      }, // 38
  { "spi0_SCLK",  "sm_D3", "u0_CTS",   "--",     "**",      "**"      }, // 39
  { "PWM0",       "sm_D4", "**",       "--",  "spi2_MISO",  "u1_TXD"  }, // 40
  { "PWM1",       "sm_D5", "--",       "--",  "spi2_MOSI",  "u1_RXD"  }, // 41
  { "gp_CLK1",    "sm_D6", "--",       "--",  "spi2_SCLK",  "u1_RTS"  }, // 42
  { "gp_CLK2",    "sm_D7", "--",       "--",  "spi2_CE0_n", "u1_CTS"  }, // 43
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

