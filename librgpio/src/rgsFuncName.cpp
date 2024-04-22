// 2024-04-10  William A. Hudson
//
// rgsFuncName  Alternate Function Names for RPi5
//
// See:  Raspberry Pi RP1 Peripherals (2023-11-07) Draft
//      p.14-14    3.1.1 Function Select
// See:  BCM2711 ARM Peripherals (2020-10-16)
//      p.76-89    5.3  Alternative Function Assignments
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

#include "rgsFuncName.h"

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

rgsFuncName::rgsFuncName()
{
}


//--------------------------------------------------------------------------
// Class Data
//--------------------------------------------------------------------------

/*
* Alternate function name table - RPi5
*    These names are adapted from the Raspberry Pi RP1 documentation,
*    modified to parallel prior rgpio naming.
* FuncName[bit][altnum]
*    altnum = Alternate function number 0..8, function select integer
*    bit    = bit number 0..27
* Names:
*    --    = reserved
*/
const char*		rgsFuncName::FuncName[][9] = {
 // a0          a1           a2          a3           a4            a5          a6          a7         a8              bit

 { "spi0_SIO3", "dpi_PCLK",  "u1_TX",    "iic0_SDA",  "--",         "sRIO[0]",  "pRIO[0]",  "PIO[0]",  "spi2_CSn0" },// 0
 { "spi0_SIO2", "dpi_DE",    "u1_RX",    "iic0_SCL",  "--",         "sRIO[1]",  "pRIO[1]",  "PIO[1]",  "spi2_SIO1" },// 1
 { "spi0_CSn3", "dpi_VSYNC", "u1_CTS",   "iic1_SDA",  "u0_IR_RX",   "sRIO[2]",  "pRIO[2]",  "PIO[2]",  "spi2_SIO0" },// 2
 { "spi0_CSn2", "dpi_HSYNC", "u1_RTS",   "iic1_SCL",  "u0_IR_TX",   "sRIO[3]",  "pRIO[3]",  "PIO[3]",  "spi2_SCLK" },// 3
 { "gp_CLK0",   "dpi_D0",    "u2_TX",    "iic2_SDA",  "u0_RI",      "sRIO[4]",  "pRIO[4]",  "PIO[4]",  "spi3_CSn0" },// 4
 { "gp_CLK1",   "dpi_D1",    "u2_RX",    "iic2_SCL",  "u0_DTR",     "sRIO[5]",  "pRIO[5]",  "PIO[5]",  "spi3_SIO1" },// 5
 { "gp_CLK2",   "dpi_D2",    "u2_CTS",   "iic3_SDA",  "u0_DCD",     "sRIO[6]",  "pRIO[6]",  "PIO[6]",  "spi3_SIO0" },// 6
 { "spi0_CSn1", "dpi_D3",    "u2_RTS",   "iic3_SCL",  "u0_DSR",     "sRIO[7]",  "pRIO[7]",  "PIO[7]",  "spi3_SCLK" },// 7
 { "spi0_CSn0", "dpi_D4",    "u3_TX",    "iic0_SDA",  "--",         "sRIO[8]",  "pRIO[8]",  "PIO[8]",  "spi4_CSn0" },// 8
 { "spi0_SIO1", "dpi_D5",    "u3_RX",    "iic0_SCL",  "--",         "sRIO[9]",  "pRIO[9]",  "PIO[9]",  "spi4_SIO0" },// 9
 { "spi0_SIO0", "dpi_D6",    "u3_CTS",   "iic1_SDA",  "--",         "sRIO[10]", "pRIO[10]", "PIO[10]", "spi4_SIO1" },//10
 { "spi0_SCLK", "dpi_D7",    "u3_RTS",   "iic1_SCL",  "--",         "sRIO[11]", "pRIO[11]", "PIO[11]", "spi4_SCLK" },//11
 { "pwm0_CH0",  "dpi_D8",    "u4_TX",    "iic2_SDA",  "audio_OUTL", "sRIO[12]", "pRIO[12]", "PIO[12]", "spi5_CSn0" },//12
 { "pwm0_CH1",  "dpi_D9",    "u4_RX",    "iic2_SCL",  "audio_OUTR", "sRIO[13]", "pRIO[13]", "PIO[13]", "spi5_SIO1" },//13
 { "pwm0_CH2",  "dpi_D10",   "u4_CTS",   "iic3_SDA",  "u0_TX",      "sRIO[14]", "pRIO[14]", "PIO[14]", "spi5_SIO0" },//14
 { "pwm0_CH3",  "dpi_D11",   "u4_RTS",   "iic3_SCL",  "u0_RX",      "sRIO[15]", "pRIO[15]", "PIO[15]", "spi5_SCLK" },//15
 { "spi1_CSn2", "dpi_D12", "mipi0_DSI_TE", "--",      "u0_CTS",     "sRIO[16]", "pRIO[16]", "PIO[16]", "--"        },//16
 { "spi1_CSn1", "dpi_D13", "mipi1_DSI_TE", "--",      "u0_RTS",     "sRIO[17]", "pRIO[17]", "PIO[17]", "--"        },//17
 { "spi1_CSn0", "dpi_D14", "iis0_SCLK",  "pwm0_CH2",  "iis1_SCLK",  "sRIO[18]", "pRIO[18]", "PIO[18]", "gp_CLK1"   },//18
 { "spi1_SIO1", "dpi_D15", "iis0_WS",    "pwm0_CH3",  "iis1_WS",    "sRIO[19]", "pRIO[19]", "PIO[19]", "--"        },//19
 { "spi1_SIO0", "dpi_D16", "iis0_SDI0",  "gp_CLK0",   "iis1_SDI0",  "sRIO[20]", "pRIO[20]", "PIO[20]", "--"        },//20
 { "spi1_SCLK", "dpi_D17", "iis0_SDO0",  "gp_CLK1",   "iis1_SDO0",  "sRIO[21]", "pRIO[21]", "PIO[21]", "--"        },//21
 { "sd0_CLK",   "dpi_D18", "iis0_SDI1",  "iic3_SDA",  "iis1_SDI1",  "sRIO[22]", "pRIO[22]", "PIO[22]", "--"        },//22
 { "sd0_CMD",   "dpi_D19", "iis0_SDO1",  "iic3_SCL",  "iis1_SDO1",  "sRIO[23]", "pRIO[23]", "PIO[23]", "--"        },//23
 { "sd0_DAT0",  "dpi_D20", "iis0_SDI2",  "--",        "iis1_SDI2",  "sRIO[24]", "pRIO[24]", "PIO[24]", "spi2_CSn1" },//24
 { "sd0_DAT1",  "dpi_D21", "iis0_SDO2",  "audio_CLK", "iis1_SDO2",  "sRIO[25]", "pRIO[25]", "PIO[25]", "spi3_CSn1" },//25
 { "sd0_DAT2",  "dpi_D22", "iis0_SDI3",  "audio_IN0", "iis1_SDI3",  "sRIO[26]", "pRIO[26]", "PIO[26]", "spi5_CSn1" },//26
 { "sd0_DAT3",  "dpi_D23", "iis0_SDO3",  "audio_IN1", "iis1_SDO3",  "sRIO[27]", "pRIO[27]", "PIO[27]", "spi1_CSn1" } //27
};


/*
* Lookup Alternate Function name string - RPi5.
*    Intended for human use.  Names may change.
*    Bank0 only.  Future extension with a Bank number.
* call:  (class or object)
*    cstr_altfuncAN( altnum, bit )
*    altnum = alternate function number 0..8
*    bit    = bit number 0..27
* return:
*    () = string identifying the alternate function.
*         Max string length is 12 char.
* exceptions:
*    std::range_error
*/
const char*
rgsFuncName::cstr_altfuncAN(
    int			altnum,
    int			bit
)
{
    if ( (altnum < 0) || (altnum > 8) ) {
	std::ostringstream      css;
	css << "cstr_altfuncAN():  altnum out-of-range:  " << altnum;
	throw std::range_error ( css.str() );
    }

    if ( (bit < 0) || (bit > 27) ) {
	std::ostringstream      css;
	css << "cstr_altfuncAN():  bit out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    return  FuncName[bit][altnum];
}

