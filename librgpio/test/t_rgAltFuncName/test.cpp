// 2023-02-20  William A. Hudson
//
// Testing:  rgAltFuncName  Alternate Function Name class
//    10-19  Constructor
//    20-29  .
//    30-39  cstr_altfunc_bit()  RPi3
//    40-49  cstr_altfunc_bit()  RPi4
//    50-59  cstr_altfunc_bit()  Exceptions
//    Sample Gpio bits, not exhaustive.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgFselPin.h"
#include "rgAltFuncName.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAltFuncName		Tx;	// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "01", "find rgRpiRev BaseAddr first" );
    try {
	rgRpiRev::Config.BaseAddr.find();
	CHECK( 0, 0 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10", "constructor" );
    try {
	rgAltFuncName	tx;
	CHECK( 0, 0 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "cstr_altfunc_bit() class call" );
    try {
	CHECK( "iic0_SDA",
	    rgAltFuncName::cstr_altfunc_bit( rgFselPin::f_Alt0,  0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## cstr_altfunc_bit()  RPi3
//--------------------------------------------------------------------------

  CASE( "30a", "config RPi3" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2837 );
	CHECK( "BCM2837", rgRpiRev::Config.SocEnum.cstr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "30", "cstr_altfunc_bit()" );
    try {
	CHECK( "iic0_SDA",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0,  0 ) );
	CHECK( "sd0_DAT3",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0, 27 ) );
	CHECK( "pwm0_CH2",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "cstr_altfunc_bit()" );
    try {
	CHECK( "sm_A5",      Tx.cstr_altfunc_bit( rgFselPin::f_Alt1,  0 ) );
	CHECK( "--",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt1, 27 ) );
	CHECK( "iic0_SCL",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt1, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "cstr_altfunc_bit()" );
    try {
	CHECK( "dpi_PCLK",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt2,  0 ) );
	CHECK( "dpi_D23",    Tx.cstr_altfunc_bit( rgFselPin::f_Alt2, 27 ) );
	CHECK( "iic1_SCL",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt2, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "cstr_altfunc_bit()" );
    try {
	CHECK( "**",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt3,  0 ) );
	CHECK( "sd1_DAT3",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt3, 27 ) );
	CHECK( "--",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt3, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "cstr_altfunc_bit()" );
    try {
	CHECK( "**",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt4,  0 ) );
	CHECK( "arm_TMS",    Tx.cstr_altfunc_bit( rgFselPin::f_Alt4, 27 ) );
	CHECK( "spi2_CE2_n", Tx.cstr_altfunc_bit( rgFselPin::f_Alt4, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "cstr_altfunc_bit()" );
    try {
	CHECK( "**",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt5,  0 ) );
	CHECK( "**",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt5, 27 ) );
	CHECK( "**",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt5, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## cstr_altfunc_bit()  RPi4
//--------------------------------------------------------------------------

  CASE( "40a", "config RPi4" );
    try {
	rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );
	CHECK( "BCM2711", rgRpiRev::Config.SocEnum.cstr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "40", "cstr_altfunc_bit()" );
    try {
	CHECK( "iic0_SDA",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0,  0 ) );
	CHECK( "sd0_DAT3",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0, 27 ) );
	CHECK( "pwm0_CH2",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt0, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "cstr_altfunc_bit()" );
    try {
	CHECK( "sm_A5",      Tx.cstr_altfunc_bit( rgFselPin::f_Alt1,  0 ) );
	CHECK( "--",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt1, 27 ) );
	CHECK( "iic0_SCL",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt1, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "cstr_altfunc_bit()" );
    try {
	CHECK( "dpi_PCLK",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt2,  0 ) );
	CHECK( "dpi_D23",    Tx.cstr_altfunc_bit( rgFselPin::f_Alt2, 27 ) );
	CHECK( "iic1_SCL",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt2, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "cstr_altfunc_bit()" );
    try {
	CHECK( "spi3_CE0_n", Tx.cstr_altfunc_bit( rgFselPin::f_Alt3,  0 ) );
	CHECK( "sd1_DAT3",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt3, 27 ) );
	CHECK( "--",         Tx.cstr_altfunc_bit( rgFselPin::f_Alt3, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44", "cstr_altfunc_bit()" );
    try {
	CHECK( "u2_TXD",     Tx.cstr_altfunc_bit( rgFselPin::f_Alt4,  0 ) );
	CHECK( "arm_TMS",    Tx.cstr_altfunc_bit( rgFselPin::f_Alt4, 27 ) );
	CHECK( "spi0_CE2_n", Tx.cstr_altfunc_bit( rgFselPin::f_Alt4, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "45", "cstr_altfunc_bit()" );
    try {
	CHECK( "iic6_SDA",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt5,  0 ) );
	CHECK( "spi6_CE1_n", Tx.cstr_altfunc_bit( rgFselPin::f_Alt5, 27 ) );
	CHECK( "sdc_PWR0",   Tx.cstr_altfunc_bit( rgFselPin::f_Alt5, 45 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## cstr_altfunc_bit()  Exceptions
//--------------------------------------------------------------------------

  CASE( "51", "bad cstr_altfunc_bit()" );
    try {
	Tx.cstr_altfunc_bit( rgFselPin::f_Alt0,  -1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfunc_bit():  bit out-of-range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "bad cstr_altfunc_bit()" );
    try {
	Tx.cstr_altfunc_bit( rgFselPin::f_Alt0, 54 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfunc_bit():  bit out-of-range:  54",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

