// 2018-01-01  William A. Hudson
//
// Testing:  rgUniSpi  Universal SPI Master class.
//    10-19  Constructor
//    20-29  Address of registers
//    30-39  Access Enable and IRQ bits
//    40-49  .
//    50-59  .
//    60-69  .
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

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgUniSpi		Tx   ( 1, &Bx );	// test object
rgUniSpi		Tx1  ( 1, &Bx );	// test object, Spi1
rgUniSpi		Tx2  ( 2, &Bx );	// test object, Spi2


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
  CASE( "11a", "constructor, bad spi number" );
    try {
	rgUniSpi	tx  ( 0, &Bx );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgUniSpi:  constructor requires spi number {1,2}:  0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "constructor, bad spi number" );
    try {
	rgUniSpi	tx  ( 3, &Bx );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgUniSpi:  constructor requires spi number {1,2}:  3",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "19", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e215000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers
//--------------------------------------------------------------------------

  CASE( "20a", "addr_AuxIrq()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_AuxIrq();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
	CHECK(  1,                Tx.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "addr_AuxEnable()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.addr_AuxEnable();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
	CHECK(  1,                Tx.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "addr_Cntl0() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_Cntl0();
	CHECKX( 0x00000080, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "addr_Cntl0() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_Cntl0();
	CHECKX( 0x000000c0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "addr_Cntl1() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_Cntl1();
	CHECKX( 0x00000084, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "addr_Cntl1() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_Cntl1();
	CHECKX( 0x000000c4, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "addr_Stat() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_Stat();
	CHECKX( 0x00000088, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "addr_Stat() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_Stat();
	CHECKX( 0x000000c8, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "addr_Peek() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_Peek();
	CHECKX( 0x0000008c, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "addr_Peek() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_Peek();
	CHECKX( 0x000000cc, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "26a", "addr_Fifo() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_Fifo();
	CHECKX( 0x000000a0, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26b", "addr_Fifo() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_Fifo();
	CHECKX( 0x000000e0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "27a", "addr_FifoH() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.addr_FifoH();
	CHECKX( 0x000000b0, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27b", "addr_FifoH() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.addr_FifoH();
	CHECKX( 0x000000f0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Access Enable and IRQ bits
//--------------------------------------------------------------------------

  CASE( "30a", "read_AuxIrq() Spi1" );
    try {
	uint32_t		vv;
	Tx1.write_AuxIrq( 0x00000002 );
	CHECKX(           0x00000002, Tx1.read_AuxIrq() );
	vv = Tx1.read_Spi_IRQ_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "30b", "read_AuxIrq() Spi1" );
    try {
	uint32_t		vv;
	Tx1.write_AuxIrq( 0xfffffffd );
	CHECKX(           0xfffffffd, Tx1.read_AuxIrq() );
	vv = Tx1.read_Spi_IRQ_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "31a", "read_AuxIrq() Spi2" );
    try {
	uint32_t		vv;
	Tx2.write_AuxIrq( 0x00000004 );
	CHECKX(           0x00000004, Tx2.read_AuxIrq() );
	vv = Tx2.read_Spi_IRQ_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "read_AuxIrq() Spi2" );
    try {
	uint32_t		vv;
	Tx2.write_AuxIrq( 0xfffffffb );
	CHECKX(           0xfffffffb, Tx2.read_AuxIrq() );
	vv = Tx2.read_Spi_IRQ_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "read_Spi_Enable_1() Spi1" );
    try {
	uint32_t		vv;
	Tx1.write_AuxEnable( 0x00000000 );
	CHECKX(              0x00000000, Tx1.read_AuxEnable() );
	Tx1.write_Spi_Enable_1( 1 );
	CHECKX(              0x00000002, Tx1.read_AuxEnable() );
	vv = Tx1.read_Spi_Enable_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "read_Spi_Enable_1() Spi1" );
    try {
	uint32_t		vv;
	Tx1.write_AuxEnable( 0xffffffff );
	CHECKX(              0xffffffff, Tx1.read_AuxEnable() );
	Tx1.write_Spi_Enable_1( 0 );
	CHECKX(              0xfffffffd, Tx1.read_AuxEnable() );
	vv = Tx1.read_Spi_Enable_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "read_Spi_Enable_1() Spi2" );
    try {
	uint32_t		vv;
	Tx2.write_AuxEnable( 0x00000000 );
	CHECKX(              0x00000000, Tx2.read_AuxEnable() );
	Tx2.write_Spi_Enable_1( 1 );
	CHECKX(              0x00000004, Tx2.read_AuxEnable() );
	vv = Tx2.read_Spi_Enable_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33b", "read_Spi_Enable_1() Spi2" );
    try {
	uint32_t		vv;
	Tx2.write_AuxEnable( 0xffffffff );
	CHECKX(              0xffffffff, Tx2.read_AuxEnable() );
	Tx2.write_Spi_Enable_1( 0 );
	CHECKX(              0xfffffffb, Tx2.read_AuxEnable() );
	vv = Tx2.read_Spi_Enable_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
//## Object Field Accessors
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "60a", "get_Speed_12()" );
    try {
	Tx.put_Cntl0(  0x00000000 );
	CHECKX(        0x00000000, Tx.get_Cntl0() );
	Tx.put_Speed_12( 0xfff );
	CHECKX(        0xfff00000, Tx.get_Cntl0() );
	CHECK(         0xfff,      Tx.get_Speed_12() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60b", "get_Speed_12()" );
    try {
	Tx.put_Cntl0(  0xffffffff );
	CHECKX(        0xffffffff, Tx.get_Cntl0() );
	Tx.put_Speed_12( 0x000 );
	CHECKX(        0x000fffff, Tx.get_Cntl0() );
	CHECK(         0x000,      Tx.get_Speed_12() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60c", "put_Speed_12() bad value" );
    try {
	Tx.put_Speed_12( 0x1000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgUniSpi::put_field():  value exceeds 0xfff:  0x1000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "66a", "get_EnableSerial_1()" );
    try {
	Tx.put_Cntl0(  0x00000000 );
	CHECKX(        0x00000000, Tx.get_Cntl0() );
	Tx.put_EnableSerial_1( 0x1 );
	CHECKX(        0x00000800, Tx.get_Cntl0() );
	CHECK(         0x1,        Tx.get_EnableSerial_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "get_EnableSerial_1()" );
    try {
	Tx.put_Cntl0(  0xffffffff );
	CHECKX(        0xffffffff, Tx.get_Cntl0() );
	Tx.put_EnableSerial_1( 0x0 );
	CHECKX(        0xfffff7ff, Tx.get_Cntl0() );
	CHECK(         0x0,        Tx.get_EnableSerial_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66c", "put_EnableSerial_1() bad value" );
    try {
	Tx.put_EnableSerial_1( 0x2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgUniSpi::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

