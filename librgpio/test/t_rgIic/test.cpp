// 2020-06-21  William A. Hudson
//
// Testing:  rgIic I2C Master class.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers  addr()
//    30-39  Full register get(), put(), grab(), push()
//    40-49  Direct register access  read_(), write_()
//    50-59  Object State registers  init_put_reset(), grab_regs(), push_regs()
//    60-89  Object Field Accessors  get_(), put_() - complete
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
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
	Bx.open_fake_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );	// RPi4
rgIic			Tx3  ( &Bx, 3 );	// test object, Iic0

rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2837 );	// RPi3
rgIic			Tx   ( &Bx, 0 );	// test object
rgIic			Tx1  ( &Bx, 1 );	// test object, Iic0

//--------------------------------------------------------------------------
//## Constructor, get_bcm_address()
//--------------------------------------------------------------------------

  CASE( "10a", "constructor iic0" );
    try {
	rgIic		tx  ( &Bx, 0 );
	CHECKX( 0x7e205000, tx.get_bcm_address() );
	CHECK(  0,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "constructor iic3" );
    try {
	rgIic		tx  ( &Bx, 1 );
	CHECKX( 0x7e804000, tx.get_bcm_address() );
	CHECK(  1,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "constructor iic2" );
    try {
	rgIic		tx  ( &Bx, 2 );
	CHECKX( 0x7e805000, tx.get_bcm_address() );
	CHECK(  2,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "constructor, bad iic number" );
    try {
	rgIic		tx  ( &Bx, 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgIic:  constructor invalid IIC number for BCM2837:  3",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );	// RPi4

  CASE( "11a", "RPi4 constructor iic0" );
    try {
	rgIic		tx  ( &Bx, 0 );
	CHECKX( 0x7e205000, tx.get_bcm_address() );
	CHECK(  0,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "RPi4 constructor iic3" );
    try {
	rgIic		tx  ( &Bx, 1 );
	CHECKX( 0x7e804000, tx.get_bcm_address() );
	CHECK(  1,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11c", "RPi4 constructor iic2" );
    try {
	rgIic		tx  ( &Bx, 2 );
	CHECKX( 0x7e805000, tx.get_bcm_address() );
	CHECK(  2,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11d", "RPi4 constructor iic3" );
    try {
	rgIic		tx  ( &Bx, 3 );
	CHECKX( 0x7e205600, tx.get_bcm_address() );
	CHECK(  3,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11e", "RPi4 constructor iic4" );
    try {
	rgIic		tx  ( &Bx, 4 );
	CHECKX( 0x7e205800, tx.get_bcm_address() );
	CHECK(  4,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11f", "RPi4 constructor iic5" );
    try {
	rgIic		tx  ( &Bx, 5 );
	CHECKX( 0x7e205a80, tx.get_bcm_address() );
	CHECK(  5,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11g", "RPi4 constructor iic6" );
    try {
	rgIic		tx  ( &Bx, 6 );
	CHECKX( 0x7e205c00, tx.get_bcm_address() );
	CHECK(  6,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11h", "RPi4 constructor iic7" );
    try {
	rgIic		tx  ( &Bx, 7 );
	CHECKX( 0x7e205e00, tx.get_bcm_address() );	//#!! guess
	CHECK(  7,          tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11i", "RPi4 constructor, bad iic number" );
    try {
	rgIic		tx  ( &Bx, 8 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgIic:  constructor requires IIC number {0..7}:  8",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Stand-alone Register constructor init has NULL address.

  CASE( "13a", "rgIic_Cntl constructor init null" );
    try {
	rgIic_Cntl	rx;
	CHECK( 1, rx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13b", "rgIic_Stat constructor init null" );
    try {
	rgIic_Stat	rx;
	CHECK( 1, rx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
// Register copy constructor initialization.

  CASE( "14a", "reg constructor rgIic_Cntl" );
    try {
	rgIic_Cntl	rx  ( Tx.Cntl );
	CHECKX( 0x00, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register base byte address offset.
// Difference in (uint32_t*) pointers is a word offset.
// (Note fake memory is word aligned, so page masking is not useful.)

  CASE( "15d", "register base iic3" );
    try {
	rgIic		tx  ( &Bx, 3 );
	CHECK(  3,          tx.get_iic_num() );
	CHECKX( 0x7e205600, tx.get_bcm_address() );
	CHECKX( 0x00000600,
	    (tx.get_base_addr() - Bx.get_mem_block(0x7e205000))*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15e", "register base iic4" );
    try {
	rgIic		tx  ( &Bx, 4 );
	CHECK(  4,          tx.get_iic_num() );
	CHECKX( 0x7e205800, tx.get_bcm_address() );
	CHECKX( 0x00000800,
	    (tx.get_base_addr() - Bx.get_mem_block(0x7e205000))*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15f", "register base iic5" );
    try {
	rgIic		tx  ( &Bx, 5 );
	CHECK(  5,          tx.get_iic_num() );
	CHECKX( 0x7e205a80, tx.get_bcm_address() );
	CHECKX( 0x00000a80,
	    (tx.get_base_addr() - Bx.get_mem_block(0x7e205000))*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15g", "register base iic6" );
    try {
	rgIic		tx  ( &Bx, 6 );
	CHECK(  6,          tx.get_iic_num() );
	CHECKX( 0x7e205c00, tx.get_bcm_address() );
	CHECKX( 0x00000c00,
	    (tx.get_base_addr() - Bx.get_mem_block(0x7e205000))*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

// Compile check function type, must be volatile.
  CASE( "20", "Cntl.addr() function type" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Cntl.addr();
	CHECKX( 0x00, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Address values (sample)
  CASE( "21a", "Cntl.addr()" );
    try {
	CHECKX( 0x00, (Tx.Cntl.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
	CHECKX( 0x00, (Tx1.Cntl.addr() - Tx1.get_base_addr())*4 );
	CHECK(  1,     Tx1.get_iic_num() );
	CHECKX( 0x00, (Tx3.Cntl.addr() - Tx3.get_base_addr())*4 );
	CHECK(  3,     Tx3.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22a", "Stat.addr()" );
    try {
	CHECKX( 0x04, (Tx.Stat.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
	CHECKX( 0x04, (Tx1.Stat.addr() - Tx1.get_base_addr())*4 );
	CHECK(  1,     Tx1.get_iic_num() );
	CHECKX( 0x04, (Tx3.Stat.addr() - Tx3.get_base_addr())*4 );
	CHECK(  3,     Tx3.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23a", "DatLen.addr()" );
    try {
	CHECKX( 0x08, (Tx.DatLen.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
	CHECKX( 0x08, (Tx1.DatLen.addr() - Tx1.get_base_addr())*4 );
	CHECK(  1,     Tx1.get_iic_num() );
	CHECKX( 0x08, (Tx3.DatLen.addr() - Tx3.get_base_addr())*4 );
	CHECK(  3,     Tx3.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24a", "Addr.addr()" );
    try {
	CHECKX( 0x0c, (Tx.Addr.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25a", "Fifo.addr()" );
    try {
	CHECKX( 0x10, (Tx.Fifo.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26a", "ClkDiv.addr()" );
    try {
	CHECKX( 0x14, (Tx.ClkDiv.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27a", "Delay.addr()" );
    try {
	CHECKX( 0x18, (Tx.Delay.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "28a", "ClkStr.addr()" );
    try {
	CHECKX( 0x1c, (Tx.ClkStr.addr() - Tx.get_base_addr())*4 );
	CHECK(  0,     Tx.get_iic_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Full register get(), put(), grab(), push()
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "30", "condition registers" );
    try {
	Tx.Cntl.write(   0x00000000 );
	Tx.Stat.write(   0x00000000 );
	Tx.DatLen.write( 0x00000000 );
	Tx.Addr.write(   0x00000000 );
	Tx.Fifo.write(   0x00000000 );
	Tx.ClkDiv.write( 0x00000000 );
	Tx.Delay.write(  0x00000000 );
	Tx.ClkStr.write( 0x00000000 );
	CHECKX(          0x00000000, Tx.Cntl.read()   );
	CHECKX(          0x00000000, Tx.Stat.read()   );
	CHECKX(          0x00000000, Tx.DatLen.read() );
	CHECKX(          0x00000000, Tx.Addr.read()   );
	CHECKX(          0x00000000, Tx.Fifo.read()   );
	CHECKX(          0x00000000, Tx.ClkDiv.read() );
	CHECKX(          0x00000000, Tx.Delay.read()  );
	CHECKX(          0x00000000, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "Cntl.grab(), get()" );
    try {
	Tx.Cntl.write(   0xccc33ccc );
	Tx.Cntl.put(     0x77444477 );
	CHECKX(          0xccc33ccc, Tx.Cntl.read()   );
	CHECKX(          0x77444477, Tx.Cntl.get()    );
	Tx.Cntl.grab();
	CHECKX(          0xccc33ccc, Tx.Cntl.read()   );
	CHECKX(          0xccc33ccc, Tx.Cntl.get()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "Cntl.put(), push()" );
    try {
	CHECKX(          0xccc33ccc, Tx.Cntl.read()   );
	CHECKX(          0xccc33ccc, Tx.Cntl.get()    );
	Tx.Cntl.put(     0xbeef0303 );
	CHECKX(          0xccc33ccc, Tx.Cntl.read()   );
	CHECKX(          0xbeef0303, Tx.Cntl.get()    );
	Tx.Cntl.push();
	CHECKX(          0xbeef0303, Tx.Cntl.read()   );
	CHECKX(          0xbeef0303, Tx.Cntl.get()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read(), write()
//--------------------------------------------------------------------------
// Test read/write to fake memory.

  CASE( "40", "condition registers" );
    try {
	Tx.Cntl.write(   0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.DatLen.write( 0xffffffff );
	Tx.Addr.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.ClkDiv.write( 0xffffffff );
	Tx.Delay.write(  0xffffffff );
	Tx.ClkStr.write( 0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.DatLen.read() );
	CHECKX(          0xffffffff, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.read() );
	CHECKX(          0xffffffff, Tx.Delay.read()  );
	CHECKX(          0xffffffff, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "Cntl.read()" );
    try {
	Tx.Cntl.write(   0x11eeee11 );
	CHECKX(          0x11eeee11, Tx.Cntl.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "Stat.read()" );
    try {
	Tx.Stat.write(   0x22dddd22 );
	CHECKX(          0x22dddd22, Tx.Stat.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "DatLen.read()" );
    try {
	Tx.DatLen.write( 0x33cccc33 );
	CHECKX(          0x33cccc33, Tx.DatLen.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "Addr.read()" );
    try {
	Tx.Addr.write(   0x44bbbb44 );
	CHECKX(          0x44bbbb44, Tx.Addr.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "Fifo.read()" );
    try {
	Tx.Fifo.write(   0x55aaaa55 );
	CHECKX(          0x55aaaa55, Tx.Fifo.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "ClkDiv.read()" );
    try {
	Tx.ClkDiv.write( 0xee1111ee );
	CHECKX(          0xee1111ee, Tx.ClkDiv.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "47", "Delay.read()" );
    try {
	Tx.Delay.write(  0xdd2222dd );
	CHECKX(          0xdd2222dd, Tx.Delay.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "48", "ClkStr.read()" );
    try {
	Tx.ClkStr.write( 0xcc3333cc );
	CHECKX(          0xcc3333cc, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "49", "check final register pattern" );
    try {
	CHECKX(          0x11eeee11, Tx.Cntl.read()   );
	CHECKX(          0x22dddd22, Tx.Stat.read()   );
	CHECKX(          0x33cccc33, Tx.DatLen.read() );
	CHECKX(          0x44bbbb44, Tx.Addr.read()   );
	CHECKX(          0x55aaaa55, Tx.Fifo.read()   );
	CHECKX(          0xee1111ee, Tx.ClkDiv.read() );
	CHECKX(          0xdd2222dd, Tx.Delay.read()  );
	CHECKX(          0xcc3333cc, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State  init_put_reset(), grab_regs(), push_regs()
//--------------------------------------------------------------------------

  CASE( "50", "condition registers" );
    try {
	Tx.Cntl.write(   0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.DatLen.write( 0xffffffff );
	Tx.Addr.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.ClkDiv.write( 0xffffffff );
	Tx.Delay.write(  0xffffffff );
	Tx.ClkStr.write( 0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.DatLen.read() );
	CHECKX(          0xffffffff, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.read() );
	CHECKX(          0xffffffff, Tx.Delay.read()  );
	CHECKX(          0xffffffff, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51", "init_put_reset()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.DatLen.put(   0xffffffff );
	Tx.Addr.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.ClkDiv.put(   0xffffffff );
	Tx.Delay.put(    0xffffffff );
	Tx.ClkStr.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get()   );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.DatLen.get() );
	CHECKX(          0xffffffff, Tx.Addr.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.get() );
	CHECKX(          0xffffffff, Tx.Delay.get()  );
	CHECKX(          0xffffffff, Tx.ClkStr.get() );
	Tx.init_put_reset();
	CHECKX(          0x00000000, Tx.Cntl.get()   );
	CHECKX(          0x00000050, Tx.Stat.get()   );
	CHECKX(          0x00000000, Tx.DatLen.get() );
	CHECKX(          0x00000000, Tx.Addr.get()   );
	CHECKX(          0x00000000, Tx.Fifo.get()   );	// include FIFO
	CHECKX(          0x000005dc, Tx.ClkDiv.get() );
	CHECKX(          0x00300030, Tx.Delay.get()  );
	CHECKX(          0x00000040, Tx.ClkStr.get() );
	//
	CHECKX(          0xffffffff, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.DatLen.read() );
	CHECKX(          0xffffffff, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.read() );
	CHECKX(          0xffffffff, Tx.Delay.read()  );
	CHECKX(          0xffffffff, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53", "grab_regs(), not Fifo" );
    try {
	Tx.Cntl.write(   0x11111111 );
	Tx.Stat.write(   0x22222222 );
	Tx.DatLen.write( 0x33333333 );
	Tx.Addr.write(   0x44444444 );
	Tx.Fifo.write(   0x55555555 );
	Tx.ClkDiv.write( 0x66666666 );
	Tx.Delay.write(  0x77777777 );
	Tx.ClkStr.write( 0x88888888 );
	//
	Tx.Cntl.put(     0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.DatLen.put(   0xffffffff );
	Tx.Addr.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.ClkDiv.put(   0xffffffff );
	Tx.Delay.put(    0xffffffff );
	Tx.ClkStr.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get()   );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.DatLen.get() );
	CHECKX(          0xffffffff, Tx.Addr.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.get() );
	CHECKX(          0xffffffff, Tx.Delay.get()  );
	CHECKX(          0xffffffff, Tx.ClkStr.get() );
	Tx.grab_regs();
	CHECKX(          0x11111111, Tx.Cntl.get()   );
	CHECKX(          0x22222222, Tx.Stat.get()   );
	CHECKX(          0x33333333, Tx.DatLen.get() );
	CHECKX(          0x44444444, Tx.Addr.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );	// not Fifo
	CHECKX(          0x66666666, Tx.ClkDiv.get() );
	CHECKX(          0x77777777, Tx.Delay.get()  );
	CHECKX(          0x88888888, Tx.ClkStr.get() );
	//
	CHECKX(          0x11111111, Tx.Cntl.read()   );
	CHECKX(          0x22222222, Tx.Stat.read()   );
	CHECKX(          0x33333333, Tx.DatLen.read() );
	CHECKX(          0x44444444, Tx.Addr.read()   );
	CHECKX(          0x55555555, Tx.Fifo.read()   );
	CHECKX(          0x66666666, Tx.ClkDiv.read() );
	CHECKX(          0x77777777, Tx.Delay.read()  );
	CHECKX(          0x88888888, Tx.ClkStr.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "54", "push_regs(), not Fifo, not Stat" );
    try {
	Tx.Cntl.write(   0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.DatLen.write( 0xffffffff );
	Tx.Addr.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.ClkDiv.write( 0xffffffff );
	Tx.Delay.write(  0xffffffff );
	Tx.ClkStr.write( 0xffffffff );
	//
	Tx.Cntl.put(     0x11111111 );
	Tx.Stat.put(     0x22222222 );
	Tx.DatLen.put(   0x33333333 );
	Tx.Addr.put(     0x44444444 );
	Tx.Fifo.put(     0x55555555 );
	Tx.ClkDiv.put(   0x66666666 );
	Tx.Delay.put(    0x77777777 );
	Tx.ClkStr.put(   0x88888888 );
	CHECKX(          0xffffffff, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.DatLen.read() );
	CHECKX(          0xffffffff, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.read() );
	CHECKX(          0xffffffff, Tx.Delay.read()  );
	CHECKX(          0xffffffff, Tx.ClkStr.read() );
	Tx.push_regs();
	CHECKX(          0x11111111, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );	// not Stat
	CHECKX(          0x33333333, Tx.DatLen.read() );
	CHECKX(          0x44444444, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );	// not Fifo
	CHECKX(          0x66666666, Tx.ClkDiv.read() );
	CHECKX(          0x77777777, Tx.Delay.read()  );
	CHECKX(          0x88888888, Tx.ClkStr.read() );
	//
	CHECKX(          0x11111111, Tx.Cntl.get()   );
	CHECKX(          0x22222222, Tx.Stat.get()   );
	CHECKX(          0x33333333, Tx.DatLen.get() );
	CHECKX(          0x44444444, Tx.Addr.get()   );
	CHECKX(          0x55555555, Tx.Fifo.get()   );
	CHECKX(          0x66666666, Tx.ClkDiv.get() );
	CHECKX(          0x77777777, Tx.Delay.get()  );
	CHECKX(          0x88888888, Tx.ClkStr.get() );
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
	Tx.Cntl.write(   0xffffffff );
	Tx.Stat.write(   0xffffffff );
	Tx.DatLen.write( 0xffffffff );
	Tx.Addr.write(   0xffffffff );
	Tx.Fifo.write(   0xffffffff );
	Tx.ClkDiv.write( 0xffffffff );
	Tx.Delay.write(  0xffffffff );
	Tx.ClkStr.write( 0xffffffff );
	//
	Tx.Cntl.put(     0xffffffff );
	Tx.Stat.put(     0xffffffff );
	Tx.DatLen.put(   0xffffffff );
	Tx.Addr.put(     0xffffffff );
	Tx.Fifo.put(     0xffffffff );
	Tx.ClkDiv.put(   0xffffffff );
	Tx.Delay.put(    0xffffffff );
	Tx.ClkStr.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.read()   );
	CHECKX(          0xffffffff, Tx.Stat.read()   );
	CHECKX(          0xffffffff, Tx.DatLen.read() );
	CHECKX(          0xffffffff, Tx.Addr.read()   );
	CHECKX(          0xffffffff, Tx.Fifo.read()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.read() );
	CHECKX(          0xffffffff, Tx.Delay.read()  );
	CHECKX(          0xffffffff, Tx.ClkStr.read() );
	//
	CHECKX(          0xffffffff, Tx.Cntl.get()   );
	CHECKX(          0xffffffff, Tx.Stat.get()   );
	CHECKX(          0xffffffff, Tx.DatLen.get() );
	CHECKX(          0xffffffff, Tx.Addr.get()   );
	CHECKX(          0xffffffff, Tx.Fifo.get()   );
	CHECKX(          0xffffffff, Tx.ClkDiv.get() );
	CHECKX(          0xffffffff, Tx.Delay.get()  );
	CHECKX(          0xffffffff, Tx.ClkStr.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Example heavy-weight bit-field test.

  CASE( "61a", "get_IicEnable_1()" );
    try {
	Tx.Cntl.put(     0x00000000 );
	CHECKX(          0x00000000, Tx.Cntl.get() );
	Tx.Cntl.put_IicEnable_1( 1 );
	CHECKX(          0x00008000, Tx.Cntl.get() );
	CHECK(                    1, Tx.Cntl.get_IicEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "get_IicEnable_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_IicEnable_1( 0 );
	CHECKX(          0xffff7fff, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_IicEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "put_IicEnable_1() bad value" );
    try {
	Tx.Cntl.put_IicEnable_1( 2 );
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
// Light-weight bit-field test.

  CASE( "62b", "get_IrqRxHalf_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_IrqRxHalf_1( 0 );
	CHECKX(          0xfffffbff, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_IrqRxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "get_IrqTxHalf_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_IrqTxHalf_1( 0 );
	CHECKX(          0xfffffdff, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_IrqTxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_IrqDone_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_IrqDone_1( 0 );
	CHECKX(          0xfffffeff, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_IrqDone_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "get_StartTrans_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_StartTrans_1( 0 );
	CHECKX(          0xffffff7f, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_StartTrans_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "get_ClearFifo_2()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_ClearFifo_2( 0 );
	CHECKX(          0xffffffcf, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_ClearFifo_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "get_ReadPacket_1()" );
    try {
	Tx.Cntl.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Cntl.get() );
	Tx.Cntl.put_ReadPacket_1( 0 );
	CHECKX(          0xfffffffe, Tx.Cntl.get() );
	CHECK(                    0, Tx.Cntl.get_ReadPacket_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Stat

  CASE( "70b", "get_ClkTimeout_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_ClkTimeout_1( 0 );
	CHECKX(          0xfffffdff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_ClkTimeout_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "get_AckErr_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_AckErr_1( 0 );
	CHECKX(          0xfffffeff, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_AckErr_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "get_RxFull_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxFull_1( 0 );
	CHECKX(          0xffffff7f, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxFull_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "get_TxEmpty_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxEmpty_1( 0 );
	CHECKX(          0xffffffbf, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxEmpty_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74b", "get_RxHasData_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxHasData_1( 0 );
	CHECKX(          0xffffffdf, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxHasData_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75b", "get_TxHasSpace_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxHasSpace_1( 0 );
	CHECKX(          0xffffffef, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxHasSpace_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "get_RxHalf_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_RxHalf_1( 0 );
	CHECKX(          0xfffffff7, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_RxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "77b", "get_TxHalf_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TxHalf_1( 0 );
	CHECKX(          0xfffffffb, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "78b", "get_TransDone_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TransDone_1( 0 );
	CHECKX(          0xfffffffd, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TransDone_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "79b", "get_TransActive_1()" );
    try {
	Tx.Stat.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Stat.get() );
	Tx.Stat.put_TransActive_1( 0 );
	CHECKX(          0xfffffffe, Tx.Stat.get() );
	CHECK(                    0, Tx.Stat.get_TransActive_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// DatLen

  CASE( "81b", "get_DataLen_16()" );
    try {
	Tx.DatLen.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.DatLen.get() );
	Tx.DatLen.put_DataLen_16( 0x03c0 );
	CHECKX(          0xffff03c0, Tx.DatLen.get() );
	CHECKX(              0x03c0, Tx.DatLen.get_DataLen_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Addr

  CASE( "82b", "get_DataLen_16()" );
    try {
	Tx.Addr.put(     0xffffffff );
	CHECKX(          0xffffffff, Tx.Addr.get() );
	Tx.Addr.put_SlaveAddr_7( 0x00 );
	CHECKX(          0xffffff80, Tx.Addr.get() );
	CHECKX(                0x00, Tx.Addr.get_SlaveAddr_7() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// ClkDiv

  CASE( "84b", "get_ClkDiv_16()" );
    try {
	Tx.ClkDiv.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.ClkDiv.get() );
	Tx.ClkDiv.put_ClkDiv_16( 0x0cc0 );
	CHECKX(          0xffff0cc0, Tx.ClkDiv.get() );
	CHECKX(              0x0cc0, Tx.ClkDiv.get_ClkDiv_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Delay

  CASE( "85b", "get_Fall2Out_16()" );
    try {
	Tx.Delay.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.Delay.get() );
	Tx.Delay.put_Fall2Out_16( 0x0330 );
	CHECKX(          0x0330ffff, Tx.Delay.get() );
	CHECKX(              0x0330, Tx.Delay.get_Fall2Out_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "86b", "get_Rise2In_16()" );
    try {
	Tx.Delay.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.Delay.get() );
	Tx.Delay.put_Rise2In_16( 0x0cc0 );
	CHECKX(          0xffff0cc0, Tx.Delay.get() );
	CHECKX(              0x0cc0, Tx.Delay.get_Rise2In_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// ClkStr

  CASE( "87b", "get_TimeOut_16()" );
    try {
	Tx.ClkStr.put(   0xffffffff );
	CHECKX(          0xffffffff, Tx.ClkStr.get() );
	Tx.ClkStr.put_TimeOut_16( 0x0770 );
	CHECKX(          0xffff0770, Tx.ClkStr.get() );
	CHECKX(              0x0770, Tx.ClkStr.get_TimeOut_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

