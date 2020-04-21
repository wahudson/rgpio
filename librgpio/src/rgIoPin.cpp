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
* Optional address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_gpiomem();	// select and open device file
* call:
*    rgIoPin	gpx;		// no GpioBase init, use seperate init
*    gpx.init_addr( &amx );
* call:
*    rgIoPin	gpx  ( &amx );	// constructor with address map
*/
rgIoPin::rgIoPin(
    rgAddrMap		*xx	// default NULL
)
{
    GpioBase   = NULL;
    if ( xx ) {
	this->init_addr( xx );
    }
}


//--------------------------------------------------------------------------
// GpioBase address
//--------------------------------------------------------------------------

/*
* Init GpioBase address.
*    The class knows its BCM documentation base address.
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
// These are intended for the normal read/write capable registers.
// The special register accessor functions (inline in the .h file) are
// preferred wherever possible.

//#!! Need to ensure GpioBase is initialized before use.

/*
* Get raw register address.
*    Good for all registers.
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
* Write raw register value.
*    No check for read-only or special set/clear registers.
*    Beware effects on special registers that set-bits or clear-bits.
*/
void
rgIoPin::write_reg(
    rgIoReg_enum	reg,
    uint32_t		value
)
{
    *(GpioBase + reg) = value;
}


/*
* Modify raw register.
*    Does read/modify/write only for normal read/write capable registers.
*    Not applicable for write-only or read-only registers.
*    Not applicable for registers that operate by set-bit or clear-bit.
* exception:
*    Throw 'logic_error' on non-normal read/write registers.
*/
void
rgIoPin::modify_reg(
    rgIoReg_enum	reg,
    uint32_t		mask,
    uint32_t		value
)
{
    uint32_t		x;

    if ( (reg == rgPinSet_w0) ||
	 (reg == rgPinSet_w1) ||
	 (reg == rgPinClr_w0) ||
	 (reg == rgPinClr_w1) ||
	 (reg == rgPinRead_w0)     ||
	 (reg == rgPinRead_w1)     ||
	 (reg == rgEventStatus_w0) ||
	 (reg == rgEventStatus_w1)
    ) {
	std::ostringstream	css;
	css << "inappropriate register in rgIoPin::modify_reg():  "
	    << this->str_IoReg_enum( reg );
	throw std::logic_error ( css.str() );
    }

    x = *(GpioBase + reg);
    x = (x & (~ mask) ) | (value & mask);
    *(GpioBase + reg) = x;
}


/*
* Set register bits according to mask.
*    Direct write to set-only registers.
*    Does read/modify/write on normal read/write capable registers.
* exception:
*    Throw 'logic_error' on non-setable registers.
*/
void
rgIoPin::set_reg(
    rgIoReg_enum	reg,
    uint32_t		mask
)
{
    if ( (reg == rgPinSet_w0)      ||
	 (reg == rgPinSet_w1)
    ) {
	write_reg( reg, mask );		// hardware does set
    }
    else if (
	 (reg == rgPinClr_w0)      ||
	 (reg == rgPinClr_w1)      ||
	 (reg == rgPinRead_w0)     ||
	 (reg == rgPinRead_w1)     ||
	 (reg == rgEventStatus_w0) ||
	 (reg == rgEventStatus_w1)
    ) {
	std::ostringstream	css;
	css << "inappropriate register in rgIoPin::set_reg():  "
	    << this->str_IoReg_enum( reg );
	throw std::logic_error ( css.str() );
    }
    else {
	modify_reg( reg, mask, mask );
    }
}


/*
* Clear register bits according to mask.
*    Direct write to clear-only registers.
*    Does read/modify/write on normal read/write capable registers.
* exception:
*    Throw 'logic_error' on non-clearable registers.
*/
void
rgIoPin::clr_reg(
    rgIoReg_enum	reg,
    uint32_t		mask
)
{
    if ( (reg == rgPinClr_w0)      ||
	 (reg == rgPinClr_w1)      ||
	 (reg == rgEventStatus_w0) ||
	 (reg == rgEventStatus_w1)
    ) {
	write_reg( reg, mask );		// hardware does clear
    }
    else if (
	 (reg == rgPinSet_w0)      ||
	 (reg == rgPinSet_w1)      ||
	 (reg == rgPinRead_w0)     ||
	 (reg == rgPinRead_w1)
    ) {
	std::ostringstream	css;
	css << "inappropriate register in rgIoPin::clr_reg():  "
	    << this->str_IoReg_enum( reg );
	throw std::logic_error ( css.str() );
    }
    else {
	modify_reg( reg, mask, 0 );
    }
}


//--------------------------------------------------------------------------
// Special Register Accessors (non-normal read/write)
//--------------------------------------------------------------------------
// All defined inline in .h file.


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
    "rgPinSet_w0",
    "rgPinSet_w1",
    NULL,
    "rgPinClr_w0",
    "rgPinClr_w1",
    NULL,
    "rgPinRead_w0",
    "rgPinRead_w1",
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
*    rgIoPin::str_IoReg_enum( rgIoPin::rgPinRead_w0 )	class method
*           x.str_IoReg_enum( rgIoPin::rgPinRead_w0 )	object method
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
*    rgIoPin::find_IoReg_enum( "rgPinRead_w0" )		class method
*           x.find_IoReg_enum( "rgPinRead_w0" )		object method
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

