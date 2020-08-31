// 2020-07-11  William A. Hudson
//
// Testing:  Live rgIic I2C Master class.
//    10-19  Explore register width
//    20-29  Explore FIFO read/write
//    30-39  Explore FIFO size and Rx/Tx
//    40-49  .
//    50-59  .
//    60-89  .
//
// Provide external configuration:  Iic1 GPIO pins for open-drain operation.
//    % rgpio fsel --mode=Alt0  2 3
//    Gpio[2] = pin 3,  iic1_SDA
//    Gpio[3] = pin 5,  iic1_SCL
//      These pins should be unconnected.  They are bidirectional with
//      open-drain drivers, and must be active for the I2C receiver to get the
//      correct values.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgIic.h"

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

rgIic			Tx   ( 1, &Bx );	// test object  iic1

  CASE( "01", "verify using iic1" );
    try {
	CHECK( 1, Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Explore register width
//--------------------------------------------------------------------------

  CASE( "10", "full reset registers" );
    try {
	Tx.init_put_reset();
	Tx.Stat.put(          0xffffffff );	// Clear bits
	Tx.Cntl.put_ClearFifo_2(       3 );	// one-shot clear
	Tx.DatLen.put_DataLen_16(      0 );
	Tx.ClkDiv.put_ClkDiv_16(    2500 );	// 10 us clock period
	Tx.push_regs();
	Tx.Stat.push();
	Tx.grab_regs();
	CHECKX(          0x00000000, Tx.Cntl.get()   );
	CHECKX(          0x00000050, Tx.Stat.get()   );
	CHECKX(          0x00000000, Tx.DatLen.get() );
	CHECKX(          0x00000000, Tx.Addr.get()   );
	CHECK(                 2500, Tx.ClkDiv.get() );
	CHECKX(          0x00300030, Tx.Delay.get()  );
	CHECKX(          0x00000040, Tx.ClkStr.get() );
	//
	CHECKX(          0x00000000, Tx.Cntl.read()   );
	CHECKX(          0x00000050, Tx.Stat.read()   );
	CHECKX(          0x00000000, Tx.DatLen.read() );
	CHECKX(          0x00000000, Tx.Addr.read()   );
	CHECK(                 2500, Tx.ClkDiv.read() );
	CHECKX(          0x00300030, Tx.Delay.read()  );
	CHECKX(          0x00000040, Tx.ClkStr.read() );
	//
	CHECK(                    1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(                    1, Tx.Stat.get_TxHasSpace_1()  );
	//
	CHECK(                    0, Tx.DatLen.get_DataLen_16()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "DatLen width" );
    try {
	Tx.DatLen.write( 0xffffffff );
	CHECKX(          0x0000ffff, Tx.DatLen.read() );
	Tx.DatLen.write( 0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "Addr width" );
    try {
	Tx.Addr.write(   0xffffffff );
	CHECKX(          0x0000007f, Tx.Addr.read() );
	Tx.Addr.write(   0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "Fifo Width" );
    try {
	Tx.Fifo.write(   0xffffffff );
	CHECKX(          0x000000ff, Tx.Fifo.read() );
	Tx.Fifo.write(   0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "ClkDiv width" );
    try {
	Tx.ClkDiv.write( 0xffffffff );
	CHECKX(          0x0000ffff, Tx.ClkDiv.read() );
	Tx.ClkDiv.write( 0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "Delay width" );
    try {
	Tx.Delay.write(  0xffffffff );
	CHECKX(          0xffffffff, Tx.Delay.read() );
	Tx.Delay.write(  0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16", "ClkStr width" );
    try {
	Tx.ClkStr.write( 0xffffffff );
	CHECKX(          0x0000ffff, Tx.ClkStr.read() );
	Tx.ClkStr.write( 0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Explore FIFO read/write
//--------------------------------------------------------------------------

  CASE( "20", "full reset registers" );
    try {
	Tx.init_put_reset();
	Tx.Stat.put(          0xffffffff );	// Clear bits
	Tx.Cntl.put_IicEnable_1(       1 );
	Tx.Cntl.put_ClearFifo_2(       3 );	// one-shot clear
	Tx.DatLen.put_DataLen_16(      0 );
	Tx.ClkDiv.put_ClkDiv_16(    2500 );	// 10 us clock period
	Tx.push_regs();
	Tx.Stat.push();
	Tx.grab_regs();
	CHECKX(          0x00008000, Tx.Cntl.get()   );
	CHECKX(          0x00000050, Tx.Stat.get()   );
	CHECKX(          0x00000000, Tx.DatLen.get() );
	CHECKX(          0x00000000, Tx.Addr.get()   );
	CHECK(                 2500, Tx.ClkDiv.get() );
	CHECKX(          0x00300030, Tx.Delay.get()  );
	CHECKX(          0x00000040, Tx.ClkStr.get() );
	//
	CHECKX(          0x00008000, Tx.Cntl.read()   );
	CHECKX(          0x00000050, Tx.Stat.read()   );
	CHECKX(          0x00000000, Tx.DatLen.read() );
	CHECKX(          0x00000000, Tx.Addr.read()   );
	CHECK(                 2500, Tx.ClkDiv.read() );
	CHECKX(          0x00300030, Tx.Delay.read()  );
	CHECKX(          0x00000040, Tx.ClkStr.read() );
	//
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     0, Tx.Cntl.get_ReadPacket_1()  );
	//
	CHECK(     0, Tx.Stat.get_ClkTimeout_1()  );
	CHECK(     0, Tx.Stat.get_AckErr_1()      );
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TransDone_1()   );
	CHECK(     0, Tx.Stat.get_TransActive_1() );
	//
	CHECK(     0, Tx.DatLen.get_DataLen_16()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21", "Read empty Fifo - invalid" );
    try {
	// Fifo.read() returns 32-bit value of whatever register was read last.
	CHECKX(          0x00000050, Tx.Stat.read()   );
	CHECKX(          0x00000050, Tx.Fifo.read()   );	// invalid
	CHECKX(          0x00000000, Tx.Addr.read()   );
	CHECKX(          0x00000000, Tx.Fifo.read()   );	// invalid
	CHECKX(          0x00300030, Tx.Delay.read()  );
	CHECKX(          0x00300030, Tx.Fifo.read()   );	// invalid
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "Fifo write" );
    try {
	Tx.Fifo.write(   0x00000011 );
	Tx.Fifo.write(   0x00000022 );
	Tx.Fifo.write(   0x00000033 );
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "Fifo read" );	// flaky
    try {
	CHECKX(          0x00000011, Tx.Fifo.read() );
	CHECKX(          0x00000022, Tx.Fifo.read() );
	CHECKX(          0x00000033, Tx.Fifo.read() );
	CHECKX(          0x00000033, Tx.Fifo.read() );	// invalid
	CHECKX(          0x00000033, Tx.Fifo.read() );	// invalid
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22c", "Fifo read empty - invalid" );	// flaky
    try {
	// value from Stat.grab()
	CHECKX(          0x00000050, Tx.Fifo.read() );	// invalid
	CHECKX(          0x00000050, Tx.Fifo.read() );	// invalid
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "Fifo write" );
    try {
	Tx.Fifo.write(   0x00000077 );
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "Clear Fifo" );
    try {
	Tx.Cntl.get();
	Tx.Cntl.put_ClearFifo_2(       3 );	// one-shot clear
	Tx.Cntl.push();
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "Fifo depth 16 - write" );
    try {
	for ( int i=0;  i<=15;  i++ ) {
	    Tx.Fifo.write( 0x40 | i );
	}
	Tx.Stat.grab();
	CHECK(     1, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     0, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "Fifo depth 16 - read" );
    try {
	for ( int i=0;  i<=13;  i++ ) {
	    Tx.Fifo.read();
	}
	CHECKX(          0x0000004e, Tx.Fifo.read() );
	CHECKX(          0x0000004f, Tx.Fifo.read() );
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "Fifo depth - 17 write" );
    try {
	for ( int i=0;  i<=16;  i++ ) {		// entry i=16 not written
	    Tx.Fifo.write( i );
	}
	Tx.Stat.grab();
	CHECK(     1, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     0, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "Fifo depth - 17 read" );
    try {
	for ( int i=0;  i<=12;  i++ ) {
	    Tx.Fifo.read();
	}
	CHECKX(          0x0000000d, Tx.Fifo.read() );
	CHECKX(          0x0000000e, Tx.Fifo.read() );
	CHECKX(          0x0000000f, Tx.Fifo.read() );	// i=15
//	CHECKX(          0x0000000f, Tx.Fifo.read() );  // i=16 empty
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "26", "Fifo Status while write until full" );
    try {
	Tx.Stat.grab();				// is empty
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	Tx.Fifo.write(   0x00000080 );
	Tx.Stat.grab();
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	for ( int i=1;  i<=14;  i++ ) {
	    Tx.Fifo.write( 0x80 | i );
	    Tx.Stat.grab();
	    CHECK(     0, Tx.Stat.get_RxFull_1()      );
	    CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	    CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	    CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	    CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	    CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	}
	Tx.Fifo.write(   0x0000008f );
	Tx.Stat.grab();				// is full
	CHECK(     1, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     0, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Note:  both RxHalf_1==0 and TxHalf_1==0

//--------------------------------------
  CASE( "27", "See RxHalf_1=1 on a full FIFO" );
    try {
	Tx.Cntl.grab();
	Tx.Cntl.put_ReadPacket_1(      1 );
	Tx.Cntl.push();
	Tx.Cntl.grab();
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     1, Tx.Cntl.get_ReadPacket_1()  );
	Tx.Stat.grab();
	CHECK(     1, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     0, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     1, Tx.Stat.get_RxHalf_1()      );	// needs ReadPacket_1
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Explore FIFO size and Rx/Tx
//--------------------------------------------------------------------------
// Sending address only, no data.  No slave response.

  CASE( "30", "full reset registers" );
    try {
	Tx.init_put_reset();
	Tx.Stat.put(          0xffffffff );	// Clear bits
	Tx.Cntl.put_IicEnable_1(       1 );
	Tx.Cntl.put_ClearFifo_2(       3 );	// one-shot clear
	Tx.DatLen.put_DataLen_16(      0 );
	Tx.ClkDiv.put_ClkDiv_16(    2500 );	// 10 us clock period
	Tx.push_regs();
	Tx.Stat.push();
	Tx.grab_regs();
	CHECKX(          0x00008000, Tx.Cntl.get()   );
	CHECKX(          0x00000050, Tx.Stat.get()   );
	CHECKX(          0x00000000, Tx.DatLen.get() );
	CHECKX(          0x00000000, Tx.Addr.get()   );
	CHECK(                 2500, Tx.ClkDiv.get() );
	CHECKX(          0x00300030, Tx.Delay.get()  );
	CHECKX(          0x00000040, Tx.ClkStr.get() );
	//
	CHECKX(          0x00008000, Tx.Cntl.read()   );
	CHECKX(          0x00000050, Tx.Stat.read()   );
	CHECKX(          0x00000000, Tx.DatLen.read() );
	CHECKX(          0x00000000, Tx.Addr.read()   );
	CHECK(                 2500, Tx.ClkDiv.read() );
	CHECKX(          0x00300030, Tx.Delay.read()  );
	CHECKX(          0x00000040, Tx.ClkStr.read() );
	//
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     0, Tx.Cntl.get_ReadPacket_1()  );
	//
	CHECK(     0, Tx.Stat.get_ClkTimeout_1()  );
	CHECK(     0, Tx.Stat.get_AckErr_1()      );
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     1, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     0, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TransDone_1()   );
	CHECK(     0, Tx.Stat.get_TransActive_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "Tx - setup" );
    try {
	Tx.DatLen.put_DataLen_16(      1 );
	Tx.DatLen.push();
	Tx.Addr.put_SlaveAddr_7(    0x40 );
	Tx.Addr.push();
	Tx.Fifo.write(         0x11 );
	Tx.grab_regs();
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     0, Tx.Cntl.get_ReadPacket_1()  );
	//
	CHECK(     0, Tx.Stat.get_ClkTimeout_1()  );
	CHECK(     0, Tx.Stat.get_AckErr_1()      );
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TransDone_1()   );
	CHECK(     0, Tx.Stat.get_TransActive_1() );
	//
	CHECK(     1, Tx.DatLen.get_DataLen_16()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "Tx - start" );
    try {
	Tx.Cntl.put_StartTrans_1(      1 );
	Tx.Cntl.push();
	Tx.grab_regs();
	Tx.grab_regs();
	Tx.grab_regs();		// delay for TransActive_1=1
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     0, Tx.Cntl.get_ReadPacket_1()  );
	//
	CHECK(     0, Tx.Stat.get_ClkTimeout_1()  );
	CHECK(     0, Tx.Stat.get_AckErr_1()      );
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TransDone_1()   );
	CHECK(     1, Tx.Stat.get_TransActive_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31c", "Tx - wait for result" );
    try {
	Tx.Cntl.grab();
	CHECK(     1, Tx.Cntl.get_IicEnable_1()   );
	CHECK(     0, Tx.Cntl.get_ReadPacket_1()  );
	//
	int		ii;
	for ( ii=1;  ii<=1000;  ii++ ) {
	    Tx.Stat.grab();
	    if ( Tx.Stat.get_TransDone_1() == 1 ) { break; }
	}
	cout << "# wait for TransDone_1=1:  ii= " << ii <<endl;
//	CHECK(   999, ii );	// variable
	//
	CHECK(     0, Tx.Stat.get_ClkTimeout_1()  );
	CHECK(     1, Tx.Stat.get_AckErr_1()      );	// no slave response
	CHECK(     0, Tx.Stat.get_RxFull_1()      );
	CHECK(     0, Tx.Stat.get_TxEmpty_1()     );
	CHECK(     1, Tx.Stat.get_RxHasData_1()   );
	CHECK(     1, Tx.Stat.get_TxHasSpace_1()  );
	CHECK(     0, Tx.Stat.get_RxHalf_1()      );
	CHECK(     0, Tx.Stat.get_TxHalf_1()      );
	CHECK(     1, Tx.Stat.get_TransDone_1()   );
	CHECK(     0, Tx.Stat.get_TransActive_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

