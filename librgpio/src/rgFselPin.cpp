// 2018-01-01  William A. Hudson

// GPIO Pin Function Select class.
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

#include "rgAddrMap.h"
#include "rgIoPin.h"
#include "rgFselPin.h"


/*
* Constructor.
* Pass in an initialized rgIoPin object, e.g.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_gpiomem();	// select and open device file
*    rgIoPin	gpx  ( &amx );	// construct with address map
* call:
*    rgFselPin	fx   ( &gpx );	// construct with io pin object
*/
rgFselPin::rgFselPin(
    rgIoPin		*xx
)
{
    IoPinX   = xx;

    FselReg[0] = 0;
    FselReg[1] = 0;
    FselReg[2] = 0;
    FselReg[3] = 0;
    FselReg[4] = 0;
    FselReg[5] = 0;
}


//--------------------------------------------------------------------------
// Direct Register access
//--------------------------------------------------------------------------

/*
* Read Function Select pin.
*    No copy in the object.
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgFselPin::rgFsel_enum
rgFselPin::read_Fsel_bit(
    int			bit
)
{
    rgIoPin::rgIoReg_enum	reg;
    int				pos;
    uint32_t			rv;
    uint32_t			mode;

    reg = fselreg_bit( bit, &pos );

    rv = IoPinX->read_reg( reg );

    mode = (rv >> pos) & 0x7;

    return  rgFselPin::rgFsel_enum( mode );
}


/*
* Modify Function Select pin.
*    Does read/modify/write.
*    No copy in the object.
* exceptions:
*    Throw range_error for bit out-of-range.
*/
void
rgFselPin::modify_Fsel_bit(
    int				bit,
    rgFselPin::rgFsel_enum	mode
)
{
    rgIoPin::rgIoReg_enum	reg;
    int				pos;
    uint32_t			mask;
    uint32_t			value;

    reg = fselreg_bit( bit, &pos );

    mask  = (                 0x7) << pos;
    value = ((uint32_t)mode & 0x7) << pos;

    IoPinX->modify_reg( reg, mask, value );
}


//--------------------------------------------------------------------------
// Register field position
//--------------------------------------------------------------------------
// Convert bit number into register enum and field position in register.
// Class methods.

/*
* Get Fsel register and field position for bit number.
* call:
*    reg = fselreg_bit( bit, &pos )
*    bit  = pin bit number 0..53
*    pos  = field position within register, {0,3,6,..,27}
* return:
*    ()  = rgIoReg_enum  of the Fsel register for that bit
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgIoPin::rgIoReg_enum
rgFselPin::fselreg_bit(
    int			bit,		// pin bit number
    int			*pos
)
{
    if ( (bit < 0) || (bit > 53) ) {
	std::ostringstream	css;
	css << "rgFselPin::fselreg_bit():  out-of-range bit= " << bit;
	throw std::range_error ( css.str() );
    }

    int  rnum = (int)bit / (int)10;		// register number 0..5

    int  kk = bit - ( rnum * (int)10 );		// sub-bit in register

    *pos = kk * 3;			// location of 3-bit field

//  cout << "bit=" << bit << "  rnum=" << rnum << "  pos=" << *pos <<endl;

    return  rgIoPin::rgIoReg_enum( rgIoPin::rgFsel0 + rnum );
	    // explicit enum conversion from int
}


//--------------------------------------------------------------------------
// Enum string conversion
//--------------------------------------------------------------------------

// Fsel mode string names, indexed by rgFsel_enum.  (Private)
//    Entry order must match enum value order, verify by testing.
const char*        rgFselPin::ModeStr[] = {
    "In",
    "Out",
    "Alt5",
    "Alt4",
    "Alt0",
    "Alt1",
    "Alt2",
    "Alt3"
};


/*
* Get string for an rgFsel_enum (Function Select mode).
*    Compiler checks type of enum.
* call:
*    rgFselPin::str_rgFsel_enum( rgFselPin::f_Alt3 )	class method
*             x.str_rgFsel_enum( rgFselPin::f_Alt3 )	object method
* return:
*    () = char string
* exception:
*    Throw range_error if enum is invalid.
*/
const char*
rgFselPin::str_rgFsel_enum(
    rgFsel_enum		mode
)
{
    if ( (mode < 0) || (mode > 7) ) {
	std::ostringstream	css;
	css << "rgFselPin::str_rgFsel_enum():  bad enum= " << mode;
	throw std::range_error ( css.str() );
    }

    const char		*name;
    name = rgFselPin::ModeStr[ mode ];

    return  name;
}


/*
* Find rgFsel_enum for the corresponding string name.
*    Currently is a linear search, good enough for the typical single lookup.
*    #!! Use std::map for multiple lookups.
* call:
*    rgFselPin::find_rgFsel_enum( "Alt3" )		class method
*             x.find_rgFsel_enum( "Alt3" )		object method
* return:
*    () = rgFsel_enum
* exception:
*    Throw range_error if enum string is not found.
*    Throw logic_error if string pointer is NULL.
*/
rgFselPin::rgFsel_enum
rgFselPin::find_rgFsel_enum(
    const char		*name
)
{
    if ( name == NULL ) {
	throw std::logic_error ( "rgFselPin::find_rgFsel_enum():  NULL string pointer" );
    }

    // Walk the string array not relying on a maximum enum.

    int			max = (sizeof( rgFselPin::ModeStr ) / sizeof( char * ));

    for ( int key=0;  key<max;  key++ )
    {
	const char*	reg = rgFselPin::ModeStr[key];

	if ( reg == NULL ) { continue; }

	if ( strcmp( reg, name ) == 0 ) {
	    return  (rgFselPin::rgFsel_enum) key;
	}
    }

    // Not found
    std::ostringstream	css;
    css << "rgFselPin::find_rgFsel_enum() no enum for string:  " << name;
    throw std::range_error ( css.str() );
}

