// 2017-06-10  William A. Hudson

// GPIO IO Pin control class.
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.89-101  6.  General Purpose I/O (GPIO)
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>
#include <string.h>	// strcmp()

using namespace std;

//#include "Error.h"
#include "rgAddrMap.h"
#include "rgIoPin.h"


/*
* Constructor.
*/
rgIoPin::rgIoPin()
{
    GpioBase   = NULL;

    FselReg[0] = 0;
    FselReg[1] = 0;
    FselReg[2] = 0;
    FselReg[3] = 0;
    FselReg[4] = 0;
    FselReg[5] = 0;
}


//--------------------------------------------------------------------------
// GpioBase address
//--------------------------------------------------------------------------

/*
* Init GpioBase address.
*/
void
rgIoPin::init_addr(
    rgAddrMap		*xx
)
{
    GpioBase = xx->get_mem_block( 0x7e200000 );
}

// inline volatile uint32_t*   get_base_addr()


//--------------------------------------------------------------------------
// Generic Register access
//--------------------------------------------------------------------------

//#!! Need to ensure GpioBase is initialized before use.

/*
* Get raw register address.
*/
volatile uint32_t*
rgIoPin::addr_reg(
    rgIoReg_enum	reg
)
{
    if ( NULL == GpioBase ) {
	std::ostringstream	css;
	css << "rgIoPin:: not initialized in:  addr_reg( 0x"
	    <<hex << (reg*4) << "/4 )";
	throw std::logic_error ( css.str() );
    }

    return  (GpioBase + reg);
}


/*
* Read raw register value.
*    No copy in the object.  #!!
*    No check for write-only registers.
*/
uint32_t
rgIoPin::read_reg(
    rgIoReg_enum	reg
)
{
    return  *(GpioBase + reg);
}


/*
* Modify register.
*    Does read/modify/write.
*    No copy in the object.  #!!
*    Not applicable for write-only registers.
*/
void
rgIoPin::mod_reg(
    rgIoReg_enum	reg,
    uint32_t		value,
    uint32_t		mask
)
{
    uint32_t		x;

    x = *(GpioBase + reg);
    x = (x & (~ mask) ) | (value & mask);
    *(GpioBase + reg) = x;
}

//--------------------------------------------------------------------------
// IO Register read/write
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Enum string conversion
//--------------------------------------------------------------------------

// Register string names, indexed by rgIoReg_enum.  (Private)
//    Entry order must match enum value order, verify by testing.
const char*        rgIoPin::RegStr[] = {
    "rgFsel0",
    "rgFsel1",
    "rgFsel2",
    "rgFsel3",
    "rgFsel4",
    "rgFsel5",
    NULL,
    "rgOutSet_w0",
    "rgOutSet_w1",
    NULL,
    "rgOutClr_w0",
    "rgOutClr_w1",
    NULL,
    "rgPinLevel_w0",
    "rgPinLevel_w1",
    NULL,
    "rgEventStatus_w0",
    "rgEventStatus_w1",
    NULL,
    "rgDetectRising_w0",
    "rgDetectRising_w1",
    NULL,
    "rgDetectFalling_w0",
    "rgDetectFalling_w1",
    NULL,
    "rgDetectHigh_w0",
    "rgDetectHigh_w1",
    NULL,
    "rgDetectLow_w0",
    "rgDetectLow_w1",
    NULL,
    "rgDetectAsyncRising_w0",
    "rgDetectAsyncRising_w1",
    NULL,
    "rgDetectAsyncFalling_w0",
    "rgDetectAsyncFalling_w1",
    NULL,
    "rgPullUpDown",
    "rgPullUpDownClk_w0",
    "rgPullUpDownClk_w1",
    NULL
};


/*
* Get string name of an rgIoReg_enum register enum.
*    Compiler checks type of enum.
* call:
*    rgIoPin::str_IoReg_enum( rgIoPin::rgPinLevel_w0 )	class method
*           x.str_IoReg_enum( rgIoPin::rgPinLevel_w0 )	object method
* return:
*    () = char string
* exception:
*    Throw range_error if enum is invalid.
*/
const char*
rgIoPin::str_IoReg_enum(
    rgIoReg_enum	reg
)
{
    const char		*name;
    name = rgIoPin::RegStr[ reg ];

    if ( name == NULL ) {
	std::ostringstream	css;
	css << "rgIoPin::  bad rgIoReg_enum in:  str_IoReg_enum()";
	throw std::range_error ( css.str() );
    }

    return  name;
}


/*
* Find rgIoReg_enum for the corresponding string name.
*    Currently is a linear search, good enough for the typical single lookup.
*    #!! Use std::map for multiple lookups.
* call:
*    rgIoPin::find_IoReg_enum( "rgPinLevel_w0" )	class method
*           x.find_IoReg_enum( "rgPinLevel_w0" )	object method
* return:
*    () = rgIoReg_enum
* exception:
*    Throw range_error if enum string is not found.
*    Throw logic_error if string pointer is NULL.
*/
rgIoPin::rgIoReg_enum
rgIoPin::find_IoReg_enum(
    const char		*name
)
{
    if ( name == NULL ) {
	std::ostringstream	css;
	css << "rgIoPin::  NULL string pointer in:  find_IoReg_enum()";
	throw std::logic_error ( css.str() );
    }

    // Walk the string array not relying on a maximum enum.

    int			max = (sizeof( rgIoPin::RegStr ) / sizeof( char * ));

    for ( int key=0;  key<max;  key++ )
    {
	const char*	reg = rgIoPin::RegStr[key];

	if ( reg == NULL ) { continue; }

	if ( strcmp( reg, name ) == 0 ) {
	    return  (rgIoPin::rgIoReg_enum) key;
	}
    }

    // Not found
    std::ostringstream	css;
    css << "rgIoPin::  str_IoReg_enum() no enum for string:  " << name;
    throw std::range_error ( css.str() );
}

