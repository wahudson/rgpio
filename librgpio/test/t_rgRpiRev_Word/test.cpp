// 2020-12-03  William A. Hudson
//
// Testing:  rgWord class in rgRpiRev.h - Raspberry Pi Revision
//    10-19  Constructor, get(), put(), mark_final(), clear_final()
//    20-29  Generic get_field(), using Dx
//    30-39  Generic put_field(), using Dx
//    40-49  .
//    50-59  .
//    60-68  .
//    70-78  .
//    80-89  .
//    90-99  .
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"		// contains rgWord

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

// Test class for generic field accessors, which are protected.
class tcWord : public rgWord {
  public:
    inline uint32_t	get_field(
	const uint32_t		pos,
	const uint32_t		mask
    ) { return  rgWord::get_field( pos, mask ); };

    inline void		put_field(
	const uint32_t		pos,
	const uint32_t		mask,
	const uint32_t		val
    ) {         rgWord::put_field( pos, mask, val ); };
};

rgWord			Tx;		// test object, the base class
tcWord			Dx;		// derived object

//--------------------------------------------------------------------------
//## Constructor, get(), put(), mark_final(), clear_final()
//--------------------------------------------------------------------------

  CASE( "10", "constructor init to zero" );
    try {
	rgWord	tx;
	CHECKX( 0x00000000, tx.get() );
	CHECK(  0,          tx.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "get() as initialized" );
    try {
	CHECKX( 0x00000000, Tx.get() );
	CHECK(  0,          Tx.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "put()" );
    try {
	CHECK(  0,          Tx.is_final() );
	CHECKX( 0x00000000, Tx.get() );
	Tx.put( 0xabba5225 );
	CHECK(  0,          Tx.is_final() );
	CHECKX( 0xabba5225, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "mark_final()" );
    try {
	CHECK(  0,          Tx.is_final() );
	CHECKX( 0xabba5225, Tx.get() );
	Tx.mark_final();
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xabba5225, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "put() not limited by Final" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xabba5225, Tx.get() );
	Tx.put( 0xf00fc33c );
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "clear_final()" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xf00fc33c, Tx.get() );
	Tx.clear_final();
	CHECK(  0,          Tx.is_final() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic get_field(), using Dx
//--------------------------------------------------------------------------

  CASE( "20", "init for get_field()" );
    try {
	Dx.put( 0x00000000 );
	Dx.clear_final();
	CHECKX( 0x00000000, Dx.get() );
	CHECK(  0,          Dx.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "get_field() 1-bit" );
    try {
	Dx.put( 0x00000001 );
	CHECKX( 0x00000001, Dx.get_field(  0, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "get_field() 1-bit" );
    try {
	Dx.put( 0x80000000 );
	CHECKX( 0x00000001, Dx.get_field( 31, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "get_field() 1-bit" );
    try {
	Dx.put( 0x00000400 );
	CHECKX( 0x00000001, Dx.get_field( 10, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21d", "get_field() 1-bit" );
    try {
	Dx.put( 0xfffffbff );
	CHECKX( 0x00000000, Dx.get_field( 10, 0x1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "get_field() 2-bit" );
    try {
	Dx.put( 0x00000003 );
	CHECKX( 0x00000003, Dx.get_field(  0, 0x3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "get_field() 2-bit" );
    try {
	Dx.put( 0x000c0000 );
	CHECKX( 0x00000003, Dx.get_field( 18, 0x3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "get_field() 8-bit" );
    try {
	Dx.put( 0x000000ff );
	CHECKX( 0x000000ff, Dx.get_field(  0, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "get_field() 8-bit" );
    try {
	Dx.put( 0x3fc00000 );
	CHECKX( 0x000000ff, Dx.get_field( 22, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23c", "get_field() 8-bit" );
    try {
	Dx.put( 0xf0f0fa50 );
	CHECKX( 0x000000a5, Dx.get_field(  4, 0xff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "get_field() 31-bit" );
    try {
	Dx.put( 0xfffffffe );
	CHECKX( 0x7fffffff, Dx.get_field(  1, 0x7fffffff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25a", "get_field() pos too large" );
    try {
	Dx.put( 0xffff0000 );
	CHECKX( 0x00007fff, Dx.get_field( 17, 0x0000ffff ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic put_field(), using Dx
//--------------------------------------------------------------------------

  CASE( "30", "init for put_field()" );
    try {
	Dx.put( 0x00000000 );
	Dx.clear_final();
	CHECKX( 0x00000000, Dx.get() );
	CHECK(  0,          Dx.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "31a", "put_field() 8-bit" );
    try {
	Dx.put( 0x00000000 );
	CHECKX( 0x00000000, Dx.get() );
	Dx.put_field(  8, 0x000000ff, 0x000000a7 );
	CHECKX( 0x0000a700, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "put_field() 8-bit" );
    try {
	Dx.put( 0xffffffff );
	CHECKX( 0xffffffff, Dx.get() );
	Dx.put_field(  8, 0x000000ff, 0x000000a7 );
	CHECKX( 0xffffa7ff, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "put_field() 1-bit" );
    try {
	Dx.put( 0x00000000 );
	CHECKX( 0x00000000, Dx.get() );
	Dx.put_field(  9, 0x00000001, 0x00000001 );
	CHECKX( 0x00000200, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "put_field() 1-bit" );
    try {
	Dx.put( 0xffffffff );
	CHECKX( 0xffffffff, Dx.get() );
	Dx.put_field(  9, 0x00000001, 0x00000000 );
	CHECKX( 0xfffffdff, Dx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "put_field() value too large" );
    try {
	Dx.put( 0xabba1111 );
	CHECKX( 0xabba1111, Dx.get() );
	Dx.put_field(  9, 0x000000ff, 0x000001c3 );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error e ) {
	CHECKX( 0xabba1111, Dx.get() );
	CHECK( "rgWord::put_field():  value exceeds 0xff:  0x1c3", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

