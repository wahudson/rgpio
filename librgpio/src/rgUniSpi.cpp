// 2018-01-24  William A. Hudson

// rGPIO Universal SPI Master class.  Spi1, Spi2
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.20-27  ch 2.3 Universal SPI Master
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgUniSpi.h"


/*
* Constructor.
* Require address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_mem();	// select and open device file
* call:
*    rgUniSpi	spx  ( &amx, N );	// constructor with address map
*    &amx  = pointer to address map object with open device file
*    N     = SPI number {1,2}
*/
rgUniSpi::rgUniSpi(
    rgAddrMap		*xx,
    uint32_t		spinum	// SPI number {1,2}
)
{
    uint32_t		delta;

    if ( ! ((spinum == 1) || (spinum == 2)) ) {
	std::ostringstream	css;
	css << "rgUniSpi:  constructor requires spi number {1,2}:  " << spinum;
	throw std::range_error ( css.str() );
    }

    GpioBase = xx->get_mem_block( FeatureAddr );
    SpiNum   = spinum;

    delta    = (SpiNum - 1) * (0x40 /4);	// word distance to Spi2.Cntl0

    AuxIrq.init_addr( GpioBase + AuxIrq_offset );
     AuxEn.init_addr( GpioBase + AuxEn_offset  );

    Cntl0.init_addr( GpioBase + delta + Cntl0_offset );
    Cntl1.init_addr( GpioBase + delta + Cntl1_offset );
     Stat.init_addr( GpioBase + delta +  Stat_offset );
     Peek.init_addr( GpioBase + delta +  Peek_offset );
     Fifo.init_addr( GpioBase + delta +  Fifo_offset );
    FifoH.init_addr( GpioBase + delta + FifoH_offset );
}

//--------------------------------------------------------------------------
// Access Enable and IRQ bits
//--------------------------------------------------------------------------
// Note SpiNum is also the bit position in these two registers.

/*
* Read the SPI Interrupt Request bit.  (Read-only)
*    Hardware register read, object not modified.
*    Returns 1 if an interrupt is pending.
*/
uint32_t
rgUniSpi::read_SpiIrq_1()
{
    return  ( ( AuxIrq.read() >> SpiNum ) & 0x1 );
}

/*
* Read the SPI Access Enable bit.
*    Hardware register read, object not modified.
*/
uint32_t
rgUniSpi::read_SpiEnable_1()
{
    return  ( ( AuxEn.read() >> SpiNum ) & 0x1 );
}

/*
* Write the SPI Access Enable bit.
*    Direct register read/modify/write, not Atomic.
*/
void
rgUniSpi::write_SpiEnable_1( uint32_t  val )
{
    volatile uint32_t*		addr;
    uint32_t			mask;

    addr = AuxEn.addr();

    if ( val ) {
	mask =   0x1 << SpiNum;
	*addr |= mask;
    }
    else {
	mask = ~(0x1 << SpiNum);
	*addr &= mask;
    }
}


//--------------------------------------------------------------------------
// Object state opterations
//--------------------------------------------------------------------------

/*
* Initialize all object registers to the power-on reset state.
*    Hardware registers are unchanged.
*    Intended for use with the put_*() field accessor functions.
*    Can be called multiple times.
*    Virtually all fields are zero, or are the inactive state.
*/
void
rgUniSpi::init_put_reset()
{
    AuxIrq.put( 0x00000000 );
    AuxEn.put(  0x00000000 );

    Cntl0.put(  0x00000000 );
    Cntl1.put(  0x00000000 );
    Stat.put(   0x00000000 );
    Peek.put(   0x00000000 );
    Fifo.put(   0x00000000 );
    FifoH.put(  0x00000000 );

    Cntl0.put_ChipSelects_3( 0x7 );	// all 3 inactive

    Stat.put_TxEmpty_1( 1 );
    Stat.put_RxEmpty_1( 1 );
}


/*
* Read control/status registers into the object.
*/
void
rgUniSpi::grab_regs()
{
    Cntl0.grab();
    Cntl1.grab();
    Stat.grab();
}


/*
* Write control registers from the object.
*     Note the hardware Stat register, is read-only, and is not copied.
*/
void
rgUniSpi::push_regs()
{
    Cntl0.push();
    Cntl1.push();
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
rgUniSpi::show_debug( std::ostream& sout )
{
    sout << "SpiNum= "     <<dec << SpiNum
	 << "  Cntl0= 0x"  <<hex << Cntl0.get()
	 << "  Cntl1= 0x"  <<hex << Cntl1.get()
	 << "  Stat=  0x"  <<hex << Stat.get()
	 << endl;
    sout <<dec;
}

