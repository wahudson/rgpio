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
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_gpiomem();	// select and open device file
* call:
*    rgFselPin	fx   ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
* Note:
*    FeatureAddr and register offsets could come from rgIoPins class, but
*    this is more stand-alone.
*/
rgFselPin::rgFselPin(
    rgAddrMap		*xx
)
{
    GpioBase     = xx->get_mem_block( FeatureAddr );

    Fsel0.init_addr( GpioBase + (0x00 /4) );
    Fsel1.init_addr( GpioBase + (0x04 /4) );
    Fsel2.init_addr( GpioBase + (0x08 /4) );
    Fsel3.init_addr( GpioBase + (0x0c /4) );
    Fsel4.init_addr( GpioBase + (0x10 /4) );
    Fsel5.init_addr( GpioBase + (0x14 /4) );

    FselReg[0].init_addr( GpioBase + (0x00 /4) );
    FselReg[1].init_addr( GpioBase + (0x04 /4) );
    FselReg[2].init_addr( GpioBase + (0x08 /4) );
    FselReg[3].init_addr( GpioBase + (0x0c /4) );
    FselReg[4].init_addr( GpioBase + (0x10 /4) );
    FselReg[5].init_addr( GpioBase + (0x14 /4) );

}


/*
* Constructor.  (Deprecated)
*    This was a cool idea, but does not parallel other Features.
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
    GpioBase     = xx->get_base_addr();

    Fsel0.init_addr( xx->Fsel0.addr() );
    Fsel1.init_addr( xx->Fsel1.addr() );
    Fsel2.init_addr( xx->Fsel2.addr() );
    Fsel3.init_addr( xx->Fsel3.addr() );
    Fsel4.init_addr( xx->Fsel4.addr() );
    Fsel5.init_addr( xx->Fsel5.addr() );

    FselReg[0] = xx->Fsel0;	// copy register objects
    FselReg[1] = xx->Fsel1;
    FselReg[2] = xx->Fsel2;
    FselReg[3] = xx->Fsel3;
    FselReg[4] = xx->Fsel4;
    FselReg[5] = xx->Fsel5;
}


//--------------------------------------------------------------------------
// rgFselPin_reg  class functions.
//--------------------------------------------------------------------------

/*
* Modify function select mode at field number.
*    Does read/modify/write - not atomic.
* call:
*    modify_field( field, mode )
*        field  = logical field number {0, 1, .. 9}, right-to-left
*        mode   = function select enum {f_In, f_Out, f_Alt0, .. f_Alt5}
* exceptions:
*    range_error	field out-of-range
*/
void
rgFselPin::rgFselPin_reg::modify_field(
    unsigned		field,
    rgFsel_enum		mode
)
{
    unsigned		pos;		// field position
    uint32_t		mask;
    uint32_t		value;

    if ( field > 9 ) {
	std::ostringstream	css;
	css << "rgFselPin_reg::modify_field():  field out-of-range:  " << field;
	throw std::range_error ( css.str() );
    }

    pos = field * 3;			// location of 3-bit field

    mask  =         0x7  << pos;
    value = (mode & 0x7) << pos;

    modify( mask, value );
}


/*
* Modify function select mode by mask.
*    Does read/modify/write - not atomic.
* call:
*    modify_mask( mask, mode )
*        mask  = packed bit mask, only bits [9:0] are used
*        mode  = function select enum {f_In, f_Out, f_Alt0, .. f_Alt5}
* exceptions:
*    none
*/
void
rgFselPin::rgFselPin_reg::modify_mask(
    uint32_t		mask,
    rgFsel_enum		mode
)
{
    uint32_t		mv;
    uint32_t		fv;
    uint32_t		rmask = 0;	// expanded register mask
    uint32_t		value = 0;	// expanded register value

    fv = mode & 0x7;					// 3-bit field value

    for ( int ii=9;  ii>=0;  ii-- )	// scan mask bits left to right
    {
	mv    = ( mask & 0x00000200 ) ? 0x7 : 0x0;	// 3-bit mask value
	rmask = (rmask << 3) | mv;
	value = (value << 3) | fv;

	mask  = mask << 1;
    }

    modify( rmask, value );
}


