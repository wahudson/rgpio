// 2020-12-03  William A. Hudson
//
// Testing:  rgRpiRev - Raspberry Pi Revision
//    10-19  Constructors, accessors
//    20-29  rgRpiRev_Soc  get(), put()
//    30-39  rgRpiRev_Soc  find()
//    40-49  rgRpiRev_Base  get(), put()
//    50-59  rgRpiRev_Base  find()
//    60-68  Global Config find() chain
//    70-78  Soc_enum - soc_enum2cstr()
//    80-89  Soc_enum - int2soc_enum()
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
	CHECK( "/proc/cpuinfo", tx.RevCode.init_file() );
	CHECK(  0,              tx.SocEnum.get() );
	CHECK(  0,              tx.SocEnum.is_final() );
	CHECK(  0,              tx.SocEnum.is_fail() );
	CHECKX( 0x00000000,     tx.BaseAddr.get() );
	CHECK(  0,              tx.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "Tx as initialized" );
    try {
	CHECKX( 0x00000000,     Tx.RevCode.get() );
	CHECK(  0,              Tx.RevCode.is_final() );
	CHECK( "/proc/cpuinfo", Tx.RevCode.init_file() );
	CHECK(  0,              Tx.SocEnum.get() );
	CHECK(  0,              Tx.SocEnum.is_final() );
	CHECK(  0,              Tx.SocEnum.is_fail() );
	CHECKX( 0x00000000,     Tx.BaseAddr.get() );
	CHECK(  0,              Tx.BaseAddr.is_final() );
//	CHECK(  0,              (uint64_t) Tx.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "Tx.Config object as initialized" );
    try {
	CHECKX( 0x00000000,     Tx.Config.RevCode.get() );
	CHECK(  0,              Tx.Config.RevCode.is_final() );
	CHECK( "/proc/cpuinfo", Tx.Config.RevCode.init_file() );
	CHECK(  0,              Tx.Config.SocEnum.get() );
	CHECK(  0,              Tx.Config.SocEnum.is_final() );
	CHECK(  0,              Tx.Config.SocEnum.is_fail() );
	CHECKX( 0x00000000,     Tx.Config.BaseAddr.get() );
	CHECK(  0,              Tx.Config.BaseAddr.is_final() );
//	CHECK(  0,              (uint64_t) Tx.Config.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "rgRpiRev::Config global as initialized" );
    try {
	CHECKX( 0x00000000,     rgRpiRev::Config.RevCode.get() );
	CHECK(  0,              rgRpiRev::Config.RevCode.is_final() );
	CHECK( "/proc/cpuinfo", rgRpiRev::Config.RevCode.init_file() );
	CHECKX( 0x00000000,     rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,              rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,              rgRpiRev::Config.SocEnum.is_fail() );
	CHECKX( 0x00000000,     rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,              rgRpiRev::Config.BaseAddr.is_final() );
//	CHECK(  0,   (uint64_t) rgRpiRev::Config.BaseAddr.init_ptr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "verify rgRpiRev::Config is Tx.Config" );
    try {
	CHECK(  1, (&rgRpiRev::Config          == &Tx.Config ) );
	CHECK(  1, (&rgRpiRev::Config.RevCode  == &Tx.Config.RevCode  ) );
	CHECK(  1, (&rgRpiRev::Config.SocEnum  == &Tx.Config.SocEnum  ) );
	CHECK(  1, (&rgRpiRev::Config.BaseAddr == &Tx.Config.BaseAddr ) );
	CHECK(  1, ( rgRpiRev::Config.SocEnum.init_ptr()  ==
			    Tx.Config.SocEnum.init_ptr()  ) );
	CHECK(  1, ( rgRpiRev::Config.BaseAddr.init_ptr() ==
			    Tx.Config.BaseAddr.init_ptr() ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Soc  get(), put()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "21a", "Config.SocEnum.get() clear_final" );
    try {
	rgRpiRev::Config.SocEnum.clear_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "Config.SocEnum.get() mark_final" );
    try {
	rgRpiRev::Config.SocEnum.mark_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "Config.SocEnum.put() bad enum" );
    try {
	rgRpiRev::Config.SocEnum.mark_final();
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Config.SocEnum.get() );
	rgRpiRev::Config.SocEnum.put( (rgRpiRev::Soc_enum) -1 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::rgRpiRev_Soc::put() enum out of range:  -1",
	    e.what()
	);
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Config.SocEnum.get() );
	// no change, does not mark failed
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

/*****
  CASE( "21d", "Config.SocEnum.put() not enum type" );
    try {
	rgRpiRev::Config.SocEnum.put( 99 );	// fail compile
	FAIL( "no compile error" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
*****/

//--------------------------------------
  CASE( "22a", "SocEnum.get() clear_final" );
    try {
	Tx.SocEnum.clear_final();
	Tx.SocEnum.put( rgRpiRev::soc_BCM2835 );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  0,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, Tx.SocEnum.get() );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  0,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "SocEnum.get() mark_final" );
    try {
	Tx.SocEnum.mark_final();
	Tx.SocEnum.put( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22c", "SocEnum.put() bad enum" );
    try {
	Tx.SocEnum.mark_final();
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
	Tx.SocEnum.put( (rgRpiRev::Soc_enum) 99 );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::rgRpiRev_Soc::put() enum out of range:  99",
	    e.what()
	);
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, Tx.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Soc  find()
//--------------------------------------------------------------------------

  CASE( "31", "SocEnum.find() mark_final" );
    try {
	Tx.SocEnum.mark_final();
	Tx.SocEnum.put( rgRpiRev::soc_BCM2837 );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, Tx.SocEnum.find() );
	CHECK(  0,                    Tx.SocEnum.is_fail() );
	CHECK(  1,                    Tx.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Config.SocEnum.find() RPi3" );
    try {
	rgRpiRev::Config.RevCode.put( 0x00a22082 );	// RPi3
	rgRpiRev::Config.RevCode.mark_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Config.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.find() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "Config.SocEnum.find() with RevCode failure" );
    try {
	rgRpiRev::Config.RevCode.put( 0x00000000 );	// failed
	rgRpiRev::Config.RevCode.mark_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Config.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	//
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "Config.SocEnum.find() chip number not enum" );
    try {
	rgRpiRev::Config.RevCode.put( 0x00a2f082 );	// RPi3, ChipNumber=15
	rgRpiRev::Config.RevCode.mark_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Config.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	rgRpiRev::Config.SocEnum.find();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev_Soc::find() ChipNumber has no enum:  15", e.what() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Base  get(), put()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "41a", "Config.BaseAddr.get() clear_final" );
    try {
	rgRpiRev::Config.BaseAddr.clear_final();
	rgRpiRev::Config.BaseAddr.put( 0xcc33ee55 );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0xcc33ee55,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "Config.BaseAddr.get() mark_final" );
    try {
	rgRpiRev::Config.BaseAddr.mark_final();
	rgRpiRev::Config.BaseAddr.put( 0x22222222 );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x22222222,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "Config.BaseAddr.get() clear_final" );
    try {
	Tx.BaseAddr.clear_final();
	Tx.BaseAddr.put( 0xcc33ee55 );
	CHECK(  0,                    Tx.BaseAddr.is_final() );
	CHECKX( 0xcc33ee55,           Tx.BaseAddr.get() );
	CHECK(  0,                    Tx.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "Config.BaseAddr.get() mark_final" );
    try {
	Tx.BaseAddr.mark_final();
	Tx.BaseAddr.put( 0x22222222 );
	CHECK(  1,                    Tx.BaseAddr.is_final() );
	CHECKX( 0x22222222,           Tx.BaseAddr.get() );
	CHECK(  1,                    Tx.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgRpiRev_Base  find()
//--------------------------------------------------------------------------

  CASE( "51", "Config.BaseAddr.find() mark_final" );
    try {
	rgRpiRev::Config.BaseAddr.put( 0x00abbaff );
	rgRpiRev::Config.BaseAddr.mark_final();
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x00abbaff,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "Config.BaseAddr.find() RPi1" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Config.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	rgRpiRev::Config.BaseAddr.put( 0x33333333 );
	rgRpiRev::Config.BaseAddr.clear_final();
	CHECKX( 0x33333333,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x20000000,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52b", "Config.BaseAddr.find() RPi2" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2836 );
	rgRpiRev::Config.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	rgRpiRev::Config.BaseAddr.put( 0xffffffff );
	rgRpiRev::Config.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52c", "Config.BaseAddr.find() RPi3" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2837 );
	rgRpiRev::Config.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	rgRpiRev::Config.BaseAddr.put( 0xffffffff );
	rgRpiRev::Config.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52d", "Config.BaseAddr.find() RPi4" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );
	rgRpiRev::Config.SocEnum.mark_final();
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	rgRpiRev::Config.BaseAddr.put( 0xffffffff );
	rgRpiRev::Config.BaseAddr.clear_final();
	CHECKX( 0xffffffff,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0xfe000000,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "Setup SocEnum as failed" );
    try {
	rgRpiRev::Config.RevCode.put( 0x00000000 );	// failed
	rgRpiRev::Config.RevCode.mark_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2837 );
	rgRpiRev::Config.SocEnum.clear_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	//
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53b", "Config.BaseAddr.find() with SocEnum failure" );
    try {
	rgRpiRev::Config.BaseAddr.put( 0x77777777 );
	rgRpiRev::Config.BaseAddr.clear_final();
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECKX( 0x77777777,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Config.BaseAddr.is_final() );
	//
	CHECKX( 0x00000000,           rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,                    rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_fail() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Global Config find() chain
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "61a", "RevCode.find() good file" );
    try {
	rgRpiRev::Config.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Config.RevCode.clear_final();
	rgRpiRev::Config.RevCode.put( 0xffffffff );
	CHECK( "ref/rpi3.in", rgRpiRev::Config.RevCode.init_file() );
	CHECK(  0,            rgRpiRev::Config.RevCode.is_final() );
	CHECKX( 0x00a22082,   rgRpiRev::Config.RevCode.find() );  // action
	CHECK(  1,            rgRpiRev::Config.RevCode.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "RevCode.find() second" );
    try {
	CHECKX( 0x00a22082,   rgRpiRev::Config.RevCode.find() );  // action
	CHECK(  1,            rgRpiRev::Config.RevCode.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "62a", "SocEnum.find() good file" );
    try {
	rgRpiRev::Config.SocEnum.clear_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62b", "find() second" );
    try {
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.find() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "63a", "BaseAddr.find() good file" );
    try {
	rgRpiRev::Config.BaseAddr.clear_final();
	rgRpiRev::Config.BaseAddr.put( 0xffffffff );
	CHECK(  0,            rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x3f000000,   rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "find() second" );
    try {
	CHECKX( 0x3f000000,   rgRpiRev::Config.BaseAddr.find() );
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "find_SocEnum() good file" );
    try {
	rgRpiRev::Config.SocEnum.clear_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::find_SocEnum() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "find_SocEnumind() second" );
    try {
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::find_SocEnum() );
	CHECK(  1,                    rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,                    rgRpiRev::Config.SocEnum.is_fail() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "66a", "BaseAddr.find() good file" );
    try {
	rgRpiRev::Config.BaseAddr.clear_final();
	rgRpiRev::Config.BaseAddr.put( 0xffffffff );
	CHECK(  0,            rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x3f000000,   rgRpiRev::find_BaseAddr() );
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "find() second" );
    try {
	CHECKX( 0x3f000000,   rgRpiRev::find_BaseAddr() );
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Ripple thru all
  CASE( "67a", "BaseAddr.find() good file" );
    try {
	rgRpiRev::Config.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Config.RevCode.clear_final();
	rgRpiRev::Config.RevCode.put( 0xffffffff );
	rgRpiRev::Config.SocEnum.clear_final();
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2835 );
	rgRpiRev::Config.BaseAddr.clear_final();
	rgRpiRev::Config.BaseAddr.put( 0x77777777 );
	//
	CHECKX( 0xffffffff,           rgRpiRev::Config.RevCode.get() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Config.SocEnum.get() );
	CHECKX( 0x77777777,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  0,            rgRpiRev::Config.RevCode.is_final() );
	CHECK(  0,            rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  0,            rgRpiRev::Config.BaseAddr.is_final() );
	//
	CHECKX( 0x3f000000,   rgRpiRev::Config.BaseAddr.find() );  // action
	//
	CHECKX( 0x00a22082,           rgRpiRev::Config.RevCode.get() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Config.SocEnum.get() );
	CHECKX( 0x3f000000,           rgRpiRev::Config.BaseAddr.get() );
	CHECK(  1,            rgRpiRev::Config.RevCode.is_final() );
	CHECK(  1,            rgRpiRev::Config.SocEnum.is_final() );
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
	CHECK(  0,            rgRpiRev::Config.SocEnum.is_fail() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "find() second" );
    try {
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
	CHECKX( 0x3f000000,   rgRpiRev::Config.BaseAddr.find() );  // action
	CHECK(  1,            rgRpiRev::Config.BaseAddr.is_final() );
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

//--------------------------------------
  CASE( "99", "Done" );
}

