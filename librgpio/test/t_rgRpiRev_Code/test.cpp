// 2020-12-03  William A. Hudson
//
// Testing:  rgRpiRev_Code class in rgRpiRev.h - Raspberry Pi Revision
//    10-19  Constructor, get(), put(), putFU() flags
//    20-29  override() flags
//    30-39  read_rev_code( istream* )
//    40-49  read_rev_code( "file" )
//    50-59  find()
//    60-79  Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <sstream>	// std::istringstream
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"		// contains rgRpiRev_Code

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgRpiRev_Code		Tx;		// test object

//--------------------------------------------------------------------------
//## Constructor, get(), put(), putFU() flags
//--------------------------------------------------------------------------

  CASE( "10", "rgRpiRev_Code constructor init to zero" );
    try {
	rgRpiRev_Code	tx;
	CHECKX( 0x00000000,     tx.get() );
	CHECK(  0,              tx.is_final() );
	CHECK(  1,              tx.is_unknown() );
	CHECK( "/proc/cpuinfo", tx.init_file() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "Tx as initialized" );
    try {
	CHECKX( 0x00000000,     Tx.get() );
	CHECK(  0,              Tx.is_final() );
	CHECK(  1,              Tx.is_unknown() );
	CHECK( "/proc/cpuinfo", Tx.init_file() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12a", "put() flags unchanged" );
    try {
	Tx.put( 0x00000000 );
	Tx.putFU( 0, 0 );
	CHECKX( 0x00000000, Tx.get() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	Tx.put( 0xf00fc33c );
	CHECK(  0,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12b", "put() flags unchanged" );
    try {
	Tx.put( 0xffffffff );
	Tx.putFU( 1, 1 );
	CHECKX( 0xffffffff, Tx.get() );
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	Tx.put( 0xf00fc33c );
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## override() flags
//--------------------------------------------------------------------------

  CASE( "21", "override()" );
    try {
	Tx.put( 0x00000000 );
	Tx.putFU( 0, 0 );
	CHECKX( 0x00000000, Tx.get() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	Tx.override( 0xf00fc33c );			// sets Final
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "override()" );
    try {
	Tx.put( 0xffffffff );
	Tx.putFU( 0, 1 );
	CHECKX( 0xffffffff, Tx.get() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	Tx.override( 0xf00fc33c );			// sets Final
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "override()" );
    try {
	Tx.put( 0xffffffff );
	Tx.putFU( 1, 0 );
	CHECKX( 0xffffffff, Tx.get() );
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	Tx.override( 0xf00fc33c );			// sets Final
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "override()" );
    try {
	Tx.put( 0x00000000 );
	Tx.putFU( 1, 1 );
	CHECKX( 0x00000000, Tx.get() );
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	Tx.override( 0xf00fc33c );			// sets Final
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
	CHECKX( 0xf00fc33c, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## read_rev_code( istream* )
//--------------------------------------------------------------------------

  CASE( "31", "read_rev_code() Revision on first line" );
    try {
	std::istringstream	iss  (
	    "Revision  : a22082\n"	// good
	    "hello     : foobar\n"
	);
	CHECKX( 0x00a22082, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "read_rev_code() misleading revision lines" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revisio   : abba00\n"	// incomplete
	    "Revision    baab00\n"	// no colon
	    "Revision  : a22082\n"	// good
	);
	CHECKX( 0x00a22082, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "read_rev_code() 0x hex prefix OK" );
    try {
	std::istringstream	iss  (
	    "Revision  : 0xabba4321\n"	// good
	    "hello     : foobar\n"
	);
	CHECKX( 0xabba4321, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "read_rev_code() minimal white space" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revision:a22082\n"
	);
	CHECKX( 0x00a22082, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "read_rev_code() overlook extra letters before colon" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revisionary   : baad00\n"	// extra letters
	    "chello    : bar\n"
	    "Revision  : a22082\n"	// good not found
	);
	CHECKX( 0x00baad00, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "read_rev_code() bad char in hex value" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revision  : a22082y\n"
	);
	Tx.read_rev_code( &iss );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "read_rev_code() bad line:  'Revision  : a22082y'", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "37", "read_rev_code() missing number" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revision  : \n"
	);
	Tx.read_rev_code( &iss );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "read_rev_code() bad line:  'Revision  : '", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38", "read_rev_code() out of range" );
    try {
	std::istringstream	iss  (
	    "hello     : foobar\n"
	    "Revision  : 0xffffffff7654321099\n"
	);
	Tx.read_rev_code( &iss );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "read_rev_code() bad line:  'Revision  : 0xffffffff7654321099'",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "read_rev_code() not found" );
    try {
	std::istringstream	iss  (
	    "Rev\n"
	);
	CHECKX( 0x00000000, Tx.read_rev_code( &iss ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## read_rev_code( "file" )
//--------------------------------------------------------------------------

  CASE( "40", "read_rev_code() Setup" );	// should have no effect
    try {
	Tx.put( 0xffffffff );
	Tx.putFU( 0, 1 );
	CHECKX( 0xffffffff, Tx.get() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "read_rev_code() file not exist" );
    try {
	Tx.read_rev_code( "not_exist" );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "read_rev_code() cannot open file:  not_exist", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "read_rev_code() empty file" );
    try {
	uint32_t	rc;
	rc = Tx.read_rev_code( "/dev/null" );
	CHECKX( 0x00000000, rc );
    }
    catch ( std::runtime_error& e ) {
	FAIL( "unexpected runtime_error" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "read_rev_code() good file" );
    try {
	uint32_t	rc;
	rc = Tx.read_rev_code( "ref/rpi3.in" );
	CHECKX( 0x00a22082, rc );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## find()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "50a", "find() null file" );
    try {
	Tx.init_file( "/dev/null" );
	Tx.put( 0xffabbaff );
	Tx.putFU( 0, 1 );
	CHECK( "/dev/null", Tx.init_file() );
	CHECKX( 0xffabbaff, Tx.get() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xffabbaff, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xffabbaff, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "find() second" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xffabbaff, Tx.get() );
	CHECKX( 0xffabbaff, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "51a", "find() no revision" );
    try {
	Tx.init_file( "ref/no_rev.in" );
	Tx.put( 0x0033cc77 );
	Tx.putFU( 0, 1 );
	CHECK( "ref/no_rev.in", Tx.init_file() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0x0033cc77, Tx.get() );
	CHECKX( 0x0033cc77, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0x0033cc77, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51b", "find() second" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0x0033cc77, Tx.get() );
	CHECKX( 0x0033cc77, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "find() good file" );
    try {
	Tx.init_file( "ref/rpi3.in" );
	Tx.put( 0xffffffff );
	Tx.putFU( 0, 1 );
	CHECK( "ref/rpi3.in", Tx.init_file() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xffffffff, Tx.get() );
	CHECKX( 0x00a22082, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );	// success
	CHECKX( 0x00a22082, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52b", "find() second" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0x00a22082, Tx.get() );
	CHECKX( 0x00a22082, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  0,          Tx.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "find() file not found" );
    try {
	Tx.init_file( "not_exist" );
	Tx.put( 0xabba5335 );
	Tx.putFU( 0, 1 );
	CHECK( "not_exist", Tx.init_file() );
	CHECK(  0,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xabba5335, Tx.get() );
	CHECKX( 0xabba5335, Tx.find() );	// action
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
	CHECKX( 0xabba5335, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53b", "find() second" );
    try {
	CHECK(  1,          Tx.is_final() );
	CHECKX( 0xabba5335, Tx.get() );
	CHECKX( 0xabba5335, Tx.find() );
	CHECK(  1,          Tx.is_final() );
	CHECK(  1,          Tx.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Using put_*(0) tests field width and position without a range exception.

//--------------------------------------
// Sample exception.

  CASE( "60c", "put_ChipNumber_4() bad value" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECKX(                 0xf, Tx.get_ChipNumber_4() );
	Tx.put_ChipNumber_4(   0x10 );
	FAIL( "no throw" );
    }
    catch ( std::range_error& e ) {
	CHECK( "rgWord::put_field():  value exceeds 0xf:  0x10", e.what() );
	CHECKX(          0xffffffff, Tx.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Light-weight bit-field test.

  CASE( "61b", "get_OverVoltageDis_1()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    1, Tx.get_OverVoltageDis_1() );
	Tx.put_OverVoltageDis_1(  0 );
	CHECKX(          0x7fffffff, Tx.get() );
	CHECK(                    0, Tx.get_OverVoltageDis_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62b", "get_OtpWriteDis_1()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    1, Tx.get_OtpWriteDis_1() );
	Tx.put_OtpWriteDis_1(     0 );
	CHECKX(          0xbfffffff, Tx.get() );
	CHECK(                    0, Tx.get_OtpWriteDis_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "get_OtpReadDis_1()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    1, Tx.get_OtpReadDis_1() );
	Tx.put_OtpReadDis_1(      0 );
	CHECKX(          0xdfffffff, Tx.get() );
	CHECK(                    0, Tx.get_OtpReadDis_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "get_WarrantyVoid_1()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    1, Tx.get_WarrantyVoid_1() );
	Tx.put_WarrantyVoid_1(    0 );
	CHECKX(          0xfdffffff, Tx.get() );
	CHECK(                    0, Tx.get_WarrantyVoid_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "get_NewStyle_1()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    1, Tx.get_NewStyle_1() );
	Tx.put_NewStyle_1(        0 );
	CHECKX(          0xff7fffff, Tx.get() );
	CHECK(                    0, Tx.get_NewStyle_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "get_MemSize_3()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECK(                    7, Tx.get_MemSize_3() );
	Tx.put_MemSize_3(         0 );
	CHECKX(          0xff8fffff, Tx.get() );
	CHECK(                    0, Tx.get_MemSize_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "get_MfgNumber_4()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECKX(                 0xf, Tx.get_MfgNumber_4() );
	Tx.put_MfgNumber_4(     0x0 );
	CHECKX(          0xfff0ffff, Tx.get() );
	CHECKX(                 0x0, Tx.get_MfgNumber_4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74b", "get_ChipNumber_4()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECKX(          0x0000000f, Tx.get_ChipNumber_4() );
	Tx.put_ChipNumber_4(    0x0 );
	CHECKX(          0xffff0fff, Tx.get() );
	CHECKX(          0x00000000, Tx.get_ChipNumber_4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75b", "get_BoardType_8()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECKX(                0xff, Tx.get_BoardType_8() );
	Tx.put_BoardType_8(    0x00 );
	CHECKX(          0xfffff00f, Tx.get() );
	CHECKX(                0x00, Tx.get_BoardType_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "get_BoardRev_4()" );
    try {
	Tx.put(          0xffffffff );
	CHECKX(          0xffffffff, Tx.get() );
	CHECKX(                 0xf, Tx.get_BoardRev_4() );
	Tx.put_BoardRev_4(      0x0 );
	CHECKX(          0xfffffff0, Tx.get() );
	CHECKX(                 0x0, Tx.get_BoardRev_4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

