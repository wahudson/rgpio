// 2020-12-03  William A. Hudson
//
// Testing:  rgRpiRev - Raspberry Pi Revision
//    10-19  Constructors, accessors
//    20-29  rgRpiRev_Soc  get(), override()
//    30-39  rgRpiRev_Soc  find(), cstr()
//    40-49  rgRpiRev_Base  get(), override()
//    50-59  rgRpiRev_Base  find()
//    60-68  Global object find() chain
//    70-78  Soc_enum - soc_enum2cstr()
//    80-89  Soc_enum - int2soc_enum()
//    90-99  Global object  cstr_SocEnum()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgRpiRev		Tx;		// test object

//--------------------------------------------------------------------------
//## Constructors, accessors
//--------------------------------------------------------------------------

  CASE( "10", "rgRpiRev constructor" );
    try {
	rgRpiRev	tx;
	CHECKX( 0x00000000,     tx.RevCode.get() );
	CHECK(  0,              tx.RevCode.is_final() );
	CHECK(  1,              tx.RevCode.is_unknown() );
	CHECK( "/proc/cpuinfo", tx.RevCode.init_file() );
	CHECK(  0,              tx.SocEnum.get() );
	CHECK(  0,              tx.SocEnum.is_final() );
	CHECK(  1,              tx.SocEnum.is_unknown() );
	CHECKX( 0x00000000,     tx.BaseAddr.get() );
	CHECK(  0,              tx.BaseAddr.is_final() );
	CHECK(  1,              tx.BaseAddr.is_unknown() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "Tx as initialized" );
    try {
	CHECKX( 0x00000000,     Tx.RevCode.get() );
	CHECK(  0,              Tx.RevCode.is_final() );
	CHECK(  1,              Tx.RevCode.is_unknown() );
	CHECK( "/proc/cpuinfo", Tx.RevCode.init_file() );
	CHECK(  0,              Tx.SocEnum.get() );
	CHECK(  0,              Tx.SocEnum.is_final() );
	CHECK(  1,              Tx.SocEnum.is_unknown() );
	CHECKX( 0x00000000,     Tx.BaseAddr.get() );
	CHECK(  0,              Tx.BaseAddr.is_final() );
	CHECK(  1,              Tx.BaseAddr.is_unknown() );
//	CHECK(  0,              (uint64_t) Tx.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "Tx.Global object as initialized" );
    try {
	CHECKX( 0x00000000,     Tx.Global.RevCode.get() );
	CHECK(  0,              Tx.Global.RevCode.is_final() );
	CHECK(  1,              Tx.Global.RevCode.is_unknown() );
	CHECK( "/proc/cpuinfo", Tx.Global.RevCode.init_file() );
	CHECK(  0,              Tx.Global.SocEnum.get() );
	CHECK(  0,              Tx.Global.SocEnum.is_final() );
	CHECK(  1,              Tx.Global.SocEnum.is_unknown() );
	CHECKX( 0x00000000,     Tx.Global.BaseAddr.get() );
	CHECK(  0,              Tx.Global.BaseAddr.is_final() );
	CHECK(  1,              Tx.Global.BaseAddr.is_unknown() );
//	CHECK(  0,              (uint64_t) Tx.Global.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "rgRpiRev::Global global as initialized" );
    try {
	CHECKX( 0x00000000,     rgRpiRev::Global.RevCode.get() );
	CHECK(  0,              rgRpiRev::Global.RevCode.is_final() );
	CHECK( "/proc/cpuinfo", rgRpiRev::Global.RevCode.init_file() );
	CHECKX( 0x00000000,     rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,              rgRpiRev::Global.SocEnum.is_final() );
	CHECK(  1,              rgRpiRev::Global.SocEnum.is_unknown() );
	CHECKX( 0x00000000,     rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,              rgRpiRev::Global.BaseAddr.is_final() );
//	CHECK(  0,   (uint64_t) rgRpiRev::Global.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "verify rgRpiRev::Global is Tx.Global" );
    try {
	CHECK(  1, (&rgRpiRev::Global          == &Tx.Global ) );
	CHECK(  1, (&rgRpiRev::Global.RevCode  == &Tx.Global.RevCode  ) );
	CHECK(  1, (&rgRpiRev::Global.SocEnum  == &Tx.Global.SocEnum  ) );
	CHECK(  1, (&rgRpiRev::Global.BaseAddr == &Tx.Global.BaseAddr ) );
	CHECK(  1, ( rgRpiRev::Global.SocEnum.init_ptr()  ==
			    Tx.Global.SocEnum.init_ptr()  ) );
	CHECK(  1, ( rgRpiRev::Global.BaseAddr.init_ptr() ==
			    Tx.Global.BaseAddr.init_ptr() ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "Nested dereference rgRpiRev::Global" ); // but why would you?
    try {
	rgRpiRev*	rp;
	rp = &rgRpiRev::Global;
	CHECK(  1, (rp == &rgRpiRev::Global ) );
	CHECK(  1, (rp == &rgRpiRev::Global.Global ) );
	CHECK(  1, (rp == &rgRpiRev::Global.Global.Global ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16", "verify rgRpiRev::Global.SocEnum is NOT Tx.SocEnum" );
    try {
	CHECK(  1, (&rgRpiRev::Global.RevCode  != &Tx.RevCode  ) );
	CHECK(  1, (&rgRpiRev::Global.SocEnum  != &Tx.SocEnum  ) );
	CHECK(  1, (&rgRpiRev::Global.BaseAddr != &Tx.BaseAddr ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Soc  get(), override()
//--------------------------------------------------------------------------

// Global object
//--------------------------------------
  CASE( "21a", "Global.SocEnum.get() Final=0" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "Global.SocEnum.get() Final=1" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "Global.SocEnum.override() Final=0" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "Global.SocEnum.override() Final=1" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23", "Global.SocEnum.override() bad enum" );
    try {
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	rgRpiRev::Global.SocEnum.override( (rgRpiRev::Soc_enum) -1 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::rgRpiRev_Soc::override() enum out of range:  -1",
	    e.what()
	);
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	// no change, does not mark unknown
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

/*****
  CASE( "21d", "Global.SocEnum.override() not enum type" );
    try {
	rgRpiRev::Global.SocEnum.override( 99 );	// fail compile
	FAIL( "no compile error" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
*****/

// Tx Object
//--------------------------------------
  CASE( "27a", "SocEnum.get() Final=0" );
    try {
	Tx.SocEnum.override( rgRpiRev::soc_BCM2835 );
	Tx.SocEnum.clear_final();
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  0,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, Tx.SocEnum.get() );
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  0,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27b", "SocEnum.get() Final=1" );
    try {
	Tx.SocEnum.override( rgRpiRev::soc_BCM2711 );
	Tx.SocEnum.mark_final();
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27c", "SocEnum.override() bad enum" );
    try {
	Tx.SocEnum.mark_final();
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
	Tx.SocEnum.override( (rgRpiRev::Soc_enum) 99 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::rgRpiRev_Soc::override() enum out of range:  99",
	    e.what()
	);
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Soc  find(), cstr()
//--------------------------------------------------------------------------

  CASE( "31", "SocEnum.find() Final=1" );
    try {
	Tx.SocEnum.override( rgRpiRev::soc_BCM2837 );
	Tx.SocEnum.mark_final();
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, Tx.SocEnum.find() );
	CHECK(  0,                    Tx.SocEnum.is_unknown() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Global.SocEnum.find() RPi3" );
    try {
	rgRpiRev::Global.RevCode.override( 0x00a22082 );	// RPi3
	rgRpiRev::Global.RevCode.mark_final();
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.find() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "Global.SocEnum.find() with RevCode failure" );
    try {
	rgRpiRev::Global.RevCode.override( 0x00000000 );	// failed
	rgRpiRev::Global.RevCode.mark_final();
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2836 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( 0x00000000,            rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	//
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "Global.SocEnum.find() chip number not enum" );
    try {
	rgRpiRev::Global.RevCode.override( 0x00a2f082 ); // RPi3, ChipNumber=15
	rgRpiRev::Global.RevCode.mark_final();
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2836 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.SocEnum.find();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev_Soc::find() ChipNumber has no enum:  15", e.what() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "37", "Global.SocEnum.cstr() Final=1" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2837 );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( "BCM2837",             rgRpiRev::Global.SocEnum.cstr() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38", "SocEnum.cstr() local tx" );
    try {
	rgRpiRev	tx;
	tx.SocEnum.override( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    tx.SocEnum.is_unknown() );
	CHECK(  1,                    tx.SocEnum.is_final() );
	CHECK( "BCM2711",             tx.SocEnum.cstr() );
	CHECK(  0,                    tx.SocEnum.is_unknown() );
	CHECK(  1,                    tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "SocEnum.cstr() local tx" );
    try {
	rgRpiRev	tx;
	tx.RevCode.override( 0x00000000 );	// failed
	CHECK(  1,                    tx.RevCode.is_final() );
	CHECK(  1,                    tx.SocEnum.is_unknown() );
	CHECK(  0,                    tx.SocEnum.is_final() );
	CHECK( "BCM2835",             tx.SocEnum.cstr() );
	CHECK(  1,                    tx.SocEnum.is_unknown() );
	CHECK(  1,                    tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Base  get(), override()
//--------------------------------------------------------------------------

// Global object
//--------------------------------------
  CASE( "41a", "Global.BaseAddr.get() Final=0" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0xcc33ee55 );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECKX( 0xcc33ee55,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "Global.BaseAddr.get() Final=1" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0x22222222 );
	rgRpiRev::Global.BaseAddr.mark_final();
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECKX( 0x22222222,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "Global.BaseAddr.override() Final=0" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0x33333333 );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECKX( 0x33333333,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0x55aaaa55 );
	CHECKX( 0x55aaaa55,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "Global.BaseAddr.override() Final=1" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0x33333333 );
	rgRpiRev::Global.BaseAddr.mark_final();
	CHECKX( 0x33333333,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0x55aaaa55 );
	CHECKX( 0x55aaaa55,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

// Tx Object
//--------------------------------------
  CASE( "47a", "BaseAddr.get() Final=0" );
    try {
	Tx.BaseAddr.override( 0xcc33ee55 );
	Tx.BaseAddr.clear_final();
	CHECK(  0,                    Tx.BaseAddr.is_unknown() );
	CHECK(  0,                    Tx.BaseAddr.is_final() );
	CHECKX( 0xcc33ee55,           Tx.BaseAddr.get() );
	CHECK(  0,                    Tx.BaseAddr.is_unknown() );
	CHECK(  0,                    Tx.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "47b", "BaseAddr.get() Final=1" );
    try {
	Tx.BaseAddr.override( 0x22222222 );
	Tx.BaseAddr.mark_final();
	CHECK(  0,                    Tx.BaseAddr.is_unknown() );
	CHECK(  1,                    Tx.BaseAddr.is_final() );
	CHECKX( 0x22222222,           Tx.BaseAddr.get() );
	CHECK(  0,                    Tx.BaseAddr.is_unknown() );
	CHECK(  1,                    Tx.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Base  find()
//--------------------------------------------------------------------------

  CASE( "51", "Global.BaseAddr.find() Final=1" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0x00abbaff );
	rgRpiRev::Global.BaseAddr.mark_final();
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECKX( 0x00abbaff,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "Global.BaseAddr.find() RPi1" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0x33333333 );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECKX( 0x33333333,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x20000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52b", "Global.BaseAddr.find() RPi2" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2836 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0xffffffff );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52c", "Global.BaseAddr.find() RPi3" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2837 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0xffffffff );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52d", "Global.BaseAddr.find() RPi4" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	rgRpiRev::Global.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0xffffffff );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0xfe000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "Setup SocEnum as failed" );
    try {
	rgRpiRev::Global.RevCode.override( 0x00000000 );	// failed
	rgRpiRev::Global.RevCode.mark_final();
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2837 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	//
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53b", "Global.BaseAddr.find() with SocEnum failure" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0x77777777 );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x77777777,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Global object find() chain
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "61a", "RevCode.find() good file" );
    try {
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.override( 0xffffffff );
	rgRpiRev::Global.RevCode.clear_final();
	CHECK( "ref/rpi3.in", rgRpiRev::Global.RevCode.init_file() );
	CHECK(  0,            rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,            rgRpiRev::Global.RevCode.is_final() );
	CHECKX( 0x00a22082,   rgRpiRev::Global.RevCode.find() );  // action
	CHECK(  0,            rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,            rgRpiRev::Global.RevCode.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "RevCode.find() second" );
    try {
	CHECKX( 0x00a22082,   rgRpiRev::Global.RevCode.find() );  // action
	CHECK(  0,            rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,            rgRpiRev::Global.RevCode.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "62a", "SocEnum.find() from RevCode" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	//
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.find() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62b", "find() second" );
    try {
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.find() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "63a", "BaseAddr.find() from SocEnum" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0xffffffff );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0xffffffff,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "find() second" );
    try {
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "find_SocEnum() from RevCode" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::find_SocEnum() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "find_SocEnum() second" );
    try {
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::find_SocEnum() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "66a", "find_BaseAddr() from SocEnum" );
    try {
	rgRpiRev::Global.BaseAddr.override( 0xffffffff );
	rgRpiRev::Global.BaseAddr.clear_final();
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0xffffffff,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::find_BaseAddr() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "find_BaseAddr() second" );
    try {
	CHECKX( 0x3f000000,           rgRpiRev::find_BaseAddr() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Ripple thru all
  CASE( "67a", "BaseAddr.find() ripple to good file" );
    try {
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.override( 0xffffffff );
	rgRpiRev::Global.RevCode.putFU( 0, 1 );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.putFU( 0, 1 );
	rgRpiRev::Global.BaseAddr.override( 0x77777777 );
	rgRpiRev::Global.BaseAddr.putFU( 0, 1 );
	//
	CHECKX( 0xffffffff,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x77777777,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	//
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );

    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "find() second" );
    try {
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.find() );
	//
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Soc_enum - soc_enum2cstr()
//--------------------------------------------------------------------------

  CASE( "70", "Soc_enum values" );
    try {
	CHECK(  0, rgRpiRev::soc_BCM2835 );
	CHECK(  1, rgRpiRev::soc_BCM2836 );
	CHECK(  2, rgRpiRev::soc_BCM2837 );
	CHECK(  3, rgRpiRev::soc_BCM2711 );
	CHECK(  3, rgRpiRev::soc_MaxEnum );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71", "soc_enum2cstr() all names" );
    try {
	CHECK( "BCM2835",  Tx.soc_enum2cstr( rgRpiRev::soc_BCM2835  ) );
	CHECK( "BCM2836",  Tx.soc_enum2cstr( rgRpiRev::soc_BCM2836  ) );
	CHECK( "BCM2837",  Tx.soc_enum2cstr( rgRpiRev::soc_BCM2837  ) );
	CHECK( "BCM2711",  Tx.soc_enum2cstr( rgRpiRev::soc_BCM2711  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72", "soc_enum2cstr() class call" );
    try {
	CHECK( "BCM2835",  rgRpiRev::soc_enum2cstr( rgRpiRev::soc_BCM2835  ) );
	CHECK( "BCM2836",  rgRpiRev::soc_enum2cstr( rgRpiRev::soc_BCM2836  ) );
	CHECK( "BCM2837",  rgRpiRev::soc_enum2cstr( rgRpiRev::soc_BCM2837  ) );
	CHECK( "BCM2711",  rgRpiRev::soc_enum2cstr( rgRpiRev::soc_BCM2711  ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73", "soc_enum2cstr() bad enum" );	// cast to a bad value
    try {
	Tx.soc_enum2cstr( (rgRpiRev::Soc_enum) 99 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRpiRev::soc_enum2cstr() invalid Soc_enum:  99",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Soc_enum - int2soc_enum()
//--------------------------------------------------------------------------

  CASE( "80", "int2soc_enum() verify return type" );
    try {
	CHECK( "BCM2835", Tx.soc_enum2cstr( Tx.int2soc_enum( 0 ) ) );
	CHECK( "BCM2836", Tx.soc_enum2cstr( Tx.int2soc_enum( 1 ) ) );
	CHECK( "BCM2837", Tx.soc_enum2cstr( Tx.int2soc_enum( 2 ) ) );
	CHECK( "BCM2711", Tx.soc_enum2cstr( Tx.int2soc_enum( 3 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81", "int2soc_enum() class call" );
    try {
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::int2soc_enum( 0 ) );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::int2soc_enum( 1 ) );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::int2soc_enum( 2 ) );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::int2soc_enum( 3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "82a", "int2soc_enum() bad int" );
    try {
	Tx.int2soc_enum( -1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRpiRev::int2soc_enum() int out of range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82b", "int2soc_enum() bad int" );
    try {
	Tx.int2soc_enum( 4 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRpiRev::int2soc_enum() int out of range:  4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Global object  cstr_SocEnum()
//--------------------------------------------------------------------------

  CASE( "90", "cstr_SocEnum()" );
    try {
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( "BCM2711",             rgRpiRev::cstr_SocEnum() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "91", "cstr_SocEnum() from RevCode" );
    try {
	rgRpiRev::Global.RevCode.override( 0x00a22082 );	// RPi3
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECK( "BCM2837",             rgRpiRev::cstr_SocEnum() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

