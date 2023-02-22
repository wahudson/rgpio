// 2018-01-05  William A. Hudson
//
// rGPIO Alternate Function Names - documentation
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.102-103  6.2  Alternative Function Assignments
// See:  BCM2711 ARM Peripherals (2020-10-16)
//      p.76-89    5.3  Alternative Function Assignments
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "rgRpiRev.h"
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
 // Alt0          Alt1       Alt2        Alt3     Alt4       Alt5          bit

 { "iic0_SDA",   "sm_A5",   "dpi_PCLK",  "**",    "**",      "**"       },// 0
 { "iic0_SCL",   "sm_A4",   "dpi_DE",    "**",    "**",      "**"       },// 1
 { "iic1_SDA",   "sm_A3",   "dpi_VSYNC", "**",    "**",      "**"       },// 2
 { "iic1_SCL",   "sm_A2",   "dpi_HSYNC", "**",    "**",      "**"       },// 3
 { "gp_CLK0",    "sm_A1",   "dpi_D0",    "**",    "**",      "arm_TDI"  },// 4
 { "gp_CLK1",    "sm_A0",   "dpi_D1",    "**",    "**",      "arm_TDO"  },// 5
 { "gp_CLK2",    "sm_OE_n", "dpi_D2",    "**",    "**",      "arm_RTCK" },// 6
 { "spi0_CE1_n", "sm_WE_n", "dpi_D3",    "**",    "**",      "**"       },// 7
 { "spi0_CE0_n", "sm_D0",   "dpi_D4",    "**",    "**",      "**"       },// 8
 { "spi0_MISO",  "sm_D1",   "dpi_D5",    "**",    "**",      "**"       },// 9
 { "spi0_MOSI",  "sm_D2",   "dpi_D6",    "**",    "**",      "**"       },// 10
 { "spi0_SCLK",  "sm_D3",   "dpi_D7",    "**",    "**",      "**"       },// 11
 { "pwm0_CH1",   "sm_D4",   "dpi_D8",    "**",    "**",      "arm_TMS"  },// 12
 { "pwm0_CH2",   "sm_D5",   "dpi_D9",    "**",    "**",      "arm_TCK"  },// 13
 { "u0_TXD",     "sm_D6",  "dpi_D10",    "**",    "**",         "u1_TXD"  },//14
 { "u0_RXD",     "sm_D7",  "dpi_D11",    "**",    "**",         "u1_RXD"  },//15
 { "--",         "sm_D8",  "dpi_D12", "u0_CTS",   "spi1_CE2_n", "u1_CTS"  },//16
 { "--",         "sm_D9",  "dpi_D13", "u0_RTS",   "spi1_CE1_n", "u1_RTS"  },//17
 { "pcm_CLK",    "sm_D10", "dpi_D14", "bsc_MOSI", "spi1_CE0_n", "pwm0_CH1"},//18
 { "pcm_FS",     "sm_D11", "dpi_D15", "bsc_SCLK", "spi1_MISO",  "pwm0_CH2"},//19
 { "pcm_DIN",    "sm_D12", "dpi_D16", "bsc_MISO", "spi1_MOSI",  "gp_CLK0" },//20
 { "pcm_DOUT",   "sm_D13", "dpi_D17", "bsc_CE_n", "spi1_SCLK",  "gp_CLK1" },//21
 { "sd0_CLK",    "sm_D14", "dpi_D18", "sd1_CLK",  "arm_TRST",   "**"    },// 22
 { "sd0_CMD",    "sm_D15", "dpi_D19", "sd1_CMD",  "arm_RTCK",   "**"    },// 23
 { "sd0_DAT0",   "sm_D16", "dpi_D20", "sd1_DAT0", "arm_TDO",    "**"    },// 24
 { "sd0_DAT1",   "sm_D17", "dpi_D21", "sd1_DAT1", "arm_TCK",    "**"    },// 25
 { "sd0_DAT2",   "--",     "dpi_D22", "sd1_DAT2", "arm_TDI",    "**"    },// 26
 { "sd0_DAT3",   "--",     "dpi_D23", "sd1_DAT3", "arm_TMS",    "**"    },// 27

 { "iic0_SDA",   "sm_A5",  "pcm_CLK",  "--",       "**",       "**"     },// 28
 { "iic0_SCL",   "sm_A4",  "pcm_FS",   "--",       "**",       "**"     },// 29
 { "--",         "sm_A3",  "pcm_DIN",  "u0_CTS",   "**",       "u1_CTS" },// 30
 { "--",         "sm_A2",  "pcm_DOUT", "u0_RTS",   "**",       "u1_RTS" },// 31
 { "gp_CLK0",    "sm_A1",    "--",     "u0_TXD",   "**",       "u1_TXD" },// 32
 { "--" ,        "sm_A0",    "--",     "u0_RXD",   "**",       "u1_RXD" },// 33
 { "gp_CLK0",    "sm_OE_n",  "--",   "sd1_CLK",    "**",       "**"     },// 34
 { "spi0_CE1_n", "sm_WE_n",  "**",   "sd1_CMD",    "**",       "**"     },// 35
 { "spi0_CE0_n", "sm_D0",  "u0_TXD", "sd1_DAT0",   "**",       "**"     },// 36
 { "spi0_MISO",  "sm_D1",  "u0_RXD", "sd1_DAT1",   "**",       "**"     },// 37
 { "spi0_MOSI",  "sm_D2",  "u0_RTS", "sd1_DAT2",   "**",       "**"     },// 38
 { "spi0_SCLK",  "sm_D3",  "u0_CTS", "sd1_DAT3",   "**",       "**"     },// 39
 { "pwm0_CH1",   "sm_D4",  "**",     "sd1_DAT4", "spi2_MISO",  "u1_TXD" },// 40
 { "pwm0_CH2",   "sm_D5",  "--",     "sd1_DAT5", "spi2_MOSI",  "u1_RXD" },// 41
 { "gp_CLK1",    "sm_D6",  "--",     "sd1_DAT6", "spi2_SCLK",  "u1_RTS" },// 42
 { "gp_CLK2",    "sm_D7",  "--",     "sd1_DAT7", "spi2_CE0_n", "u1_CTS" },// 43
 { "gp_CLK1",  "iic0_SDA", "iic1_SDA",  "--",    "spi2_CE1_n", "**"     },// 44
 { "pwm0_CH2", "iic0_SCL", "iic1_SCL",  "--",    "spi2_CE2_n", "**"     },// 45

 { "--",         "**",     "**",        "**",      "**",       "**"     },// 46
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 47
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 48
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 49
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 50
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 51
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 52
 { "--",         "**",     "**",        "**",      "**",       "**"     },// 53
};


