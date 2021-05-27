// 2018-01-01  William A. Hudson

// rGPIO Pin Function Select class.
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
#include "rgIoPins.h"
#include "rgFselPin.h"


/*
* Constructor.
* Pass in an initialized rgIoPins object, e.g.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_gpiomem();	// select and open device file
*    rgIoPins	gpx  ( &amx );	// construct with address map
* call:
*    rgFselPin	fx   ( &gpx );	// construct with io pin object
*/
rgFselPin::rgFselPin(
    rgIoPins		*xx
)
{
    IoPinX   = xx;		//#!! not really needed

    FselReg[0] = xx->Fsel0;	// copy register objects
    FselReg[1] = xx->Fsel1;
    FselReg[2] = xx->Fsel2;
    FselReg[3] = xx->Fsel3;
    FselReg[4] = xx->Fsel4;
    FselReg[5] = xx->Fsel5;
}


//--------------------------------------------------------------------------
// Direct Register access
//--------------------------------------------------------------------------

/*
* Read Function Select mode of a pin number.
*    No copy in the object.
* call:
*    read_Fsel_bit( bit )
*        bit = IO pin bit number {0..53}
* return:
*    ()  = Fsel mode enum {In, .., Alt5}
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgFselPin::rgFsel_enum
rgFselPin::read_Fsel_bit(
    int			bit
)
{
    rgReg_rw			*rp;	// register pointer
    int				pos;
    uint32_t			rv;
    uint32_t			mode;

    rp = fselreg_bit( bit, &pos );	// get register and field position

    rv = rp->read();

    mode = (rv >> pos) & 0x7;

    return  rgFselPin::rgFsel_enum( mode );
	// explicit int to enum conversion, in range of enum
}


/*
* Modify Function Select mode of a pin number.
*    Does read/modify/write.
*    No copy in the object.
* call:
*    modify_Fsel_bit( bit, mode )
*        bit  = IO pin bit number {0..53}
*        mode = function select mode {f_In, f_Out, .. f_Alt5}
* exceptions:
*    Throw range_error for bit out-of-range.
*/
void
rgFselPin::modify_Fsel_bit(
    int				bit,
    rgFselPin::rgFsel_enum	mode
)
{
    rgReg_rw			*rp;	// register pointer
    int				pos;
    uint32_t			mask;
    uint32_t			value;

    rp = fselreg_bit( bit, &pos );	// get register and field position

    mask  = (                 0x7) << pos;
    value = ((uint32_t)mode & 0x7) << pos;

    rp->modify( mask, value );
}


//--------------------------------------------------------------------------
// Register field position
//--------------------------------------------------------------------------

/*
* Get Fsel register pointer and field position for a bit number.
*    The field position pos is the LSB of the 3-bit Fsel mode value.
* call:
*    rp = fselreg_bit( bit, &pos )
*        bit  = pin bit number 0..53
*        pos  = field position within register, {0,3,6,..,27}
* return:
*    ()  = pointer to the Fsel register for that bit
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgReg_rw*
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

    return  &( FselReg[rnum] );
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

