// 2019-12-10  William A. Hudson

// rGPIO  rgClkMan - Clock Manager base class
//
// Has all the Clock Manager operations, but no addresses.
// Derived class rgClk should be the main user interface.
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
#include "rgClkMan.h"


/*
* Constructor.
*    No address initialization here, do in the derived class.
*/
rgClkMan::rgClkMan()
{
    WaitTime_ns = 1000;
    WaitCount   = 3;
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
*    Both Cntl and Divr are applied with password fields set to 0x5a.
*    Use accessors to configure WaitTime_ns and WaitCount.
*    See wait_while_busy().
* call:
*    apply_nicely()
* return:
*    ()  = status, 0= success, 1= was busy after disable
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

    if ( Cntl.get_Enable_1() ) {
	Cntl.put_Enable_1( 0 );		// apply registers without enable
	Cntl.apply();
	Divr.apply();

	Cntl.put_Enable_1( 1 );		// then enable
	Cntl.apply();
    }
    else {
	Cntl.apply();
	Divr.apply();
    }

    return  busy;
}


/*
* Wait until not Busy or time limit reached.
*    Does not disable the clock, will time out if still enabled.
*    Cntl object and hardware registers are unchanged.
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