/*
* Delta alternate function name table - RPi4.
*    A null value indicates name string is the same as for RPi3 above.
*
* AltDelta4[altN][bit]
*    altN  = Alternate function number 0..5 (for Alt0..Alt5).
*    bit   = bit number 0..53
* Names:
*    0     = (NULL) same as AltFunc[][] for RPi3 and earlier
*    "--"  = reserved
*    "**"  = do NOT USE, may have unexpected results
* Notes:
*    Did they really replace spi2_* pins with spi0_* ?
*        The footnote says spi2 is not user accessable.  (bummer)
*    Slave I2C bsc_* moved.
*    Undocumented functions miia_* and rgmii_*.
*    Undocumented sdc_* seem to be for an SD Card.
*    Pulse Width Modulation channel cross-numbering is corrected here, since
*        a new pwm1_* unit is added.  CH1 = PWM0, CH2 = PWM1
*/
const char*		rgAltFuncName::AltDelta4[][6] = {
 // Alt0       Alt1  Alt2  Alt3         Alt4           Alt5               bit

  { 0,            0,  0,  "spi3_CE0_n", "u2_TXD",      "iic6_SDA"    }, // 0
  { 0,            0,  0,  "spi3_MISO",  "u2_RXD",      "iic6_SCL"    }, // 1
  { 0,            0,  0,  "spi3_MOSI",  "u2_CTS",      "iic3_SDA"    }, // 2
  { 0,            0,  0,  "spi3_SCLK",  "u2_RTS",      "iic3_SCL"    }, // 3
  { 0,            0,  0,  "spi4_CE0_n", "u3_TXD",      "iic3_SDA"    }, // 4
  { 0,            0,  0,  "spi4_MISO",  "u3_RXD",      "iic3_SCL"    }, // 5
  { 0,            0,  0,  "spi4_MOSI",  "u3_CTS",      "iic4_SDA"    }, // 6
  { 0,            0,  0,  "spi4_SCLK",  "u3_RTS",      "iic4_SCL"    }, // 7
  { 0,            0,  0,  "bsc_CE_n",   "u4_TXD",      "iic4_SDA"    }, // 8
  { 0,            0,  0,  "bsc_MISO",   "u4_RXD",      "iic4_SCL"    }, // 9
  { 0,            0,  0,  "bsc_MOSI",   "u4_CTS",      "iic5_SDA"    }, // 10
  { 0,            0,  0,  "bsc_SCLK",   "u4_RTS",      "iic5_SCL"    }, // 11
  { 0,            0,  0,  "spi5_CE0_n", "u5_TXD",      "iic5_SDA"    }, // 12
  { 0,            0,  0,  "spi5_MISO",  "u5_RXD",      "iic5_SCL"    }, // 13
  { 0,            0,  0,  "spi5_MOSI",  "u5_CTS",      0,            }, // 14
  { 0,            0,  0,  "spi5_SCLK",  "u5_RTS",      0,            }, // 15
  { 0,            0,  0,  0,            0,             0,            }, // 16
  { 0,            0,  0,  0,            0,             0,            }, // 17
  { 0,            0,  0,  "spi6_CE0_n", 0,             0,            }, // 18
  { 0,            0,  0,  "spi6_MISO",  0,             0,            }, // 19
  { 0,            0,  0,  "spi6_MOSI",  0,             0,            }, // 20
  { 0,            0,  0,  "spi6_SCLK",  0,             0,            }, // 21
  { 0,            0,  0,  0,            0,             "iic6_SDA"    }, // 22
  { 0,            0,  0,  0,            0,             "iic6_SCL"    }, // 23
  { 0,            0,  0,  0,            0,             "spi3_CE1_n"  }, // 24
  { 0,            0,  0,  0,            0,             "spi4_CE1_n"  }, // 25
  { 0,            0,  0,  0,            0,             "spi5_CE1_n"  }, // 26
  { 0,            0,  0,  0,            0,             "spi6_CE1_n"  }, // 27

  { 0,            0,  0,  0,            "miia_RX_ERR", "rgmii_MDIO"  }, // 28
  { 0,            0,  0,  0,            "miia_TX_ERR", "rgmii_MDC"   }, // 29
  { 0,            0,  0,  0,            "miia_CRS",    0,            }, // 30
  { 0,            0,  0,  0,            "miia_COL",    0,            }, // 31
  { 0,            0,  0,  0,            "sdc_PRES",    0,            }, // 32
  { 0,            0,  0,  0,            "sdc_WRPROT",  0,            }, // 33
  { 0,            0,  0,  0,            "sdc_LED",     "rgmii_IRQ"   }, // 34
  { 0,            0,  0,  0,            "rgmii_START", "**"          }, // 35
  { 0,            0,  0,  0,            "rgmii_RX_OK", "miia_RX_ERR" }, // 36
  { 0,            0,  0,  0,            "rgmii_MDIO",  "miia_TX_ERR" }, // 37
  { 0,            0,  0,  0,            "rgmii_MDC",   "miia_CRS"    }, // 38
  { 0,            0,  0,  0,            "rgmii_IRQ",   "miia_COL"    }, // 39
  { "pwm1_CH1",   0,  0,  0,            "spi0_MISO",   0,            }, // 40
  { "pwm1_CH2",   0,  0,  0,            "spi0_MOSI",   0,            }, // 41
  { 0,            0,  0,  0,            "spi0_SCLK",   0,            }, // 42
  { 0,            0,  0,  0,            "spi0_CE0_n",  0,            }, // 43
  { 0,            0,  0,  0,            "spi0_CE1_n",  "sdc_VOLT"    }, // 44
  { 0,            0,  0,  0,            "spi0_CE2_n",  "sdc_PWR0"    }, // 45

  { 0,            0,  0,  0,            0,             0             }, // 46
  { 0,            0,  0,  0,            0,             0             }, // 47
  { 0,            0,  0,  0,            0,             0             }, // 48
  { 0,            0,  0,  0,            0,             0             }, // 49
  { 0,            0,  0,  0,            0,             0             }, // 50
  { 0,            0,  0,  0,            0,             0             }, // 51
  { 0,            0,  0,  0,            0,             0             }, // 52
  { 0,            0,  0,  0,            0,             0             }, // 53
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
*    cstr_altfunc_bit( mode, bit )
*    mode = rgFsel_enum, alternate function Fsel mode
*    bit  = bit number 0..53
* return:
*    () = string identifying the alternate function.
*         Max string length is 12 char (currently 10 char).
*/
const char*
rgAltFuncName::cstr_altfunc_bit(
    rgFselPin::rgFsel_enum	mode,
    int				bit
)
{
    const char			*func;	// return string
    int				nalt;	// altN 0..5

    if ( (bit < 0) || (bit > 53) ) {
	std::ostringstream      css;
	css << "cstr_altfunc_bit():  bit out-of-range:" << bit;
	throw std::range_error ( css.str() );
    }

    nalt = altN_enum[mode];

    if      ( nalt == 6 ) {
	func = "input";
    }
    else if ( nalt == 7 ) {
	func = "output";
    }
    else if ( rgRpiRev::find_SocEnum() >= rgRpiRev::soc_BCM2711 ) {  // RPi4
	func = AltDelta4[bit][nalt];
	if ( ! func ) {
	    func = AltFunc[bit][nalt];
	}
    }
    else {
	func = AltFunc[bit][nalt];
    }

    return  func;
}

