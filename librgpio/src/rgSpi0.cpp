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

    CntlStat_reg = 0;
    ClkDiv_reg   = 0;
    DmaLen_reg   = 0;
    Lossi_reg    = 0;
    DmaReq_reg   = 0;
}

//--------------------------------------------------------------------------
// Address of registers
//--------------------------------------------------------------------------
// Address value is calculated as a pointer to uint32_t (i.e. a word address).
// The address pointer itself (uint32_t*) is a byte address, word aligned.
// Thus offsets are word offsets (byte offset divide by 4).
// All inline.


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
# #!! Need to validate values against hardware reset.
*/
void
rgSpi0::init_put_reset()
{
    CntlStat_reg = 0x00040000;	// TxHasSpace_1 is read-only
    ClkDiv_reg   = 0x00000000;
    DmaLen_reg   = 0x00000000;
    Lossi_reg    = 0x00000000;
    DmaReq_reg   = 0x00000000;

//  put_TxHasSpace_1(     0x1 );
    put_ReadEnable_1(     0x1 );
    put_LossiHoldDly_4(   0x1 );

    put_DmaRxPanicLev_8( 0x30 );
    put_DmaRxReqLev_8(   0x20 );
    put_DmaTxPanicLev_8( 0x10 );
    put_DmaTxReqLev_8(   0x20 );
}


/*
* Read control/status registers into the object.
*/
void
rgSpi0::grab_regs()
{
    CntlStat_reg = read_CntlStat();
    ClkDiv_reg   = read_ClkDiv();
    DmaLen_reg   = read_DmaLen();
    Lossi_reg    = read_Lossi();
    DmaReq_reg   = read_DmaReq();
}


/*
* Write control registers from the object.
*/
void
rgSpi0::push_regs()
{
    write_CntlStat( CntlStat_reg );
    write_ClkDiv(   ClkDiv_reg   );
    write_DmaLen(   DmaLen_reg   );
    write_Lossi(    Lossi_reg    );
    write_DmaReq(   DmaReq_reg   );
}


/*
* Read status registers into the object.
*     This saves execution time compared to reading all 3 registers.
*/
void
rgSpi0::grab_CntlStat()
{
    CntlStat_reg = read_CntlStat();
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
rgSpi0::get_field(
    const uint32_t		rval,	// register value
    const uint32_t		pos,	// field bit position	#!! LSB
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
rgSpi0::put_field(
    uint32_t*			rp,	// register pointer
    const uint32_t		pos,	// field bit position
    const uint32_t		mask,	// mask field size
    const uint32_t		value	// value to insert
)
{
    if ( value > mask ) {
	std::ostringstream	css;
	css << "rgSpi0::put_field():  value exceeds 0x" <<hex << mask
	    << ":  0x" <<hex << value;
	throw std::range_error ( css.str() );
    }

    *rp = (*rp & ~(mask << pos)) | (value << pos);
}


//--------------------------------------------------------------------------
// Object Field Accessors
//--------------------------------------------------------------------------

/*
* CntlStat_reg fields.
*/

uint32_t  rgSpi0::get_LossiWord_1() {
			return  get_field(  CntlStat_reg, 25, 0x1    );
}
void      rgSpi0::put_LossiWord_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 25, 0x1, v );
}

uint32_t  rgSpi0::get_LossiDmaEn_1() {
			return  get_field(  CntlStat_reg, 24, 0x1    );
}
void      rgSpi0::put_LossiDmaEn_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 24, 0x1, v );
}

uint32_t  rgSpi0::get_CsPolarity_3() {
			return  get_field(  CntlStat_reg, 21, 0x7    );
}
void      rgSpi0::put_CsPolarity_3( uint32_t  v ) {
				put_field( &CntlStat_reg, 21, 0x7, v );
}

uint32_t  rgSpi0::get_RxFullStop_1() {
			return  get_field(  CntlStat_reg, 20, 0x1    );
}
//void      rgSpi0::put_RxFullStop_1( uint32_t  v ) {
//				put_field( &CntlStat_reg, 20, 0x1, v );
//}

uint32_t  rgSpi0::get_RxHalf_1() {
			return  get_field(  CntlStat_reg, 19, 0x1    );
}
//void      rgSpi0::put_RxHalf_1( uint32_t  v ) {
//				put_field( &CntlStat_reg, 19, 0x1, v );
//}

uint32_t  rgSpi0::get_TxHasSpace_1() {
			return  get_field(  CntlStat_reg, 18, 0x1    );
}
//void      rgSpi0::put_TxHasSpace_1( uint32_t  v ) {
//				put_field( &CntlStat_reg, 18, 0x1, v );
//}

uint32_t  rgSpi0::get_RxHasData_1() {
			return  get_field(  CntlStat_reg, 17, 0x1    );
}
//void      rgSpi0::put_RxHasData_1( uint32_t  v ) {
//				put_field( &CntlStat_reg, 17, 0x1, v );
//}

uint32_t  rgSpi0::get_TxEmpty_1() {
			return  get_field(  CntlStat_reg, 16, 0x1    );
}
//void      rgSpi0::put_TxEmpty_1( uint32_t  v ) {
//				put_field( &CntlStat_reg, 16, 0x1, v );
//}

uint32_t  rgSpi0::get_LossiEnable_1() {
			return  get_field(  CntlStat_reg, 13, 0x1    );
}
void      rgSpi0::put_LossiEnable_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 13, 0x1, v );
}

