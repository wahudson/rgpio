// 2018-01-01  William A. Hudson
//
// Testing:  rgClock  Clock class.
//    10-19  Constructor
//    20-29  Direct low-level access
//    30-39  Direct control enable_clock(), kill_generator(), wait_while_Busy()
//    40-49  Object state operations grab_regs(), raw_write_regs(), apply_regs()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgClock.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;
rgClock			Tx   ( 0 );	// test object
rgClock			Tx1  ( 1 );	// test object, Clock1
rgClock			Tx2  ( 2 );	// test object, Clock2


  CASE( "00", "Common object" );
    try {
	Bx.open_fake_mem();
	Tx.init_addr( &Bx );
	Tx1.init_addr( &Bx );		// currently a different mem page
	Tx2.init_addr( &Bx );		// currently a different mem page
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10b", "constructor" );
    try {
	rgClock		tx  ( 1, &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "constructor" );
    try {
	rgClock		tx  ( 2, &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "constructor, bad clock number" );
    try {
	rgClock		tx  ( 3, &Bx );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClock:  require clock number {0,1,2}:  3",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "access uninitialized GpioBase" );
    try {
	rgClock			tx  ( 0 );
	volatile uint32_t*      aa;
	aa = tx.get_base_addr();
	if ( aa ) { FAIL( "not null" ); } else { PASS( "NULL" ); }
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14a", "addr_CtlReg() uninitialized" );
    try {
	rgClock			tx  ( 0 );
	tx.addr_CtlReg();
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgClock:  not initialized",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14b", "addr_DivReg() uninitialized" );
    try {
	rgClock			tx  ( 0 );
	tx.addr_DivReg();
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgClock:  not initialized",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15a", "read_CtlReg() uninitialized" );
    try {
	rgClock			tx  ( 0 );
	tx.read_CtlReg();
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgClock:  not initialized",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15b", "read_DivReg() uninitialized" );
    try {
	rgClock			tx  ( 0 );
	tx.read_DivReg();
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgClock:  not initialized",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "19", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e101000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct low-level access
//--------------------------------------------------------------------------

  CASE( "20a", "addr_CtlReg() clock0" );
    try {
	volatile uint32_t	*vp;
	vp = Tx.addr_CtlReg();
	CHECKX( 0x00000070, (vp - Tx.get_base_addr())*4 );
	CHECK(  0,                Tx.get_clock_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "addr_CtlReg() clock1" );
    try {
	volatile uint32_t	*vp;
	vp = Tx1.addr_CtlReg();
	CHECKX( 0x00000078, (vp - Tx1.get_base_addr())*4 );
	CHECK(  1,                Tx1.get_clock_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20c", "addr_CtlReg() clock2" );
    try {
	volatile uint32_t	*vp;
	vp = Tx2.addr_CtlReg();
	CHECKX( 0x00000080, (vp - Tx2.get_base_addr())*4 );
	CHECK(  2,                Tx2.get_clock_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "addr_DivReg() clock0" );
    try {
	volatile uint32_t	*vp;
	vp = Tx.addr_DivReg();
	CHECKX( 0x00000074, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "addr_DivReg() clock1" );
    try {
	volatile uint32_t	*vp;
	vp = Tx1.addr_DivReg();
	CHECKX( 0x0000007c, (vp - Tx1.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "addr_DivReg() clock2" );
    try {
	volatile uint32_t	*vp;
	vp = Tx2.addr_DivReg();
	CHECKX( 0x00000084, (vp - Tx2.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "read_CtlReg()" );
    try {
	uint32_t	vv;
	vv = Tx.read_CtlReg();
	CHECKX( 0x00000000, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "raw_write_CtlReg()" );
    try {
	uint32_t	vv;
	Tx.raw_write_CtlReg( 0xf2345678 );
	vv = Tx.read_CtlReg();
	CHECKX( 0xf2345678, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "read_DivReg()" );
    try {
	uint32_t	vv;
	vv = Tx.read_DivReg();
	CHECKX( 0x00000000, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "raw_write_DivReg()" );
    try {
	uint32_t	vv;
	Tx.raw_write_DivReg( 0xf7654321 );
	vv = Tx.read_DivReg();
	CHECKX( 0xf7654321, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "read_Busy()" );
    try {
	uint32_t	vv;
	Tx.raw_write_CtlReg( 0x00000080 );
	vv = Tx.read_Busy();
	CHECK( 1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "read_Busy()" );
    try {
	uint32_t	vv;
	Tx.raw_write_CtlReg( 0xffffff7f );
	vv = Tx.read_Busy();
	CHECK( 0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "read_Enable()" );
    try {
	uint32_t	vv;
	Tx.raw_write_CtlReg( 0x00000010 );
	vv = Tx.read_Enable();
	CHECK( 1, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "read_Enable()" );
    try {
	uint32_t	vv;
	Tx.raw_write_CtlReg( 0xffffffef );
	vv = Tx.read_Enable();
	CHECK( 0, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct control  enable_clock(), kill_generator(), wait_while_Busy()
//--------------------------------------------------------------------------

  CASE( "30", "enable_clock()" );
    try {
	Tx.raw_write_CtlReg( 0x00000000 );
	Tx.enable_clock();
	CHECKX( 0x00000010, Tx.read_CtlReg() );
	CHECK( 1, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31", "disable_clock()" );
    try {
	Tx.raw_write_CtlReg( 0xffffffff );
	Tx.disable_clock();
	CHECKX( 0xffffffef, Tx.read_CtlReg() );
	CHECK( 0, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "kill_generator()" );
    try {
	Tx.raw_write_CtlReg( 0x00000000 );
	Tx.kill_generator();
	CHECKX( 0x00000020, Tx.read_CtlReg() );		// Kill=1
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "kill_generator()" );
    try {
	Tx.raw_write_CtlReg( 0xffffffdf );
	Tx.kill_generator();
	CHECKX( 0xffffffff, Tx.read_CtlReg() );		// Kill=1
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "wait_disable()" );
    try {
	bool		rv;
	Tx.raw_write_CtlReg( 0x00000010 );		// Busy=0, Enable=1
	CHECK( 1, Tx.read_Enable() );
	rv = Tx.wait_disable();
	CHECK( 0, rv );
	CHECKX( 0x00000000, Tx.read_CtlReg() );		// Busy=0, Enable=0
	CHECK( 0, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33b", "wait_disable()" );
    try {
	bool		rv;
	Tx.raw_write_CtlReg( 0xffffffff );		// Busy=1, Enable=1
	CHECK( 1, Tx.read_Enable() );
	rv = Tx.wait_disable();
	CHECK( 1, rv );
	CHECKX( 0xffffffef, Tx.read_CtlReg() );		// Busy=1, Enable=0
	CHECK( 0, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "34a", "wait_while_Busy()" );
    try {
	bool		rv;
	Tx.raw_write_CtlReg( 0x00000000 );		// Busy=0, Enable=0
	rv = Tx.wait_while_Busy( 1000, 1 );
	CHECK( 0, rv );
	CHECKX( 0x00000000, Tx.read_CtlReg() );		// Busy=0, Enable=0
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34b", "wait_while_Busy()" );
    try {
	bool		rv;
	Tx.raw_write_CtlReg( 0xffffffff );		// Busy=1, Enable=1
	rv = Tx.wait_while_Busy( 1000, 1 );
	CHECK( 1, rv );
	CHECKX( 0xffffffff, Tx.read_CtlReg() );		// Busy=1, Enable=1
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object state operations  grab_regs(), raw_write_regs(), apply_regs()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "40", "grab_regs()" );
    try {
	Tx.raw_write_CtlReg( 0xaa55cc33 );
	Tx.raw_write_DivReg( 0xff008877 );
	Tx.grab_regs();
	CHECKX( 0xaa55cc33, Tx.get_CtlReg() );
	CHECKX( 0xff008877, Tx.get_DivReg() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41", "raw_write_regs()" );
    try {
	Tx.put_CtlReg( 0x55aa33cc );
	Tx.put_DivReg( 0x00ff7788 );
	Tx.raw_write_regs();
	CHECKX( 0x55aa33cc, Tx.get_CtlReg() );
	CHECKX( 0x00ff7788, Tx.get_DivReg() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "apply_regs() enable=0" );
    try {
	Tx.raw_write_DivReg( 0xff008877 );
	Tx.raw_write_CtlReg( 0xaa55cc93 );	// Busy=1, Enable=1
	Tx.put_CtlReg(       0x55aa336c );	// Enable=0
	Tx.put_DivReg(       0x00ff7788 );
	Tx.apply_regs();
	CHECKX( 0x00ff7788, Tx.read_DivReg() );
	CHECKX( 0x55aa336c, Tx.read_CtlReg() );
	CHECKX( 0x55aa336c, Tx.get_CtlReg() );
	CHECK( 0, Tx.get_Enable() );
	CHECK( 0, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "apply_regs() enable=1" );
    try {
	Tx.raw_write_DivReg( 0xff008877 );
	Tx.raw_write_CtlReg( 0xaa55cc63 );	// Busy=0, Enable=0
	Tx.put_CtlReg(       0x55aa339c );	// Enable=1
	Tx.put_DivReg(       0x00ff7788 );
	Tx.apply_regs();
	CHECKX( 0x00ff7788, Tx.read_DivReg() );
	CHECKX( 0x55aa339c, Tx.read_CtlReg() );
	CHECKX( 0x55aa339c, Tx.get_CtlReg() );
	CHECK( 1, Tx.get_Enable() );
	CHECK( 1, Tx.read_Enable() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Note:  Cannot test internal actions of apply_regs().


//--------------------------------------------------------------------------
//## Object Field Accessors  get_() put_()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "31a", "get_Mash()" );
    try {
	Tx.put_CtlReg( 0x00000000 );
	CHECKX(        0x00000000, Tx.get_CtlReg() );
	Tx.put_Mash(   0x3 );
	CHECKX(        0x00000600, Tx.get_CtlReg() );
	CHECK(         0x3,        Tx.get_Mash()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "get_Mash()" );
    try {
	Tx.put_CtlReg( 0xffffffff );
	CHECKX(        0xffffffff, Tx.get_CtlReg() );
	Tx.put_Mash(   0x0 );
	CHECKX(        0xfffff9ff, Tx.get_CtlReg() );
	CHECK(         0x0,        Tx.get_Mash()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31c", "put_Mash() bad value" );
    try {
	Tx.put_Mash( 0x4 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClock::put_Mash():  require 2-bit arg:  4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "38a", "get_DivI()" );
    try {
	Tx.put_DivReg( 0x00000000 );
	CHECKX(        0x00000000, Tx.get_DivReg() );
	Tx.put_DivI(   0xfff );
	CHECKX(        0x00fff000, Tx.get_DivReg() );
	CHECKX(        0xfff,      Tx.get_DivI()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38b", "get_DivI()" );
    try {
	Tx.put_DivReg( 0xffffffff );
	CHECKX(        0xffffffff, Tx.get_DivReg() );
	Tx.put_DivI(   0x000 );
	CHECKX(        0xff000fff, Tx.get_DivReg() );
	CHECKX(        0x000,      Tx.get_DivI()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38c", "put_DivI() bad value" );
    try {
	Tx.put_DivI( 0x1000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClock::put_DivI():  require 12-bit arg:  0x1000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Debug
//--------------------------------------------------------------------------

  CASE( "90", "show_debug()" );
    try {
	Tx.show_debug( cout );
	PASS( "no throw" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

