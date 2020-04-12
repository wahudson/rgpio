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

// This file contains two classes declared in rgClk.h:
//    rgClkMan::  Base Class
//    rgClk::     Derived Class

//==========================================================================
// rgClkMan::  Base Class
//==========================================================================

/*
* Constructor.
*    No address initialization here, do in the derived class.
*/
rgClkMan::rgClkMan()
{
    WaitTime_ns = 0;		// no nanosleep
    WaitCount   = 10000;	// about 10x margin
    BusyCount   = 0;
}


//--------------------------------------------------------------------------
// Object state operations
//--------------------------------------------------------------------------

/*
* Initialize object control/status registers to the power-on reset state.
*    Intended for use with the put_*() field accessor functions, and can be
*    called multiple times.
*    Virtually all fields are zero, or are the inactive state.
*    Hardware registers are unchanged.
*/
void
rgClkMan::init_put_reset()
{
    Cntl.put(     0x00000000 );
    Divr.put(     0x00000000 );
}


/*
* Read hardware registers into the object.
*/
void
rgClkMan::grab_regs()
{
    Cntl.grab();
    Divr.grab();
}


/*
* Write hardware registers from the object.
*/
void
rgClkMan::push_regs()
{
    Cntl.push();
    Divr.push();
}


/*
* Apply passwd, then write hardware registers from the object.
*/
void
rgClkMan::apply_regs()
{
    Cntl.apply();
    Divr.apply();
}


//--------------------------------------------------------------------------
// Special functions
//--------------------------------------------------------------------------

/*
* Apply registers safely, first disabling clock and waiting for not busy.
*    The safe sequence is:
*        Disable clock if it was running, then wait for not busy.
*        Apply registers without simultaneous enable.
*        Apply Enable_1 if requested.
*    Register values are NOT written if still busy after wait timeout.
*    Both Cntl and Divr are applied with password fields set to 0x5a.
*    Use accessors to configure WaitTime_ns and WaitCount.
*    See wait_while_busy().
* Note:  In general, changing field values when busy can result in a corrupted
*    state where the clock output is the wrong frequency or hung.
* call:
*    apply_nicely()
* return:
*    ()  = status:  0= success
*                   1= wait on busy timeout, clock is disabled
*/
bool
rgClkMan::apply_nicely()
{
    bool		busy = 0;
    rgClk_Cntl		cx  ( this->Cntl );	// construct copy

    cx.grab();				// update register

    if ( cx.get_Enable_1() ) {
	cx.put_Enable_1( 0 );
	cx.apply();			// disable clock
    }

    busy = wait_while_busy();		// check Busy_1 and set BusyCount

    if ( ! busy ) {
	if ( Cntl.get_Enable_1() ) {
	    Cntl.put_Enable_1( 0 );	// apply registers without enable
	    Cntl.apply();
	    Divr.apply();

	    Cntl.put_Enable_1( 1 );	// then enable
	    Cntl.apply();
	}
	else {
	    Cntl.apply();
	    Divr.apply();
	}
    }

    return  busy;
}


/*
* Wait until not Busy or time limit reached.
*    Does not disable the clock, will time out if still enabled.
*    Cntl object and hardware registers are unchanged.
*    Intrinsic loop iteration time is ~310 ns, dominated by the Cntl register
*    read time.
* call:
*    wait_while_busy()
*    WaitCount   = limit number of wait cycles, 0= no wait
*    WaitTime_ns = nanoseconds to delay each cycle, 0= none
* return:
*    ()        = busy status, 0= success clock stopped, 1= still busy
*    BusyCount = number of wait cycles until Busy_1=0 or limit reached.
*/
bool
rgClkMan::wait_while_busy()
{
    rgClk_Cntl		cx  ( this->Cntl );	// construct copy
    bool		busy;

    BusyCount = 0;
    cx.grab();
    busy = cx.get_Busy_1();

    while ( busy && (BusyCount < WaitCount) )
    {
	BusyCount++;		// count number of wait periods

	if ( WaitTime_ns ) {
	    //#!! nanosleep()
	}

	cx.grab();
	busy = cx.get_Busy_1();
    }

    return  busy;
}


//--------------------------------------------------------------------------
// Debug
//--------------------------------------------------------------------------

/*
* Show debug output.
* call:
*    self.show_debug( cout )
*/
void
rgClkMan::show_debug( std::ostream& sout )
{
    sout.fill('0');
    sout <<hex
	 << "Clk.Cntl= 0x"  <<setw(8) << Cntl.get() <<endl
	 << "Clk.Divr= 0x"  <<setw(8) << Divr.get() <<endl
	 ;

    sout <<dec;
    sout.fill(' ');
}


//==========================================================================
// rgClk::  Derived Class
//==========================================================================

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

