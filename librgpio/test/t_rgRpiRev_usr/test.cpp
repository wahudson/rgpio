// 2024-03-26  William A. Hudson
//
// Testing:  rgRpiRev class, main user interface
//    10-19  Global constructed state
//    20-29  RevCode.defaultv(), SocEnum.defaultv()
//    30-39  default_RevCode(), default_SocEnum()
//    40-49  simulate_RevCode(), simulate_SocEnum(), simulate()
//    50-59  Register override(), Low-level
//    60-68  .
//    70-78  .
//    80-89  initialize_ioRPi() Exceptions
//    90-99  .
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"

using namespace std;

/*
* Check all state except predecessor pointers.
*/
void  Check_Global()
{
	CHECK( "/proc/cpuinfo",       rgRpiRev::Global.RevCode.init_file() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );

	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );

	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );

	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );

	CHECK(  0,                    rgRpiRev::ioRPiReal() );
}

/*
* Reset to constructed state.
*/
void  Reset_Global()
{
	rgRpiRev::Global.RevCode.init_file( "/proc/cpuinfo" );
	rgRpiRev::Global.RevCode.override_realpi( 0 );
	rgRpiRev::Global.RevCode.override(  0x00000000 );
	rgRpiRev::Global.RevCode.putFU( 0, 1 );
	rgRpiRev::Global.SocEnum.override(  rgRpiRev::soc_BCM2835 );
	rgRpiRev::Global.SocEnum.putFU( 0, 1 );
	rgRpiRev::Global.BaseAddr.override( 0x00000000 );
	rgRpiRev::Global.BaseAddr.putFU( 0, 1 );
	Check_Global();
}

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

//#!! rgRpiRev		Tx;		// test object

