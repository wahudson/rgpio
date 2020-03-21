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
//    Jumper MISO (Gpio[9], pin 21) to MOSI (Gpio[10], pin 19).
//    All other GPIO pins unconnected.
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
//## Fifo.write() with RunActive_1=0
//--------------------------------------------------------------------------
// Writing Fifo with RunActive_1=0 modifies part of CntlStat.

  CASE( "10", "reset all registers" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_ClearRxTxFifo_2(     3 );	// one-shot, reads as 0
	Tx.CntlStat.put_ReadEnable_1(        0 );	// allow MOSI drive
	Tx.ClkDiv.put(     0x000000ff );	// make it not too fast
	CHECKX(            0x00000030, Tx.CntlStat.get() );
	CHECKX(            0x000000ff, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
	Tx.push_regs();
	CHECKX(            0x00040000, Tx.CntlStat.read() );
	CHECKX(            0x000000ff, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Successful write/read fifo in loop back.
  CASE( "11a", "setup RunActive_1=1" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(         1 );
	CHECKX(            0x00040080, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00050080, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "verify bits after set RunActive_1=1" );
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

  CASE( "11c", "Fifo.write()" );
    try {
	Tx.Fifo.write(     0x00000088 );
	Tx.Fifo.write(     0x00000077 );
	Tx.Fifo.write(     0x00000099 );
	Tx.grab_regs();
	CHECKX(            0x00040080, Tx.CntlStat.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11d", "wait for Tx" );
    try {
	int		ii;
	for ( ii=0;  ii<=1000;  ii++ )
	{
	    Tx.CntlStat.grab();
	    if ( 1 == Tx.CntlStat.get_TxEmpty_1() ) { break; }
	}
	cout << "11d:  wait ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00070080, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11e", "Fifo.read()" );
    try {
	CHECKX(            0x00000088, Tx.Fifo.read() );
	CHECKX(            0x00000077, Tx.Fifo.read() );
	CHECKX(            0x00000099, Tx.Fifo.read() );
	Tx.grab_regs();
	CHECKX(            0x00050080, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
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
	Tx.CntlStat.put(   0x00000000 );
	Tx.CntlStat.put_ClearRxTxFifo_2(     3 );	// one-shot, reads as 0
	Tx.CntlStat.put_ReadEnable_1(        0 );	// allow MOSI drive
	Tx.CntlStat.put_RunActive_1(         1 );
	Tx.ClkDiv.put(     0x000000ff );	// make it not too fast
	CHECKX(            0x000000b0, Tx.CntlStat.get() );
	CHECKX(            0x000000ff, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
	Tx.push_regs();
	CHECKX(            0x00050080, Tx.CntlStat.read() );
	CHECKX(            0x000000ff, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21a", "write Tx until full" );
    try {
	uint32_t	ii;
	for ( ii=0;  ii<=1000;  ii++ )
	{
	    Tx.Fifo.write(     ii );
	    Tx.CntlStat.grab();
//	    if ( 1 == Tx.CntlStat.get_RxFullStop_1() ) { break; }
	    if ( 0 == Tx.CntlStat.get_TxHasSpace_1() ) { break; }
	}
	cout << "21a:  Tx full ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00060080, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "wait for Rx full" );
    try {
	int		ii;
	for ( ii=0;  ii<=100000;  ii++ )
	{
	    Tx.CntlStat.grab();
	    if ( 1 == Tx.CntlStat.get_RxFullStop_1() ) { break; }
	}
	cout << "21b:  wait ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x001e0080, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      1, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      1, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "stop Tx" );
    try {
	Tx.grab_regs();
	Tx.CntlStat.put_RunActive_1(         0 );
	Tx.CntlStat.put_ClearRxTxFifo_2(     1 );	// clear Tx
	CHECKX(            0x001e0010, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x001e0010, Tx.CntlStat.get() );
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
	    rv = Tx.Fifo.read();
//	    cout << "21d:  Rx[" << ii << "]= " << rv <<endl;	// see data
	    Tx.CntlStat.grab();
	    if ( 0 == Tx.CntlStat.get_RxHasData_1() ) { break; }
	}
	CHECK(                     64, ii );	// Rx is 64 entries
	CHECK(                     63, rv );	// last entry received
//	cout << "21d:  Rx empty ii= " << ii <<endl;
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21e", "check status" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00040000, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
  CASE( "99", "Done" );
}

