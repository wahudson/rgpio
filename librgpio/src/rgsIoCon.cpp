// 2024-05-12  William A. Hudson

// rGPIO  rgsIoCon - IO Control/Status Interface class for RPi5
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

#include "rgsIoCon.h"

/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgsIoCon	iox  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgsIoCon::rgsIoCon(
    rgAddrMap		*xx,
    uint32_t		bank_num
)
{
    if ( !(rgRpiRev::Global.SocEnum.find() == rgRpiRev::soc_BCM2712) ) {
	throw std::domain_error ( "rgsIoCon::  require RPi5 (soc_BCM2712)" );
    }

    if ( bank_num > 2 ) {
	throw std::range_error ( "rgsIoCon::  require bank in {0,1,2}" );
    }
    // this derived class name in message

    BankNum    = bank_num;

    DocAddress = FeatureAddr + (BankNum * 0x00004000);

    GpioBase   = xx->get_mem_block( DocAddress );

    for ( int ii=0;  ii<=MaxBit;  ii++ )
    {
	StatReg[ii].init_addr( GpioBase + (2*ii)     );
	CntlReg[ii].init_addr( GpioBase + (2*ii) + 1 );
    }
    // Interleaved word index, byte offsets *0x04
}


//--------------------------------------------------------------------------
// Register Array accessors
//--------------------------------------------------------------------------

/*
* Index the CntlReg[] register array.
* call:
*    IoCntl(27)
* return:
*    () = Reference to the indexed register.
*/
rgsIo_Cntl&
rgsIoCon::IoCntl( int bit )
{
    if ( (bit < 0) || (bit > MaxBit) ) {
	std::ostringstream      css;
	css << "rgsIo_Cntl::IoCntl():  bit index out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    return  CntlReg[bit];
}


/*
* Index the StatReg[] register array.
* call:
*    IoStat(27)
* return:
*    () = Reference to the indexed register.
*/
rgsIo_Stat&
rgsIoCon::IoStat( int bit )
{
    if ( (bit < 0) || (bit > MaxBit) ) {
	std::ostringstream      css;
	css << "rgsIo_Stat::IoStat():  bit index out-of-range:  " << bit;
	throw std::range_error ( css.str() );
    }

    return  StatReg[bit];
}

