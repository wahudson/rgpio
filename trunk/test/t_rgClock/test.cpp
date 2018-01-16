// 2018-01-01  William A. Hudson
//
// Testing:  rgClock  Clock class.
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

//--------------------------------------------------------------------------
//## Direct Register access
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
	Tx.raw_write_CtlReg( 0xf2345678);
	vv = Tx.read_CtlReg();
	CHECKX( 0xf2345678, vv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Field Accessor functions  get_() put_()
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