//--------------------------------------------------------------------------
//## Global constructed state
//--------------------------------------------------------------------------

  CASE( "10a", "verify constructed ioRPi flags" );
    try {
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "verify constructed state" );
    try {
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECK( "/proc/cpuinfo",       rgRpiRev::Global.RevCode.init_file() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "verify Check_Global()" );
    try {
	Check_Global();
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "verify Reset_Global() matches" );
    try {
	Reset_Global();
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "non-RPi initialize_ioRPi()" );
    try {
	rgRpiRev::Global.RevCode.init_file( "/dev/null" );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  1,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "RPi3 initialize_ioRPi()" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  1,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() );
	CHECK(  1,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## RevCode.defaultv(), SocEnum.defaultv()
//--------------------------------------------------------------------------

  CASE( "21", "defaultv() non-RPi, defaults stick" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "/dev/null" );
	rgRpiRev::Global.RevCode.defaultv( 0x33333333 );
	rgRpiRev::Global.SocEnum.defaultv( rgRpiRev::soc_BCM2836 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  1,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "defaultv() real-RPi, defaults replaced" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.defaultv( 0x33333333 );
	rgRpiRev::Global.SocEnum.defaultv( rgRpiRev::soc_BCM2836 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x3f000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  1,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() );
	CHECK(  1,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## default_RevCode(), default_SocEnum()
//--------------------------------------------------------------------------

  CASE( "30", "default_RevCode(), default_SocEnum() win over override()" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi0.in" );
	rgRpiRev::Global.RevCode.override( 0xffffffff );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2836 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0xffffffff,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.default_RevCode( 0x00803000 );
	rgRpiRev::Global.default_SocEnum( rgRpiRev::soc_BCM2837 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## simulate_RevCode(), simulate_SocEnum(), simulate()
//--------------------------------------------------------------------------

  CASE( "41", "all simulate_*(), non-RPi" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "/dev/null" );
	rgRpiRev::Global.simulate_RevCode( 0x33333333 );
	rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2836 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  1,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "all simulate_*(), real-RPi, same as 41" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi0.in" );
	rgRpiRev::Global.simulate_RevCode( 0x33333333 );
	rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2836 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x33333333,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2836, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  1,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "simulate() constructor default, real-RPi" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.simulate();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  1,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  0,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "simulate_SocEnum( BCM2711 ), real-RPi" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  1,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "simulate_RevCode( RPi4 ), real-RPi" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi0.in" );
	rgRpiRev::Global.simulate_RevCode( 0x00803000 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() );
	CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  1,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "all simulate_*(), real-RPi, wins over override" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi0.in" );
	rgRpiRev::Global.RevCode.override( 0x00802000 );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2836 );
	rgRpiRev::Global.simulate_RevCode( 0x00803000 );
	rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	CHECK(  0,                    rgRpiRev::ioRPiReal() );
	CHECK(  0,                    rgRpiRev::ioRPi0() ); CHECK(  0,                    rgRpiRev::ioRPi3() );
	CHECK(  1,                    rgRpiRev::ioRPi4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register override(), Low-level
//--------------------------------------------------------------------------

  CASE( "51", "RevCode.override(), no side-effects" );
    try {
	Reset_Global();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.RevCode.override( 0x00803000 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00803000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "SocEnum.override(), no side-effects" );
    try {
	Reset_Global();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2711, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53", "BaseAddr.override(), no side-effects" );
    try {
	Reset_Global();
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::Global.BaseAddr.override( 0x9000001f00ee0000 );
	CHECK(  0,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00ee0000,           rgRpiRev::Global.BaseAddr.get() );
	CHECKX( 0x9000001f,           rgRpiRev::Global.BaseAddr.get() >>32 );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	//#!! kludge BaseAddr check
	cout << "BaseAddr.get() = 0x"
	     <<hex << rgRpiRev::Global.BaseAddr.get()
	     <<dec <<endl;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## initialize_ioRPi() Exceptions
//--------------------------------------------------------------------------

  CASE( "81", "initialize_ioRPi() unknown RevCode exception" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.override_realpi( 1 );
	rgRpiRev::Global.RevCode.putFU( 1, 1 );
	rgRpiRev::Global.SocEnum.putFU( 0, 1 );
	rgRpiRev::Global.BaseAddr.putFU( 0, 1 );
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::initialize_ioRPi() RevCode is Unknown",
	    e.what()
	);
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "initialize_ioRPi() unknown SocEnum exception" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.override_realpi( 1 );
	rgRpiRev::Global.RevCode.putFU( 0, 1 );
	rgRpiRev::Global.SocEnum.putFU( 1, 1 );
	rgRpiRev::Global.BaseAddr.putFU( 0, 1 );
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::initialize_ioRPi() SocEnum is Unknown",
	    e.what()
	);
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "initialize_ioRPi() unknown BaseAddr exception" );
    try {
	Reset_Global();
	rgRpiRev::Global.RevCode.init_file( "ref/rpi3.in" );
	rgRpiRev::Global.RevCode.override_realpi( 1 );
	rgRpiRev::Global.RevCode.putFU( 0, 1 );
	rgRpiRev::Global.SocEnum.putFU( 0, 1 );
	rgRpiRev::Global.BaseAddr.putFU( 1, 1 );
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00000000,           rgRpiRev::Global.RevCode.get() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2835, rgRpiRev::Global.SocEnum.get() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
	rgRpiRev::initialize_ioRPi();
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "rgRpiRev::initialize_ioRPi() BaseAddr is Unknown",
	    e.what()
	);
	CHECK(  1,                    rgRpiRev::Global.RevCode.get_realpi() );
	CHECKX( 0x00a22082,           rgRpiRev::Global.RevCode.get() );
	CHECK(  0,                    rgRpiRev::Global.RevCode.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.RevCode.is_final() );
	CHECK( rgRpiRev::soc_BCM2837, rgRpiRev::Global.SocEnum.get() );
	CHECK(  0,                    rgRpiRev::Global.SocEnum.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.SocEnum.is_final() );
	CHECKX( 0x00000000,           rgRpiRev::Global.BaseAddr.get() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_unknown() );
	CHECK(  1,                    rgRpiRev::Global.BaseAddr.is_final() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