/*
* Read function select mode at field number.
* call:
*    read_field( field )
*        field  = logical field number {0, 1, .. 9}, right-to-left
* return:
*    ()  = function select enum {f_In, f_Out, f_Alt0, .. f_Alt5}
* exceptions:
*    range_error	field out-of-range
*/
rgFselPin::rgFsel_enum
rgFselPin::rgFselPin_reg::read_field(
    unsigned		field
)
{
    unsigned		pos;		// field position
    uint32_t		value;

    if ( field > 9 ) {
	std::ostringstream	css;
	css << "rgFselPin_reg::read_field():  field out-of-range:  " << field;
	throw std::range_error ( css.str() );
    }

    pos = field * 3;			// location of 3-bit field

    value = (read() >> pos) & 0x7;

    return  rgFsel_enum( value );	// explicit enum conversion
}


/*
* Return mask of bits corresponding to a given function select mode.
* call:
*    read_mask( mode )
*        mode  = function select enum {f_In, f_Out, f_Alt0, .. f_Alt5}
* return:
*        ()  = packed bit mask, [9:0]
* exceptions:
*    none
*/
uint32_t
rgFselPin::rgFselPin_reg::read_mask(
    rgFsel_enum		mode
)
{
    uint32_t		fv = mode;	// 3-bit field value
    uint32_t		value;		// register value
    uint32_t		mask = 0;	// return mask

    value = read();

    for ( int ii=0;  ii<=9;  ii++ )	// scan fields right to left
    {
	mask = mask >> 1;		// next mask bit

	if ( (value & 0x7) == fv ) {
	    mask |= 0x00000200;		// insert at bit position 9
	}

	value = value >> 3;		// next field
    }

    return  mask;
}

//--------------------------------------------------------------------------
// Top level Read/Modify Fsel pins
//--------------------------------------------------------------------------

/*
* Read Function Select mode of a pin number.
*    No copy in the object.
* call:
*    read_Fsel_bit( bit )
*        bit = IO pin bit number {0..57}
* return:
*    ()  = function select enum {f_In, f_Out, f_Alt0, .. f_Alt5}
* exceptions:
*    range_error	bit number out-of-range
*/
rgFselPin::rgFsel_enum
rgFselPin::read_Fsel_bit(
    unsigned		bit
)
{
    unsigned		ebit = bit;
    rgFsel_enum		mode;

    if       ( bit        <= 9 ) {	// Gpio[9:0]
	mode = Fsel0.read_field( bit );
    }
    else if ( (bit -= 10) <= 9 ) {	// Gpio[19:10]
	mode = Fsel1.read_field( bit );
    }
    else if ( (bit -= 10) <= 9 ) {
	mode = Fsel2.read_field( bit );
    }
    else if ( (bit -= 10) <= 9 ) {
	mode = Fsel3.read_field( bit );
    }
    else if ( (bit -= 10) <= 9 ) {
	mode = Fsel4.read_field( bit );
    }
    else if ( (bit -= 10) <= 7 ) {	// Gpio[57:50], RPi4
	mode = Fsel5.read_field( bit );
    }
    else {
	std::ostringstream	css;
	css << "rgFselPin::read_Fsel_bit():  out-of-range bit= " << ebit;
	throw std::range_error ( css.str() );
    }

    return  mode;
}


