// 2019-10-21  William A. Hudson

// rGPIO SPI0 Master class.  Spi0
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.148-159  ch10 - SPI
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgSpi0.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgSpi0	spx  ( &amx );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*/
rgSpi0::rgSpi0(
    rgAddrMap		*xx
)
{
    GpioBase     = xx->get_mem_block( FeatureAddr );

    CntlStat.init_addr( GpioBase + CntlStat_offset );
	Fifo.init_addr( GpioBase +     Fifo_offset );
      ClkDiv.init_addr( GpioBase +   ClkDiv_offset );
      DmaLen.init_addr( GpioBase +   DmaLen_offset );
       Lossi.init_addr( GpioBase +    Lossi_offset );
      DmaReq.init_addr( GpioBase +   DmaReq_offset );
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
rgSpi0::init_put_reset()
{
    CntlStat.put( 0x00000000 );
    Fifo.put(     0x00000000 );		// include Fifo object state
    ClkDiv.put(   0x00000000 );
    DmaLen.put(   0x00000000 );
    Lossi.put(    0x00000000 );
    DmaReq.put(   0x00000000 );

    CntlStat.put_TxHasSpace_1(     1 );
    CntlStat.put_ReadEnable_1(     1 );

    Lossi.put_LossiHoldDly_4(    0x1 );

    DmaReq.put_DmaRxPanicLev_8( 0x30 );
    DmaReq.put_DmaRxReqLev_8(   0x20 );
    DmaReq.put_DmaTxPanicLev_8( 0x10 );
    DmaReq.put_DmaTxReqLev_8(   0x20 );
}


/*
* Read hardware registers into the object (not Fifo).
*/
void
rgSpi0::grab_regs()
{
    CntlStat.grab();
    // NOT Fifo
    ClkDiv.grab();
    DmaLen.grab();
    Lossi.grab();
    DmaReq.grab();
}


/*
* Write hardware registers from the object (not Fifo).
*/
void
rgSpi0::push_regs()
{
    CntlStat.push();
    // NOT Fifo
    ClkDiv.push();
    DmaLen.push();
    Lossi.push();
    DmaReq.push();
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
rgSpi0::show_debug( std::ostream& sout )
{
    sout.fill('0');
    sout <<hex
	 << "Spi0.CntlStat= 0x"  <<setw(8) << CntlStat.get() <<endl
	 << "Spi0.ClkDiv=   0x"  <<setw(8) <<   ClkDiv.get() <<endl
	 << "Spi0.DmaLen=   0x"  <<setw(8) <<   DmaLen.get() <<endl
	 << "Spi0.Lossi=    0x"  <<setw(8) <<    Lossi.get() <<endl
	 << "Spi0.DmaReq=   0x"  <<setw(8) <<   DmaReq.get() <<endl;
    sout <<dec;
    sout.fill(' ');
}

