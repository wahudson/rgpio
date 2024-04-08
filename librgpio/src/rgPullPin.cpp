// 2021-01-28  William A. Hudson

// rgPullPin IO Pin Pull Up/Down for RPi4
//
// See:  BCM2711 ARM Peripherals (2020-10-16)
//      p.64-80  5.  General Purpose I/O (GPIO)
//
// Note:  RPi4 is completely different than RPi3 and earlier.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgIoPins.h"	// class rgReg_rw
#include "rgPullPin.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgPullPin	gx  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
* Note:
*    FeatureAddr and register offsets could come from rgIoPins class, but
*    this is more stand-alone.
*/
rgPullPin::rgPullPin(
    rgAddrMap		*xx
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() == rgRpiRev::soc_BCM2711) ) {
	std::ostringstream	css;
	css << "rgPullPin:  require RPi4 (soc_BCM2711)";
	throw std::domain_error ( css.str() );
    }

    GpioBase     = xx->get_mem_block( FeatureAddr );

    PullSel0.init_addr( GpioBase + (0xe4 /4) );
    PullSel1.init_addr( GpioBase + (0xe8 /4) );
    PullSel2.init_addr( GpioBase + (0xec /4) );
    PullSel3.init_addr( GpioBase + (0xf0 /4) );
}

//--------------------------------------------------------------------------
// rgPullPin_reg  class functions.
//--------------------------------------------------------------------------

/*
* Modify pull direction at field number.
*    Setting reserved value 3 (pd_Unknown) is allowed, but not recommended.
* call:
*    modify_field( field, dir )
*        field  = logical field number {0, 1, .. 15}, right-to-left
*        dir    = pull direction enum {pd_Off, pd_Up, pd_Down}
* exceptions:
*    range_error	field out-of-range
*    range_error	invalid pull direction
*/
void
rgPullPin::rgPullPin_reg::modify_field(
    unsigned		field,
    rgPull_enum		dir
)
{
    unsigned		pos;		// field position
    uint32_t		mask;
    uint32_t		value;

    if ( field > 15 ) {
	std::ostringstream	css;
	css << "rgPullPin_reg::modify_field():  field out-of-range:  " << field;
	throw std::range_error ( css.str() );
    }

    pos = field * 2;			// location of 2-bit field

    mask  =        0x3  << pos;
    value = (dir & 0x3) << pos;

    modify( mask, value );
}


/*
* Modify pull direction by mask.
*    Setting reserved value 3 (pd_Unknown) is allowed, but not recommended.
* call:
*    modify_mask( mask, dir )
*        mask  = packed bit mask, only bits [15:0] are used
*        dir   = pull direction enum {pd_Off, pd_Up, pd_Down}
* exceptions:
*    none
*/
void
rgPullPin::rgPullPin_reg::modify_mask(
    uint32_t		mask,
    rgPull_enum		dir
)
{
    uint32_t		mv;
    uint32_t		fv;
    uint32_t		rmask = 0;	// expanded register mask
    uint32_t		value = 0;	// expanded register value

    fv = dir & 0x3;					// 2-bit field value

    for ( int ii=0;  ii<=15;  ii++ )
    {
	mv    = ( mask & 0x00008000 ) ? 0x3 : 0x0;	// 2-bit mask value
	rmask = (rmask << 2) | mv;
	value = (value << 2) | fv;

	mask = mask << 1;	// scan mask bits left to right
    }

    modify( rmask, value );
}


/*
* Read pull direction at field number.
* call:
*    read_field( field )
*        field  = logical field number {0, 1, .. 15}, right-to-left
* return:
*    ()  = Pull direction enum {pd_Off, pd_Up, pd_Down, pd_Unknown}
* exceptions:
*    range_error	field out-of-range
*/
rgPullPin::rgPull_enum
rgPullPin::rgPullPin_reg::read_field(
    unsigned		field
)
{
    unsigned		pos;		// field position
    uint32_t		value;

    if ( field > 15 ) {
	std::ostringstream	css;
	css << "rgPullPin_reg::read_field():  field out-of-range:  " << field;
	throw std::range_error ( css.str() );
    }

    pos = field * 2;			// location of 2-bit field

    value = (read() >> pos) & 0x3;

    return  rgPull_enum( value );	// explicit enum conversion
}


/*
* Return mask of bits corresponding to pull direction.
* call:
*    read_mask( dir )
*        dir   = pull direction enum {pd_Off, pd_Up, pd_Down}
* return:
*        ()  = packed bit mask, [15:0]
* exceptions:
*    none
*/
uint32_t
rgPullPin::rgPullPin_reg::read_mask(
    rgPull_enum		dir
)
{
    uint32_t		fv = dir;	// 2-bit field value
    uint32_t		value;		// register value
    uint32_t		mask = 0;	// return mask

    value = read();

    for ( int ii=0;  ii<=15;  ii++ )
    {
	mask  >>= 1;

	if ( (value & 0x00000003) == fv ) {
	      mask |= 0x00008000;
	}

	value >>= 2;
    }

    return  mask;
}


//--------------------------------------------------------------------------
// Top level Read/Modify Pull pins
//--------------------------------------------------------------------------

/*
* Read Pull Direction of a pin number.
* call:
*    read_Pull_bit( bit )
*        bit = IO pin bit number {0..57}
* return:
*    ()  = Pull direction enum {pd_Off, pd_Up, pd_Down, pd_Unknown}
* exceptions:
*    range_error	bit number out-of-range
*/
rgPullPin::rgPull_enum
rgPullPin::read_Pull_bit(
    unsigned		bit
)
{
    uint32_t		dir;

    if ( bit > 57 ) {
	std::ostringstream	css;
	css << "rgPullPin::read_Pull_bit():  bit out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    if ( bit <= 15 ) {
	dir = PullSel0.read_field( bit );
    }
    else if ( (bit -= 16) <= 15 ) {
	dir = PullSel1.read_field( bit );
    }
    else if ( (bit -= 16) <= 15 ) {
	dir = PullSel2.read_field( bit );
    }
    else if ( (bit -= 16) <= 15 ) {
	dir = PullSel3.read_field( bit );
    }

    return  rgPull_enum( dir );		// explicit enum conversion
}


