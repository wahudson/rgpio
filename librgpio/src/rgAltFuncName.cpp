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
*     Named set from BCM Doc "BCM2835 ARM Peripherals (06 February 2012)" and
*     CM3 Compute Module Datasheet.
*     The FL0 .. and TE0 .. names are not included.
*
* AltFunc[altN][bit]
*     altN  = Alternate function number 0..5 (for Alt0..Alt5).
*     bit   = bit number 0..53
* Names:
*     --    = reserved
*     **    = do NOT USE, may have unexpected results
*/
const char*		rgAltFuncName::AltFunc[][6] = {
 // Alt0          Alt1       Alt2         Alt3     Alt4     Alt5           bit

  { "iic0_SDA",   "sm_A5",   "dpi_PCLK",  "**",    "**",    "**"       }, // 0
  { "iic0_SCL",   "sm_A4",   "dpi_DE",    "**",    "**",    "**"       }, // 1
  { "iic1_SDA",   "sm_A3",   "dpi_VSYNC", "**",    "**",    "**"       }, // 2
  { "iic1_SCL",   "sm_A2",   "dpi_HSYNC", "**",    "**",    "**"       }, // 3
  { "gp_CLK0",    "sm_A1",   "dpi_D0",    "**",    "**",    "arm_TDI"  }, // 4
  { "gp_CLK1",    "sm_A0",   "dpi_D1",    "**",    "**",    "arm_TDO"  }, // 5
  { "gp_CLK2",    "sm_OE_n", "dpi_D2",    "**",    "**",    "arm_RTCK" }, // 6
  { "spi0_CE1_n", "sm_WE_n", "dpi_D3",    "**",    "**",    "**"       }, // 7
  { "spi0_CE0_n", "sm_D0",  "dpi_D4",     "**",    "**",    "**"       }, // 8
  { "spi0_MISO",  "sm_D1",  "dpi_D5",     "**",    "**",    "**"       }, // 9
  { "spi0_MOSI",  "sm_D2",  "dpi_D6",     "**",    "**",    "**"       }, // 10
  { "spi0_SCLK",  "sm_D3",  "dpi_D7",     "**",    "**",    "**"       }, // 11
  { "PWM0",       "sm_D4",  "dpi_D8",     "**",    "**",    "arm_TMS"  }, // 12
  { "PWM1",       "sm_D5",  "dpi_D9",     "**",    "**",    "arm_TCK"  }, // 13
  { "u0_TXD",     "sm_D6",  "dpi_D10",    "**",    "**",         "u1_TXD"  }, // 14
  { "u0_RXD",     "sm_D7",  "dpi_D11",    "**",    "**",         "u1_RXD"  }, // 15
  { "--",         "sm_D8",  "dpi_D12", "u0_CTS",   "spi1_CE2_n", "u1_CTS"  }, // 16
  { "--",         "sm_D9",  "dpi_D13", "u0_RTS",   "spi1_CE1_n", "u1_RTS"  }, // 17
  { "pcm_CLK",    "sm_D10", "dpi_D14", "bsc_MOSI", "spi1_CE0_n", "PWM0"    }, // 18
  { "pcm_FS",     "sm_D11", "dpi_D15", "bsc_SCLK", "spi1_MISO",  "PWM1"    }, // 19
  { "pcm_DIN",    "sm_D12", "dpi_D16", "bsc_MISO", "spi1_MOSI",  "gp_CLK0" }, // 20
  { "pcm_DOUT",   "sm_D13", "dpi_D17", "bsc_CE_n", "spi1_SCLK",  "gp_CLK1" }, // 21
  { "sd0_CLK",    "sm_D14", "dpi_D18", "sd1_CLK",  "arm_TRST",   "**"  }, // 22
  { "sd0_CMD",    "sm_D15", "dpi_D19", "sd1_CMD",  "arm_RTCK",   "**"  }, // 23
  { "sd0_DAT0",   "sm_D16", "dpi_D20", "sd1_DAT0", "arm_TDO",    "**"  }, // 24
  { "sd0_DAT1",   "sm_D17", "dpi_D21", "sd1_DAT1", "arm_TCK",    "**"  }, // 25
  { "sd0_DAT2",   "--",     "dpi_D22", "sd1_DAT2", "arm_TDI",    "**"  }, // 26
  { "sd0_DAT3",   "--",     "dpi_D23", "sd1_DAT3", "arm_TMS",    "**"  }, // 27

  { "iic0_SDA",   "sm_A5", "pcm_CLK",  "--",     "**",      "**"      }, // 28
  { "iic0_SCL",   "sm_A4", "pcm_FS",   "--",     "**",      "**"      }, // 29
  { "--",         "sm_A3", "pcm_DIN",  "u0_CTS", "**",      "u1_CTS"  }, // 30
  { "--",         "sm_A2", "pcm_DOUT", "u0_RTS", "**",      "u1_RTS"  }, // 31
  { "gp_CLK0",    "sm_A1", "--",       "u0_TXD", "**",      "u1_TXD"  }, // 32
  { "--" ,        "sm_A0", "--",       "u0_RXD", "**",      "u1_RXD"  }, // 33
  { "gp_CLK0",    "sm_OE_n", "--",   "sd1_CLK",  "**",      "**"      }, // 34
  { "spi0_CE1_n", "sm_WE_n", "**",   "sd1_CMD",  "**",      "**"      }, // 35
  { "spi0_CE0_n", "sm_D0", "u0_TXD", "sd1_DAT0", "**",      "**"      }, // 36
  { "spi0_MISO",  "sm_D1", "u0_RXD", "sd1_DAT1", "**",      "**"      }, // 37
  { "spi0_MOSI",  "sm_D2", "u0_RTS", "sd1_DAT2", "**",      "**"      }, // 38
  { "spi0_SCLK",  "sm_D3", "u0_CTS", "sd1_DAT3", "**",      "**"      }, // 39
  { "PWM0",       "sm_D4", "**",     "sd1_DAT4", "spi2_MISO",  "u1_TXD" },// 40
  { "PWM1",       "sm_D5", "--",     "sd1_DAT5", "spi2_MOSI",  "u1_RXD" },// 41
  { "gp_CLK1",    "sm_D6", "--",     "sd1_DAT6", "spi2_SCLK",  "u1_RTS" },// 42
  { "gp_CLK2",    "sm_D7", "--",     "sd1_DAT7", "spi2_CE0_n", "u1_CTS" },// 43
  { "gp_CLK1", "iic0_SDA", "iic1_SDA", "--",     "spi2_CE1_n", "**"     },// 44
  { "PWM1",    "iic0_SCL", "iic1_SCL", "--",     "spi2_CE2_n", "**"     },// 45

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

