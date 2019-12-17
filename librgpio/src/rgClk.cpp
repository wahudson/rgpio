// 2019-12-10  William A. Hudson

// rGPIO  rgClk - Clock Manager enumerated class
//
// Derived from rgClkMan and is the primary user interface.
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.105-108  6.3 General Purpose GPIO Clocks
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgClk.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgClk	px  ( enum, &amx );	// constructor with address map
*    enum  = rgClk_enum enumerator naming the clock manager
*    &amx  = pointer to address map object with open device file
* exceptions:
*    Any from rgAddrMap.
*/
rgClk::rgClk(
    rgClk_enum		clknum,
    rgAddrMap		*xx
)
{
    uint32_t		offset;

    ClkEnum   = clknum;
    GpioBase  = xx->get_mem_block( FeatureAddr );

    offset = get_Cntl_offset( clknum );

    Cntl.init_addr( GpioBase + offset     );
    Divr.init_addr( GpioBase + offset + 1 );
}


//--------------------------------------------------------------------------
// Register Addresses
//--------------------------------------------------------------------------

/*
* Get Cntl register word offset of given clock manager enum.
*    Stat register is 1 word after in each clock manager.
* call:
*    get_Cntl_offset( cm_Clk0 );
*/
const uint32_t
rgClk::get_Cntl_offset(
    rgClk_enum		clknum
)
{
    static const uint32_t	offset[5] = {	// index by rgClk_enum
	0x70/4,
	0x78/4,
	0x80/4,
	0x98/4,
	0xa0/4
    };

    return  offset[ clknum ];
}


//--------------------------------------------------------------------------
// rgClk_enum  Conversions
//--------------------------------------------------------------------------

/*
* Return const string name for a clock enum.
* call:
*    cstr_clk_enum( cm_Clk0 );
*/
const char*
rgClk::cstr_clk_enum(
    rgClk_enum		clknum
)
{
    static const char*	cstr[5] = {	// index by rgClk_enum
	"Clk0",
	"Clk1",
	"Clk2",
	"ClkPwm",
	"ClkPcm",
    };

    return  cstr[ clknum ];
}

