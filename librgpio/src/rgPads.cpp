// 2021-03-08  William A. Hudson

// rGPIO  rgPads - GPIO Pads Control class.
//
// See:  GPIO pads control,  Gert van Loo 2-August-2012.
//    http://www.scribd.com/doc/101830961
//    https://www.raspberrypi.org/documentation/hardware/raspberrypi/gpio/gpio_pads_control.md
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
#include "rgPads.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgPads	px  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgPads::rgPads(
    rgAddrMap		*xx
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() <= rgRpiRev::soc_BCM2711) ) {
	throw std::domain_error
	    ( "rgPads:  require RPi4 (soc_BCM2711) or earlier" );
    }

    GpioBase    = xx->get_mem_block( FeatureAddr );

    PadsA_27s0.init_addr(  GpioBase + (0x2c /4) );
    PadsB_45s28.init_addr( GpioBase + (0x30 /4) );
    PadsC_53s46.init_addr( GpioBase + (0x34 /4) );
}

