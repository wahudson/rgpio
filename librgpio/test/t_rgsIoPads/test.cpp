// 2024-05-03  William A. Hudson
//
// Testing:  rgsIoPads  IO Pads Interface class for RPi5
//    10-19  Constructor, get_bcm_address(), rgsIo_Pad register constructor
//    20-29  Register accessor IoPad(), addr():  _flip _set _clr
//    30-39  Hardware read(), write():  _peek _flip _set _clr
//    40-49  Object grab(), push():  _peek _flip _set _clr
//    41     Object get(), put()
//    50-59  .
//    60-98  Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgsIoPads.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );    // RPi5

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	CHECKX( 0x00004000, Bx.config_BlockSize() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgsIoPads		Tx   ( &Bx );		// test object

//--------------------------------------------------------------------------
//## Constructor, get_bcm_address(), rgsIo_Pad register constructor
//--------------------------------------------------------------------------

  CASE( "10a", "rgsIoPads constructor" );
    try {
	rgsIoPads	tx  ( &Bx );
	CHECK(           0, tx.get_bank_num() );
	CHECKX( 0x400f0000, tx.get_bcm_address() );
	CHECK(          27, tx.get_MaxBit() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "rgsIoPads constructor bank 2" );
    try {
	rgsIoPads	tx  ( &Bx, 2 );
	CHECK(           2, tx.get_bank_num() );
	CHECKX( 0x400f0000, tx.get_bcm_address() );	//#!! 0x400f8000
	CHECK(          27, tx.get_MaxBit() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "rgsIoPads constructor bank 3" );
    try {
	rgsIoPads	tx  ( &Bx, 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsIoPads::  require bank in {0,1,2}",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Pads bank 0" );
    try {
	CHECKX( 0x400f0000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12", "rgsIo_Pad register constructor init has NULL address" );
    try {
	rgsIo_Pad	rx;
	CHECK( 1, rx.addr()      == NULL );
	CHECK( 0, rx.addr_flip() == NULL );
	CHECK( 0, rx.addr_set()  == NULL );
	CHECK( 0, rx.addr_clr()  == NULL );
	// Not a normal use of constructor.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor copy initialization.

  CASE( "15", "rgsIo_Pad register copy constructor" );
    try {
	rgsIo_Pad		rx  ( Tx.IoPad(0) );
	CHECKX( 0x00000004, (rx.addr()      - Tx.get_base_addr())*4 );
	CHECKX( 0x00001004, (rx.addr_flip() - Tx.get_base_addr())*4 );
	CHECKX( 0x00002004, (rx.addr_set()  - Tx.get_base_addr())*4 );
	CHECKX( 0x00003004, (rx.addr_clr()  - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor fail on RPi4 or earlier

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2711 );    // RPi4

  CASE( "17", "rgsIoPads constructor" );
    try {
	rgsIoPads	tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( domain_error& e ) {
	CHECK( "rgsIoPads::  require RPi5 (soc_BCM2712)",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register accessor IoPad(), addr():  _flip _set _clr
//--------------------------------------------------------------------------

  CASE( "20a", "accessor IoPad(0)" );
    try {
	CHECKX( 0x00000004, (Tx.IoPad(0).addr()      - Tx.get_base_addr())*4 );
	CHECKX( 0x00001004, (Tx.IoPad(0).addr_flip() - Tx.get_base_addr())*4 );
	CHECKX( 0x00002004, (Tx.IoPad(0).addr_set()  - Tx.get_base_addr())*4 );
	CHECKX( 0x00003004, (Tx.IoPad(0).addr_clr()  - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "accessor IoPad(27)" );
    try {
	CHECKX( 0x00000070, (Tx.IoPad(27).addr()      - Tx.get_base_addr())*4 );
	CHECKX( 0x00001070, (Tx.IoPad(27).addr_flip() - Tx.get_base_addr())*4 );
	CHECKX( 0x00002070, (Tx.IoPad(27).addr_set()  - Tx.get_base_addr())*4 );
	CHECKX( 0x00003070, (Tx.IoPad(27).addr_clr()  - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20c", "accessor IoPad(28)" );
    try {
	Tx.IoPad(28);
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsIo_Pad::IoPad():  bit index out-of-range:  28",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20d", "accessor IoPad(3)" );
    try {
	CHECKX( 0x00000010, (Tx.IoPad(3).addr()      - Tx.get_base_addr())*4 );
	CHECKX( 0x00001010, (Tx.IoPad(3).addr_flip() - Tx.get_base_addr())*4 );
	CHECKX( 0x00002010, (Tx.IoPad(3).addr_set()  - Tx.get_base_addr())*4 );
	CHECKX( 0x00003010, (Tx.IoPad(3).addr_clr()  - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Hardware read(), write():  _peek _flip _set _clr
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition memory" );
    try {
	Tx.IoPad(3).write(      0xffffffff );
	Tx.IoPad(3).write_flip( 0xffffffff );
	Tx.IoPad(3).write_set(  0xffffffff );
	Tx.IoPad(3).write_clr(  0xffffffff );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_set() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "pattern write(), read()" );
    try {
	CHECKX(                 0xffffffff, Tx.IoPad(3).read() );
	Tx.IoPad(3).write(      0x34abba56 );
	CHECKX(                 0x34abba56, Tx.IoPad(3).read() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_set() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "pattern write_flip(), read_peek()" );
    try {
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_peek() );
	Tx.IoPad(3).write_flip( 0x11aa22bb );
	CHECKX(                 0x11aa22bb, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_set() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31c", "pattern write_set(), read_set()" );
    try {
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_set() );
	Tx.IoPad(3).write_set(  0x33cc44dd );
	CHECKX(                 0x33cc44dd, Tx.IoPad(3).read_set() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31d", "pattern write_clr(), read_clr()" );
    try {
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
	Tx.IoPad(3).write_clr(  0x55ee66ff );
	CHECKX(                 0x55ee66ff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31e", "check memory block" );
    try {
	CHECKX(                 0x34abba56, Tx.IoPad(3).read() );
	CHECKX(                 0x11aa22bb, Tx.IoPad(3).read_peek() );
	CHECKX(                 0x33cc44dd, Tx.IoPad(3).read_set() );
	CHECKX(                 0x55ee66ff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object grab(), push():  _peek _flip _set _clr
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "40", "condition memory" );
    try {
	Tx.IoPad(3).write(      0xff1111ff );
	Tx.IoPad(3).write_flip( 0xff2222ff );
	Tx.IoPad(3).write_set(  0xff3333ff );
	Tx.IoPad(3).write_clr(  0xff4444ff );
	CHECKX(                 0xff1111ff, Tx.IoPad(3).read() );
	CHECKX(                 0xff2222ff, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xff3333ff, Tx.IoPad(3).read_set() );
	CHECKX(                 0xff4444ff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "IoPad(3).get(), put()" );
    try {
	Tx.IoPad(3).put(    0x5555aaaa );
	CHECKX(             0x5555aaaa, Tx.IoPad(3).get() );
	Tx.IoPad(3).put(    0xbbbb6666 );
	CHECKX(             0xbbbb6666, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "IoPad(3).grab()" );
    try {
	Tx.IoPad(3).put(    0xf0f0f0f0 );
	CHECKX(             0xf0f0f0f0, Tx.IoPad(3).get() );
	Tx.IoPad(3).grab();
	CHECKX(             0xff1111ff, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "IoPad(3).grab_peek()" );
    try {
	Tx.IoPad(3).put(    0xf0f0f0f0 );
	CHECKX(             0xf0f0f0f0, Tx.IoPad(3).get() );
	Tx.IoPad(3).grab_peek();
	CHECKX(             0xff2222ff, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42c", "IoPad(3).grab_set()" );
    try {
	Tx.IoPad(3).put(    0xf0f0f0f0 );
	CHECKX(             0xf0f0f0f0, Tx.IoPad(3).get() );
	Tx.IoPad(3).grab_set();
	CHECKX(             0xff3333ff, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42d", "IoPad(3).grab_clr()" );
    try {
	Tx.IoPad(3).put(    0xf0f0f0f0 );
	CHECKX(             0xf0f0f0f0, Tx.IoPad(3).get() );
	Tx.IoPad(3).grab_clr();
	CHECKX(             0xff4444ff, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "IoPad(3).push()" );
    try {
	Tx.IoPad(3).write(      0xf0f0f0f0 );
	CHECKX(                 0xf0f0f0f0, Tx.IoPad(3).read() );
	Tx.IoPad(3).put(        0xaa6666aa );
	Tx.IoPad(3).push();
	CHECKX(                 0xaa6666aa, Tx.IoPad(3).read() );
	CHECKX(                 0xaa6666aa, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43b", "IoPad(3).push_flip()" );
    try {
	Tx.IoPad(3).write_flip( 0xf0f0f0f0 );
	CHECKX(                 0xf0f0f0f0, Tx.IoPad(3).read_peek() );
	Tx.IoPad(3).put(        0xbb7777bb );
	Tx.IoPad(3).push_flip();
	CHECKX(                 0xbb7777bb, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xbb7777bb, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43c", "IoPad(3).push_set()" );
    try {
	Tx.IoPad(3).write_set(  0xf0f0f0f0 );
	CHECKX(                 0xf0f0f0f0, Tx.IoPad(3).read_set() );
	Tx.IoPad(3).put(        0xcc8888cc );
	Tx.IoPad(3).push_set();
	CHECKX(                 0xcc8888cc, Tx.IoPad(3).read_set() );
	CHECKX(                 0xcc8888cc, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43d", "IoPad(3).push_clr()" );
    try {
	Tx.IoPad(3).write_clr(  0xf0f0f0f0 );
	CHECKX(                 0xf0f0f0f0, Tx.IoPad(3).read_clr() );
	Tx.IoPad(3).put(        0xdd9999dd );
	Tx.IoPad(3).push_clr();
	CHECKX(                 0xdd9999dd, Tx.IoPad(3).read_clr() );
	CHECKX(                 0xdd9999dd, Tx.IoPad(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

  CASE( "60", "condition memory" );
    try {
	Tx.IoPad(3).write(      0xffffffff );
	Tx.IoPad(3).write_flip( 0xffffffff );
	Tx.IoPad(3).write_set(  0xffffffff );
	Tx.IoPad(3).write_clr(  0xffffffff );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_peek() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_set() );
	CHECKX(                 0xffffffff, Tx.IoPad(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Example heavy-weight bit-field test.

  CASE( "61a", "get_OutDisable_1()" );
    try {
	Tx.IoPad(3).put(     0x00000000 );
	CHECKX(              0x00000000, Tx.IoPad(3).get() );
	Tx.IoPad(3).put_OutDisable_1( 1 );
	CHECKX(              0x00000080, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_OutDisable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "get_OutDisable_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	Tx.IoPad(3).put_OutDisable_1( 0 );
	CHECKX(              0xffffff7f, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_OutDisable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "put_OutDisable_1() bad value" );
    try {
	Tx.IoPad(3).put_OutDisable_1( 2 );
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

  CASE( "62b", "get_InEnable_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_InEnable_1() );
	Tx.IoPad(3).put_InEnable_1(   0 );
	CHECKX(              0xffffffbf, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_InEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "get_DriveStr_2()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       3, Tx.IoPad(3).get_DriveStr_2() );
	Tx.IoPad(3).put_DriveStr_2(   0 );
	CHECKX(              0xffffffcf, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_DriveStr_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_PullUpEn_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_PullUpEn_1() );
	Tx.IoPad(3).put_PullUpEn_1(   0 );
	CHECKX(              0xfffffff7, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_PullUpEn_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "get_PullDnEn_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_PullDnEn_1() );
	Tx.IoPad(3).put_PullDnEn_1(   0 );
	CHECKX(              0xfffffffb, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_PullDnEn_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "get_HystEn_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_HystEn_1() );
	Tx.IoPad(3).put_HystEn_1(     0 );
	CHECKX(              0xfffffffd, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_HystEn_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "get_SlewFast_1()" );
    try {
	Tx.IoPad(3).put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.IoPad(3).get() );
	CHECKX(                       1, Tx.IoPad(3).get_SlewFast_1() );
	Tx.IoPad(3).put_SlewFast_1(   0 );
	CHECKX(              0xfffffffe, Tx.IoPad(3).get() );
	CHECKX(                       0, Tx.IoPad(3).get_SlewFast_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "70b", "get_LowVolt_1()" );
    try {
	Tx.IoVoltage.put(    0xffffffff );
	CHECKX(              0xffffffff, Tx.IoVoltage.get() );
	CHECKX(                       1, Tx.IoVoltage.get_LowVolt_1() );
	Tx.IoVoltage.put_LowVolt_1(   0 );
	CHECKX(              0xfffffffe, Tx.IoVoltage.get() );
	CHECKX(                       0, Tx.IoVoltage.get_LowVolt_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

