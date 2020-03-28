// 2018-01-01  William A. Hudson
//
// Testing:  rgUniSpi  Universal SPI Master class.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers
//    30-39  Access Enable and IRQ bits
//    40-49  Direct register access  read_(), write_()
//    50-59  Object State registers  init_put_reset(), grab_regs(), push_regs()
//    60-69  Object Field Accessors  #!! incomplete
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

  CASE( "20a", "AuxIrq.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.AuxIrq.addr();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
	CHECK(  1,                Tx.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "AuxEn.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.AuxEn.addr();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
	CHECK(  1,                Tx.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "Cntl0.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.Cntl0.addr();
	CHECKX( 0x00000080, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "Cntl0.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.Cntl0.addr();
	CHECKX( 0x000000c0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "Cntl1.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.Cntl1.addr();
	CHECKX( 0x00000084, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "Cntl1.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.Cntl1.addr();
	CHECKX( 0x000000c4, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "Stat.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.Stat.addr();
	CHECKX( 0x00000088, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "Stat.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.Stat.addr();
	CHECKX( 0x000000c8, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "Peek.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.Peek.addr();
	CHECKX( 0x0000008c, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "Peek.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.Peek.addr();
	CHECKX( 0x000000cc, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "26a", "Fifo.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.Fifo.addr();
	CHECKX( 0x000000a0, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26b", "Fifo.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.Fifo.addr();
	CHECKX( 0x000000e0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "27a", "FifoH.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.FifoH.addr();
	CHECKX( 0x000000b0, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27b", "FifoH.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.FifoH.addr();
	CHECKX( 0x000000f0, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Access Enable and IRQ bits
//--------------------------------------------------------------------------

  CASE( "30a", "AuxIrq.read() Spi1" );
    try {
	uint32_t		vv;
	Tx1.AuxIrq.write( 0x00000002 );
	CHECKX(           0x00000002, Tx1.AuxIrq.read() );
	vv = Tx1.read_SpiIrq_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "30b", "AuxIrq.read() Spi1" );
    try {
	uint32_t		vv;
	Tx1.AuxIrq.write( 0xfffffffd );
	CHECKX(           0xfffffffd, Tx1.AuxIrq.read() );
	vv = Tx1.read_SpiIrq_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "AuxIrq.read() Spi2" );
    try {
	uint32_t		vv;
	Tx2.AuxIrq.write( 0x00000004 );
	CHECKX(           0x00000004, Tx2.AuxIrq.read() );
	vv = Tx2.read_SpiIrq_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "AuxIrq.read() Spi2" );
    try {
	uint32_t		vv;
	Tx2.AuxIrq.write( 0xfffffffb );
	CHECKX(           0xfffffffb, Tx2.AuxIrq.read() );
	vv = Tx2.read_SpiIrq_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "read_SpiEnable_1() Spi1" );
    try {
	uint32_t		vv;
	Tx1.AuxEn.write(     0x00000000 );
	CHECKX(              0x00000000, Tx1.AuxEn.read() );
	Tx1.write_SpiEnable_1( 1 );
	CHECKX(              0x00000002, Tx1.AuxEn.read() );
	vv = Tx1.read_SpiEnable_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "read_SpiEnable_1() Spi1" );
    try {
	uint32_t		vv;
	Tx1.AuxEn.write(     0xffffffff );
	CHECKX(              0xffffffff, Tx1.AuxEn.read() );
	Tx1.write_SpiEnable_1( 0 );
	CHECKX(              0xfffffffd, Tx1.AuxEn.read() );
	vv = Tx1.read_SpiEnable_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "read_SpiEnable_1() Spi2" );
    try {
	uint32_t		vv;
	Tx2.AuxEn.write(     0x00000000 );
	CHECKX(              0x00000000, Tx2.AuxEn.read() );
	Tx2.write_SpiEnable_1( 1 );
	CHECKX(              0x00000004, Tx2.AuxEn.read() );
	vv = Tx2.read_SpiEnable_1();
	CHECK(  1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33b", "read_SpiEnable_1() Spi2" );
    try {
	uint32_t		vv;
	Tx2.AuxEn.write(     0xffffffff );
	CHECKX(              0xffffffff, Tx2.AuxEn.read() );
	Tx2.write_SpiEnable_1( 0 );
	CHECKX(              0xfffffffb, Tx2.AuxEn.read() );
	vv = Tx2.read_SpiEnable_1();
	CHECK(  0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access
//--------------------------------------------------------------------------
// Test read/write to fake memory.

//--------------------------------------
  CASE( "40a", "Cntl0.read()" );
    try {
	Tx.Cntl0.write( 0x00000000 );
	CHECKX(         0x00000000, Tx.Cntl0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "40b", "Cntl0.read()" );
    try {
	Tx.Cntl0.write( 0xffffffff );
	CHECKX(         0xffffffff, Tx.Cntl0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "Cntl1.read()" );
    try {
	Tx.Cntl1.write( 0x00000000 );
	CHECKX(         0x00000000, Tx.Cntl1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "Cntl1.read()" );
    try {
	Tx.Cntl1.write( 0xffffffff );
	CHECKX(         0xffffffff, Tx.Cntl1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Note Stat is a read-only register.
  CASE( "42a", "Stat.read()" );
    try {
	Tx.Stat.write(  0x00000000 );
	CHECKX(         0x00000000, Tx.Stat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "Stat.read()" );
    try {
	Tx.Stat.write(  0xffffffff );
	CHECKX(         0xffffffff, Tx.Stat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "Fifo.read()" );
    try {
	Tx.Fifo.write(  0x00000000 );
	CHECKX(         0x00000000, Tx.Fifo.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43b", "Fifo.read()" );
    try {
	Tx.Fifo.write(  0xffffffff );
	CHECKX(         0xffffffff, Tx.Fifo.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "44a", "FifoH.read()" );
    try {
	Tx.FifoH.write( 0x00000000 );
	CHECKX(         0x00000000, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44b", "FifoH.read()" );
    try {
	Tx.FifoH.write( 0xffffffff );
	CHECKX(         0xffffffff, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Object State registers
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "50a", "init_put_reset()" );
    try {
	Tx.Cntl0.write( 0xfff1ffff );
	Tx.Cntl1.write( 0xffffffff );
	Tx.Stat.write(  0xffffffff );
	Tx.Cntl0.put(   0xffffffff );
	Tx.Cntl1.put(   0xffffffff );
	Tx.Stat.put(    0xffffffff );
	CHECKX(         0xfff1ffff, Tx.Cntl0.read() );
	CHECKX(         0xffffffff, Tx.Cntl1.read() );
	CHECKX(         0xffffffff, Tx.Stat.read() );
	CHECKX(         0xffffffff, Tx.Cntl0.get() );
	CHECKX(         0xffffffff, Tx.Cntl1.get() );
	CHECKX(         0xffffffff, Tx.Stat.get()  );
	Tx.init_put_reset();
	CHECKX(         0x000e0000, Tx.Cntl0.get() );
	CHECKX(         0x00000000, Tx.Cntl1.get() );
	CHECKX(         0x00000280, Tx.Stat.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "grab_regs()" );
    try {
	Tx.Cntl0.write( 0x55555555 );
	Tx.Cntl1.write( 0x33333333 );
	Tx.Stat.write(  0x66666666 );
	Tx.Fifo.write(  0x00000000 );
	Tx.FifoH.write( 0x00000000 );
	Tx.Cntl0.put(   0xaaaaaaaa );
	Tx.Cntl1.put(   0xcccccccc );
	Tx.Stat.put(    0x99999999 );
	CHECKX(         0xaaaaaaaa, Tx.Cntl0.get() );
	CHECKX(         0xcccccccc, Tx.Cntl1.get() );
	CHECKX(         0x99999999, Tx.Stat.get()  );
	Tx.grab_regs();
	CHECKX(         0x55555555, Tx.Cntl0.get() );
	CHECKX(         0x33333333, Tx.Cntl1.get() );
	CHECKX(         0x66666666, Tx.Stat.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "54a", "push_regs()" );
    try {
	Tx.Cntl0.put(   0xaaaaaaaa );
	Tx.Cntl1.put(   0xcccccccc );
	Tx.Stat.put(    0x99999999 );
	Tx.Cntl0.write( 0x55555555 );
	Tx.Cntl1.write( 0x33333333 );
	Tx.Stat.write(  0x66666666 );
	Tx.Fifo.write(  0x00000000 );
	Tx.FifoH.write( 0x00000000 );
	CHECKX(         0x55555555, Tx.Cntl0.read() );
	CHECKX(         0x33333333, Tx.Cntl1.read() );
	CHECKX(         0x66666666, Tx.Stat.read()  );
	Tx.push_regs();
	CHECKX(         0xaaaaaaaa, Tx.Cntl0.read() );
	CHECKX(         0xcccccccc, Tx.Cntl1.read() );
	CHECKX(         0x66666666, Tx.Stat.read()  );	// read-only
	CHECKX(         0x00000000, Tx.Fifo.read()  );
	CHECKX(         0x00000000, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Object Field Accessors
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "60a", "get_Speed_12()" );
    try {
	Tx.Cntl0.put(  0x00000000 );
	CHECKX(        0x00000000, Tx.Cntl0.get() );
	Tx.Cntl0.put_Speed_12( 0xfff );
	CHECKX(        0xfff00000, Tx.Cntl0.get() );
	CHECK(         0xfff,      Tx.Cntl0.get_Speed_12() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60b", "get_Speed_12()" );
    try {
	Tx.Cntl0.put(  0xffffffff );
	CHECKX(        0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_Speed_12( 0x000 );
	CHECKX(        0x000fffff, Tx.Cntl0.get() );
	CHECK(         0x000,      Tx.Cntl0.get_Speed_12() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60c", "put_Speed_12() bad value" );
    try {
	Tx.Cntl0.put_Speed_12( 0x1000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xfff:  0x1000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "66a", "get_EnableSerial_1()" );
    try {
	Tx.Cntl0.put(  0x00000000 );
	CHECKX(        0x00000000, Tx.Cntl0.get() );
	Tx.Cntl0.put_EnableSerial_1( 0x1 );
	CHECKX(        0x00000800, Tx.Cntl0.get() );
	CHECK(         0x1,        Tx.Cntl0.get_EnableSerial_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "get_EnableSerial_1()" );
    try {
	Tx.Cntl0.put(  0xffffffff );
	CHECKX(        0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_EnableSerial_1( 0x0 );
	CHECKX(        0xfffff7ff, Tx.Cntl0.get() );
	CHECK(         0x0,        Tx.Cntl0.get_EnableSerial_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66c", "put_EnableSerial_1() bad value" );
    try {
	Tx.Cntl0.put_EnableSerial_1( 0x2 );
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


  CASE( "99", "Done" );
}

