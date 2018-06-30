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
*    rgUniSpi	spx  ( N, &amx );	// constructor with address map
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
rgUniSpi::write_Spi_Enable_1( uint32_t  bit1 )
{
    put_field( (uint32_t*) addr_AuxEnable(), SpiNum, 0x1, bit1 );

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
* Initialize object control/status registers to the power-on reset state.
* Hardware registers are unchanged.
* Intended for use with the put_*() field accessor functions, and can be
* called multiple times.
* Virtually all fields are zero, or are the inactive state.
*/
void
rgUniSpi::init_put_reset()
{
    Cntl0Reg = 0x00000000;
    Cntl1Reg = 0x00000000;
    StatReg  = 0x00000280;	// TxEmpty=1, RxEmpty=1

    put_ChipSelects_3( 0x7 );	// all 3 inactive
}


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
* Write control registers from the object.
*     Note the hardware Stat register, is read-only, and is not copied.
*/
void
rgUniSpi::push_regs()
{
    write_Cntl0( Cntl0Reg );
    write_Cntl1( Cntl1Reg );
}


/*
* Read status registers into the object.
*     This saves execution time compared to reading all 3 registers.
*/
void
rgUniSpi::grab_Stat()
{
    StatReg  = read_Stat();
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
    const uint32_t		rval,	// register value
    const uint32_t		pos,	// field bit position
    const uint32_t		mask	// mask field size
)
{
    return  ( (rval >> pos) & mask );
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

/*
* Cntl0 fields.
*/

uint32_t  rgUniSpi::get_Speed_12() {
			return  get_field(  Cntl0Reg, 20, 0xfff    );
}
void      rgUniSpi::put_Speed_12( uint32_t  v ) {
				put_field( &Cntl0Reg, 20, 0xfff, v );
}

uint32_t  rgUniSpi::get_ChipSelects_3() {
			return  get_field(  Cntl0Reg, 17, 0x7    );
}
void      rgUniSpi::put_ChipSelects_3( uint32_t  v ) {
				put_field( &Cntl0Reg, 17, 0x7, v );
}

uint32_t  rgUniSpi::get_PostInMode_1() {
			return  get_field(  Cntl0Reg, 16, 0x1    );
}
void      rgUniSpi::put_PostInMode_1( uint32_t  v ) {
				put_field( &Cntl0Reg, 16, 0x1, v );
}

uint32_t  rgUniSpi::get_VariableCS_1() {
			return  get_field(  Cntl0Reg, 15, 0x1    );
}
void      rgUniSpi::put_VariableCS_1( uint32_t  v ) {
				put_field( &Cntl0Reg, 15, 0x1, v );
}

uint32_t  rgUniSpi::get_VariableWidth_1() {
			return  get_field(  Cntl0Reg, 14, 0x1    );
}
void      rgUniSpi::put_VariableWidth_1( uint32_t  v ) {
				put_field( &Cntl0Reg, 14, 0x1, v );
}

uint32_t  rgUniSpi::get_DoutHoldTime_2() {
			return  get_field(  Cntl0Reg, 12, 0x3    );
}
void      rgUniSpi::put_DoutHoldTime_2( uint32_t  v ) {
				put_field( &Cntl0Reg, 12, 0x3, v );
}

uint32_t  rgUniSpi::get_EnableSerial_1() {
			return  get_field(  Cntl0Reg, 11, 0x1    );
}
void      rgUniSpi::put_EnableSerial_1( uint32_t  v ) {
				put_field( &Cntl0Reg, 11, 0x1, v );
}

uint32_t  rgUniSpi::get_InRising_1() {
			return  get_field(  Cntl0Reg, 10, 0x1    );
}
void      rgUniSpi::put_InRising_1( uint32_t  v ) {
				put_field( &Cntl0Reg, 10, 0x1, v );
}

uint32_t  rgUniSpi::get_ClearFifos_1() {
			return  get_field(  Cntl0Reg,  9, 0x1    );
}
void      rgUniSpi::put_ClearFifos_1( uint32_t  v ) {
				put_field( &Cntl0Reg,  9, 0x1, v );
}

uint32_t  rgUniSpi::get_OutRising_1() {
			return  get_field(  Cntl0Reg,  8, 0x1    );
}
void      rgUniSpi::put_OutRising_1( uint32_t  v ) {
				put_field( &Cntl0Reg,  8, 0x1, v );
}

uint32_t  rgUniSpi::get_InvertClk_1() {
			return  get_field(  Cntl0Reg,  7, 0x1    );
}
void      rgUniSpi::put_InvertClk_1( uint32_t  v ) {
				put_field( &Cntl0Reg,  7, 0x1, v );
}

uint32_t  rgUniSpi::get_OutMsbFirst_1() {
			return  get_field(  Cntl0Reg,  6, 0x1    );
}
void      rgUniSpi::put_OutMsbFirst_1( uint32_t  v ) {
				put_field( &Cntl0Reg,  6, 0x1, v );
}

uint32_t  rgUniSpi::get_ShiftLength_6() {
			return  get_field(  Cntl0Reg,  0, 0x3f    );
}
void      rgUniSpi::put_ShiftLength_6( uint32_t  v ) {
				put_field( &Cntl0Reg,  0, 0x3f, v );
}


/*
* Cntl1 fields.
*/

uint32_t  rgUniSpi::get_CsHighTime_3() {
			return  get_field(  Cntl1Reg,  8, 0x7    );
}
void      rgUniSpi::put_CsHighTime_3( uint32_t  v ) {
				put_field( &Cntl1Reg,  8, 0x7, v );
}

uint32_t  rgUniSpi::get_TxEmptyIRQ_1() {
			return  get_field(  Cntl1Reg,  7, 0x1    );
}
void      rgUniSpi::put_TxEmptyIRQ_1( uint32_t  v ) {
				put_field( &Cntl1Reg,  7, 0x1, v );
}

uint32_t  rgUniSpi::get_DoneIRQ_1() {
			return  get_field(  Cntl1Reg,  6, 0x1    );
}
void      rgUniSpi::put_DoneIRQ_1( uint32_t  v ) {
				put_field( &Cntl1Reg,  6, 0x1, v );
}

uint32_t  rgUniSpi::get_InMsbFirst_1() {
			return  get_field(  Cntl1Reg,  1, 0x1    );
}
void      rgUniSpi::put_InMsbFirst_1( uint32_t  v ) {
				put_field( &Cntl1Reg,  1, 0x1, v );
}

uint32_t  rgUniSpi::get_KeepInput_1() {
			return  get_field(  Cntl1Reg,  0, 0x1    );
}
void      rgUniSpi::put_KeepInput_1( uint32_t  v ) {
				put_field( &Cntl1Reg,  0, 0x1, v );
}


/*
* Status fields.  Read-only?
*    Field positions determined by experiment.
*    The BCM doc seems incompletely adapted from another version of SPI.
*/

uint32_t  rgUniSpi::get_TxLevel_3() {
			return  get_field( StatReg, 28, 0x7 );
}

uint32_t  rgUniSpi::get_RxLevel_3() {
			return  get_field( StatReg, 20, 0x7 );
}

uint32_t  rgUniSpi::get_TxFull_1() {
			return  get_field( StatReg, 10, 0x1 );
}

uint32_t  rgUniSpi::get_TxEmpty_1() {
			return  get_field( StatReg,  9, 0x1 );
}

uint32_t  rgUniSpi::get_RxFull_1() {
			return  get_field( StatReg,  8, 0x1 );
}

uint32_t  rgUniSpi::get_RxEmpty_1() {
			return  get_field( StatReg,  7, 0x1 );
}

uint32_t  rgUniSpi::get_Busy_1() {
			return  get_field( StatReg,  6, 0x1 );
}

uint32_t  rgUniSpi::get_BitCount_6() {
			return  get_field( StatReg,  0, 0x3f );
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

