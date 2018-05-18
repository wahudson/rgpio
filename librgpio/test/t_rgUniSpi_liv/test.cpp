// 2018-05-05  William A. Hudson
//
// Testing:  rgUniSpi  Universal SPI Master class.  Live.
//    10-19  Constructor, config
//    20-29  Data Sampling:  OutRising_1, InRising_1, InvertClk_1, PostInMode_1
//    30-39  MSB First:  OutMsbFirst_1, InMsbFirst_1
//    40-49  Data Length:  ShiftLength_6, VariableWidth_1
//    50-59  .
//    60-69  .
//
// Configure Uspi1 for loopback:
//    Wire MISO Gpio19 to MOSI Gpio20.
//    rgpio fsel --mode=Alt4  16 17 18 19 20 21
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgUniSpi.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;

  CASE( "00", "Address map object on /dev/mem" );
    try {
	Bx.open_dev_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgUniSpi		Tx   ( 1, &Bx );	// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10a", "constructor" );
    try {
	rgUniSpi	tx  ( 1, &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "constructor" );
    try {
	rgUniSpi	tx  ( 2, &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11a", "Access disabled" );
    try {
	Tx.write_Spi_Enable_1( 0 );
	CHECK( 0, Tx.read_Spi_Enable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "Access enabled" );
    try {
	Tx.write_Spi_Enable_1( 1 );
	CHECK( 1, Tx.read_Spi_Enable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12", "Clear fifo" );
    try {
	Tx.init_put_reset();
	Tx.put_ClearFifos_1( 1 );
	Tx.push_regs();
	Tx.put_ClearFifos_1( 0 );
	Tx.push_regs();
	CHECKX( 0x000e0000, Tx.read_Cntl0() );
	CHECKX( 0x00000000, Tx.read_Cntl1() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "14", "Config Spi1" );
    try {
	Tx.init_put_reset();
	Tx.put_Speed_12( 0xfff );	// slowest
	Tx.put_EnableSerial_1( 1 );
	Tx.put_ShiftLength_6( 8 );
	Tx.put_OutRising_1( 1 );	// change on edge 1	#!!
	Tx.put_InRising_1( 0 );		// sample on edge 2
	Tx.push_regs();
	CHECKX( 0xfffe0908, Tx.read_Cntl0() );
	CHECKX( 0x00000000, Tx.read_Cntl1() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Data Sampling:  OutRising_1, InRising_1, InvertClk_1, PostInMode_1
//--------------------------------------------------------------------------
// Note we only exercise safe timing of OutRising_1 and InRising_1.

/*
	int			i;
	for ( i=0;  i<200;  i++ )
	{
	    Tx.grab_Stat();
	    if ( ! Tx.get_Busy_1() )  break;
	}
	cout << "Busy loop:  " << i <<endl;
*/

  CASE( "20", "common config" );
    try {
	Tx.init_put_reset();
	Tx.put_Speed_12(     200 );
	Tx.put_EnableSerial_1( 1 );
	Tx.put_ChipSelects_3( 0x4 );
	Tx.put_ShiftLength_6( 16 );
	Tx.put_PostInMode_1(   0 );
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	CHECKX( 0x0c880c10, Tx.read_Cntl0() );
	CHECKX( 0x00000000, Tx.read_Cntl1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "good sample-change" );
    try {
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0x00005557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x55570000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "good sample-change" );
    try {
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0xffffaaa8 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xaaa80000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "missing first bit change-sample" );
    try {
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    1 );
	Tx.put_InRising_1(     0 );
	Tx.push_regs();
	Tx.write_Fifo( 0x00005557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x2aab0000, Tx.read_Fifo() );	// is (0x55570000 >> 1)
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "missing first bit change-sample" );
    try {
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    1 );
	Tx.put_InRising_1(     0 );
	Tx.push_regs();
	Tx.write_Fifo( 0xffffaaa8 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xd5540000, Tx.read_Fifo() );	// is (0xaaa80000 >> 1)
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Note:  loopback last data bit is bit16 of sent data.

//--------------------------------------
  CASE( "23a", "invert SCLK good sample-change" );
    try {
	Tx.put_InvertClk_1(    1 );
	Tx.put_OutRising_1(    1 );
	Tx.put_InRising_1(     0 );
	Tx.push_regs();
	Tx.write_Fifo( 0x00005557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x55570000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "invert SCLK good sample-change" );
    try {
	Tx.put_InvertClk_1(    1 );
	Tx.put_OutRising_1(    1 );
	Tx.put_InRising_1(     0 );
	Tx.push_regs();
	Tx.write_Fifo( 0xffffaaa8 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xaaa80000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "invert SCLK missing first bit change-sample" );
    try {
	Tx.put_InvertClk_1(    1 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0x00005557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x2aab0000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "invert SCLK missing first bit change-sample" );
    try {
	Tx.put_InvertClk_1(    1 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0xffffaaa8 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xd5540000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "post input, good sample-change" );
    try {
	Tx.put_PostInMode_1(   1 );
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0x00005557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x2aab0000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "post input, good sample-change" );
    try {
	Tx.put_PostInMode_1(   1 );
	Tx.put_InvertClk_1(    0 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	Tx.write_Fifo( 0xffffaaa8 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xd5540000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Note:  loopback last data bit is bit16 of sent data.


//--------------------------------------------------------------------------
//## MSB First:  OutMsbFirst_1, InMsbFirst_1
//--------------------------------------------------------------------------

  CASE( "30", "common config" );
    try {
	Tx.init_put_reset();
	Tx.put_Speed_12(     200 );
	Tx.put_EnableSerial_1( 1 );
	Tx.put_ChipSelects_3( 0x4 );
	Tx.put_ShiftLength_6( 16 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	CHECKX( 0x0c880c10, Tx.read_Cntl0() );
	CHECKX( 0x00000000, Tx.read_Cntl1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "Out Lsb >>, In Msb >>" );
    try {
	Tx.put_OutMsbFirst_1(  0 );
	Tx.put_InMsbFirst_1(   0 );
	Tx.push_regs();
	Tx.write_Fifo( 0x0000c557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xc5570000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "Out Msb <<, In Msb >>, reversing" );
    try {
	Tx.put_OutMsbFirst_1(  1 );
	Tx.put_InMsbFirst_1(   0 );
	Tx.push_regs();
	Tx.write_Fifo( 0xc5570000 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xeaa30000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "Out Lsb >>, In Lsb <<, reversing" );
    try {
	Tx.put_OutMsbFirst_1(  0 );
	Tx.put_InMsbFirst_1(   1 );
	Tx.push_regs();
	Tx.write_Fifo( 0x0000c557 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x0000eaa3, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "34a", "Out Msb <<, In Lsb <<" );
    try {
	Tx.put_OutMsbFirst_1(  1 );
	Tx.put_InMsbFirst_1(   1 );
	Tx.push_regs();
	Tx.write_Fifo( 0x35570000 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x00003557, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Data Length:  ShiftLength_6, VariableWidth_1
//--------------------------------------------------------------------------

  CASE( "40", "common config" );
    try {
	Tx.init_put_reset();
	Tx.put_Speed_12(     200 );
	Tx.put_EnableSerial_1( 1 );
	Tx.put_ChipSelects_3( 0x4 );
	Tx.put_ShiftLength_6(  8 );
	Tx.put_OutRising_1(    0 );
	Tx.put_InRising_1(     1 );
	Tx.push_regs();
	CHECKX( 0x0c880c08, Tx.read_Cntl0() );
	CHECKX( 0x00000000, Tx.read_Cntl1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "length" );
    try {
	Tx.put_ShiftLength_6(  8 );
	Tx.push_regs();
	Tx.write_Fifo( 0x0000ff37 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x37000000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "length" );
    try {
	Tx.put_ShiftLength_6(  3 );
	Tx.push_regs();
	Tx.write_Fifo( 0x000000ff );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xe0000000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41c", "length" );
    try {
	Tx.put_ShiftLength_6( 24 );
	Tx.push_regs();
	Tx.write_Fifo( 0x0176666f );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x76666f00, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41d", "length" );
    try {
	Tx.put_ShiftLength_6( 32 );
	Tx.push_regs();
	Tx.write_Fifo( 0xf666666f );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xf666666f, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41e", "length" );
    try {
	Tx.put_ShiftLength_6( 31 );
	Tx.push_regs();
	Tx.write_Fifo( 0xf6666667 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xecccccce, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "44", "variable length" );
    try {
	Tx.put_VariableWidth_1( 1 );
	Tx.put_ShiftLength_6(  3 );
	Tx.push_regs();
	CHECKX( 0x0c884c03, Tx.read_Cntl0() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44a", "variable length 8" );
    try {
	Tx.write_Fifo( 0x0800ff37 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x37000000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44b", "variable length 12" );
    try {
	Tx.write_Fifo( 0x0cfff337 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x33700000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44c", "variable length 24" );
    try {
	Tx.write_Fifo( 0x18ccccc7 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0xccccc700, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44d", "variable length 3" );
    try {
	Tx.write_Fifo( 0x03fffff3 );
	do {
	    Tx.grab_Stat();
	} while ( Tx.get_Busy_1() );
	CHECKX( 0x00100200, Tx.get_Stat()  );
	CHECK(           1, Tx.get_RxLevel_3() );
	CHECK(           1, Tx.get_TxEmpty_1() );
	CHECKX( 0x60000000, Tx.read_Fifo() );
	CHECKX( 0x00000280, Tx.read_Stat() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

