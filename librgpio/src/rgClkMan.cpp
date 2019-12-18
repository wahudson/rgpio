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

