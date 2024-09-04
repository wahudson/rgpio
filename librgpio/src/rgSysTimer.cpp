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

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgSysTimer.h"


//==========================================================================
//## rgSysTimer_TimeDw::  Virtual 64-bit timer counter class
//==========================================================================

/*
* Grab System Timer words for coherent preperation.
*    Read the two 32-bit counter registers in preperation for making a
*    coherent 64-bit value.  The required order is:  W1, W0, W1B
*    Intended to be used before get64().
* call:
*    grab()
*/
void
rgSysTimer_TimeDw::grab()
{
    W1.grab();
    W0.grab();		// reference time point
    W1B.grab();
}


/*
* Get coherent 64-bit System Timer value.
*    Reading the two 32-bit counter register values is not atomic.
*    Special fixup is applied when the w0 counter rolls over after the w1
*    count was read.  Required read order:  W1, W0, W1B
*    Intended to run after grab(), which is split out for testing.
*    Note:  Can be called repeatedly to get the same value.
* call:
*    get64()
* return:
*    ()  = 64-bit System Timer value.
*/
uint64_t
rgSysTimer_TimeDw::get64()
{
    uint64_t		rv;	// return value
    uint32_t		w1a;
    uint32_t		w0;
    uint32_t		w1b;

    w1a = W1.get();
    w0  = W0.get();		// reference time point
    w1b = W1B.get();

    if ( w1a != w1b ) {		// fixup w0 rollover
	if ( w0 < 0x80000000 ) {
	    W1.put( w1b );
	}
    }

    rv = W1.get();		// 64-bit value
    rv = (rv << 32) | w0;

    return  rv;
}


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
    if ( !(rgRpiRev::Global.SocEnum.find() <= rgRpiRev::soc_BCM2711) ) {
	throw std::domain_error
	    ( "rgSysTimer:  require RPi4 (soc_BCM2711) or earlier" );
    }

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

    TimeDw.W0.init_addr( TimeW0.addr() );
    TimeDw.W1.init_addr( TimeW1.addr() );

    TimeDw.W1B.init_addr( TimeW1.addr() );
}


//--------------------------------------------------------------------------
// Object state operations
//--------------------------------------------------------------------------

/*
* Read hardware registers into the object.
* Note values are NOT coherent.
*/
void
rgSysTimer::grab_regs()
{
    Stat.grab();
    TimeW0.grab();
    TimeW1.grab();
    TimeDw.grab();	// virtual double-word register
    Cmp0.grab();
    Cmp1.grab();
    Cmp2.grab();
    Cmp3.grab();
}

