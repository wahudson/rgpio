// 2024-05-24  William A. Hudson

// rGPIO  rgsRio - Register Input/Output (RIO) class for RPi5
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgRpiRev.h"
#include "rgAddrMap.h"

#include "rgsRio.h"

/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgsRio	iox  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgsRio::rgsRio(
    rgAddrMap		*xx,
    uint32_t		bank_num
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() == rgRpiRev::soc_BCM2712) ) {
	throw std::domain_error ( "rgsRio::  require RPi5 (soc_BCM2712)" );
    }

    if ( bank_num > 2 ) {
	throw std::range_error ( "rgsRio::  require bank in {0,1,2}" );
    }
    // want this derived class name in the message

    BankNum    = bank_num;

    DocAddress = FeatureAddr + (BankNum * 0x00004000);

    GpioBase   = xx->get_mem_block( DocAddress );

    RioOut.init_addr(    GpioBase + (0x00 /4) );
    RioOutEn.init_addr(  GpioBase + (0x04 /4) );
    RioIn.init_addr(     GpioBase + (0x08 /4) );
    RioInSync.init_addr( GpioBase + (0x0c /4) );
}