uint32_t  rgSpi0::get_ReadEnable_1() {
			return  get_field(  CntlStat_reg, 12, 0x1    );
}
void      rgSpi0::put_ReadEnable_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 12, 0x1, v );
}

uint32_t  rgSpi0::get_DmaEndCs_1() {
			return  get_field(  CntlStat_reg, 11, 0x1    );
}
void      rgSpi0::put_DmaEndCs_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 11, 0x1, v );
}

uint32_t  rgSpi0::get_IrqRxHalf_1() {
			return  get_field(  CntlStat_reg, 10, 0x1    );
}
void      rgSpi0::put_IrqRxHalf_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 10, 0x1, v );
}

uint32_t  rgSpi0::get_IrqTxEmpty_1() {
			return  get_field(  CntlStat_reg, 9, 0x1    );
}
void      rgSpi0::put_IrqTxEmpty_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 9, 0x1, v );
}

uint32_t  rgSpi0::get_DmaEnable_1() {
			return  get_field(  CntlStat_reg, 8, 0x1    );
}
void      rgSpi0::put_DmaEnable_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 8, 0x1, v );
}

uint32_t  rgSpi0::get_RunActive_1() {
			return  get_field(  CntlStat_reg, 7, 0x1    );
}
void      rgSpi0::put_RunActive_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 7, 0x1, v );
}

uint32_t  rgSpi0::get_CsPolarity_1() {
			return  get_field(  CntlStat_reg, 6, 0x1    );
}
void      rgSpi0::put_CsPolarity_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 6, 0x1, v );
}

uint32_t  rgSpi0::get_ClearRxTxFifo_2() {
			return  get_field(  CntlStat_reg, 4, 0x3    );
}
void      rgSpi0::put_ClearRxTxFifo_2( uint32_t  v ) {
				put_field( &CntlStat_reg, 4, 0x3, v );
}

uint32_t  rgSpi0::get_ClockPolarity_1() {
			return  get_field(  CntlStat_reg, 3, 0x1    );
}
void      rgSpi0::put_ClockPolarity_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 3, 0x1, v );
}

uint32_t  rgSpi0::get_ClockPhase_1() {
			return  get_field(  CntlStat_reg, 2, 0x1    );
}
void      rgSpi0::put_ClockPhase_1( uint32_t  v ) {
				put_field( &CntlStat_reg, 2, 0x1, v );
}

uint32_t  rgSpi0::get_ChipSelectN_2() {
			return  get_field(  CntlStat_reg, 0, 0x3    );
}
void      rgSpi0::put_ChipSelectN_2( uint32_t  v ) {
				put_field( &CntlStat_reg, 0, 0x3, v );
}


/*
* ClkDiv_reg fields.
*/

uint32_t  rgSpi0::get_ClockDiv_16() {
			return  get_field(  ClkDiv_reg,   0, 0xffff    );
}
void      rgSpi0::put_ClockDiv_16( uint32_t  v ) {
				put_field( &ClkDiv_reg,   0, 0xffff, v );
}

/*
* DmaLen_reg fields.
*/

uint32_t  rgSpi0::get_DmaDataLen_16() {
			return  get_field(  DmaLen_reg,   0, 0xffff    );
}
void      rgSpi0::put_DmaDataLen_16( uint32_t  v ) {
				put_field( &DmaLen_reg,   0, 0xffff, v );
}

/*
* Lossi_reg fields.
*/

uint32_t  rgSpi0::get_LossiHoldDly_4() {
			return  get_field(  Lossi_reg,    0, 0xffff    );
}
void      rgSpi0::put_LossiHoldDly_4( uint32_t  v ) {
				put_field( &Lossi_reg,    0, 0xffff, v );
}

/*
* DmaReq_reg fields.
*/

uint32_t  rgSpi0::get_DmaRxPanicLev_8() {
			return  get_field(  DmaReq_reg,   24, 0xff    );
}
void      rgSpi0::put_DmaRxPanicLev_8( uint32_t  v ) {
				put_field( &DmaReq_reg,   24, 0xff, v );
}

uint32_t  rgSpi0::get_DmaRxReqLev_8() {
			return  get_field(  DmaReq_reg,   16, 0xff    );
}
void      rgSpi0::put_DmaRxReqLev_8( uint32_t  v ) {
				put_field( &DmaReq_reg,   16, 0xff, v );
}

uint32_t  rgSpi0::get_DmaTxPanicLev_8() {
			return  get_field(  DmaReq_reg,    8, 0xff    );
}
void      rgSpi0::put_DmaTxPanicLev_8( uint32_t  v ) {
				put_field( &DmaReq_reg,    8, 0xff, v );
}

uint32_t  rgSpi0::get_DmaTxReqLev_8() {
			return  get_field(  DmaReq_reg,    0, 0xff    );
}
void      rgSpi0::put_DmaTxReqLev_8( uint32_t  v ) {
				put_field( &DmaReq_reg,    0, 0xff, v );
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
	 << "Spi0.CntlStat= 0x"  <<setw(8) << CntlStat_reg <<endl
	 << "Spi0.ClkDiv=   0x"  <<setw(8) << ClkDiv_reg   <<endl
	 << "Spi0.DmaLen=   0x"  <<setw(8) << DmaLen_reg   <<endl
	 << "Spi0.Lossi=    0x"  <<setw(8) << Lossi_reg    <<endl
	 << "Spi0.DmaReq=   0x"  <<setw(8) << DmaReq_reg   <<endl;
    sout <<dec;
    sout.fill(' ');
}

