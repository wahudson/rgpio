// 2024-05-24  William A. Hudson
//
// Testing:  rgsRio - Register Input/Output (RIO) class for RPi5
//    10-19  Constructor, get_bcm_address(), rgsRegAtom register constructor
//    20-29  .
//    30-39  Hardware read(), write():  _peek _flip _set _clr
//    40-49  Object grab(), push():  _peek _flip _set _clr
//    41     Object get(), put()
//    50-59  .
//    60-78  .
//    80-98  .
// Test only Bank0, as the others are undocumented.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgsRio.h"

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

rgsRio			Tx   ( &Bx );		// test object, Bank0

//--------------------------------------------------------------------------
//## Constructor, get_bcm_address(), rgsRegAtom register constructor
//--------------------------------------------------------------------------

  CASE( "10a", "rgsRio constructor, default Bank0" );
    try {
	rgsRio		tx  ( &Bx );
	CHECK(           0, tx.get_bank_num() );
	CHECKX( 0x400e0000, tx.get_bcm_address() );
	CHECKX( 0x400e0000, tx.get_doc_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "rgsRio constructor Bank2" );
    try {
	rgsRio		tx  ( &Bx, 2 );
	CHECK(           2, tx.get_bank_num() );
	CHECKX( 0x400e0000, tx.get_bcm_address() );
	CHECKX( 0x400e8000, tx.get_doc_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "rgsRio constructor Bank3 exception" );
    try {
	rgsRio		tx  ( &Bx, 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgsRio::  require bank in {0,1,2}",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Bank0" );
    try {
	CHECKX( 0x400e0000, Tx.get_bcm_address() );
	CHECKX( 0x400e0000, Tx.get_doc_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register addresses

  CASE( "12a", "RioOut.addr() Bank0" );
    try {
	CHECKX( 0x00000000, Tx.get_doc_offset( Tx.RioOut.addr() )      );
	CHECKX( 0x00001000, Tx.get_doc_offset( Tx.RioOut.addr_flip() ) );
	CHECKX( 0x00002000, Tx.get_doc_offset( Tx.RioOut.addr_set() )  );
	CHECKX( 0x00003000, Tx.get_doc_offset( Tx.RioOut.addr_clr() )  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "RioOutEn.addr() Bank0" );
    try {
	CHECKX( 0x00000004, Tx.get_doc_offset( Tx.RioOutEn.addr() )      );
	CHECKX( 0x00001004, Tx.get_doc_offset( Tx.RioOutEn.addr_flip() ) );
	CHECKX( 0x00002004, Tx.get_doc_offset( Tx.RioOutEn.addr_set() )  );
	CHECKX( 0x00003004, Tx.get_doc_offset( Tx.RioOutEn.addr_clr() )  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12c", "RioIn.addr() Bank0" );
    try {
	CHECKX( 0x00000008, Tx.get_doc_offset( Tx.RioIn.addr() )      );
	CHECKX( 0x00001008, Tx.get_doc_offset( Tx.RioIn.addr_flip() ) );
	CHECKX( 0x00002008, Tx.get_doc_offset( Tx.RioIn.addr_set() )  );
	CHECKX( 0x00003008, Tx.get_doc_offset( Tx.RioIn.addr_clr() )  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12d", "RioInSync.addr() Bank0" );
    try {
	CHECKX( 0x0000000c, Tx.get_doc_offset( Tx.RioInSync.addr() )      );
	CHECKX( 0x0000100c, Tx.get_doc_offset( Tx.RioInSync.addr_flip() ) );
	CHECKX( 0x0000200c, Tx.get_doc_offset( Tx.RioInSync.addr_set() )  );
	CHECKX( 0x0000300c, Tx.get_doc_offset( Tx.RioInSync.addr_clr() )  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register Copy constructor.

  CASE( "15a", "RioOut register copy constructor" );
    try {
	rgsRegAtom		rx  ( Tx.RioOut );
	CHECKX( 0x00000000, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x00001000, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x00002000, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x00003000, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15b", "RioOutEn register copy constructor" );
    try {
	rgsRegAtom		rx  ( Tx.RioOutEn );
	CHECKX( 0x00000004, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x00001004, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x00002004, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x00003004, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15c", "RioIn register copy constructor" );
    try {
	rgsRegAtom		rx  ( Tx.RioIn );
	CHECKX( 0x00000008, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x00001008, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x00002008, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x00003008, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15d", "RioInSync register copy constructor" );
    try {
	rgsRegAtom		rx  ( Tx.RioInSync );
	CHECKX( 0x0000000c, Tx.get_doc_offset( rx.addr()      ) );
	CHECKX( 0x0000100c, Tx.get_doc_offset( rx.addr_flip() ) );
	CHECKX( 0x0000200c, Tx.get_doc_offset( rx.addr_set()  ) );
	CHECKX( 0x0000300c, Tx.get_doc_offset( rx.addr_clr()  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor fail on RPi4 or earlier

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2711 );    // RPi4

  CASE( "17", "rgsRio constructor" );
    try {
	rgsRio		tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( domain_error& e ) {
	CHECK( "rgsRio::  require RPi5 (soc_BCM2712)",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );    // RPi5

//--------------------------------------------------------------------------
//## Hardware read(), write():  _peek _flip _set _clr
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "31", "RioOut write(), read()" );
    try {
	Tx.RioOut.write(         0x11111111 );
	Tx.RioOut.write_flip(    0x22222222 );
	Tx.RioOut.write_set(     0x33333333 );
	Tx.RioOut.write_clr(     0x44444444 );
	CHECKX(                  0x11111111, Tx.RioOut.read() );
	CHECKX(                  0x22222222, Tx.RioOut.read_peek() );
	CHECKX(                  0x33333333, Tx.RioOut.read_set() );
	CHECKX(                  0x44444444, Tx.RioOut.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "RioInSync write(), read()" );
    try {
	Tx.RioInSync.write(      0x332222cc );
	Tx.RioInSync.write_flip( 0x334444cc );
	Tx.RioInSync.write_set(  0x336666cc );
	Tx.RioInSync.write_clr(  0x338888cc );
	CHECKX(                  0x332222cc, Tx.RioInSync.read() );
	CHECKX(                  0x334444cc, Tx.RioInSync.read_peek() );
	CHECKX(                  0x336666cc, Tx.RioInSync.read_set() );
	CHECKX(                  0x338888cc, Tx.RioInSync.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object grab(), push():  _peek _flip _set _clr
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "40a", "RioOut condition memory" );
    try {
	Tx.RioOut.write(         0xff1111ff );
	Tx.RioOut.write_flip(    0xff2222ff );
	Tx.RioOut.write_set(     0xff3333ff );
	Tx.RioOut.write_clr(     0xff4444ff );
	CHECKX(                  0xff1111ff, Tx.RioOut.read() );
	CHECKX(                  0xff2222ff, Tx.RioOut.read_peek() );
	CHECKX(                  0xff3333ff, Tx.RioOut.read_set() );
	CHECKX(                  0xff4444ff, Tx.RioOut.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "RioOut.get(), put()" );
    try {
	Tx.RioOut.put(       0x5555aaaa );
	CHECKX(              0x5555aaaa, Tx.RioOut.get() );
	Tx.RioOut.put(       0xbbbb6666 );
	CHECKX(              0xbbbb6666, Tx.RioOut.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "RioOut.grab()" );
    try {
	Tx.RioOut.put(       0xf0f0f0f0 );
	CHECKX(              0xf0f0f0f0, Tx.RioOut.get() );
	Tx.RioOut.grab();
	CHECKX(              0xff1111ff, Tx.RioOut.get() );
	Tx.RioOut.grab_peek();
	CHECKX(              0xff2222ff, Tx.RioOut.get() );
	Tx.RioOut.grab_set();
	CHECKX(              0xff3333ff, Tx.RioOut.get() );
	Tx.RioOut.grab_clr();
	CHECKX(              0xff4444ff, Tx.RioOut.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "RioOut.push()" );
    try {
	Tx.RioOut.put(           0xaa6666aa );
	Tx.RioOut.push();
	CHECKX(                  0xaa6666aa, Tx.RioOut.read() );
	Tx.RioOut.put(           0xbb7777bb );
	Tx.RioOut.push_flip();
	CHECKX(                  0xbb7777bb, Tx.RioOut.read_peek() );
	Tx.RioOut.put(           0xcc8888cc );
	Tx.RioOut.push_set();
	CHECKX(                  0xcc8888cc, Tx.RioOut.read_set() );
	Tx.RioOut.put(           0xdd9999dd );
	Tx.RioOut.push_clr();
	CHECKX(                  0xdd9999dd, Tx.RioOut.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

