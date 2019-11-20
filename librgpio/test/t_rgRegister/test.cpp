// 2019-11-12  William A. Hudson
//
// Testing:  rgRegister  base class.
//    10-19  Constructor, addr()
//    20-29  Register read(), write(), put(), get(), grab(), push()
//    30-39  Generic get_field();  using Dx
//    40-49  Generic put_field();  using Dx
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRegister.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

// Test class for generic field accessors, which are protected.
class tcReg : public rgRegister {
  public:
    inline uint32_t	get_field(
	const uint32_t		pos,
	const uint32_t		mask
    ) { return  rgRegister::get_field( pos, mask ); };

    inline void		put_field(
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    ) {         rgRegister::put_field( pos, mask, val ); };
};

rgRegister		Tx;		// test object, the base class
tcReg			Dx;		// derived register
uint32_t		Hreg;		// simulate hardware register

Tx.init_addr( &Hreg );
Dx.init_addr( &Hreg );

//--------------------------------------------------------------------------
//## Constructor, addr()
//--------------------------------------------------------------------------

  CASE( "10", "constructor init to zero" );
    try {
	rgRegister		tx;
	volatile uint32_t*	vp;
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
	volatile uint32_t*	vp;
	vp = Tx.addr();
	CHECKX( 0x00000000, (vp - &Hreg) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "addr() of register Dx" );
    try {
	volatile uint32_t*	vp;
	vp = Dx.addr();
	CHECKX( 0x00000000, (vp - &Hreg) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register read(), write(), put(), get(), grab(), push();  using Tx
//--------------------------------------------------------------------------

  CASE( "20", "init for wrong access" );
    try {
	Hreg =    0x3c3c3c3c;
	Tx.put(   0x3c3c3c3c );
	Dx.put(   0x3c3c3c3c );
	CHECKX(   0x3c3c3c3c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "read()" );
    try {
	Hreg =    0x00000000;
	CHECKX(   0x00000000, Tx.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "read()" );
    try {
	Hreg =    0xffffffff;
	CHECKX(   0xffffffff, Tx.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "write()" );
    try {
	Tx.write( 0x00000000 );
	CHECKX(   0x00000000, Hreg );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "write()" );
    try {
	Tx.write( 0xffffffff );
	CHECKX(   0xffffffff, Hreg );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "put(), get()" );
    try {
	Hreg =    0x00000000;
	Tx.put(   0xffffffff );
	CHECKX(   0x00000000, Hreg );
	CHECKX(   0xffffffff, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "put(), get()" );
    try {
	Hreg =    0xffffffff;
	Tx.put(   0x00000000 );
	CHECKX(   0xffffffff, Hreg );
	CHECKX(   0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "grab()" );
    try {
	Hreg =    0x00000000;
	Tx.put(   0xffffffff );
	CHECKX(   0xffffffff, Tx.get() );
	Tx.grab();
	CHECKX(   0x00000000, Hreg );
	CHECKX(   0x00000000, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "grab()" );
    try {
	Hreg =    0xffffffff;
	Tx.put(   0x00000000 );
	CHECKX(   0x00000000, Tx.get() );
	Tx.grab();
	CHECKX(   0xffffffff, Hreg );
	CHECKX(   0xffffffff, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "push()" );
    try {
	Hreg =    0x00000000;
	Tx.put(   0x5555cccc );
	CHECKX(   0x5555cccc, Tx.get() );
	Tx.push();
	CHECKX(   0x5555cccc, Hreg );
	CHECKX(   0x5555cccc, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "push()" );
    try {
	Hreg =    0x00000000;
	Tx.put(   0xaaaa3333 );
	CHECKX(   0xaaaa3333, Tx.get() );
	Tx.push();
	CHECKX(   0xaaaa3333, Hreg );
	CHECKX(   0xaaaa3333, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic get_field();  using Dx
//--------------------------------------------------------------------------

  CASE( "30", "init for get_field()" );
    try {
	Hreg =    0x00000000;
	CHECKX(   0x00000000, Hreg );
	Tx.put(   0x00000000 );		// help avoid accidents
	Dx.put(   0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "get_field() 1-bit" );
    try {
	Dx.put(   0x00000001 );
	CHECKX(   0x00000001, Dx.get_field(  0, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "get_field() 1-bit" );
    try {
	Dx.put(   0x80000000 );
	CHECKX(   0x00000001, Dx.get_field( 31, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31c", "get_field() 1-bit" );
    try {
	Dx.put(   0x00000400 );
	CHECKX(   0x00000001, Dx.get_field( 10, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31d", "get_field() 1-bit" );
    try {
	Dx.put(   0xfffffbff );
	CHECKX(   0x00000000, Dx.get_field( 10, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "get_field() 2-bit" );
    try {
	Dx.put(   0x00000003 );
	CHECKX(   0x00000003, Dx.get_field(  0, 0x3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "get_field() 2-bit" );
    try {
	Dx.put(   0x000c0000 );
	CHECKX(   0x00000003, Dx.get_field( 18, 0x3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "get_field() 8-bit" );
    try {
	Dx.put(   0x000000ff );
	CHECKX(   0x000000ff, Dx.get_field(  0, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33b", "get_field() 8-bit" );
    try {
	Dx.put(   0x3fc00000 );
	CHECKX(   0x000000ff, Dx.get_field( 22, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33c", "get_field() 8-bit" );
    try {
	Dx.put(   0xf0f0fa50 );
	CHECKX(   0x000000a5, Dx.get_field(  4, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "34a", "get_field() 31-bit" );
    try {
	Dx.put(   0xfffffffe );
	CHECKX(   0x7fffffff, Dx.get_field(  1, 0x7fffffff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "35a", "get_field() pos too large" );
    try {
	Dx.put(   0xffff0000 );
	CHECKX(   0x00007fff, Dx.get_field( 17, 0x0000ffff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic put_field();  using Dx
//--------------------------------------------------------------------------

  CASE( "40", "init for put_field()" );
    try {
	Hreg =    0x00000000;
	CHECKX(   0x00000000, Hreg );
	Tx.put(   0x00000000 );		// help avoid accidents
	Dx.put(   0x00000000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "put_field() 8-bit" );
    try {
	Dx.put(   0x00000000 );
	CHECKX(   0x00000000, Dx.get() );
	Dx.put_field(  8, 0x000000ff, 0x000000a7 );
	CHECKX(   0x0000a700, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "put_field() 8-bit" );
    try {
	Dx.put(   0xffffffff );
	CHECKX(   0xffffffff, Dx.get() );
	Dx.put_field(  8, 0x000000ff, 0x000000a7 );
	CHECKX(   0xffffa7ff, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "put_field() 1-bit" );
    try {
	Dx.put(   0x00000000 );
	CHECKX(   0x00000000, Dx.get() );
	Dx.put_field(  9, 0x00000001, 0x00000001 );
	CHECKX(   0x00000200, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "put_field() 1-bit" );
    try {
	Dx.put(   0xffffffff );
	CHECKX(   0xffffffff, Dx.get() );
	Dx.put_field(  9, 0x00000001, 0x00000000 );
	CHECKX(   0xfffffdff, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "put_field() fault" );
    try {
	Dx.put(   0x00000000 );
	CHECKX(   0x00000000, Dx.get() );
	Dx.put_field(  9, 0x00000001, 0x00000002 );
	FAIL( "no throw" );
    }
    catch (...) {
	CHECKX(   0x00000000, Dx.get() );
    }

//#!!  Add throw message capture

//--------------------------------------
  CASE( "99", "Done" );
}

