// 2021-03-09  William A. Hudson
//
// Testing:  rgPads  System Timer
//    10-19  Constructor, get_bcm_address(), rgPads_Cntl constructor
//    20-29  Address of registers  addr()
//    30-39  Direct register access  read(), write()
//    40-49  Full register get(), put(), grab(), push()
//    50-59  .
//    60-98  Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgPads.h"

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

rgPads			Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Constructor, get_bcm_address(), rgPads_Cntl constructor
//--------------------------------------------------------------------------

  CASE( "10", "rgPads constructor" );
    try {
	rgPads		tx  ( &Bx );
	CHECKX( 0x7e100000, tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e100000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12", "rgPads_Cntl constructor init has NULL address" );
    try {
	rgPads_Cntl		tx;
	CHECK( 1, tx.addr() == NULL );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor copy initialization, all classes.

  CASE( "15", "rgPads_Cntl constructor init has NULL address" );
    try {
	rgPads_Cntl		rx  ( Tx.PadsA_27s0 );
	CHECKX( 0x2c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "PadsA_27s0.addr()" );
    try {
	CHECKX( 0x2c, (Tx.PadsA_27s0.addr()  - Tx.get_base_addr())*4 );
	CHECKX( 0x30, (Tx.PadsB_45s28.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x34, (Tx.PadsC_53s46.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition write(), read()" );
    try {
	Tx.PadsA_27s0.write(  0xffffffff );
	Tx.PadsB_45s28.write( 0xffffffff );
	Tx.PadsC_53s46.write( 0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.read()  );
	CHECKX(               0xffffffff, Tx.PadsB_45s28.read() );
	CHECKX(               0xffffffff, Tx.PadsC_53s46.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "PadsA_27s0.read()" );
    try {
	Tx.PadsA_27s0.write(  0x44bbbb44 );
	CHECKX(               0x44bbbb44, Tx.PadsA_27s0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "PadsB_45s28.read()" );
    try {
	Tx.PadsB_45s28.write( 0x55aaaa55 );
	CHECKX(               0x55aaaa55, Tx.PadsB_45s28.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "PadsC_53s46.read()" );
    try {
	Tx.PadsC_53s46.write( 0x33cccc33 );
	CHECKX(               0x33cccc33, Tx.PadsC_53s46.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "check final register pattern" );
    try {
	CHECKX(               0x44bbbb44, Tx.PadsA_27s0.read()  );
	CHECKX(               0x55aaaa55, Tx.PadsB_45s28.read() );
	CHECKX(               0x33cccc33, Tx.PadsC_53s46.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Full register get(), put(), grab(), push()
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "40", "condition write(), read()" );
    try {
	Tx.PadsA_27s0.write(  0xffffffff );
	Tx.PadsB_45s28.write( 0xffffffff );
	Tx.PadsC_53s46.write( 0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.read()  );
	CHECKX(               0xffffffff, Tx.PadsB_45s28.read() );
	CHECKX(               0xffffffff, Tx.PadsC_53s46.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "PadsA_27s0.grab(), get()" );
    try {
	Tx.PadsA_27s0.write(  0xccc33ccc );
	Tx.PadsA_27s0.put(    0x77444477 );
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.read()  );
	CHECKX(               0x77444477, Tx.PadsA_27s0.get()   );
	Tx.PadsA_27s0.grab();
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.read()  );
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "PadsA_27s0.put(), push()" );
    try {
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.read()  );
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.get()   );
	Tx.PadsA_27s0.put(    0xbeef0303 );
	CHECKX(               0xccc33ccc, Tx.PadsA_27s0.read()  );
	CHECKX(               0xbeef0303, Tx.PadsA_27s0.get()   );
	Tx.PadsA_27s0.push();
	CHECKX(               0xbeef0303, Tx.PadsA_27s0.read()  );
	CHECKX(               0xbeef0303, Tx.PadsA_27s0.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

  CASE( "60", "condition write(), read()" );
    try {
	Tx.PadsA_27s0.write(  0xffffffff );
	Tx.PadsB_45s28.write( 0xffffffff );
	Tx.PadsC_53s46.write( 0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.read()  );
	CHECKX(               0xffffffff, Tx.PadsB_45s28.read() );
	CHECKX(               0xffffffff, Tx.PadsC_53s46.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Example heavy-weight bit-field test.

  CASE( "61a", "get_Passwd_8()" );
    try {
	Tx.PadsA_27s0.put(    0x00000000 );
	CHECKX(               0x00000000, Tx.PadsA_27s0.get() );
	Tx.PadsA_27s0.put_Passwd_8( 0xff );
	CHECKX(               0xff000000, Tx.PadsA_27s0.get() );
	CHECKX(                     0xff, Tx.PadsA_27s0.get_Passwd_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "get_Passwd_8()" );
    try {
	Tx.PadsA_27s0.put(    0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.get() );
	Tx.PadsA_27s0.put_Passwd_8(    0 );
	CHECKX(               0x00ffffff, Tx.PadsA_27s0.get() );
	CHECKX(                        0, Tx.PadsA_27s0.get_Passwd_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "put_Passwd_8() bad value" );
    try {
	Tx.PadsA_27s0.put_Passwd_8( 0x100 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xff:  0x100",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Light-weight bit-field test.

  CASE( "62b", "get_SlewMax_1()" );
    try {
	Tx.PadsA_27s0.put(    0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.get() );
	Tx.PadsA_27s0.put_SlewMax_1(   0 );
	CHECKX(               0xffffffef, Tx.PadsA_27s0.get() );
	CHECK(                         0, Tx.PadsA_27s0.get_SlewMax_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "get_HystEn_1()" );
    try {
	Tx.PadsA_27s0.put(    0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.get() );
	Tx.PadsA_27s0.put_HystEn_1(    0 );
	CHECKX(               0xfffffff7, Tx.PadsA_27s0.get() );
	CHECK(                         0, Tx.PadsA_27s0.get_HystEn_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_DriveStr_3()" );
    try {
	Tx.PadsA_27s0.put(    0xffffffff );
	CHECKX(               0xffffffff, Tx.PadsA_27s0.get() );
	Tx.PadsA_27s0.put_DriveStr_3(  0 );
	CHECKX(               0xfffffff8, Tx.PadsA_27s0.get() );
	CHECK(                         0, Tx.PadsA_27s0.get_DriveStr_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

