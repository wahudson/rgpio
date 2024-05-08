// 2024-05-03  William A. Hudson
//
// Testing:  rgsRegAtom - Atomic Register base class for RPi5
//    10-19  Constructor, addr(), addr_flip(), addr_set(), addr_clr()
//    20-29  Register put(), get()
//    30-39  Register read(), write():  _peek _flip _set _clear
//    40-49  Register grab(), push():  _peek _flip _set _clr
//    50-59  .
//    60-98  .
//
// Testing to fake memory, not real hardware operation.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRegister.h"
#include "rgsRegAtom.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgsRegAtom		Tx;		// test object, the base class
volatile uint32_t	MBlock[0x1000];	// fake memory block (16 kByte)

Tx.init_addr( MBlock );

//--------------------------------------------------------------------------
//## Constructor, addr(), addr_flip(), addr_set(), addr_clr()
//--------------------------------------------------------------------------

  CASE( "10", "constructor init to zero" );
    try {
	rgsRegAtom		tx;
	volatile uint32_t*      vp;
	vp = tx.addr();
	CHECKX( 0x00000000, (vp - (volatile uint32_t*) 0) );	// funky cast
	CHECKX( 0x00000000, tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11a", "addr() of register Tx" );
    try {
	volatile uint32_t*      vp;
	vp = Tx.addr();
	CHECKX( 0x00000000, (vp - MBlock) );
	CHECKX( 0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "addr_flip() of register Tx" );
    try {
	CHECKX( 0x00000400, (Tx.addr_flip() - MBlock) );
	CHECKX( 0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11c", "addr_set() of register Tx" );
    try {
	CHECKX( 0x00000800, (Tx.addr_set() - MBlock) );
	CHECKX( 0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11d", "addr_clr() of register Tx" );
    try {
	CHECKX( 0x00000c00, (Tx.addr_clr() - MBlock) );
	CHECKX( 0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register put(), get()
//--------------------------------------------------------------------------

  CASE( "20", "setup MBlock" );
    try {
	CHECKX( 0x00000000, MBlock[0x000] );
	CHECKX( 0x00000000, MBlock[0x400] );
	CHECKX( 0x00000000, MBlock[0x800] );
	CHECKX( 0x00000000, MBlock[0xc00] );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "put()" );
    try {
	Tx.put( 0x55aa00ff );
	CHECKX( 0x55aa00ff, Tx.get() );
	Tx.put( 0xff00aa55 );
	CHECKX( 0xff00aa55, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "29", "verify MBlock unchanged" );
    try {
	CHECKX( 0x00000000, MBlock[0x000] );
	CHECKX( 0x00000000, MBlock[0x400] );
	CHECKX( 0x00000000, MBlock[0x800] );
	CHECKX( 0x00000000, MBlock[0xc00] );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register read(), write():  _peek _flip _set _clear
//--------------------------------------------------------------------------

  CASE( "30", "read() setup" );
    try {
	CHECKX(        0x00000000, Tx.read() );
	CHECKX(        0x00000000, Tx.read_peek() );
	CHECKX(        0x00000000, Tx.read_set() );
	CHECKX(        0x00000000, Tx.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "read()" );
    try {
	MBlock[0x000] = 0x00ff4444;
	MBlock[0x400] = 0x11ff5555;
	MBlock[0x800] = 0x22ff6666;
	MBlock[0xc00] = 0x33ff7777;
	CHECKX(         0x00ff4444, Tx.read() );
	CHECKX(         0x11ff5555, Tx.read_peek() );
	CHECKX(         0x22ff6666, Tx.read_set() );
	CHECKX(         0x33ff7777, Tx.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "write()" );
    try {
	Tx.write(       0x3333bbbb );
	Tx.write_flip(  0x4444cccc );
	Tx.write_set(   0x5555dddd );
	Tx.write_clr(   0x6666eeee );
	CHECKX(         0x3333bbbb, MBlock[0x000] );
	CHECKX(         0x4444cccc, MBlock[0x400] );
	CHECKX(         0x5555dddd, MBlock[0x800] );
	CHECKX(         0x6666eeee, MBlock[0xc00] );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "write(), read()" );
    try {
	Tx.write(       0x1111aaaa );
	Tx.write_flip(  0x2222bbbb );
	Tx.write_set(   0x3333cccc );
	Tx.write_clr(   0x4444dddd );
	CHECKX(         0x1111aaaa, Tx.read() );
	CHECKX(         0x2222bbbb, Tx.read_peek() );
	CHECKX(         0x3333cccc, Tx.read_set() );
	CHECKX(         0x4444dddd, Tx.read_clr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register grab(), push():  _peek _flip _set _clr
//--------------------------------------------------------------------------

  CASE( "41", "grab()" );
    try {
	MBlock[0x000] = 0x00ff4444;
	MBlock[0x400] = 0x11ff5555;
	MBlock[0x800] = 0x22ff6666;
	MBlock[0xc00] = 0x33ff7777;
	Tx.put(         0x22222222 );
	CHECKX(         0x22222222, Tx.get() );
	Tx.grab();
	CHECKX(         0x00ff4444, Tx.get() );
	Tx.grab_peek();
	CHECKX(         0x11ff5555, Tx.get() );
	Tx.grab_set();
	CHECKX(         0x22ff6666, Tx.get() );
	Tx.grab_clr();
	CHECKX(         0x33ff7777, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "push()" );
    try {
	MBlock[0x000] = 0xffffffff;
	MBlock[0x400] = 0xffffffff;
	MBlock[0x800] = 0xffffffff;
	MBlock[0xc00] = 0xffffffff;
	CHECKX(         0xffffffff, MBlock[0x000] );
	Tx.put(         0x22222222 );
	Tx.push();
	CHECKX(         0xffffffff, MBlock[0x400] );
	Tx.put(         0x33333333 );
	Tx.push_flip();
	CHECKX(         0xffffffff, MBlock[0x800] );
	Tx.put(         0x44444444 );
	Tx.push_set();
	CHECKX(         0xffffffff, MBlock[0xc00] );
	Tx.put(         0x55555555 );
	Tx.push_clr();
	CHECKX(         0x22222222, MBlock[0x000] );
	CHECKX(         0x33333333, MBlock[0x400] );
	CHECKX(         0x44444444, MBlock[0x800] );
	CHECKX(         0x55555555, MBlock[0xc00] );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