/*
* Modify Function Select mode of a pin number.
*    Does read/modify/write - not atomic.
*    No copy in the object.
* call:
*    modify_Fsel_bit( bit, mode )
*        bit  = IO pin bit number {0..57}
*        mode = function select mode {f_In, f_Out, .. f_Alt5}
* exceptions:
*    range_error	bit number out-of-range
*/
void
rgFselPin::modify_Fsel_bit(
    unsigned			bit,
    rgFselPin::rgFsel_enum	mode
)
{
    unsigned			ebit = bit;

    if       ( bit        <= 9 ) {	// Gpio[9:0]
	Fsel0.modify_field( bit, mode );
    }
    else if ( (bit -= 10) <= 9 ) {	// Gpio[19:10]
	Fsel1.modify_field( bit, mode );
    }
    else if ( (bit -= 10) <= 9 ) {
	Fsel2.modify_field( bit, mode );
    }
    else if ( (bit -= 10) <= 9 ) {
	Fsel3.modify_field( bit, mode );
    }
    else if ( (bit -= 10) <= 9 ) {
	Fsel4.modify_field( bit, mode );
    }
    else if ( (bit -= 10) <= 7 ) {	// Gpio[57:50], RPi4
	Fsel5.modify_field( bit, mode );
    }
    else {
	std::ostringstream	css;
	css << "rgFselPin::modify_Fsel_bit():  out-of-range bit= " << ebit;
	throw std::range_error ( css.str() );
    }
}


/*
* Return mask of bits corresponding to a given function select mode.
* call:
*    read_Fsel_w0( mode )
*        mode = function select mode {f_In, f_Out, .. f_Alt5}
* return:
*    ()  = bit mask, [31:0]
* exceptions:
*    none
*/
uint32_t
rgFselPin::read_Fsel_w0(
    rgFselPin::rgFsel_enum	mode
)
{
    uint32_t			mask;

    mask  = Fsel3.read_mask( mode ) << 10;
    mask |= Fsel2.read_mask( mode );  mask = mask << 10;
    mask |= Fsel1.read_mask( mode );  mask = mask << 10;
    mask |= Fsel0.read_mask( mode );

    return  mask;
}

uint32_t
rgFselPin::read_Fsel_w1(
    rgFselPin::rgFsel_enum	mode
)
{
    uint32_t			mask;

    mask  = Fsel5.read_mask( mode ) << 10;
    mask |= Fsel4.read_mask( mode );  mask = mask << 8;
    mask |= Fsel3.read_mask( mode ) >> 2;

    return  mask;
}


/*
* Modify function select mode by mask.
*    Does read/modify/write - not atomic.
*    Only registers with mask bits are accessed.
* call:
*    modify_Fsel_w0( mask, mode )
*        mask = bit mask, [31:0]
*        mode = function select mode {f_In, f_Out, .. f_Alt5}
* exceptions:
*    none
*/
void
rgFselPin::modify_Fsel_w0(
    uint32_t			mask,
    rgFselPin::rgFsel_enum	mode
)
{
    if ( mask & 0x3ff ) { Fsel0.modify_mask( mask, mode ); }

    mask = mask >> 10;
    if ( mask & 0x3ff ) { Fsel1.modify_mask( mask, mode ); }

    mask = mask >> 10;
    if ( mask & 0x3ff ) { Fsel2.modify_mask( mask, mode ); }

    mask = mask >> 10;
    if ( mask & 0x3ff ) { Fsel3.modify_mask( mask, mode ); }
}

void
rgFselPin::modify_Fsel_w1(
    uint32_t			mask,
    rgFselPin::rgFsel_enum	mode
)
{
    mask = mask << 2;			// align to Gpio[32] in Fsel3
    if ( mask & 0x3ff ) { Fsel3.modify_mask( mask, mode ); }

    mask = mask >> 10;
    if ( mask & 0x3ff ) { Fsel4.modify_mask( mask, mode ); }

    mask = mask >> 10;
    if ( mask & 0x3ff ) { Fsel5.modify_mask( mask, mode ); }
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
rgFselPin::read_Fsel_bitO(
    unsigned			bit
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
rgFselPin::modify_Fsel_bitO(
    unsigned			bit,
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

