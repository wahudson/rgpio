// 2020-06-16  William A. Hudson

// rGPIO IIC (I2C) Master class.
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.28-37  ch 3. BSC (Broadcom Serial Controller)
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
#include "rgIic.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgIic	icx  ( &amx, N );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*    N     = IIC number {0,1,2}  for BCM2837 RPi3 and earlier
*                       {0..7}   for BCM2711 RPi4
*/
rgIic::rgIic(
    rgAddrMap		*xx,
    uint32_t		iicnum	// IIC number {0..7}
)
{
    uint32_t		addr;

    switch ( iicnum ) {
    case  0:  addr = 0x7e205000;  break;
    case  1:  addr = 0x7e804000;  break;
    case  2:  addr = 0x7e805000;  break;
    case  3:  addr = 0x7e205600;  break;
    case  4:  addr = 0x7e205800;  break;
    case  5:  addr = 0x7e205a80;  break;
    case  6:  addr = 0x7e205c00;  break;
    case  7:  addr = 0x7e205e00;  break;	//#!! guess, not documented
    default:
	std::ostringstream	css;
	css << "rgIic:  constructor requires IIC number {0..7}:  " << iicnum;
	throw std::range_error ( css.str() );
    }

    if ( (rgRpiRev::find_SocEnum() <= rgRpiRev::soc_BCM2837) &&
	(iicnum >= 3)
    ) {
	std::ostringstream	css;
	css << "rgIic:  constructor invalid IIC number for "
	    << rgRpiRev::soc_enum2cstr( rgRpiRev::find_SocEnum() )
	    << ":  " << iicnum;
	throw std::range_error ( css.str() );
    }

    IicNum      = iicnum;
    FeatureAddr = addr;
    GpioBase    = xx->get_mem_addr( addr );

      Cntl.init_addr( GpioBase +   Cntl_offset );
      Stat.init_addr( GpioBase +   Stat_offset );
    DatLen.init_addr( GpioBase + DatLen_offset );
      Addr.init_addr( GpioBase +   Addr_offset );
      Fifo.init_addr( GpioBase +   Fifo_offset );
    ClkDiv.init_addr( GpioBase + ClkDiv_offset );
     Delay.init_addr( GpioBase +  Delay_offset );
    ClkStr.init_addr( GpioBase + ClkStr_offset );
}


//--------------------------------------------------------------------------
// Object state operations
//--------------------------------------------------------------------------

/*
* Initialize all object registers to the power-on reset state.
*    Hardware registers are unchanged.
*    Intended for use with the put_*() field accessor functions.
*    Can be called multiple times.
*    Virtually all fields are zero, or are the inactive state.
*/
void
rgIic::init_put_reset()
{
    Cntl.put(   0x00000000 );
    Stat.put(   0x00000000 );
    DatLen.put( 0x00000000 );
    Addr.put(   0x00000000 );
    Fifo.put(   0x00000000 );
    ClkDiv.put( 0x00000000 );
    Delay.put(  0x00000000 );
    ClkStr.put( 0x00000000 );

    Stat.put_TxEmpty_1(    1 );
    Stat.put_TxHasSpace_1( 1 );

    ClkDiv.put_ClkDiv_16(  0x05dc );

    Delay.put_Fall2Out_16( 0x0030 );
    Delay.put_Rise2In_16(  0x0030 );

    ClkStr.put_TimeOut_16( 0x0040 );
}


/*
* Read control/status registers into the object.
*/
void
rgIic::grab_regs()
{
    Cntl.grab();
    Stat.grab();
    DatLen.grab();
    Addr.grab();
    // Fifo.grab();	// NOT Fifo, since it has side-effects.
    ClkDiv.grab();
    Delay.grab();
    ClkStr.grab();
}


/*
* Write control registers from the object.
*     Note the hardware Stat register, is read-only, and is not copied.
*/
void
rgIic::push_regs()
{
    Cntl.push();
    // Stat.push();	// NOT Stat
    DatLen.push();
    Addr.push();
    // Fifo.push();	// NOT Fifo, since it has side-effects.
    ClkDiv.push();
    Delay.push();
    ClkStr.push();
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
rgIic::show_debug( std::ostream& sout )
{
    sout << "IicNum= "     <<dec << IicNum
	 << "  Cntl= 0x"   <<hex << Cntl.get()
	 << "  Stat= 0x"   <<hex << Stat.get()
	 << endl;
    sout <<dec;
}

