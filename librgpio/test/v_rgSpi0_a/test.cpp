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
	Tx.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00041030, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "push_regs() reset regs, clear Fifo" );
    try {
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11a", "write_CntlStat( 0x00000000 )" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "CntlStat grab_regs() confirm bits" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	CHECK(                    0x1, Tx.get_TxHasSpace_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Blasting all 1s may not be the best, as it enables DMA.
  CASE( "12a", "write_CntlStat( 0xffffffff ) a bit heavy" );
    try {
	Tx.write_CntlStat( 0xffffffff );
	CHECKX(            0x00e5ff4f, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "CntlStat bits after write_CntlStat( 0xffffffff )" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00e5ff4f, Tx.get_CntlStat() );
	CHECK(                      0, Tx.get_LossiWord_1()     );
	CHECK(                      0, Tx.get_LossiDmaEn_1()    );
	CHECK(                      7, Tx.get_CsPolarity_3()    );
	CHECK(                      0, Tx.get_RxFullStop_1()    );
	CHECK(                      0, Tx.get_RxHalf_1()        );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      1, Tx.get_TxEmpty_1()       );
	CHECK(                      1, Tx.get_LossiEnable_1()   );
	CHECK(                      1, Tx.get_ReadEnable_1()    );
	CHECK(                      1, Tx.get_DmaEndCs_1()      );
	CHECK(                      1, Tx.get_IrqRxHalf_1()     );
	CHECK(                      1, Tx.get_IrqTxEmpty_1()    );
	CHECK(                      1, Tx.get_DmaEnable_1()     );
	CHECK(                      0, Tx.get_RunActive_1()     );  // why?
	CHECK(                      1, Tx.get_CsPolarity_1()    );
	CHECK(                      0, Tx.get_ClearRxTxFifo_2() );
	CHECK(                      1, Tx.get_ClockPolarity_1() );
	CHECK(                      1, Tx.get_ClockPhase_1()    );
	CHECK(                      3, Tx.get_ChipSelectN_2()   );
	// Note RunActive_1 is not set, maybe from DmaEnable_1=1.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12c", "write_CntlStat( 0x00000000 ) restore to zero" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "13a", "setup write_CntlStat( 0x00000000 )" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13b", "put_RunActive_1  CntlStat[7]" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	Tx.put_RunActive_1(         1 );
	CHECKX(            0x00040080, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x00050080, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13c", "verify bits after set RunActive_1=1" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00050080, Tx.get_CntlStat() );
	CHECK(                      0, Tx.get_LossiWord_1()     );
	CHECK(                      0, Tx.get_LossiDmaEn_1()    );
	CHECK(                      0, Tx.get_CsPolarity_3()    );
	CHECK(                      0, Tx.get_RxFullStop_1()    );
	CHECK(                      0, Tx.get_RxHalf_1()        );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      1, Tx.get_TxEmpty_1()       );
	CHECK(                      0, Tx.get_LossiEnable_1()   );
	CHECK(                      0, Tx.get_ReadEnable_1()    );
	CHECK(                      0, Tx.get_DmaEndCs_1()      );
	CHECK(                      0, Tx.get_IrqRxHalf_1()     );
	CHECK(                      0, Tx.get_IrqTxEmpty_1()    );
	CHECK(                      0, Tx.get_DmaEnable_1()     );
	CHECK(                      1, Tx.get_RunActive_1()     );
	CHECK(                      0, Tx.get_CsPolarity_1()    );
	CHECK(                      0, Tx.get_ClearRxTxFifo_2() );
	CHECK(                      0, Tx.get_ClockPolarity_1() );
	CHECK(                      0, Tx.get_ClockPhase_1()    );
	CHECK(                      0, Tx.get_ChipSelectN_2()   );
	// Note TxEmpty_1 is active only when RunActive_1=1.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "14a", "setup write_CntlStat( 0x00000000 )" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14b", "set CntlStat[7:0] = 01001111" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	Tx.put_RunActive_1(         0 );	// avoid side effects
	Tx.put_CsPolarity_1(        1 );
	Tx.put_ClearRxTxFifo_2(     0 );	// not tested here
	Tx.put_ClockPolarity_1(     1 );
	Tx.put_ClockPhase_1(        1 );
	Tx.put_ChipSelectN_2(       3 );
	CHECKX(            0x0004004f, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x0004004f, Tx.read_CntlStat() );
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
	Tx.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00041030, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "write_ClkDiv( 0xffffffff )" );
    try {
	Tx.write_ClkDiv(   0xffffffff );
	CHECKX(            0x0000ffff, Tx.read_ClkDiv() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "write_ClkDiv( 0x00000000 )" );
    try {
	Tx.write_ClkDiv(   0x00000000 );
	CHECKX(            0x00000000, Tx.read_ClkDiv() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "write_DmaLen( 0xffffffff )" );
    try {
	Tx.write_DmaLen(   0xffffffff );
	CHECKX(            0x0000ffff, Tx.read_DmaLen() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "write_DmaLen( 0x00000000 )" );
    try {
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00000000, Tx.read_DmaLen() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "write_Lossi( 0xffffffff )" );
    try {
	Tx.write_Lossi(    0xffffffff );
	CHECKX(            0x0000000f, Tx.read_Lossi() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "write_Lossi( 0x00000000 )" );
    try {
	Tx.write_Lossi(    0x00000000 );
	CHECKX(            0x00000000, Tx.read_Lossi() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "write_DmaReq( 0xffffffff )" );
    try {
	Tx.write_DmaReq(   0xffffffff );
	CHECKX(            0xffffffff, Tx.read_DmaReq() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "write_DmaReq( 0x00000000 )" );
    try {
	Tx.write_DmaReq(   0x00000000 );
	CHECKX(            0x00000000, Tx.read_DmaReq() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "29", "check register final state" );
    try {
	CHECKX(            0x00041000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000000, Tx.read_Lossi()    );
	CHECKX(            0x00000000, Tx.read_DmaReq()   );
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
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00000030, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31", "set LossiWord_1=1, no effect" );
    try {
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_LossiWord_1(         1 );
	CHECK(                      1, Tx.get_LossiWord_1()     );
	CHECKX(            0x02000000, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	Tx.grab_regs();
	CHECK(                      0, Tx.get_LossiWord_1()     );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32", "set LossiWord_1=1, no effect, with LossiEnable_1=1" );
    try {
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_LossiEnable_1(       1 );
	Tx.put_LossiWord_1(         1 );
	CHECK(                      1, Tx.get_LossiEnable_1()   );
	CHECK(                      1, Tx.get_LossiWord_1()     );
	CHECKX(            0x02002000, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x00042000, Tx.read_CntlStat() );
	Tx.grab_regs();
	CHECK(                      1, Tx.get_LossiEnable_1()   );
	CHECK(                      0, Tx.get_LossiWord_1()     );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33", "set LossiDmaEn_1=1, no effect" );
    try {
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_LossiDmaEn_1(        1 );
	CHECK(                      1, Tx.get_LossiDmaEn_1()    );
	CHECKX(            0x01000000, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	Tx.grab_regs();
	CHECK(                      0, Tx.get_LossiDmaEn_1()    );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "34", "set LossiDmaEn_1=1, no effect, with LossiEnable_1=1" );
    try {
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_LossiEnable_1(       1 );
	Tx.put_LossiDmaEn_1(        1 );
	CHECK(                      1, Tx.get_LossiEnable_1()   );
	CHECK(                      1, Tx.get_LossiDmaEn_1()    );
	CHECKX(            0x01002000, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x00042000, Tx.read_CntlStat() );
	Tx.grab_regs();
	CHECK(                      1, Tx.get_LossiEnable_1()   );
	CHECK(                      0, Tx.get_LossiDmaEn_1()    );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Explore Tx Fifo write with RunActive_1=0
//--------------------------------------------------------------------------
// DmaLen[15:0]    = write_Fifo[31:16]  ==> DmaDataLen_16
// CntlStat[23:21] = write_Fifo[10:8]   ==> CsPolarity_3
// CntlStat[7:0]   = write_Fifo[7:0]    ==> { RunActive_1, CsPolarity_1,
//	ClearRxTxFifo_2, ClockPolarity_1, ClockPhase_1, ChipSelectN_2 }

  CASE( "40", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00000030, Tx.get_CntlStat() );
	CHECKX(            0x00000000, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41", "write Tx Fifo=0xffffffff" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	Tx.write_Fifo(     0xffffffff );
	CHECKX(            0x00e500cf, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );	// no change
	CHECKX(            0x0000ffff, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );	// no change
	CHECKX(            0x30201020, Tx.read_DmaReq()   );	// no change
	Tx.grab_regs();
	CHECKX(            0x00e500cf, Tx.get_CntlStat() );
	CHECK(                      0, Tx.get_LossiWord_1()     );
	CHECK(                      0, Tx.get_LossiDmaEn_1()    );
	CHECK(                      7, Tx.get_CsPolarity_3()    );
	CHECK(                      0, Tx.get_RxFullStop_1()    );
	CHECK(                      0, Tx.get_RxHalf_1()        );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      1, Tx.get_TxEmpty_1()       );
	CHECK(                      0, Tx.get_LossiEnable_1()   );
	CHECK(                      0, Tx.get_ReadEnable_1()    );
	CHECK(                      0, Tx.get_DmaEndCs_1()      );
	CHECK(                      0, Tx.get_IrqRxHalf_1()     );
	CHECK(                      0, Tx.get_IrqTxEmpty_1()    );
	CHECK(                      0, Tx.get_DmaEnable_1()     );
	CHECK(                      1, Tx.get_RunActive_1()     );
	CHECK(                      1, Tx.get_CsPolarity_1()    );
	CHECK(                      0, Tx.get_ClearRxTxFifo_2() );
	CHECK(                      1, Tx.get_ClockPolarity_1() );
	CHECK(                      1, Tx.get_ClockPhase_1()    );
	CHECK(                      3, Tx.get_ChipSelectN_2()   );
	// CntlStat[23:21,7:0] = write_Fifo[10:8,7:0]
	// DmaLen[15:0]        = write_Fifo[31:16]
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "write Tx Fifo=0xaaaa0001" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xaaaa0001 );
	CHECKX(            0x00040001, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_ClkDiv()   );	// no change
	CHECKX(            0x0000aaaa, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );	// no change
	CHECKX(            0x30201020, Tx.read_DmaReq()   );	// no change
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "write Tx Fifo=0xbbbb0002" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xbbbb0002 );
	CHECKX(            0x00040002, Tx.read_CntlStat() );
	CHECKX(            0x0000bbbb, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "write Tx Fifo=0x56780004, set ClockPhase_1=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0x56780004 );
	CHECKX(            0x00040004, Tx.read_CntlStat() );
	CHECKX(            0x00005678, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "write Tx Fifo=0xabcd0008, set ClockPolarity_1=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xabcd0008 );
	CHECKX(            0x00040008, Tx.read_CntlStat() );
	CHECKX(            0x0000abcd, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "write Tx Fifo=0xbeef0030, set ClearRxTxFifo_2=3 read 0" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xbeef0030 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x0000beef, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "47", "write Tx Fifo=0xebcd0040, set CsPolarity_1=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xebcd0040 );
	CHECKX(            0x00040040, Tx.read_CntlStat() );
	CHECKX(            0x0000ebcd, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "48", "write Tx Fifo=0xfade0080, set RunActive_1=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xfade0080 );
	CHECKX(            0x00050080, Tx.read_CntlStat() );	// TxEmpty_1=1
	CHECKX(            0x0000fade, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "49", "write Tx Fifo=0xabba0100, set CsPolarity_3=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xabba0100 );
	CHECKX(            0x00240000, Tx.read_CntlStat() );
	CHECKX(            0x0000abba, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50", "write Tx Fifo=0xabba0200, set CsPolarity_3=2" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xabba0200 );
	CHECKX(            0x00440000, Tx.read_CntlStat() );
	CHECKX(            0x0000abba, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51", "write Tx Fifo=0xabba0400, set CsPolarity_3=4" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xabba0400 );
	CHECKX(            0x00840000, Tx.read_CntlStat() );
	CHECKX(            0x0000abba, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "write Tx Fifo=0xdeadf800, unused bits=0x0000f800" );
    try {
	Tx.write_CntlStat( 0x00000000 );
	Tx.write_DmaLen(   0x00000000 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	Tx.write_Fifo(     0xdeadf800 );
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x0000dead, Tx.read_DmaLen()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
  CASE( "99", "Done" );
}

