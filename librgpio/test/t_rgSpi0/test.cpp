// 2019-10-21  William A. Hudson
//
// Testing:  rgSpi0  SPI0 Master class.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers
//    30-39  Direct register access  read_(), write_()
//    50-59  Object State registers  init_put_reset(), grab_regs(), push_regs()
//    60-98  Object Field Accessors  get_(), put_()  #!! incomplete
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgSpi0.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgSpi0			Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgSpi0		tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "19", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e204000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers
//--------------------------------------------------------------------------

  CASE( "21", "addr_CntlStat()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_CntlStat();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "addr_Fifo()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_Fifo();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "addr_ClkDiv()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_ClkDiv();
	CHECKX( 0x00000008, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "addr_DmaLen()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_DmaLen();
	CHECKX( 0x0000000c, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "addr_Lossi()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_Lossi();
	CHECKX( 0x00000010, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26", "addr_DmaReq()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_DmaReq();
	CHECKX( 0x00000014, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read_(), write_()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition write_(), read_()" );
    try {
	Tx.write_CntlStat( 0xffffffff );
	Tx.write_Fifo(     0xffffffff );
	Tx.write_ClkDiv(   0xffffffff );
	Tx.write_DmaLen(   0xffffffff );
	Tx.write_Lossi(    0xffffffff );
	Tx.write_DmaReq(   0xffffffff );
	CHECKX(            0xffffffff, Tx.read_CntlStat() );
	CHECKX(            0xffffffff, Tx.read_Fifo()     );
	CHECKX(            0xffffffff, Tx.read_ClkDiv()   );
	CHECKX(            0xffffffff, Tx.read_DmaLen()   );
	CHECKX(            0xffffffff, Tx.read_Lossi()    );
	CHECKX(            0xffffffff, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "read_CntlStat()" );
    try {
	Tx.write_CntlStat( 0x33cccc33 );
	CHECKX(            0x33cccc33, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "read_Fifo()" );
    try {
	Tx.write_Fifo(     0x11eeee11 );
	CHECKX(            0x11eeee11, Tx.read_Fifo() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "read_ClkDiv()" );
    try {
	Tx.write_ClkDiv(   0x22dddd22 );
	CHECKX(            0x22dddd22, Tx.read_ClkDiv() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "read_DmaLen()" );
    try {
	Tx.write_DmaLen(   0x44bbbb44 );
	CHECKX(            0x44bbbb44, Tx.read_DmaLen() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "read_Lossi()" );
    try {
	Tx.write_Lossi(    0x55aaaa55 );
	CHECKX(            0x55aaaa55, Tx.read_Lossi() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "read_DmaReq()" );
    try {
	Tx.write_DmaReq(   0x66999966 );
	CHECKX(            0x66999966, Tx.read_DmaReq() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "39", "check final register pattern" );
    try {
	CHECKX(            0x33cccc33, Tx.read_CntlStat() );
	CHECKX(            0x11eeee11, Tx.read_Fifo()     );
	CHECKX(            0x22dddd22, Tx.read_ClkDiv()   );
	CHECKX(            0x44bbbb44, Tx.read_DmaLen()   );
	CHECKX(            0x55aaaa55, Tx.read_Lossi()    );
	CHECKX(            0x66999966, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State registers
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "50a", "init_put_reset() 0xffffffff" );
    try {
	Tx.write_CntlStat( 0xffffffff );
	Tx.write_Fifo(     0xffffffff );
	Tx.write_ClkDiv(   0xffffffff );
	Tx.write_DmaLen(   0xffffffff );
	Tx.write_Lossi(    0xffffffff );
	Tx.write_DmaReq(   0xffffffff );
	//
	Tx.put_CntlStat(   0xffffffff );
	Tx.put_ClkDiv(     0xffffffff );
	Tx.put_DmaLen(     0xffffffff );
	Tx.put_Lossi(      0xffffffff );
	Tx.put_DmaReq(     0xffffffff );
	Tx.init_put_reset();
	CHECKX(            0xffffffff, Tx.read_CntlStat() );
	CHECKX(            0xffffffff, Tx.read_Fifo()     );
	CHECKX(            0xffffffff, Tx.read_ClkDiv()   );
	CHECKX(            0xffffffff, Tx.read_DmaLen()   );
	CHECKX(            0xffffffff, Tx.read_Lossi()    );
	CHECKX(            0xffffffff, Tx.read_DmaReq()   );
	//
	CHECKX(            0x00041000, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "init_put_reset() 0x00000000" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_Fifo(     0x00000000 );
	Tx.write_ClkDiv(   0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	Tx.write_Lossi(    0x00000000 );
	Tx.write_DmaReq(   0x00000000 );
	//
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClkDiv(     0x00000000 );
	Tx.put_DmaLen(     0x00000000 );
	Tx.put_Lossi(      0x00000000 );
	Tx.put_DmaReq(     0x00000000 );
	Tx.init_put_reset();
	CHECKX(            0x00000000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_Fifo()     );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000000, Tx.read_Lossi()    );
	CHECKX(            0x00000000, Tx.read_DmaReq()   );
	//
	CHECKX(            0x00041000, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "grab_regs()" );
    try {
	Tx.write_CntlStat( 0x11111111 );
	Tx.write_Fifo(     0x22222222 );
	Tx.write_ClkDiv(   0x33333333 );
	Tx.write_DmaLen(   0x44444444 );
	Tx.write_Lossi(    0x55555555 );
	Tx.write_DmaReq(   0x66666666 );
	//
	Tx.put_CntlStat(   0xeeeeeeee );
	Tx.put_ClkDiv(     0xcccccccc );
	Tx.put_DmaLen(     0xbbbbbbbb );
	Tx.put_Lossi(      0xaaaaaaaa );
	Tx.put_DmaReq(     0x99999999 );
	Tx.grab_regs();
	CHECKX(            0x11111111, Tx.read_CntlStat() );
	CHECKX(            0x22222222, Tx.read_Fifo()     );
	CHECKX(            0x33333333, Tx.read_ClkDiv()   );
	CHECKX(            0x44444444, Tx.read_DmaLen()   );
	CHECKX(            0x55555555, Tx.read_Lossi()    );
	CHECKX(            0x66666666, Tx.read_DmaReq()   );
	//
	CHECKX(            0x11111111, Tx.get_CntlStat() );
	CHECKX(            0x33333333, Tx.get_ClkDiv()   );
	CHECKX(            0x44444444, Tx.get_DmaLen()   );
	CHECKX(            0x55555555, Tx.get_Lossi()    );
	CHECKX(            0x66666666, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "push_regs()" );
    try {
	Tx.write_CntlStat( 0x11111111 );
	Tx.write_Fifo(     0x22222222 );
	Tx.write_ClkDiv(   0x33333333 );
	Tx.write_DmaLen(   0x44444444 );
	Tx.write_Lossi(    0x55555555 );
	Tx.write_DmaReq(   0x66666666 );
	//
	Tx.put_CntlStat(   0xeeeeeeee );
	Tx.put_ClkDiv(     0xcccccccc );
	Tx.put_DmaLen(     0xbbbbbbbb );
	Tx.put_Lossi(      0xaaaaaaaa );
	Tx.put_DmaReq(     0x99999999 );
	Tx.push_regs();
	CHECKX(            0xeeeeeeee, Tx.read_CntlStat() );
	CHECKX(            0x22222222, Tx.read_Fifo()     );
	CHECKX(            0xcccccccc, Tx.read_ClkDiv()   );
	CHECKX(            0xbbbbbbbb, Tx.read_DmaLen()   );
	CHECKX(            0xaaaaaaaa, Tx.read_Lossi()    );
	CHECKX(            0x99999999, Tx.read_DmaReq()   );
	//
	CHECKX(            0xeeeeeeee, Tx.get_CntlStat() );
	CHECKX(            0xcccccccc, Tx.get_ClkDiv()   );
	CHECKX(            0xbbbbbbbb, Tx.get_DmaLen()   );
	CHECKX(            0xaaaaaaaa, Tx.get_Lossi()    );
	CHECKX(            0x99999999, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "54a", "grab_CntlStat()" );
    try {
	Tx.write_CntlStat( 0x11111111 );
	Tx.write_Fifo(     0x22222222 );
	Tx.write_ClkDiv(   0x33333333 );
	Tx.write_DmaLen(   0x44444444 );
	Tx.write_Lossi(    0x55555555 );
	Tx.write_DmaReq(   0x66666666 );
	//
	Tx.put_CntlStat(   0xeeeeeeee );
	Tx.put_ClkDiv(     0x00000000 );
	Tx.put_DmaLen(     0x00000000 );
	Tx.put_Lossi(      0x00000000 );
	Tx.put_DmaReq(     0x00000000 );
	Tx.grab_CntlStat();
	CHECKX(            0x11111111, Tx.read_CntlStat() );
	CHECKX(            0x22222222, Tx.read_Fifo()     );
	CHECKX(            0x33333333, Tx.read_ClkDiv()   );
	CHECKX(            0x44444444, Tx.read_DmaLen()   );
	CHECKX(            0x55555555, Tx.read_Lossi()    );
	CHECKX(            0x66666666, Tx.read_DmaReq()   );
	//
	CHECKX(            0x11111111, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000000, Tx.get_Lossi()    );
	CHECKX(            0x00000000, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

//#!! Incomplete!

//--------------------------------------
  CASE( "60", "clear object regs" );	// to avoid any wayward 1s
    try {
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClkDiv(     0x00000000 );
	Tx.put_DmaLen(     0x00000000 );
	Tx.put_Lossi(      0x00000000 );
	Tx.put_DmaReq(     0x00000000 );
	CHECKX(            0x00000000, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000000, Tx.get_Lossi()    );
	CHECKX(            0x00000000, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

/***
//--------------------------------------
  CASE( "99a", "get_Z()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_Z(     0x1 );
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_Z() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "99c", "put_Z() bad value" );
    try {
	Tx.put_Z(     0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
***/

//--------------------------------------
  CASE( "61a", "get_LossiWord_1()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_LossiWord_1(     0x1 );
	CHECKX(          0x02000000, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_LossiWord_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "put_LossiWord_1() bad value" );
    try {
	Tx.put_LossiWord_1(     0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "64a", "get_RxFullStop_1() read only" );
    try {
	Tx.put_CntlStat( 0x00100000 );
	CHECKX(          0x00100000, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_RxFullStop_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_RxFullStop_1() read only" );
    try {
	Tx.put_CntlStat( 0xffefffff );
	CHECKX(          0xffefffff, Tx.get_CntlStat() );
	CHECKX(                 0x0, Tx.get_RxFullStop_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "get_RxHalf_1() read only" );
    try {
	Tx.put_CntlStat( 0x00080000 );
	CHECKX(          0x00080000, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_RxHalf_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "get_RxHalf_1() read only" );
    try {
	Tx.put_CntlStat( 0xfff7ffff );
	CHECKX(          0xfff7ffff, Tx.get_CntlStat() );
	CHECKX(                 0x0, Tx.get_RxHalf_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "75a", "get_RunActive_1()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_RunActive_1(     0x1 );
	CHECKX(          0x00000080, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_RunActive_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75c", "put_RunActive_1() bad value" );
    try {
	Tx.put_RunActive_1(     0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "76a", "get_CsPolarity_1()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_CsPolarity_1(    0x1 );
	CHECKX(          0x00000040, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_CsPolarity_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76c", "put_CsPolarity_1() bad value" );
    try {
	Tx.put_CsPolarity_1(    0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "77a", "get_ClearRxTxFifo_2()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_ClearRxTxFifo_2( 0x3 );
	CHECKX(          0x00000030, Tx.get_CntlStat() );
	CHECKX(                 0x3, Tx.get_ClearRxTxFifo_2() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "77c", "put_ClearRxTxFifo_2() bad value" );
    try {
	Tx.put_ClearRxTxFifo_2( 0x4 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x3:  0x4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "78a", "get_ClockPolarity_1()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_ClockPolarity_1( 0x1 );
	CHECKX(          0x00000008, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_ClockPolarity_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "78c", "put_ClockPolarity_1() bad value" );
    try {
	Tx.put_ClockPolarity_1( 0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "79a", "get_ClockPhase_1()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_ClockPhase_1(    0x1 );
	CHECKX(          0x00000004, Tx.get_CntlStat() );
	CHECKX(                 0x1, Tx.get_ClockPhase_1() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "79c", "put_ClockPhase_1() bad value" );
    try {
	Tx.put_ClockPhase_1(    0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "80a", "get_ChipSelectN_2()" );
    try {
	CHECKX(          0x00000000, Tx.get_CntlStat() );
	Tx.put_ChipSelectN_2(   0x3 );
	CHECKX(          0x00000003, Tx.get_CntlStat() );
	CHECKX(                 0x3, Tx.get_ChipSelectN_2() );
	Tx.put_CntlStat( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "80c", "put_ChipSelectN_2() bad value" );
    try {
	Tx.put_ChipSelectN_2(   0x4 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0x3:  0x4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "82a", "get_ClockDiv_16()" );
    try {
	CHECKX(          0x00000000, Tx.get_ClkDiv() );
	Tx.put_ClockDiv_16(  0xffff );
	CHECKX(          0x0000ffff, Tx.get_ClkDiv() );
	CHECKX(              0xffff, Tx.get_ClockDiv_16() );
	Tx.put_ClkDiv(   0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82c", "put_ClockDiv_16() bad value" );
    try {
	Tx.put_ClockDiv_16( 0x10000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0xffff:  0x10000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "92a", "get_DmaRxReqLev_8()" );
    try {
	CHECKX(          0x00000000, Tx.get_DmaReq() );
	Tx.put_DmaRxReqLev_8(  0xff );
	CHECKX(          0x00ff0000, Tx.get_DmaReq() );
	CHECKX(                0xff, Tx.get_DmaRxReqLev_8() );
	Tx.put_DmaReq(   0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

// Is complement needed?
  CASE( "92b", "get_DmaRxReqLev_8()" );
    try {
	Tx.put_DmaReq(   0xffffffff );
	CHECKX(          0xffffffff, Tx.get_DmaReq() );
	Tx.put_DmaRxReqLev_8(  0x00 );
	CHECKX(          0xff00ffff, Tx.get_DmaReq() );
	CHECKX(                0x00, Tx.get_DmaRxReqLev_8() );
	Tx.put_DmaReq(   0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92c", "put_DmaRxReqLev_8() bad value" );
    try {
	Tx.put_DmaRxReqLev_8( 0x100 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0::put_field():  value exceeds 0xff:  0x100",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "98", "verify object regs all 0" );
    try {
	CHECKX(            0x00000000, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000000, Tx.get_Lossi()    );
	CHECKX(            0x00000000, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

