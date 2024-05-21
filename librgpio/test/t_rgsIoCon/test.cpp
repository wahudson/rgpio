// 2024-05-17  William A. Hudson
//
// Testing:  rgsIoCon  IO Control/Status Interface class for RPi5
//    10-19  Constructor, get_bcm_address(), rgsIo_Cntl register constructor
//    20-29  Register accessor IoCntl(), IoStat():  addr() _flip _set _clr
//    30-39  Hardware read(), write():  _peek _flip _set _clr
//    40-49  Object grab(), push():  _peek _flip _set _clr
//    41     Object get(), put()
//    50-59  .
//    60-78  IoCntl(3) Field Accessors  get_(), put_()
//    80-98  IoStat(3) Field Accessors  get_(), put_()
// Test only Bank0, as the others are undocumented.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgsIoCon.h"

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

rgsIoCon		Tx   ( &Bx );		// test object, Bank0

//--------------------------------------------------------------------------
//## Constructor, get_bcm_address(), rgsIo_Cntl register constructor
//--------------------------------------------------------------------------

  CASE( "10a", "rgsIoCon constructor, default Bank0" );
    try {
	rgsIoCon	tx  ( &Bx );
	CHECK(           0, tx.get_bank_num() );
	CHECKX( 0x400d0000, tx.get_bcm_address() );
	CHECKX( 0x400d0000, tx.get_doc_address() );
	CHECK(          27, tx.get_MaxBit() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "rgsIoCon constructor Bank2" );
    try {
	rgsIoCon	tx  ( &Bx, 2 );
	CHECK(           2, tx.get_bank_num() );
	CHECKX( 0x400d0000, tx.get_bcm_address() );
	CHECKX( 0x400d8000, tx.get_doc_address() );
	CHECK(          27, tx.get_MaxBit() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "rgsIoCon constructor Bank3 exception" );
    try {
	rgsIoCon	tx  ( &Bx, 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsIoCon::  require bank in {0,1,2}",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Bank0" );
    try {
	CHECKX( 0x400d0000, Tx.get_bcm_address() );
	CHECKX( 0x400d0000, Tx.get_doc_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12a", "rgsIo_Cntl register constructor init has NULL address" );
    try {
	rgsIo_Cntl	rx;
	CHECK( 1, rx.addr()      == NULL );
	CHECK( 0, rx.addr_flip() == NULL );
	CHECK( 0, rx.addr_set()  == NULL );
	CHECK( 0, rx.addr_clr()  == NULL );
	// Not a normal use of constructor.
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "rgsIo_Stat register constructor init has NULL address" );
    try {
	rgsIo_Stat	rx;
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
// Register Copy constructor initialization.

  CASE( "15", "rgsIo_Cntl register copy constructor" );
    try {
	rgsIo_Cntl		rx  ( Tx.IoCntl(0) );
	CHECKX( 0x00000004, (rx.addr()      - Tx.get_base_addr())*4 );
	CHECKX( 0x00001004, (rx.addr_flip() - Tx.get_base_addr())*4 );
	CHECKX( 0x00002004, (rx.addr_set()  - Tx.get_base_addr())*4 );
	CHECKX( 0x00003004, (rx.addr_clr()  - Tx.get_base_addr())*4 );
	// equivalent of get_doc_offset()
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15a", "rgsIo_Cntl register copy constructor" );
    try {
	rgsIo_Cntl		rx  ( Tx.IoCntl(0) );
	CHECKX( 0x00000004, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x00001004, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x00002004, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x00003004, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15b", "rgsIo_Stat register copy constructor" );
    try {
	rgsIo_Stat		rx  ( Tx.IoStat(0) );
	CHECKX( 0x00000000, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x00001000, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x00002000, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x00003000, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor fail on RPi4 or earlier

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2711 );    // RPi4

  CASE( "17", "rgsIoCon constructor" );
    try {
	rgsIoCon	tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( domain_error& e ) {
	CHECK( "rgsIoCon::  require RPi5 (soc_BCM2712)",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );    // RPi5

//--------------------------------------------------------------------------
//## Register accessor IoCntl(), IoStat():  addr() _flip _set _clr
//--------------------------------------------------------------------------

  CASE( "20a", "accessor IoCntl(0)" );
    try {
	CHECKX( 0x00000004, Tx.get_doc_offset( Tx.IoCntl(0).addr()      ) );
	CHECKX( 0x00001004, Tx.get_doc_offset( Tx.IoCntl(0).addr_flip() ) );
	CHECKX( 0x00002004, Tx.get_doc_offset( Tx.IoCntl(0).addr_set()  ) );
	CHECKX( 0x00003004, Tx.get_doc_offset( Tx.IoCntl(0).addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "accessor IoCntl(27)" );
    try {
	CHECKX( 0x000000dc, Tx.get_doc_offset( Tx.IoCntl(27).addr()      ) );
	CHECKX( 0x000010dc, Tx.get_doc_offset( Tx.IoCntl(27).addr_flip() ) );
	CHECKX( 0x000020dc, Tx.get_doc_offset( Tx.IoCntl(27).addr_set()  ) );
	CHECKX( 0x000030dc, Tx.get_doc_offset( Tx.IoCntl(27).addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20c", "accessor IoCntl(28)" );
    try {
	Tx.IoCntl(28);
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsIo_Cntl::IoCntl():  bit index out-of-range:  28",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "accessor IoStat(0)" );
    try {
	CHECKX( 0x00000000, Tx.get_doc_offset( Tx.IoStat(0).addr()      ) );
	CHECKX( 0x00001000, Tx.get_doc_offset( Tx.IoStat(0).addr_flip() ) );
	CHECKX( 0x00002000, Tx.get_doc_offset( Tx.IoStat(0).addr_set()  ) );
	CHECKX( 0x00003000, Tx.get_doc_offset( Tx.IoStat(0).addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "accessor IoStat(27)" );
    try {
	CHECKX( 0x000000d8, Tx.get_doc_offset( Tx.IoStat(27).addr()      ) );
	CHECKX( 0x000010d8, Tx.get_doc_offset( Tx.IoStat(27).addr_flip() ) );
	CHECKX( 0x000020d8, Tx.get_doc_offset( Tx.IoStat(27).addr_set()  ) );
	CHECKX( 0x000030d8, Tx.get_doc_offset( Tx.IoStat(27).addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "accessor IoStat(28)" );
    try {
	Tx.IoStat(28);
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsIo_Stat::IoStat():  bit index out-of-range:  28",
	    e.what()
	);
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
	Tx.IoCntl(3).write(      0xffff0000 );
	Tx.IoCntl(3).write_flip( 0xffff0000 );
	Tx.IoCntl(3).write_set(  0xffff0000 );
	Tx.IoCntl(3).write_clr(  0xffff0000 );
	CHECKX(                  0xffff0000, Tx.IoCntl(3).read() );
	CHECKX(                  0xffff0000, Tx.IoCntl(3).read_peek() );
	CHECKX(                  0xffff0000, Tx.IoCntl(3).read_set() );
	CHECKX(                  0xffff0000, Tx.IoCntl(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "IoCntl(3) write(), read()" );
    try {
	Tx.IoCntl(3).write(      0x332222cc );
	Tx.IoCntl(3).write_flip( 0x334444cc );
	Tx.IoCntl(3).write_set(  0x336666cc );
	Tx.IoCntl(3).write_clr(  0x338888cc );
	CHECKX(                  0x332222cc, Tx.IoCntl(3).read() );
	CHECKX(                  0x334444cc, Tx.IoCntl(3).read_peek() );
	CHECKX(                  0x336666cc, Tx.IoCntl(3).read_set() );
	CHECKX(                  0x338888cc, Tx.IoCntl(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "IoStat(3) write(), read()" );
    try {
	Tx.IoStat(3).write(      0xaa1111bb );
	Tx.IoStat(3).write_flip( 0xaa2222bb );
	Tx.IoStat(3).write_set(  0xaa3333bb );
	Tx.IoStat(3).write_clr(  0xaa4444bb );
	CHECKX(                  0xaa1111bb, Tx.IoStat(3).read() );
	CHECKX(                  0xaa2222bb, Tx.IoStat(3).read_peek() );
	CHECKX(                  0xaa3333bb, Tx.IoStat(3).read_set() );
	CHECKX(                  0xaa4444bb, Tx.IoStat(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "IoCntl(3) unchanged" );
    try {
	CHECKX(                  0x332222cc, Tx.IoCntl(3).read() );
	CHECKX(                  0x334444cc, Tx.IoCntl(3).read_peek() );
	CHECKX(                  0x336666cc, Tx.IoCntl(3).read_set() );
	CHECKX(                  0x338888cc, Tx.IoCntl(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object grab(), push():  _peek _flip _set _clr
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "40a", "IoCntl(3) condition memory" );
    try {
	Tx.IoCntl(3).write(      0xff1111ff );
	Tx.IoCntl(3).write_flip( 0xff2222ff );
	Tx.IoCntl(3).write_set(  0xff3333ff );
	Tx.IoCntl(3).write_clr(  0xff4444ff );
	CHECKX(                  0xff1111ff, Tx.IoCntl(3).read() );
	CHECKX(                  0xff2222ff, Tx.IoCntl(3).read_peek() );
	CHECKX(                  0xff3333ff, Tx.IoCntl(3).read_set() );
	CHECKX(                  0xff4444ff, Tx.IoCntl(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "40b", "IoStat(3) condition memory" );
    try {
	Tx.IoStat(3).write(      0x00111100 );
	Tx.IoStat(3).write_flip( 0x00222200 );
	Tx.IoStat(3).write_set(  0x00333300 );
	Tx.IoStat(3).write_clr(  0x00444400 );
	CHECKX(                  0x00111100, Tx.IoStat(3).read() );
	CHECKX(                  0x00222200, Tx.IoStat(3).read_peek() );
	CHECKX(                  0x00333300, Tx.IoStat(3).read_set() );
	CHECKX(                  0x00444400, Tx.IoStat(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "IoCntl(3).get(), put()" );
    try {
	Tx.IoCntl(3).put(    0x5555aaaa );
	CHECKX(              0x5555aaaa, Tx.IoCntl(3).get() );
	Tx.IoCntl(3).put(    0xbbbb6666 );
	CHECKX(              0xbbbb6666, Tx.IoCntl(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "IoStat(3).get(), put()" );
    try {
	Tx.IoStat(3).put(    0x5555aaaa );
	CHECKX(              0x5555aaaa, Tx.IoStat(3).get() );
	Tx.IoStat(3).put(    0xbbbb6666 );
	CHECKX(              0xbbbb6666, Tx.IoStat(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "IoCntl(3).grab()" );
    try {
	Tx.IoCntl(3).put(    0xf0f0f0f0 );
	CHECKX(              0xf0f0f0f0, Tx.IoCntl(3).get() );
	Tx.IoCntl(3).grab();
	CHECKX(              0xff1111ff, Tx.IoCntl(3).get() );
	Tx.IoCntl(3).grab_peek();
	CHECKX(              0xff2222ff, Tx.IoCntl(3).get() );
	Tx.IoCntl(3).grab_set();
	CHECKX(              0xff3333ff, Tx.IoCntl(3).get() );
	Tx.IoCntl(3).grab_clr();
	CHECKX(              0xff4444ff, Tx.IoCntl(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "IoStat(3).grab()" );
    try {
	Tx.IoStat(3).put(    0xf0f0f0f0 );
	CHECKX(              0xf0f0f0f0, Tx.IoStat(3).get() );
	Tx.IoStat(3).grab();
	CHECKX(              0x00111100, Tx.IoStat(3).get() );
	Tx.IoStat(3).grab_peek();
	CHECKX(              0x00222200, Tx.IoStat(3).get() );
	Tx.IoStat(3).grab_set();
	CHECKX(              0x00333300, Tx.IoStat(3).get() );
	Tx.IoStat(3).grab_clr();
	CHECKX(              0x00444400, Tx.IoStat(3).get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "IoCntl(3).push()" );
    try {
	Tx.IoCntl(3).put(        0xaa6666aa );
	Tx.IoCntl(3).push();
	CHECKX(                  0xaa6666aa, Tx.IoCntl(3).read() );
	Tx.IoCntl(3).put(        0xbb7777bb );
	Tx.IoCntl(3).push_flip();
	CHECKX(                  0xbb7777bb, Tx.IoCntl(3).read_peek() );
	Tx.IoCntl(3).put(        0xcc8888cc );
	Tx.IoCntl(3).push_set();
	CHECKX(                  0xcc8888cc, Tx.IoCntl(3).read_set() );
	Tx.IoCntl(3).put(        0xdd9999dd );
	Tx.IoCntl(3).push_clr();
	CHECKX(                  0xdd9999dd, Tx.IoCntl(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43b", "IoStat(3).push()" );
    try {
	Tx.IoStat(3).put(        0x33aaaa33 );
	Tx.IoStat(3).push();
	CHECKX(                  0x33aaaa33, Tx.IoStat(3).read() );
	Tx.IoStat(3).put(        0x33bbbb33 );
	Tx.IoStat(3).push_flip();
	CHECKX(                  0x33bbbb33, Tx.IoStat(3).read_peek() );
	Tx.IoStat(3).put(        0x22cccc22 );
	Tx.IoStat(3).push_set();
	CHECKX(                  0x22cccc22, Tx.IoStat(3).read_set() );
	Tx.IoStat(3).put(        0x22dddd22 );
	Tx.IoStat(3).push_clr();
	CHECKX(                  0x22dddd22, Tx.IoStat(3).read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## IoCntl(3) Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

  CASE( "61", "get_IrqOver_2()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            3, Tx.IoCntl(3).get_IrqOver_2() );
	Tx.IoCntl(3).put_IrqOver_2(       0 );
	CHECKX(                  0x3fffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_IrqOver_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62", "get_EdgeReset_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_EdgeReset_1() );
	Tx.IoCntl(3).put_EdgeReset_1(     0 );
	CHECKX(                  0xefffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_EdgeReset_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63", "get_ImaskFiltHigh_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskFiltHigh_1() );
	Tx.IoCntl(3).put_ImaskFiltHigh_1( 0 );
	CHECKX(                  0xf7ffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskFiltHigh_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64", "get_ImaskFiltLow_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskFiltLow_1() );
	Tx.IoCntl(3).put_ImaskFiltLow_1(  0 );
	CHECKX(                  0xfbffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskFiltLow_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65", "get_ImaskFiltRise_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskFiltRise_1() );
	Tx.IoCntl(3).put_ImaskFiltRise_1( 0 );
	CHECKX(                  0xfdffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskFiltRise_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66", "get_ImaskFiltFall_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskFiltFall_1() );
	Tx.IoCntl(3).put_ImaskFiltFall_1( 0 );
	CHECKX(                  0xfeffffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskFiltFall_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67", "get_ImaskHigh_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskHigh_1() );
	Tx.IoCntl(3).put_ImaskHigh_1(     0 );
	CHECKX(                  0xff7fffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskHigh_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "68", "get_ImaskLow_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskLow_1() );
	Tx.IoCntl(3).put_ImaskLow_1(      0 );
	CHECKX(                  0xffbfffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskLow_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "69", "get_ImaskRise_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskRise_1() );
	Tx.IoCntl(3).put_ImaskRise_1(     0 );
	CHECKX(                  0xffdfffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskRise_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70", "get_ImaskFall_1()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            1, Tx.IoCntl(3).get_ImaskFall_1() );
	Tx.IoCntl(3).put_ImaskFall_1(     0 );
	CHECKX(                  0xffefffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_ImaskFall_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71", "get_InOver_2()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            3, Tx.IoCntl(3).get_InOver_2() );
	Tx.IoCntl(3).put_InOver_2(        0 );
	CHECKX(                  0xfffcffff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_InOver_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72", "get_OutEnOver_2()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            3, Tx.IoCntl(3).get_OutEnOver_2() );
	Tx.IoCntl(3).put_OutEnOver_2(     0 );
	CHECKX(                  0xffff3fff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_OutEnOver_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73", "get_OutOver_2()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECK(                            3, Tx.IoCntl(3).get_OutOver_2() );
	Tx.IoCntl(3).put_OutOver_2(       0 );
	CHECKX(                  0xffffcfff, Tx.IoCntl(3).get() );
	CHECK(                            0, Tx.IoCntl(3).get_OutOver_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74", "get_FilterM_7()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECKX(                        0x7f, Tx.IoCntl(3).get_FilterM_7() );
	Tx.IoCntl(3).put_FilterM_7(       0 );
	CHECKX(                  0xfffff01f, Tx.IoCntl(3).get() );
	CHECKX(                        0x00, Tx.IoCntl(3).get_FilterM_7() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75", "get_FuncSel_5()" );
    try {
	Tx.IoCntl(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoCntl(3).get() );
	CHECKX(                        0x1f, Tx.IoCntl(3).get_FuncSel_5() );
	Tx.IoCntl(3).put_FuncSel_5(       0 );
	CHECKX(                  0xffffffe0, Tx.IoCntl(3).get() );
	CHECKX(                        0x00, Tx.IoCntl(3).get_FuncSel_5() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## IoStat(3) Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

  CASE( "81", "get_IrqToProc_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_IrqToProc_1() );
	Tx.IoStat(3).put_IrqToProc_1(     0 );
	CHECKX(                  0xdfffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_IrqToProc_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "get_IrqMasked_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_IrqMasked_1() );
	Tx.IoStat(3).put_IrqMasked_1(     0 );
	CHECKX(                  0xefffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_IrqMasked_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "get_InFiltHigh_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFiltHigh_1() );
	Tx.IoStat(3).put_InFiltHigh_1(    0 );
	CHECKX(                  0xf7ffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFiltHigh_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "84", "get_InFiltLow_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFiltLow_1() );
	Tx.IoStat(3).put_InFiltLow_1(     0 );
	CHECKX(                  0xfbffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFiltLow_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "85", "get_InFiltRise_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFiltRise_1() );
	Tx.IoStat(3).put_InFiltRise_1(    0 );
	CHECKX(                  0xfdffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFiltRise_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "86", "get_InFiltFall_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFiltFall_1() );
	Tx.IoStat(3).put_InFiltFall_1(    0 );
	CHECKX(                  0xfeffffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFiltFall_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "87", "get_InHigh_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InHigh_1() );
	Tx.IoStat(3).put_InHigh_1(        0 );
	CHECKX(                  0xff7fffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InHigh_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "88", "get_InLow_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InLow_1() );
	Tx.IoStat(3).put_InLow_1(         0 );
	CHECKX(                  0xffbfffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InLow_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "89", "get_InRise_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InRise_1() );
	Tx.IoStat(3).put_InRise_1(        0 );
	CHECKX(                  0xffdfffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InRise_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "90", "get_InFall_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFall_1() );
	Tx.IoStat(3).put_InFall_1(        0 );
	CHECKX(                  0xffefffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFall_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "91", "get_InToPeri_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InToPeri_1() );
	Tx.IoStat(3).put_InToPeri_1(      0 );
	CHECKX(                  0xfff7ffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InToPeri_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92", "get_InFiltered_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InFiltered_1() );
	Tx.IoStat(3).put_InFiltered_1(    0 );
	CHECKX(                  0xfffbffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InFiltered_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "93", "get_InOfPad_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InOfPad_1() );
	Tx.IoStat(3).put_InOfPad_1(       0 );
	CHECKX(                  0xfffdffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InOfPad_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "94", "get_InIsDirect_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_InIsDirect_1() );
	Tx.IoStat(3).put_InIsDirect_1(    0 );
	CHECKX(                  0xfffeffff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_InIsDirect_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "95", "get_OutEnToPad_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_OutEnToPad_1() );
	Tx.IoStat(3).put_OutEnToPad_1(    0 );
	CHECKX(                  0xffffdfff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_OutEnToPad_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "96", "get_OutEnOfPeri_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_OutEnOfPeri_1() );
	Tx.IoStat(3).put_OutEnOfPeri_1(   0 );
	CHECKX(                  0xffffefff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_OutEnOfPeri_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "97", "get_OutToPad_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_OutToPad_1() );
	Tx.IoStat(3).put_OutToPad_1(      0 );
	CHECKX(                  0xfffffdff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_OutToPad_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "98", "get_OutOfPeri_1()" );
    try {
	Tx.IoStat(3).put(        0xffffffff );
	CHECKX(                  0xffffffff, Tx.IoStat(3).get() );
	CHECK(                            1, Tx.IoStat(3).get_OutOfPeri_1() );
	Tx.IoStat(3).put_OutOfPeri_1(     0 );
	CHECKX(                  0xfffffeff, Tx.IoStat(3).get() );
	CHECK(                            0, Tx.IoStat(3).get_OutOfPeri_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

