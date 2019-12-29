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
// rgClk_enum Register Addresses
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
    uint32_t		offset = 0x70/4;	// safe for bad enum

    switch ( clknum ) {
    case  cm_Clk0:
	offset = 0x70/4;
	break;
    case  cm_Clk1:
	offset = 0x78/4;
	break;
    case  cm_Clk2:
	offset = 0x80/4;
	break;
    case  cm_ClkPcm:
	offset = 0x98/4;
	break;
    case  cm_ClkPwm:
	offset = 0xa0/4;
	break;
    }
    // No "default:" so compiler checks all enums are represented.

    return  offset;
}


//--------------------------------------------------------------------------
// rgClk_enum  Conversions
//--------------------------------------------------------------------------

/*
* Return string name for a clock enum.
* call:
*    cstr_clk_enum( cm_Clk0 );
* return:
*    ()  = enum name string, without prefix "cm_"
* exception:
*    Throw range_error if enum is invalid.
*    Throw logic_error if enum entry is missing.
*/
const char*
rgClk::cstr_clk_enum(
    rgClk_enum		clknum
)
{
    const char*		strp;	// return pointer

    static const char*	cstr[rgClk::cm_MaxEnum+1] = {	// index by rgClk_enum
	"Clk0",
	"Clk1",
	"Clk2",
	"ClkPcm",
	"ClkPwm"
    };
    // Note:  This must track rgClk_enum definition.

    if ( (unsigned) clknum > (unsigned) cm_MaxEnum ) {
	std::ostringstream      css;
	css << "rgClk::cstr_clk_enum() invalid rgClk_enum:  " << clknum;
	throw std::range_error ( css.str() );
    }

    strp = cstr[ clknum ];

    // Missing array initializers are zero.
    if ( ! strp ) {
	std::ostringstream      css;
	css << "rgClk::cstr_clk_enum() missing rgClk_enum:  " << clknum;
	throw std::logic_error ( css.str() );
    }

    return  strp;
}


/*
* Return rgClk_enum for an int.
*    The int values does not change, but the type does.
* call:
*    int2clk_enum( 2 );
* return:
*    ()  = rgClk::cm_Clk2	enum value
* exception:
*    Throw range_error if int is invalid.
*/
rgClk::rgClk_enum
rgClk::int2clk_enum(
    int			num
)
{
    if (!( (cm_Clk0 <= num) && (num <= cm_MaxEnum) )) {
	std::ostringstream      css;
	css << "rgClk::int2clk_enum() int out of range:  " << num;
	throw std::range_error ( css.str() );
    }

    return  (rgClk_enum) num;
}

