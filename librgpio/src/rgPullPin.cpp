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
    GpioBase     = xx->get_mem_block( FeatureAddr );

    PullSel0.init_addr( GpioBase + (0xe4 /4) );
    PullSel1.init_addr( GpioBase + (0xe8 /4) );
    PullSel2.init_addr( GpioBase + (0xec /4) );
    PullSel3.init_addr( GpioBase + (0xf0 /4) );

    PullSel[0] = &PullSel0;
    PullSel[1] = &PullSel1;
    PullSel[2] = &PullSel2;
    PullSel[3] = &PullSel3;
}

//--------------------------------------------------------------------------
// Read/Modify Pull pins
//--------------------------------------------------------------------------

/*
* Read Pull Direction of a pin number.
*    No copy in the object.
* call:
*    read_Pull_bit( bit )
*        bit = IO pin bit number {0..57}
* return:
*    ()  = Pull direction enum {pd_Off, pd_Up, pd_Down, pd_Unknown}
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgPullPin::rgPull_enum
rgPullPin::read_Pull_bit(
    int			bit
)
{
    rgReg_rw			*rp;	// register pointer
    int				pos;	// position in rp
    uint32_t			rv;
    uint32_t			dir;

    rp = pullreg_bit( bit, &pos );	// get register and field position

    rv = rp->read();

    dir = (rv >> pos) & 0x3;		// 2-bit field

    return  rgPullPin::rgPull_enum( dir );
	// explicit int to enum conversion, in range of enum
}


/*
* Modify Pull Direction dir of a pin number.
*    Does read/modify/write - not atomic.
*    No copy in the object.
* call:
*    modify_Pull_bit( bit, dir )
*        bit  = IO pin bit number {0..57}
*        dir  = Pull direction enum {pd_Off, pd_Up, pd_Down}
* exceptions:
*    Throw range_error for bit out-of-range.
*    Throw range_error if (dir == pd_Unknown).
*/
void
rgPullPin::modify_Pull_bit(
    int				bit,
    rgPullPin::rgPull_enum	dir
)
{
    rgReg_rw			*rp;	// register pointer
    int				pos;	// position in rp
    uint32_t			mask;
    uint32_t			value;

    if ( dir == pd_Unknown ) {
	throw std::range_error ( "rgPullPin::modify_Pull_bit() invalid "
				    "direction:  pd_Unknown" );
    }

    rp = pullreg_bit( bit, &pos );	// get register and field position

    mask  = (                0x3) << pos;
    value = ((uint32_t)dir & 0x3) << pos;

    rp->modify( mask, value );
}


//--------------------------------------------------------------------------
// Register field position
//--------------------------------------------------------------------------

/*
* Get PullSel* register pointer and field position for a bit number.
*    The field position pos is the LSB of the 2-bit pull direction value.
* call:
*    rp = pullreg_bit( bit, &pos )
*        bit  = pin bit number 0..57
*        pos  = field position within register, {0,2,4,..,30}
* return:
*    ()  = pointer to the PullSel* register for that bit
* exceptions:
*    Throw range_error for bit out-of-range.
*/
rgReg_rw*
rgPullPin::pullreg_bit(
    int			bit,		// pin bit number
    int			*pos
)
{
    if ( (bit < 0) || (bit > 57) ) {
	std::ostringstream	css;
	css << "rgPullPin::pullreg_bit():  out-of-range bit= " << bit;
	throw std::range_error ( css.str() );
    }

    int  rnum = (int)bit / (int)16;		// register number 0..3

    int  kk = bit - ( rnum * (int)16 );		// sub-bit in register

    *pos = kk * 2;			// location of 2-bit field

//    cout << "bit=" << bit << "  rnum=" << rnum << "  pos=" << *pos <<endl;

    return  ( PullSel[rnum] );
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

    return  (rgPull_enum) num;
}

