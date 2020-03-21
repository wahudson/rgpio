// 2019-11-03  William A. Hudson
//
// Testing:  rgSpi0  SPI0 Fifo function in loop back.
//    10-19  .
//    20-29  .
//    30-39  .
//    50-59  .
//    60-98  .

// Provide external configuration:
//    % rgpio fsel --mode=Alt0  11 10 9 8 7
//    GPIO[11:7] == {SCLK, MOSI, MISO, CE0_n, CE1_n}
//    All GPIO pins unconnected.  Jumper MISO (pin 9) to MOSI (pin 10).
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
//## write_Fifo() with RunActive_1=0
//--------------------------------------------------------------------------
// Writing Fifo with RunActive_1=0 modifies part of CntlStat.

  CASE( "10", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClearRxTxFifo_2(     3 );	// one-shot, reads as 0
	Tx.put_ReadEnable_1(        0 );	// allow MOSI drive
	Tx.put_ClkDiv(     0x000000ff );	// make it not too fast
	CHECKX(            0x00000030, Tx.get_CntlStat() );
	CHECKX(            0x000000ff, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.read_CntlStat() );
	CHECKX(            0x000000ff, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Successful write/read fifo in loop back.
  CASE( "11a", "setup RunActive_1=1" );
    try {
	Tx.write_CntlStat( 0x00000000 );
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

  CASE( "11b", "verify bits after set RunActive_1=1" );
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

  CASE( "11c", "write_Fifo()" );
    try {
	Tx.write_Fifo(     0x00000088 );
	Tx.write_Fifo(     0x00000077 );
	Tx.write_Fifo(     0x00000099 );
	Tx.grab_regs();
	CHECKX(            0x00040080, Tx.get_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11d", "wait for Tx" );
    try {
	int		ii;
	for ( ii=0;  ii<=1000;  ii++ )
	{
	    Tx.grab_CntlStat();
	    if ( 1 == Tx.get_TxEmpty_1() ) { break; }
	}
	cout << "11d:  wait ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00070080, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.get_RxHasData_1()     );
	CHECK(                      1, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11e", "read_Fifo()" );
    try {
	CHECKX(            0x00000088, Tx.read_Fifo() );
	CHECKX(            0x00000077, Tx.read_Fifo() );
	CHECKX(            0x00000099, Tx.read_Fifo() );
	Tx.grab_regs();
	CHECKX(            0x00050080, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      1, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Measure Fifo Depth
//--------------------------------------------------------------------------

  CASE( "20", "setup registers" );
    try {
	Tx.init_put_reset();
	Tx.put_CntlStat(   0x00000000 );
	Tx.put_ClearRxTxFifo_2(     3 );	// one-shot, reads as 0
	Tx.put_ReadEnable_1(        0 );	// allow MOSI drive
	Tx.put_RunActive_1(         1 );
	Tx.put_ClkDiv(     0x000000ff );	// make it not too fast
	CHECKX(            0x000000b0, Tx.get_CntlStat() );
	CHECKX(            0x000000ff, Tx.get_ClkDiv()   );
	CHECKX(            0x00000000, Tx.get_DmaLen()   );
	CHECKX(            0x00000001, Tx.get_Lossi()    );
	CHECKX(            0x30201020, Tx.get_DmaReq()   );
	Tx.push_regs();
	CHECKX(            0x00050080, Tx.read_CntlStat() );
	CHECKX(            0x000000ff, Tx.read_ClkDiv()   );
	CHECKX(            0x00000000, Tx.read_DmaLen()   );
	CHECKX(            0x00000001, Tx.read_Lossi()    );
	CHECKX(            0x30201020, Tx.read_DmaReq()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21a", "write Tx until full" );
    try {
	uint32_t	ii;
	for ( ii=0;  ii<=1000;  ii++ )
	{
	    Tx.write_Fifo(     ii );
	    Tx.grab_CntlStat();
//	    if ( 1 == Tx.get_RxFullStop_1() ) { break; }
	    if ( 0 == Tx.get_TxHasSpace_1() ) { break; }
	}
	cout << "21a:  Tx full ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00060080, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.get_RxHasData_1()     );
	CHECK(                      0, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "wait for Rx full" );
    try {
	int		ii;
	for ( ii=0;  ii<=100000;  ii++ )
	{
	    Tx.grab_CntlStat();
	    if ( 1 == Tx.get_RxFullStop_1() ) { break; }
	}
	cout << "21b:  wait ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x001e0080, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_RxFullStop_1()    );
	CHECK(                      1, Tx.get_RxHalf_1()        );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.get_RxHasData_1()     );
	CHECK(                      0, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "stop Tx" );
    try {
	Tx.grab_regs();
	Tx.put_RunActive_1(         0 );
	Tx.put_ClearRxTxFifo_2(     1 );	// clear Tx
	CHECKX(            0x001e0010, Tx.get_CntlStat() );
	Tx.push_regs();
	CHECKX(            0x001e0010, Tx.get_CntlStat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21d", "read Rx until empty, 64 entries" );
    try {
	uint32_t	ii;
	uint32_t	rv;
	for ( ii=1;  ii<=1000;  ii++ )
	{
	    rv = Tx.read_Fifo();
//	    cout << "21d:  Rx[" << ii << "]= " << rv <<endl;	// see data
	    Tx.grab_CntlStat();
	    if ( 0 == Tx.get_RxHasData_1() ) { break; }
	}
	CHECK(                     64, ii );	// Rx is 64 entries
	CHECK(                     63, rv );	// last entry received
//	cout << "21d:  Rx empty ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      0, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21e", "check status" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.get_CntlStat() );
	CHECK(                      1, Tx.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.get_RxHasData_1()     );
	CHECK(                      0, Tx.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
  CASE( "99", "Done" );
}

