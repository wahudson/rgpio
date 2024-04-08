// 2020-09-05  William A. Hudson

// rgPudPin IO Pin Pull Up/Down (RPi3 and earlier)
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.89-101  6.  General Purpose I/O (GPIO)
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgPudPin.h"


//==========================================================================
//## rgPudPin::  Main Class
//==========================================================================

/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgPudPin	gx  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
* Note:
*    FeatureAddr and register offsets could come from rgIoPins class, but
*    this is more stand-alone.
*/
rgPudPin::rgPudPin(
    rgAddrMap		*xx
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() <= rgRpiRev::soc_BCM2837) ) {
	std::ostringstream	css;
	css << "rgPudPin:  require RPi3 (soc_BCM2837) or earlier";
	throw std::domain_error ( css.str() );
    }

    GpioBase     = xx->get_mem_block( FeatureAddr );

    PudProgMode.init_addr(   GpioBase + (0x94 /4) );
    PudProgClk_w0.init_addr( GpioBase + (0x98 /4) );
    PudProgClk_w1.init_addr( GpioBase + (0x9c /4) );
}

//--------------------------------------------------------------------------
// Program Pull Resistor Direction
//--------------------------------------------------------------------------

/*
* Program sequence for Pull-up/down pin resistor.  (private)
* Note:  Sequence is not atomic.  Return value is a weak attempt to detect
*    register value interferance by another process.
* call:
*    program_pud( &clkreg, dir, mask )
*        clkreg = {PudProgClk_w0, PudProgClk_w1}
*        dir  = pull direction enum rgPudPin::{pd_Up, pd_Down, pd_Off}
*        mask = bit mask selecting which bits to program
*    Use the wrapper functions.
* return:
*    () = status:  0= success,
*                  1= clash with another process changing register value
* exception:
*    Throw range_error if enum is invalid.
# Note:  The Wait steps suggested in the BCM Doc are not necessary.
*/
bool
rgPudPin::program_pud(
    rgPudPin_Clk	*clkreg,
    rgPud_enum		dir,
    uint32_t		mask
)
{
    uint32_t		mode = dir;	// convert from enum
    bool		errflag;

    if ( mode > 2 ) {
	std::ostringstream	css;
	css << "rgPudPin::program_pud() bad mode:  " << mode;
	throw std::range_error ( css.str() );
	// in case of enum extension
    }

    PudProgClk_w0.write( 0x00000000 );		// disable both clocks
    PudProgClk_w1.write( 0x00000000 );

    PudProgMode.grab();
    PudProgMode.put_UpDown_2( mode );		// set mode
    PudProgMode.push();

    // for ( volatile int i=150;  i>0;  i-- ) {}	// wait setup

    clkreg->write( mask );			// enable clock pins

    // for ( volatile int i=150;  i>0;  i-- ) {}	// wait hold

    // check for process clash, ~80 cycles per read
    PudProgMode.grab();
    errflag = (clkreg->read() != mask) || (PudProgMode.get_UpDown_2() != mode);

    clkreg->write( 0x00000000 );		// disable clock pins

    return  errflag;
}


/*
* Program IO bit Pull-up/down resistor.  (public)
* call:
*    program_pud_bit( dir, num )
*        dir  = pull direction enum rgPudPin::{pd_Up, pd_Down, pd_Off}
*        num  = bit number to be programmed
* return:
*    () = status:  0= success,
*                  1= clash with another process changing register value
* exception:
*    Throw range_error if enum is invalid.
*    Throw range_error if bit number not in {53..0}
*/
bool
rgPudPin::program_pud_bit(
    rgPud_enum		dir,
    uint32_t		num
)
{
    uint32_t		mask;
    bool		errflag;

    if ( num <= 31 ) {
	mask = 1 << num;
	errflag = rgPudPin::program_pud( &PudProgClk_w0, dir, mask );
    }
    else if ( num <= 53 ) {
	mask = 1 << (num - 32);
	errflag = rgPudPin::program_pud( &PudProgClk_w1, dir, mask );
    }
    else {
	std::ostringstream	css;
	css << "rgPudPin::program_pud_bit() require bit number {53..0}:  "
	    << num;
	throw std::range_error ( css.str() );
    }

    return  errflag;
}


//--------------------------------------------------------------------------
// rgPud_enum  Conversions
//--------------------------------------------------------------------------

/*
* Return string name for a pull direction enum.
* call:
*    pud_enum2cstr( pd_Down );
* return:
*    ()  = enum name string, without prefix "pd_"
*/
const char*
rgPudPin::pud_enum2cstr(
    rgPud_enum		dir
)
{
    const char*		strp;	// return pointer

    switch ( dir ) {
	case  pd_Off:   strp = "Off";   break;
	case  pd_Down:  strp = "Down";  break;
	case  pd_Up:    strp = "Up";    break;
    }
    // No "default:" so compiler checks all enums are represented.

    return  strp;
}


/*
* Return rgPud_enum for an int.
*    The int value does not change, but the type does.
* call:
*    int2pud_enum( 2 );
* return:
*    ()  = rgPudPin::pd_Up	enum value
* exception:
*    Throw range_error if int is invalid, not in {0,1,2}.
*/
rgPudPin::rgPud_enum
rgPudPin::int2pud_enum(
    int			num
)
{
    if (!( (0 <= num) && (num <= 2) )) {
	std::ostringstream      css;
	css << "rgPudPin::int2pud_enum() int out of range:  " << num;
	throw std::range_error ( css.str() );
    }

    return  (rgPud_enum) num;
}

