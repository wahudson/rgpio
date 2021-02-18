// 2020-04-19  William A. Hudson

// rGPIO  Io Pin Registers.
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

#include "rgAddrMap.h"
#include "rgIoPins.h"


//==========================================================================
//## rgReg_rw::  Base Class
//==========================================================================

/*
* Modify register.
*    Does a read/modify/write for normal read-write capable registers.
*/
void
rgReg_rw::modify(
    uint32_t		mask,
    uint32_t		value
)
{
    uint32_t            x;

    x = read();
    x = (x & (~ mask) ) | (value & mask);
    write( x );
}


//==========================================================================
//## rgIoPins::  Main Class
//==========================================================================

/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgIoPins	gx  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgIoPins::rgIoPins(
    rgAddrMap		*xx
)
{
    GpioBase     = xx->get_mem_block( FeatureAddr );

    // Real Registers

		 Fsel0.init_addr( GpioBase + (0x00 /4) );
		 Fsel1.init_addr( GpioBase + (0x04 /4) );
		 Fsel2.init_addr( GpioBase + (0x08 /4) );
		 Fsel3.init_addr( GpioBase + (0x0c /4) );
		 Fsel4.init_addr( GpioBase + (0x10 /4) );
		 Fsel5.init_addr( GpioBase + (0x14 /4) );

	     PinSet_w0.init_addr( GpioBase + (0x1c /4) );
	     PinSet_w1.init_addr( GpioBase + (0x20 /4) );

	     PinClr_w0.init_addr( GpioBase + (0x28 /4) );
	     PinClr_w1.init_addr( GpioBase + (0x2c /4) );

	    PinRead_w0.init_addr( GpioBase + (0x34 /4) );
	    PinRead_w1.init_addr( GpioBase + (0x38 /4) );

	EventStatus_w0.init_addr( GpioBase + (0x40 /4) );
	EventStatus_w1.init_addr( GpioBase + (0x44 /4) );

	 DetectRise_w0.init_addr( GpioBase + (0x4c /4) );
	 DetectRise_w1.init_addr( GpioBase + (0x50 /4) );

	 DetectFall_w0.init_addr( GpioBase + (0x58 /4) );
	 DetectFall_w1.init_addr( GpioBase + (0x5c /4) );

	 DetectHigh_w0.init_addr( GpioBase + (0x64 /4) );
	 DetectHigh_w1.init_addr( GpioBase + (0x68 /4) );

	  DetectLow_w0.init_addr( GpioBase + (0x70 /4) );
	  DetectLow_w1.init_addr( GpioBase + (0x74 /4) );

    DetectAsyncRise_w0.init_addr( GpioBase + (0x7c /4) );
    DetectAsyncRise_w1.init_addr( GpioBase + (0x80 /4) );

    DetectAsyncFall_w0.init_addr( GpioBase + (0x88 /4) );
    DetectAsyncFall_w1.init_addr( GpioBase + (0x8c /4) );

	   PudProgMode.init_addr( GpioBase + (0x94 /4) );
	 PudProgClk_w0.init_addr( GpioBase + (0x98 /4) );
	 PudProgClk_w1.init_addr( GpioBase + (0x9c /4) );

	      PullSel0.init_addr( GpioBase + (0xe4 /4) );
	      PullSel1.init_addr( GpioBase + (0xe8 /4) );
	      PullSel2.init_addr( GpioBase + (0xec /4) );
	      PullSel3.init_addr( GpioBase + (0xf0 /4) );

    // Virtual Registers

    PinLevel_w0.init_addr(     PinRead_w0.addr() );
    PinLevel_w0.init_addr_set(  PinSet_w0.addr() );
    PinLevel_w0.init_addr_clr(  PinClr_w0.addr() );

    PinLevel_w1.init_addr(     PinRead_w1.addr() );
    PinLevel_w1.init_addr_set(  PinSet_w1.addr() );
    PinLevel_w1.init_addr_clr(  PinClr_w1.addr() );
}

