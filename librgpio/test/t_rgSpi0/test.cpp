// 2019-10-21  William A. Hudson
//
// Testing:  rgSpi0  SPI0 Master class.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers  addr()
//    30-39  Direct register access  read(), write()
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
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "CntlStat.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.CntlStat.addr();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "Fifo.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Fifo.addr();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "ClkDiv.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.ClkDiv.addr();
	CHECKX( 0x00000008, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "DmaLen.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.DmaLen.addr();
	CHECKX( 0x0000000c, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "Lossi.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Lossi.addr();
	CHECKX( 0x00000010, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26", "DmaReq.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.DmaReq.addr();
	CHECKX( 0x00000014, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition write(), read()" );
    try {
	Tx.CntlStat.write( 0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.ClkDiv.write(   0xffffffff );
	Tx.DmaLen.write(   0xffffffff );
	Tx.Lossi.write(    0xffffffff );
	Tx.DmaReq.write(   0xffffffff );
	CHECKX(            0xffffffff, Tx.CntlStat.read() );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.read()   );
	CHECKX(            0xffffffff, Tx.DmaLen.read()   );
	CHECKX(            0xffffffff, Tx.Lossi.read()    );
	CHECKX(            0xffffffff, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "CntlStat.read()" );
    try {
	Tx.CntlStat.write( 0x33cccc33 );
	CHECKX(            0x33cccc33, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Fifo.read()" );
    try {
	Tx.Fifo.write(     0x11eeee11 );
	CHECKX(            0x11eeee11, Tx.Fifo.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "ClkDiv.read()" );
    try {
	Tx.ClkDiv.write(   0x22dddd22 );
	CHECKX(            0x22dddd22, Tx.ClkDiv.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "DmaLen.read()" );
    try {
	Tx.DmaLen.write(   0x44bbbb44 );
	CHECKX(            0x44bbbb44, Tx.DmaLen.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "Lossi.read()" );
    try {
	Tx.Lossi.write(    0x55aaaa55 );
	CHECKX(            0x55aaaa55, Tx.Lossi.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "DmaReq.read()" );
    try {
	Tx.DmaReq.write(   0x66999966 );
	CHECKX(            0x66999966, Tx.DmaReq.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "39", "check final register pattern" );
    try {
	CHECKX(            0x33cccc33, Tx.CntlStat.read() );
	CHECKX(            0x11eeee11, Tx.Fifo.read()     );
	CHECKX(            0x22dddd22, Tx.ClkDiv.read()   );
	CHECKX(            0x44bbbb44, Tx.DmaLen.read()   );
	CHECKX(            0x55aaaa55, Tx.Lossi.read()    );
	CHECKX(            0x66999966, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State  init_put_reset(), grab_regs(), push_regs()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "50a", "init_put_reset() 0xffffffff" );
    try {
	Tx.CntlStat.write( 0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.ClkDiv.write(   0xffffffff );
	Tx.DmaLen.write(   0xffffffff );
	Tx.Lossi.write(    0xffffffff );
	Tx.DmaReq.write(   0xffffffff );
	//
	Tx.CntlStat.put(   0xffffffff );
	Tx.Fifo.put(       0xffffffff );
	Tx.ClkDiv.put(     0xffffffff );
	Tx.DmaLen.put(     0xffffffff );
	Tx.Lossi.put(      0xffffffff );
	Tx.DmaReq.put(     0xffffffff );
	Tx.init_put_reset();
	CHECKX(            0xffffffff, Tx.CntlStat.read() );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.read()   );
	CHECKX(            0xffffffff, Tx.DmaLen.read()   );
	CHECKX(            0xffffffff, Tx.Lossi.read()    );
	CHECKX(            0xffffffff, Tx.DmaReq.read()   );
	//
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "init_put_reset() 0x00000000" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.Fifo.write(     0x00000000 );
	Tx.ClkDiv.write(   0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	Tx.Lossi.write(    0x00000000 );
	Tx.DmaReq.write(   0x00000000 );
	//
	Tx.CntlStat.put(   0x00000000 );
	Tx.Fifo.put(       0x00000000 );
	Tx.ClkDiv.put(     0x00000000 );
	Tx.DmaLen.put(     0x00000000 );
	Tx.Lossi.put(      0x00000000 );
	Tx.DmaReq.put(     0x00000000 );
	Tx.init_put_reset();
	CHECKX(            0x00000000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.Fifo.read()     );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000000, Tx.Lossi.read()    );
	CHECKX(            0x00000000, Tx.DmaReq.read()   );
	//
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "grab_regs(), not Fifo" );
    try {
	Tx.CntlStat.write( 0x11111111 );
	Tx.Fifo.write(     0x22222222 );
	Tx.ClkDiv.write(   0x33333333 );
	Tx.DmaLen.write(   0x44444444 );
	Tx.Lossi.write(    0x55555555 );
	Tx.DmaReq.write(   0x66666666 );
	//
	Tx.CntlStat.put(   0xeeeeeeee );
	Tx.Fifo.put(       0xdddddddd );
	Tx.ClkDiv.put(     0xcccccccc );
	Tx.DmaLen.put(     0xbbbbbbbb );
	Tx.Lossi.put(      0xaaaaaaaa );
	Tx.DmaReq.put(     0x99999999 );
	Tx.grab_regs();
	CHECKX(            0x11111111, Tx.CntlStat.read() );
	CHECKX(            0x22222222, Tx.Fifo.read()     );
	CHECKX(            0x33333333, Tx.ClkDiv.read()   );
	CHECKX(            0x44444444, Tx.DmaLen.read()   );
	CHECKX(            0x55555555, Tx.Lossi.read()    );
	CHECKX(            0x66666666, Tx.DmaReq.read()   );
	//
	CHECKX(            0x11111111, Tx.CntlStat.get() );
	CHECKX(            0xdddddddd, Tx.Fifo.get()     );	// not Fifo
	CHECKX(            0x33333333, Tx.ClkDiv.get()   );
	CHECKX(            0x44444444, Tx.DmaLen.get()   );
	CHECKX(            0x55555555, Tx.Lossi.get()    );
	CHECKX(            0x66666666, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "push_regs(), not Fifo" );
    try {
	Tx.CntlStat.write( 0x11111111 );
	Tx.Fifo.write(     0x22222222 );
	Tx.ClkDiv.write(   0x33333333 );
	Tx.DmaLen.write(   0x44444444 );
	Tx.Lossi.write(    0x55555555 );
	Tx.DmaReq.write(   0x66666666 );
	//
	Tx.CntlStat.put(   0xeeeeeeee );
	Tx.Fifo.put(       0xdddddddd );
	Tx.ClkDiv.put(     0xcccccccc );
	Tx.DmaLen.put(     0xbbbbbbbb );
	Tx.Lossi.put(      0xaaaaaaaa );
	Tx.DmaReq.put(     0x99999999 );
	Tx.push_regs();
	CHECKX(            0xeeeeeeee, Tx.CntlStat.read() );
	CHECKX(            0x22222222, Tx.Fifo.read()     );	// not Fifo
	CHECKX(            0xcccccccc, Tx.ClkDiv.read()   );
	CHECKX(            0xbbbbbbbb, Tx.DmaLen.read()   );
	CHECKX(            0xaaaaaaaa, Tx.Lossi.read()    );
	CHECKX(            0x99999999, Tx.DmaReq.read()   );
	//
	CHECKX(            0xeeeeeeee, Tx.CntlStat.get() );
	CHECKX(            0xdddddddd, Tx.Fifo.get()     );
	CHECKX(            0xcccccccc, Tx.ClkDiv.get()   );
	CHECKX(            0xbbbbbbbb, Tx.DmaLen.get()   );
	CHECKX(            0xaaaaaaaa, Tx.Lossi.get()    );
	CHECKX(            0x99999999, Tx.DmaReq.get()   );
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
	Tx.CntlStat.put(   0x00000000 );
	Tx.Fifo.put(       0x00000000 );
	Tx.ClkDiv.put(     0x00000000 );
	Tx.DmaLen.put(     0x00000000 );
	Tx.Lossi.put(      0x00000000 );
	Tx.DmaReq.put(     0x00000000 );
	CHECKX(            0x00000000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000000, Tx.Lossi.get()    );
	CHECKX(            0x00000000, Tx.DmaReq.get()   );
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
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
***/

//--------------------------------------
  CASE( "61a", "CntlStat.get_LossiWord_1()" );
    try {
	CHECKX(             0x00000000, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiWord_1( 1 );
	CHECKX(             0x02000000, Tx.CntlStat.get() );
	CHECKX(                      1, Tx.CntlStat.get_LossiWord_1() );
	Tx.CntlStat.put(    0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "CntlStat.put_LossiWord_1() bad value" );
    try {
	Tx.CntlStat.put_LossiWord_1( 2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "64a", "CntlStat.get_RxFullStop_1() read only" );
    try {
	Tx.CntlStat.put( 0x00100000 );
	CHECKX(          0x00100000, Tx.CntlStat.get() );
	CHECKX(                 0x1, Tx.CntlStat.get_RxFullStop_1() );
	Tx.CntlStat.put( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "CntlStat.get_RxFullStop_1() read only" );
    try {
	Tx.CntlStat.put( 0xffefffff );
	CHECKX(          0xffefffff, Tx.CntlStat.get() );
	CHECKX(                 0x0, Tx.CntlStat.get_RxFullStop_1() );
	Tx.CntlStat.put( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "CntlStat.get_RxHalf_1() read only" );
    try {
	Tx.CntlStat.put( 0x00080000 );
	CHECKX(          0x00080000, Tx.CntlStat.get() );
	CHECKX(                 0x1, Tx.CntlStat.get_RxHalf_1() );
	Tx.CntlStat.put( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "CntlStat.get_RxHalf_1() read only" );
    try {
	Tx.CntlStat.put( 0xfff7ffff );
	CHECKX(          0xfff7ffff, Tx.CntlStat.get() );
	CHECKX(                 0x0, Tx.CntlStat.get_RxHalf_1() );
	Tx.CntlStat.put( 0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

/*****************

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
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
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
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
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
	CHECK( "rgRegister::put_field():  value exceeds 0x3:  0x4",
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
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
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
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
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
	CHECK( "rgRegister::put_field():  value exceeds 0x3:  0x4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

**************/

//--------------------------------------
  CASE( "82a", "ClkDiv.get_ClockDiv_16()" );
    try {
	CHECKX(                0x00000000, Tx.ClkDiv.get() );
	Tx.ClkDiv.put_ClockDiv_16( 0xffff );
	CHECKX(                0x0000ffff, Tx.ClkDiv.get() );
	CHECKX(                    0xffff, Tx.ClkDiv.get_ClockDiv_16() );
	Tx.ClkDiv.put(         0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82c", "ClkDiv.put_ClockDiv_16() bad value" );
    try {
	Tx.ClkDiv.put_ClockDiv_16( 0x10000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xffff:  0x10000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "92a", "DmaReq.get_DmaRxReqLev_8()" );
    try {
	CHECKX(                0x00000000, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaRxReqLev_8( 0xff );
	CHECKX(                0x00ff0000, Tx.DmaReq.get() );
	CHECKX(                      0xff, Tx.DmaReq.get_DmaRxReqLev_8() );
	Tx.DmaReq.put(         0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

// Is complement needed?
  CASE( "92b", "DmaReq.get_DmaRxReqLev_8()" );
    try {
	Tx.DmaReq.put(         0xffffffff );
	CHECKX(                0xffffffff, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaRxReqLev_8( 0x00 );
	CHECKX(                0xff00ffff, Tx.DmaReq.get() );
	CHECKX(                      0x00, Tx.DmaReq.get_DmaRxReqLev_8() );
	Tx.DmaReq.put(         0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92c", "DmaReq.put_DmaRxReqLev_8() bad value" );
    try {
	Tx.DmaReq.put_DmaRxReqLev_8( 0x100 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xff:  0x100",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "98", "verify object regs all 0" );
    try {
	CHECKX(            0x00000000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000000, Tx.Lossi.get()    );
	CHECKX(            0x00000000, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

