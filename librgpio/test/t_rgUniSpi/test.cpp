// 2020-03-29  William A. Hudson
//
// Testing:  rgUniSpi  Universal SPI Master class.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers  addr()
//    30-39  Access Enable and IRQ bits  AuxIrq, AuxEn
//    40-49  Direct register access  read_(), write_()
//    50-59  Object State registers  init_put_reset(), grab_regs(), push_regs()
//    60-89  Object Field Accessors  get_(), put_() - complete
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgUniSpi.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2837 );	// RPi3

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	CHECKX( 0x7e000000, Bx.config_DocBase() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgUniSpi		Tx   ( &Bx, 1 );	// test object
rgUniSpi		Tx1  ( &Bx, 1 );	// test object, Spi1
rgUniSpi		Tx2  ( &Bx, 2 );	// test object, Spi2


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10a", "constructor" );
    try {
	rgUniSpi	tx  ( &Bx, 1 );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "constructor" );
    try {
	rgUniSpi	tx  ( &Bx, 2 );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11a", "constructor, bad spi number" );
    try {
	rgUniSpi	tx  ( &Bx, 0 );
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
	rgUniSpi	tx  ( &Bx, 3 );
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
  CASE( "12", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e215000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register classes, sample constructor initialization, only Spi1.

  CASE( "14a", "reg constructor rgUniSpi_AuxIrq" );
    try {
	rgUniSpi_AuxIrq	rx  ( Tx.AuxIrq );
	CHECKX( 0x00, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14b", "reg constructor rgUniSpi_AuxEn" );
    try {
	rgUniSpi_AuxEn	rx  ( Tx.AuxEn );
	CHECKX( 0x04, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14c", "reg constructor rgUniSpi_Cntl0" );
    try {
	rgUniSpi_Cntl0	rx  ( Tx.Cntl0 );
	CHECKX( 0x80, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14d", "reg constructor rgUniSpi_Cntl1" );
    try {
	rgUniSpi_Cntl1	rx  ( Tx.Cntl1 );
	CHECKX( 0x84, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14e", "reg constructor rgUniSpi_Stat" );
    try {
	rgUniSpi_Stat	rx  ( Tx.Stat );
	CHECKX( 0x88, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14f", "reg constructor rgUniSpi_Peek" );
    try {
	rgUniSpi_Peek	rx  ( Tx.Peek );
	CHECKX( 0x8c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14g", "reg constructor rgUniSpi_Fifo" );
    try {
	rgUniSpi_Fifo	rx  ( Tx.Fifo );
	CHECKX( 0xa0, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14h", "reg constructor rgUniSpi_FifoH" );
    try {
	rgUniSpi_FifoH	rx  ( Tx.FifoH );
	CHECKX( 0xb0, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "20a", "AuxIrq.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.AuxIrq.addr();
	CHECKX( 0x00000000, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "AuxIrq.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.AuxIrq.addr();
	CHECKX( 0x00000000, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "AuxEn.addr() Spi1" );
    try {
	volatile uint32_t*	vp;
	vp = Tx1.AuxEn.addr();
	CHECKX( 0x00000004, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_spi_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "AuxEn.addr() Spi2" );
    try {
	volatile uint32_t*	vp;
	vp = Tx2.AuxEn.addr();
	CHECKX( 0x00000004, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_spi_num() );
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
//## Access Enable and IRQ bits  AuxIrq, AuxEn
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
//## Direct register access  read(), write()
//--------------------------------------------------------------------------
// Test read/write to fake memory.

  CASE( "40", "condition write(), read()" );
    try {
	Tx.Cntl0.write( 0xffffffff );
	Tx.Cntl1.write( 0xffffffff );
	Tx.Stat.write(  0xffffffff );
	Tx.Peek.write(  0xffffffff );
	Tx.Fifo.write(  0xffffffff );
	Tx.FifoH.write( 0xffffffff );
	CHECKX(         0xffffffff, Tx.Cntl0.read() );
	CHECKX(         0xffffffff, Tx.Cntl1.read() );
	CHECKX(         0xffffffff, Tx.Stat.read()  );
	CHECKX(         0xffffffff, Tx.Peek.read()  );
	CHECKX(         0xffffffff, Tx.Fifo.read()  );
	CHECKX(         0xffffffff, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "Cntl0.read()" );
    try {
	Tx.Cntl0.write( 0x11eeee11 );
	CHECKX(         0x11eeee11, Tx.Cntl0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "Cntl1.read()" );
    try {
	Tx.Cntl1.write( 0x22dddd22 );
	CHECKX(         0x22dddd22, Tx.Cntl1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "Stat.read()" );		// Read-only
    try {
	Tx.Stat.write(  0x33cccc33 );
	CHECKX(         0x33cccc33, Tx.Stat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "Peek.read()" );		// Read-only
    try {
	Tx.Peek.write(  0x44bbbb44 );
	CHECKX(         0x44bbbb44, Tx.Peek.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "Fifo.read()" );
    try {
	Tx.Fifo.write(  0x55aaaa55 );
	CHECKX(         0x55aaaa55, Tx.Fifo.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "FifoH.read()" );
    try {
	Tx.FifoH.write( 0x66999966 );
	CHECKX(         0x66999966, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "49", "check final register pattern" );
    try {
	CHECKX(         0x11eeee11, Tx.Cntl0.read() );
	CHECKX(         0x22dddd22, Tx.Cntl1.read() );
	CHECKX(         0x33cccc33, Tx.Stat.read()  );
	CHECKX(         0x44bbbb44, Tx.Peek.read()  );
	CHECKX(         0x55aaaa55, Tx.Fifo.read()  );
	CHECKX(         0x66999966, Tx.FifoH.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State  init_put_reset(), grab_regs(), push_regs()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "51", "init_put_reset()" );
    try {
	Tx.AuxIrq.write( 0xffffffff );
	Tx.AuxEn.write(  0xffffffff );
	Tx.Cntl0.write(  0xffffffff );
	Tx.Cntl1.write(  0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.Peek.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.FifoH.write(  0xffffffff );
	//
	Tx.AuxIrq.put(   0xffffffff );
	Tx.AuxEn.put(    0xffffffff );
	Tx.Cntl0.put(    0xffffffff );
	Tx.Cntl1.put(    0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.Peek.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.FifoH.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0xffffffff, Tx.Cntl0.read()  );
	CHECKX(          0xffffffff, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	//
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	CHECKX(          0xffffffff, Tx.AuxEn.get()  );
	CHECKX(          0xffffffff, Tx.Cntl0.get()  );
	CHECKX(          0xffffffff, Tx.Cntl1.get()  );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.Peek.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.FifoH.get()  );
	Tx.init_put_reset();
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0xffffffff, Tx.Cntl0.read()  );
	CHECKX(          0xffffffff, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	//
	CHECKX(          0x00000000, Tx.AuxIrq.get() );
	CHECKX(          0x00000000, Tx.AuxEn.get()  );
	CHECKX(          0x000e0000, Tx.Cntl0.get()  );
	CHECKX(          0x00000000, Tx.Cntl1.get()  );
	CHECKX(          0x00000280, Tx.Stat.get()   );
	CHECKX(          0x00000000, Tx.Peek.get()   );
	CHECKX(          0x00000000, Tx.Fifo.get()   );
	CHECKX(          0x00000000, Tx.FifoH.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53", "grab_regs(), not Fifo" );
    try {
	Tx.AuxIrq.write( 0x11111111 );
	Tx.AuxEn.write(  0x22222222 );
	Tx.Cntl0.write(  0x33333333 );
	Tx.Cntl1.write(  0x44444444 );
	Tx.Stat.write(   0x55555555 );
	Tx.Peek.write(   0x66666666 );
	Tx.Fifo.write(   0x77777777 );
	Tx.FifoH.write(  0x88888888 );
	//
	Tx.AuxIrq.put(   0xffffffff );
	Tx.AuxEn.put(    0xffffffff );
	Tx.Cntl0.put(    0xffffffff );
	Tx.Cntl1.put(    0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.Peek.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.FifoH.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	CHECKX(          0xffffffff, Tx.AuxEn.get()  );
	CHECKX(          0xffffffff, Tx.Cntl0.get()  );
	CHECKX(          0xffffffff, Tx.Cntl1.get()  );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.Peek.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.FifoH.get()  );
	Tx.grab_regs();
	CHECKX(          0x11111111, Tx.AuxIrq.read() );
	CHECKX(          0x22222222, Tx.AuxEn.read()  );
	CHECKX(          0x33333333, Tx.Cntl0.read()  );
	CHECKX(          0x44444444, Tx.Cntl1.read()  );
	CHECKX(          0x55555555, Tx.Stat.read()   );
	CHECKX(          0x66666666, Tx.Peek.read()   );
	CHECKX(          0x77777777, Tx.Fifo.read()   );
	CHECKX(          0x88888888, Tx.FifoH.read()  );
	//
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	CHECKX(          0xffffffff, Tx.AuxEn.get()  );
	CHECKX(          0x33333333, Tx.Cntl0.get()  );
	CHECKX(          0x44444444, Tx.Cntl1.get()  );
	CHECKX(          0x55555555, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.Peek.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.FifoH.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "54", "push_regs(), not Fifo" );
    try {
	Tx.AuxIrq.write( 0xffffffff );
	Tx.AuxEn.write(  0xffffffff );
	Tx.Cntl0.write(  0xffffffff );
	Tx.Cntl1.write(  0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.Peek.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.FifoH.write(  0xffffffff );
	//
	Tx.AuxIrq.put(   0x11111111 );
	Tx.AuxEn.put(    0x22222222 );
	Tx.Cntl0.put(    0x33333333 );
	Tx.Cntl1.put(    0x44444444 );
	Tx.Stat.put(     0x55555555 );
	Tx.Peek.put(     0x66666666 );
	Tx.Fifo.put(     0x77777777 );
	Tx.FifoH.put(    0x88888888 );
	//
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0xffffffff, Tx.Cntl0.read()  );
	CHECKX(          0xffffffff, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	Tx.push_regs();
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0x33333333, Tx.Cntl0.read()  );
	CHECKX(          0x44444444, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	//
	CHECKX(          0x11111111, Tx.AuxIrq.get() );
	CHECKX(          0x22222222, Tx.AuxEn.get()  );
	CHECKX(          0x33333333, Tx.Cntl0.get()  );
	CHECKX(          0x44444444, Tx.Cntl1.get()  );
	CHECKX(          0x55555555, Tx.Stat.get()   );
	CHECKX(          0x66666666, Tx.Peek.get()   );
	CHECKX(          0x77777777, Tx.Fifo.get()   );
	CHECKX(          0x88888888, Tx.FifoH.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

  CASE( "60", "condition hardware/object registers" );	// avoid any wayward 0s
    try {
	Tx.AuxIrq.write( 0xffffffff );
	Tx.AuxEn.write(  0xffffffff );
	Tx.Cntl0.write(  0xffffffff );
	Tx.Cntl1.write(  0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.Peek.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.FifoH.write(  0xffffffff );
	//
	Tx.AuxIrq.put(   0xffffffff );
	Tx.AuxEn.put(    0xffffffff );
	Tx.Cntl0.put(    0xffffffff );
	Tx.Cntl1.put(    0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.Peek.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.FifoH.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0xffffffff, Tx.Cntl0.read()  );
	CHECKX(          0xffffffff, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	//
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	CHECKX(          0xffffffff, Tx.AuxEn.get()  );
	CHECKX(          0xffffffff, Tx.Cntl0.get()  );
	CHECKX(          0xffffffff, Tx.Cntl1.get()  );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.Peek.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.FifoH.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Example heavy bit-field test.

  CASE( "60a", "get_Speed_12()" );
    try {
	Tx.Cntl0.put(    0x00000000 );
	CHECKX(          0x00000000, Tx.Cntl0.get() );
	Tx.Cntl0.put_Speed_12( 0xfff );
	CHECKX(          0xfff00000, Tx.Cntl0.get() );
	CHECK(                0xfff, Tx.Cntl0.get_Speed_12() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60b", "get_Speed_12()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_Speed_12( 0x000 );
	CHECKX(          0x000fffff, Tx.Cntl0.get() );
	CHECK(                0x000, Tx.Cntl0.get_Speed_12() );
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
// Light-weight bit-field test.

  CASE( "62b", "get_ChipSelects_3()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_ChipSelects_3( 0x0 );
	CHECKX(          0xfff1ffff, Tx.Cntl0.get() );
	CHECK(                  0x0, Tx.Cntl0.get_ChipSelects_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "get_PostInMode_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_PostInMode_1( 0 );
	CHECKX(          0xfffeffff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_PostInMode_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_VariableCs_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_VariableCs_1( 0 );
	CHECKX(          0xffff7fff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_VariableCs_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "get_VariableWidth_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_VariableWidth_1( 0 );
	CHECKX(          0xffffbfff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_VariableWidth_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "get_DoutHoldTime_2()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_DoutHoldTime_2( 0 );
	CHECKX(          0xffffcfff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_DoutHoldTime_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "get_EnableSerial_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_EnableSerial_1( 0 );
	CHECKX(          0xfffff7ff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_EnableSerial_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "68b", "get_InRising_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_InRising_1( 0 );
	CHECKX(          0xfffffbff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_InRising_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "69b", "get_ClearFifos_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_ClearFifos_1( 0 );
	CHECKX(          0xfffffdff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_ClearFifos_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70b", "get_OutRising_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_OutRising_1( 0 );
	CHECKX(          0xfffffeff, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_OutRising_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "get_InvertClk_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_InvertClk_1( 0 );
	CHECKX(          0xffffff7f, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_InvertClk_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "get_OutMsbFirst_1()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_OutMsbFirst_1( 0 );
	CHECKX(          0xffffffbf, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_OutMsbFirst_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "get_ShiftLength_6()" );
    try {
	Tx.Cntl0.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl0.get() );
	Tx.Cntl0.put_ShiftLength_6( 0 );
	CHECKX(          0xffffffc0, Tx.Cntl0.get() );
	CHECK(                    0, Tx.Cntl0.get_ShiftLength_6() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Cntl1

  CASE( "74b", "get_CsHighTime_3()" );
    try {
	Tx.Cntl1.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl1.get() );
	Tx.Cntl1.put_CsHighTime_3( 0 );
	CHECKX(          0xfffff8ff, Tx.Cntl1.get() );
	CHECK(                    0, Tx.Cntl1.get_CsHighTime_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75b", "get_TxEmptyIrq_1()" );
    try {
	Tx.Cntl1.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl1.get() );
	Tx.Cntl1.put_TxEmptyIrq_1( 0 );
	CHECKX(          0xffffff7f, Tx.Cntl1.get() );
	CHECK(                    0, Tx.Cntl1.get_TxEmptyIrq_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "get_DoneIrq_1()" );
    try {
	Tx.Cntl1.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl1.get() );
	Tx.Cntl1.put_DoneIrq_1( 0 );
	CHECKX(          0xffffffbf, Tx.Cntl1.get() );
	CHECK(                    0, Tx.Cntl1.get_DoneIrq_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "77b", "get_InMsbFirst_1()" );
    try {
	Tx.Cntl1.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl1.get() );
	Tx.Cntl1.put_InMsbFirst_1( 0 );
	CHECKX(          0xfffffffd, Tx.Cntl1.get() );
	CHECK(                    0, Tx.Cntl1.get_InMsbFirst_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "78b", "get_KeepInput_1()" );
    try {
	Tx.Cntl1.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl1.get() );
	Tx.Cntl1.put_KeepInput_1( 0 );
	CHECKX(          0xfffffffe, Tx.Cntl1.get() );
	CHECK(                    0, Tx.Cntl1.get_KeepInput_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Stat

  CASE( "81b", "get_TxLevel_3()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxLevel_3( 0 );
	CHECKX(          0x8fffffff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxLevel_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82b", "get_RxLevel_3()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxLevel_3( 0 );
	CHECKX(          0xff8fffff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxLevel_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83b", "get_TxFull_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxFull_1( 0 );
	CHECKX(          0xfffffbff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxFull_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "84b", "get_TxEmpty_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxEmpty_1( 0 );
	CHECKX(          0xfffffdff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxEmpty_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "85b", "get_RxFull_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxFull_1( 0 );
	CHECKX(          0xfffffeff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxFull_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "86b", "get_RxEmpty_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxEmpty_1( 0 );
	CHECKX(          0xffffff7f, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxEmpty_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "87b", "get_Busy_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_Busy_1( 0 );
	CHECKX(          0xffffffbf, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_Busy_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "88b", "get_BitCount_6()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_BitCount_6( 0 );
	CHECKX(          0xffffffc0, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_BitCount_6() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// AuxIrq

  CASE( "91b", "get_Spi2Irq_1()" );
    try {
	Tx.AuxIrq.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	Tx.AuxIrq.put_Spi2Irq_1( 0 );
	CHECKX(          0xfffffffb, Tx.AuxIrq.get() );
	CHECK(                    0, Tx.AuxIrq.get_Spi2Irq_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92b", "get_Spi1Irq_1()" );
    try {
	Tx.AuxIrq.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	Tx.AuxIrq.put_Spi1Irq_1( 0 );
	CHECKX(          0xfffffffd, Tx.AuxIrq.get() );
	CHECK(                    0, Tx.AuxIrq.get_Spi1Irq_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "93b", "get_MiniUartIrq_1()" );
    try {
	Tx.AuxIrq.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxIrq.get() );
	Tx.AuxIrq.put_MiniUartIrq_1( 0 );
	CHECKX(          0xfffffffe, Tx.AuxIrq.get() );
	CHECK(                    0, Tx.AuxIrq.get_MiniUartIrq_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// AuxEn

  CASE( "94b", "get_Spi2Enable_1()" );
    try {
	Tx.AuxEn.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxEn.get() );
	Tx.AuxEn.put_Spi2Enable_1( 0 );
	CHECKX(          0xfffffffb, Tx.AuxEn.get() );
	CHECK(                    0, Tx.AuxEn.get_Spi2Enable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "95b", "get_Spi1Enable_1()" );
    try {
	Tx.AuxEn.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxEn.get() );
	Tx.AuxEn.put_Spi1Enable_1( 0 );
	CHECKX(          0xfffffffd, Tx.AuxEn.get() );
	CHECK(                    0, Tx.AuxEn.get_Spi1Enable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "96b", "get_MiniUartEnable_1()" );
    try {
	Tx.AuxEn.put(    0xffffffff );
	CHECKX(          0xffffffff, Tx.AuxEn.get() );
	Tx.AuxEn.put_MiniUartEnable_1( 0 );
	CHECKX(          0xfffffffe, Tx.AuxEn.get() );
	CHECK(                    0, Tx.AuxEn.get_MiniUartEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "98", "verify final registers" );
    try {
	CHECKX(          0xffffffff, Tx.AuxIrq.read() );
	CHECKX(          0xffffffff, Tx.AuxEn.read()  );
	CHECKX(          0xffffffff, Tx.Cntl0.read()  );
	CHECKX(          0xffffffff, Tx.Cntl1.read()  );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.Peek.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.FifoH.read()  );
	//
	CHECKX(          0xfffffffe, Tx.AuxIrq.get() );
	CHECKX(          0xfffffffe, Tx.AuxEn.get()  );
	CHECKX(          0xffffffc0, Tx.Cntl0.get()  );
	CHECKX(          0xfffffffe, Tx.Cntl1.get()  );
	CHECKX(          0xffffffc0, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.Peek.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.FifoH.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

