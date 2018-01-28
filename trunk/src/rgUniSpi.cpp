// 2018-01-24  William A. Hudson

// rGPIO Universal SPI Master class.  Spi1, Spi2
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.20-27 Universal SPI Master (2x)
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
*    rgIoPin	spx  ( N, &amx );	// constructor with address map
*    N     = SPI number {1,2}
*    &amx  = pointer to address map object with open device file
*/
rgUniSpi::rgUniSpi(
    uint32_t		spinum,	// SPI number {1,2}
    rgAddrMap		*xx
)
{
    if ( ! ((spinum == 1) || (spinum == 2)) ) {
	std::ostringstream	css;
	css << "rgUniSpi:  constructor requires spi number {1,2}:  " << spinum;
	throw std::range_error ( css.str() );
    }

    GpioBase   = xx->get_mem_block( FeatureAddr );
    SpiNum     = spinum;
    Cntl0Reg   = 0;
    Cntl1Reg   = 0;
    StatReg    = 0;
}

//--------------------------------------------------------------------------
// Access Enable and IRQ bits
//--------------------------------------------------------------------------
// Note SpiNum is also the bit position in these two registers.

/*
* Read the SPI Interrupt Request bit.  (Read-only)
*    Is 1 if an interrupt is pending.
*/
uint32_t
rgUniSpi::read_Spi_IRQ_1()
{
    return  ( ( *(addr_AuxIrq()) >> SpiNum ) & 0x1 );
}

/*
* Read the SPI Access Enable bit
*/
uint32_t
rgUniSpi::read_Spi_Enable_1()
{
    return  ( ( *(addr_AuxEnable()) >> SpiNum ) & 0x1 );
}

/*
* Write the SPI Access Enable bit
*    1= enabled, 0= disabled, no register access
*/
void
rgUniSpi::write_Spi_Enable_1( uint32_t  val )
{
    put_field( (uint32_t*) addr_AuxEnable(), SpiNum, 0x1, val );

    // The volatile value requires a cast.
}


//--------------------------------------------------------------------------
// Address of registers
//--------------------------------------------------------------------------
// Address value is calculated as a pointer to uint32_t (i.e. a word address).
// The address pointer itself (uint32_t*) is a byte address, word aligned.
// Thus offsets are word offsets (byte offset divide by 4).

/*
* Aux Interrupt Request register address.
*/
volatile uint32_t*
rgUniSpi::addr_AuxIrq()
{
    return  (GpioBase + AuxIrq_offset);
}

/*
* Aux Enable register address.
*/
volatile uint32_t*
rgUniSpi::addr_AuxEnable()
{
    return  (GpioBase + AuxEnable_offset);
}

/*
* Control 0 register address.
*/
volatile uint32_t*
rgUniSpi::addr_Cntl0()
{
    return  (GpioBase + Cntl0_offset + ((SpiNum - 1) * 0x10));
}

/*
* Control 1 register address.
*/
volatile uint32_t*
rgUniSpi::addr_Cntl1()
{
    return  (GpioBase + Cntl1_offset + ((SpiNum - 1) * 0x10));
}

/*
* Status register address.
*/
volatile uint32_t*
rgUniSpi::addr_Stat()
{
    return  (GpioBase + Stat_offset + ((SpiNum - 1) * 0x10));
}

/*
* Fifo Peek register address.
*/
volatile uint32_t*
rgUniSpi::addr_Peek()
{
    return  (GpioBase + Peek_offset + ((SpiNum - 1) * 0x10));
}

/*
* Fifo register address.
*/
volatile uint32_t*
rgUniSpi::addr_Fifo()
{
    return  (GpioBase + Fifo_offset + ((SpiNum - 1) * 0x10));
}

/*
* Fifo Hold register address.
*/
volatile uint32_t*
rgUniSpi::addr_FifoH()
{
    return  (GpioBase + FifoH_offset + ((SpiNum - 1) * 0x10));
}


//--------------------------------------------------------------------------
// Direct register access
//--------------------------------------------------------------------------
// All inline.


//--------------------------------------------------------------------------
// Direct control:  (modify register fields)
//--------------------------------------------------------------------------


//--------------------------------------------------------------------------
// Object state opterations
//--------------------------------------------------------------------------

/*
* Read control/status registers into the object.
*/
void
rgUniSpi::grab_regs()
{
    Cntl0Reg = read_Cntl0();
    Cntl1Reg = read_Cntl1();
    StatReg  = read_Stat();
}


/*
* Write control/status registers from the object.
*/
void
rgUniSpi::write_regs()
{
    write_Cntl0( Cntl0Reg );
    write_Cntl1( Cntl1Reg );
    write_Stat(  StatReg );
}


//--------------------------------------------------------------------------
// Generic Field accessors:  (private)
//--------------------------------------------------------------------------
/*
* Register bits are numbered [31:0].
* Field postion is the bit number of the field LSB.
* Mask is the maximum field value.
* These are private because safety relies on proper arguments.
*/

/*
* Get field value.
*    Extract field value out of a 32-bit register value.
* call:
*    speed = get_field( Cntl0Reg, 20, 0xfff )
*    Extract Speed field [31:20]
*/
uint32_t
rgUniSpi::get_field(
    const uint32_t		reg,	// register value
    const uint32_t		pos,	// field bit position
    const uint32_t		mask	// mask field size
)
{
    return  ( (reg >> pos) & mask );
}


/*
* Insert field value.
*    Insert field value into a 32-bit register variable.
*    Value is range checked.
* call:
*    put_field( addr_Cntl0(), 20, 0xfff, 0x042 )
*    Insert value into Speed field [31:20]
* exception:
*    range_error  if ( value > mask )
*/
void
rgUniSpi::put_field(
    uint32_t*			rp,	// register pointer
    const uint32_t		pos,	// field bit position
    const uint32_t		mask,	// mask field size
    const uint32_t		value	// value to insert
)
{
    if ( value > mask ) {
	std::ostringstream	css;
	css << "rgUniSpi::put_field():  value exceeds 0x" <<hex << mask
	    << ":  0x" <<hex << value;
	throw std::range_error ( css.str() );
    }

    *rp = (*rp & ~(mask << pos)) | (value << pos);
}


//--------------------------------------------------------------------------
// Object Field Accessors
//--------------------------------------------------------------------------

//#!! Probably make all inline.

/*
* SPI clock speed.  Freq = system_clock_freq/(2*(speed+1))
*/
uint32_t
rgUniSpi::get_Speed_12()
{
    return  get_field( Cntl0Reg, 20, 0xfff );
}

void
rgUniSpi::put_Speed_12( uint32_t  val )
{
    put_field( &Cntl0Reg, 20, 0xfff, val );
}


//--------------------------------------------------------------------------
// Debug
//--------------------------------------------------------------------------

/*
* Show debug output.
* call:
*    self.show_debug()
*/
void
rgUniSpi::show_debug( std::ostream& sout )
{
    sout << "SpiNum= "        <<dec << SpiNum
	 << "  Cntl0Reg= 0x"  <<hex << Cntl0Reg
	 << "  Cntl1Reg= 0x"  <<hex << Cntl1Reg
	 << "  StatReg=  0x"  <<hex << StatReg
	 << endl;
    sout <<dec;
}