/*
* Modify Pull Direction dir of a pin number.
*    Does read/modify/write - not atomic.
* call:
*    modify_Pull_bit( bit, dir )
*        bit  = IO pin bit number {0..57}
*        dir  = Pull direction enum {pd_Off, pd_Up, pd_Down}
* exceptions:
*    range_error	bit number out-of-range
*    range_error	if (dir == pd_Unknown)
*/
void
rgPullPin::modify_Pull_bit(
    unsigned			bit,
    rgPullPin::rgPull_enum	dir
)
{
    if ( dir == pd_Unknown ) {
	throw std::range_error ( "rgPullPin::modify_Pull_bit() invalid "
				    "direction:  pd_Unknown" );
    }

    if ( bit > 57 ) {
	std::ostringstream	css;
	css << "rgPullPin::modify_Pull_bit():  bit out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    if ( bit <= 15 ) {
	PullSel0.modify_field( bit, dir );
    }
    else if ( (bit -= 16) <= 15 ) {
	PullSel1.modify_field( bit, dir );
    }
    else if ( (bit -= 16) <= 15 ) {
	PullSel2.modify_field( bit, dir );
    }
    else if ( (bit -= 16) <= 15 ) {
	PullSel3.modify_field( bit, dir );
    }
}


/*
* Return mask of bits corresponding to pull direction.
* call:
*    read_Pull_w0( dir )
*        dir  = Pull direction enum {pd_Off, pd_Up, pd_Down}
* return:
*    ()  = Pull direction enum {pd_Off, pd_Up, pd_Down, pd_Unknown}
* exceptions:
*    range_error	bit number out-of-range
*/
uint32_t
rgPullPin::read_Pull_w0(
    rgPull_enum			dir
)
{
    uint32_t			mask;

    mask  = PullSel1.read_mask( dir ) << 16;
    mask |= PullSel0.read_mask( dir );

    return  mask;
}

uint32_t
rgPullPin::read_Pull_w1(
    rgPull_enum			dir
)
{
    uint32_t			mask;

    mask  = PullSel3.read_mask( dir ) << 16;
    mask |= PullSel2.read_mask( dir );

    return  mask;
}


/*
* Modify pull direction by mask.
* call:
*    modify_Pull_w0( mask, dir )
*        mask  = packed bit mask, only bits [15:0] are used
*        dir   = Pull direction enum {pd_Off, pd_Up, pd_Down}
* exceptions:
*    range_error	bit number out-of-range
*    range_error	if (dir == pd_Unknown)
*/
void
rgPullPin::modify_Pull_w0(
    uint32_t			mask,
    rgPull_enum			dir
)
{
    uint32_t		mv;

    if ( dir == pd_Unknown ) {
	throw std::range_error ( "rgPullPin::modify_Pull_w0() invalid "
				    "direction:  pd_Unknown" );
    }

    mv =         mask & 0x0000ffff;
    PullSel0.modify_mask( mv, dir );

    mv = (mask >> 16) & 0x0000ffff;
    PullSel1.modify_mask( mv, dir );
}

void
rgPullPin::modify_Pull_w1(
    uint32_t			mask,
    rgPull_enum			dir
)
{
    uint32_t		mv;

    if ( dir == pd_Unknown ) {
	throw std::range_error ( "rgPullPin::modify_Pull_w1() invalid "
				    "direction:  pd_Unknown" );
    }

    mv =         mask & 0x0000ffff;
    PullSel2.modify_mask( mv, dir );

    mv = (mask >> 16) & 0x0000ffff;
    PullSel3.modify_mask( mv, dir );
}

//--------------------------------------------------------------------------
// rgPull_enum  Conversions
//--------------------------------------------------------------------------

/*
* Return string name for a pull direction enum.
* call:
*    pull_enum2cstr( pd_Down );
* return:
*    ()  = enum name string, without prefix "pd_"
* exception:
*    Throw range_error if enum is invalid.
*/
const char*
rgPullPin::pull_enum2cstr(
    rgPull_enum		dir
)
{
    const char*		strp = NULL;	// return pointer

    switch ( dir ) {
	case  pd_Off:      strp = "Off";      break;
	case  pd_Up:       strp = "Up";       break;
	case  pd_Down:     strp = "Down";     break;
	case  pd_Unknown:  strp = "Unknown";  break;
    }
    // No "default:" so compiler checks all enums are represented.

    if ( ! strp ) {
	std::ostringstream      css;
	css << "rgPullPin::pull_enum2cstr() invalid rgPull_enum:  " << dir;
	throw std::range_error ( css.str() );
    }

    return  strp;
}


/*
* Return rgPull_enum for an int.
*    The int value does not change, but the type does.
* call:
*    int2pull_enum( 2 );
* return:
*    ()  = rgPullPin::pd_Down	enum value
* exception:
*    Throw range_error if int is invalid, not in {0,1,2,3}.
*/
rgPullPin::rgPull_enum
rgPullPin::int2pull_enum(
    int			num
)
{
    if (!( (0 <= num) && (num <= 3) )) {
	std::ostringstream      css;
	css << "rgPullPin::int2pull_enum() int out of range:  " << num;
	throw std::range_error ( css.str() );
    }

    return  rgPull_enum( num );		// explicit enum conversion
}

