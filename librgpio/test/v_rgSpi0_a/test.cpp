// 2019-11-02  William A. Hudson
//
// Testing:  Live rgSpi0  SPI0 register/field function on RPi
//    10-19  Explore CntlStat register
//    20-29  Explore field width:  ClkDiv, DmaLen, Lossi, DmaReq
//    30-39  Explore LossiWord_1, LossiDmaEn_1 - both non-functional
//    40-59  Explore Tx Fifo write with RunActive_1=0
//    60-98  .

// Provide external configuration:  none needed.
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
	Bx.open_dev_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgSpi0			Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Explore CntlStat register
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "10a", "init_put_reset(), put_ClearRxTxFifo_2()" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00041030, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "push_regs() reset regs, clear Fifo" );
    try {
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11a", "CntlStat.write( 0x00000000 )" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "CntlStat grab_regs() confirm bits" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	CHECK(                    0x1, Tx.CntlStat.get_TxHasSpace_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Blasting all 1s may not be the best, as it enables DMA.
  CASE( "12a", "CntlStat.write( 0xffffffff ) a bit heavy" );
    try {
	Tx.CntlStat.write( 0xffffffff );
	CHECKX(            0x00e5ff4f, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "CntlStat bits after CntlStat.write( 0xffffffff )" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00e5ff4f, Tx.CntlStat.get() );
	CHECK(                      0, Tx.CntlStat.get_LossiWord_1()     );
	CHECK(                      0, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECK(                      7, Tx.CntlStat.get_CsPolarity_3()    );
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      1, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      1, Tx.CntlStat.get_ReadEnable_1()    );
	CHECK(                      1, Tx.CntlStat.get_DmaEndCs_1()      );
	CHECK(                      1, Tx.CntlStat.get_IrqRxHalf_1()     );
	CHECK(                      1, Tx.CntlStat.get_IrqTxEmpty_1()    );
	CHECK(                      1, Tx.CntlStat.get_DmaEnable_1()     );
	CHECK(                      0, Tx.CntlStat.get_RunActive_1()     );  // why?
	CHECK(                      1, Tx.CntlStat.get_CsPolarity_1()    );
	CHECK(                      0, Tx.CntlStat.get_ClearRxTxFifo_2() );
	CHECK(                      1, Tx.CntlStat.get_ClockPolarity_1() );
	CHECK(                      1, Tx.CntlStat.get_ClockPhase_1()    );
	CHECK(                      3, Tx.CntlStat.get_ChipSelectN_2()   );
	// Note RunActive_1 is not set, maybe from DmaEnable_1=1.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12c", "CntlStat.write( 0x00000000 ) restore to zero" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "13a", "setup CntlStat.write( 0x00000000 )" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13b", "put_RunActive_1  CntlStat[7]" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1( 1 );
	CHECKX(            0x00040080, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00050080, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13c", "verify bits after set RunActive_1=1" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00050080, Tx.CntlStat.get() );
	CHECK(                      0, Tx.CntlStat.get_LossiWord_1()     );
	CHECK(                      0, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECK(                      0, Tx.CntlStat.get_CsPolarity_3()    );
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      0, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      0, Tx.CntlStat.get_ReadEnable_1()    );
	CHECK(                      0, Tx.CntlStat.get_DmaEndCs_1()      );
	CHECK(                      0, Tx.CntlStat.get_IrqRxHalf_1()     );
	CHECK(                      0, Tx.CntlStat.get_IrqTxEmpty_1()    );
	CHECK(                      0, Tx.CntlStat.get_DmaEnable_1()     );
	CHECK(                      1, Tx.CntlStat.get_RunActive_1()     );
	CHECK(                      0, Tx.CntlStat.get_CsPolarity_1()    );
	CHECK(                      0, Tx.CntlStat.get_ClearRxTxFifo_2() );
	CHECK(                      0, Tx.CntlStat.get_ClockPolarity_1() );
	CHECK(                      0, Tx.CntlStat.get_ClockPhase_1()    );
	CHECK(                      0, Tx.CntlStat.get_ChipSelectN_2()   );
	// Note TxEmpty_1 is active only when RunActive_1=1.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "14a", "setup CntlStat.write( 0x00000000 )" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14b", "set CntlStat[7:0] = 01001111" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(     0 );	// avoid side effects
	Tx.CntlStat.put_CsPolarity_1(    1 );
	Tx.CntlStat.put_ClearRxTxFifo_2( 0 );	// not tested here
	Tx.CntlStat.put_ClockPolarity_1( 1 );
	Tx.CntlStat.put_ClockPhase_1(    1 );
	Tx.CntlStat.put_ChipSelectN_2(   3 );
	CHECKX(            0x0004004f, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x0004004f, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Explore field width:  ClkDiv, DmaLen, Lossi, DmaReq
//--------------------------------------------------------------------------

  CASE( "20", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00041030, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "ClkDiv.write( 0xffffffff )" );
    try {
	Tx.ClkDiv.write(   0xffffffff );
	CHECKX(            0x0000ffff, Tx.ClkDiv.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "ClkDiv.write( 0x00000000 )" );
    try {
	Tx.ClkDiv.write(   0x00000000 );
	CHECKX(            0x00000000, Tx.ClkDiv.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "DmaLen.write( 0xffffffff )" );
    try {
	Tx.DmaLen.write(   0xffffffff );
	CHECKX(            0x0000ffff, Tx.DmaLen.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "DmaLen.write( 0x00000000 )" );
    try {
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00000000, Tx.DmaLen.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "Lossi.write( 0xffffffff )" );
    try {
	Tx.Lossi.write(    0xffffffff );
	CHECKX(            0x0000000f, Tx.Lossi.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "Lossi.write( 0x00000000 )" );
    try {
	Tx.Lossi.write(    0x00000000 );
	CHECKX(            0x00000000, Tx.Lossi.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "DmaReq.write( 0xffffffff )" );
    try {
	Tx.DmaReq.write(   0xffffffff );
	CHECKX(            0xffffffff, Tx.DmaReq.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "DmaReq.write( 0x00000000 )" );
    try {
	Tx.DmaReq.write(   0x00000000 );
	CHECKX(            0x00000000, Tx.DmaReq.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "29", "check register final state" );
    try {
	CHECKX(            0x00041000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000000, Tx.Lossi.read()    );
	CHECKX(            0x00000000, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Explore LossiWord_1, LossiDmaEn_1 - both non-functional
//--------------------------------------------------------------------------

  CASE( "30", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00000030, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31", "set LossiWord_1=1, no effect" );
    try {
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_LossiWord_1(         1 );
	CHECK(                      1, Tx.CntlStat.get_LossiWord_1()     );
	CHECKX(            0x02000000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	Tx.grab_regs();
	CHECK(                      0, Tx.CntlStat.get_LossiWord_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32", "set LossiWord_1=1, no effect, with LossiEnable_1=1" );
    try {
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_LossiEnable_1(       1 );
	Tx.CntlStat.put_LossiWord_1(         1 );
	CHECK(                      1, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      1, Tx.CntlStat.get_LossiWord_1()     );
	CHECKX(            0x02002000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00042000, Tx.CntlStat.read() );
	Tx.grab_regs();
	CHECK(                      1, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      0, Tx.CntlStat.get_LossiWord_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33", "set LossiDmaEn_1=1, no effect" );
    try {
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_LossiDmaEn_1(        1 );
	CHECK(                      1, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECKX(            0x01000000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	Tx.grab_regs();
	CHECK(                      0, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "34", "set LossiDmaEn_1=1, no effect, with LossiEnable_1=1" );
    try {
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_LossiEnable_1(       1 );
	Tx.CntlStat.put_LossiDmaEn_1(        1 );
	CHECK(                      1, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      1, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECKX(            0x01002000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00042000, Tx.CntlStat.read() );
	Tx.grab_regs();
	CHECK(                      1, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      0, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Explore Tx Fifo write with RunActive_1=0
//--------------------------------------------------------------------------
// DmaLen[15:0]    = Fifo.write[31:16]  ==> DmaDataLen_16
// CntlStat[23:21] = Fifo.write[10:8]   ==> CsPolarity_3
// CntlStat[7:0]   = Fifo.write[7:0]    ==> { RunActive_1, CsPolarity_1,
//	ClearRxTxFifo_2, ClockPolarity_1, ClockPhase_1, ChipSelectN_2 }

  CASE( "40", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00000030, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41", "write Tx Fifo=0xffffffff" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	Tx.Fifo.write(     0xffffffff );
	CHECKX(            0x00e500cf, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );	// no change
	CHECKX(            0x0000ffff, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );	// no change
	CHECKX(            0x30201020, Tx.DmaReq.read()   );	// no change
	Tx.grab_regs();
	CHECKX(            0x00e500cf, Tx.CntlStat.get() );
	CHECK(                      0, Tx.CntlStat.get_LossiWord_1()     );
	CHECK(                      0, Tx.CntlStat.get_LossiDmaEn_1()    );
	CHECK(                      7, Tx.CntlStat.get_CsPolarity_3()    );
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      0, Tx.CntlStat.get_LossiEnable_1()   );
	CHECK(                      0, Tx.CntlStat.get_ReadEnable_1()    );
	CHECK(                      0, Tx.CntlStat.get_DmaEndCs_1()      );
	CHECK(                      0, Tx.CntlStat.get_IrqRxHalf_1()     );
	CHECK(                      0, Tx.CntlStat.get_IrqTxEmpty_1()    );
	CHECK(                      0, Tx.CntlStat.get_DmaEnable_1()     );
	CHECK(                      1, Tx.CntlStat.get_RunActive_1()     );
	CHECK(                      1, Tx.CntlStat.get_CsPolarity_1()    );
	CHECK(                      0, Tx.CntlStat.get_ClearRxTxFifo_2() );
	CHECK(                      1, Tx.CntlStat.get_ClockPolarity_1() );
	CHECK(                      1, Tx.CntlStat.get_ClockPhase_1()    );
	CHECK(                      3, Tx.CntlStat.get_ChipSelectN_2()   );
	// CntlStat[23:21,7:0] = Fifo.write[10:8,7:0]
	// DmaLen[15:0]        = Fifo.write[31:16]
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "write Tx Fifo=0xaaaa0001" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xaaaa0001 );
	CHECKX(            0x00040001, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );	// no change
	CHECKX(            0x0000aaaa, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );	// no change
	CHECKX(            0x30201020, Tx.DmaReq.read()   );	// no change
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "write Tx Fifo=0xbbbb0002" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xbbbb0002 );
	CHECKX(            0x00040002, Tx.CntlStat.read() );
	CHECKX(            0x0000bbbb, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "write Tx Fifo=0x56780004, set ClockPhase_1=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0x56780004 );
	CHECKX(            0x00040004, Tx.CntlStat.read() );
	CHECKX(            0x00005678, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "write Tx Fifo=0xabcd0008, set ClockPolarity_1=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xabcd0008 );
	CHECKX(            0x00040008, Tx.CntlStat.read() );
	CHECKX(            0x0000abcd, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "write Tx Fifo=0xbeef0030, set ClearRxTxFifo_2=3 read 0" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xbeef0030 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x0000beef, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "47", "write Tx Fifo=0xebcd0040, set CsPolarity_1=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xebcd0040 );
	CHECKX(            0x00040040, Tx.CntlStat.read() );
	CHECKX(            0x0000ebcd, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "48", "write Tx Fifo=0xfade0080, set RunActive_1=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xfade0080 );
	CHECKX(            0x00050080, Tx.CntlStat.read() );	// TxEmpty_1=1
	CHECKX(            0x0000fade, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "49", "write Tx Fifo=0xabba0100, set CsPolarity_3=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xabba0100 );
	CHECKX(            0x00240000, Tx.CntlStat.read() );
	CHECKX(            0x0000abba, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50", "write Tx Fifo=0xabba0200, set CsPolarity_3=2" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xabba0200 );
	CHECKX(            0x00440000, Tx.CntlStat.read() );
	CHECKX(            0x0000abba, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51", "write Tx Fifo=0xabba0400, set CsPolarity_3=4" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xabba0400 );
	CHECKX(            0x00840000, Tx.CntlStat.read() );
	CHECKX(            0x0000abba, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "write Tx Fifo=0xdeadf800, unused bits=0x0000f800" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	Tx.Fifo.write(     0xdeadf800 );
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x0000dead, Tx.DmaLen.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
  CASE( "99", "Done" );
}

