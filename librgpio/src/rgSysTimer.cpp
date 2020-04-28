// 2020-04-25  William A. Hudson

// GPIO  System Timer class.
//
// See:  BCM2835 ARM Peripherals (2012)
//      p.172-174  ch 12.  System Timer
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgSysTimer.h"


//==========================================================================
//## rgSysTimer_TimeDw::  Double Register Class
//==========================================================================

//==========================================================================
//## rgSysTimer::  Main Class
//==========================================================================

/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgSysTimer	gx  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgSysTimer::rgSysTimer(
    rgAddrMap		*xx
)
{
    GpioBase     = xx->get_mem_block( FeatureAddr );

    // Real Registers

      Stat.init_addr( GpioBase + (0x00 /4) );

    TimeW0.init_addr( GpioBase + (0x04 /4) );
    TimeW1.init_addr( GpioBase + (0x08 /4) );

      Cmp0.init_addr( GpioBase + (0x0c /4) );
      Cmp1.init_addr( GpioBase + (0x10 /4) );
      Cmp2.init_addr( GpioBase + (0x14 /4) );
      Cmp3.init_addr( GpioBase + (0x18 /4) );

    // Virtual Registers

    TimeDw.init_addr(    TimeW0.addr() );
    TimeDw.init_addr_W1( TimeW1.addr() );
}


//--------------------------------------------------------------------------
// Object state operations
//--------------------------------------------------------------------------

/*
* Read hardware registers into the object.
*/
void
rgSysTimer::grab_regs()
{
    Stat.grab();
    TimeW0.grab();
    TimeW1.grab();
    Cmp0.grab();
    Cmp1.grab();
    Cmp2.grab();
    Cmp3.grab();
}
