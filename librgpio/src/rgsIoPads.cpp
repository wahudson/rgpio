// 2024-05-03  William A. Hudson

// rGPIO  rgsIoPads - IO Pads Interface class for RPi5
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

#include "rgsIoPads.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgsIoPads	iox  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgsIoPads::rgsIoPads(
    rgAddrMap		*xx,
    uint32_t		bank_num
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() == rgRpiRev::soc_BCM2712) ) {
	throw std::domain_error ( "rgsIoPads::  require RPi5 (soc_BCM2712)" );
    }

    if ( bank_num > 2 ) {
	throw std::range_error ( "rgsIoPads::  require bank in {0,1,2}" );
    }

    BankNum    = bank_num;

    DocAddress = FeatureAddr + (BankNum * 0x00004000);

    GpioBase   = xx->get_mem_block( DocAddress );

    for ( int ii=0;  ii<=MaxBit;  ii++ )
    {
	PadReg[ii].init_addr( GpioBase + ((ii+1) * 0x04 /4) );
    }
    // First pad control register is at offset 0x04
}


//--------------------------------------------------------------------------
// Register Array accessors
//--------------------------------------------------------------------------

/*
* Index the PadReg[] register array.
* call:
*    IoPad(27)
* return:
*    () = Reference to the indexed register.
*/
rgsIo_Pad&
rgsIoPads::IoPad( int bit )
{
    if ( (bit < 0) || (bit > MaxBit) ) {
	std::ostringstream      css;
	css << "rgsIo_Pad::IoPad():  bit index out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    return  PadReg[bit];
}

