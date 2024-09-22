// 2024-09-20  William A. Hudson
//
// Testing:  rgsFuncName  Alternate Function Name class for RPi5
//    10-19  Constructor, class call
//    20-29  cstr_altfuncAN() range errors
//    30-39  cstr_altfuncAN() sample function names
//    40-49  .
//    50-59  .
// No dependency on RPi platform.
// Sample Gpio bits, not exhaustive.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgsFuncName.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

// Config RPi3, should be no dependency on RPi platform.
rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2837 );	// RPi3

rgsFuncName		Tx;	// test object


//--------------------------------------------------------------------------
//## Constructor, class call
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgsFuncName	tx;
	CHECK( 0, 0 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "cstr_altfuncAN() class call" );
    try {
	CHECK( "spi0_SIO3",    rgsFuncName::cstr_altfuncAN(  0,  0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "cstr_altfuncAN() class call, max Gpio" );
    try {
	CHECK( "null",         rgsFuncName::cstr_altfuncAN( 31, 27 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## cstr_altfuncAN() range errors
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "21a", "cstr_altfuncAN() bad Gpio" );
    try {
	Tx.cstr_altfuncAN( 0, -1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfuncAN():  bit out-of-range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "cstr_altfuncAN() bad Gpio" );
    try {
	Tx.cstr_altfuncAN( 0, 28 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfuncAN():  bit out-of-range:  28",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "cstr_altfuncAN() bad altnum" );
    try {
	Tx.cstr_altfuncAN( -1,  0 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfuncAN():  altnum out-of-range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "cstr_altfuncAN() bad altnum" );
    try {
	Tx.cstr_altfuncAN( 32,  0 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "cstr_altfuncAN():  altnum out-of-range:  32",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## cstr_altfuncAN() sample function names
//--------------------------------------------------------------------------

  CASE( "30a", "cstr_altfuncAN() reserved" );
    try {
	CHECK( "--",           Tx.cstr_altfuncAN(  9,  0 ) );
	CHECK( "null",         Tx.cstr_altfuncAN( 31,  0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "30b", "cstr_altfuncAN() reserved" );
    try {
	CHECK( "--",           Tx.cstr_altfuncAN( 30, 27 ) );
	CHECK( "null",         Tx.cstr_altfuncAN( 31, 27 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "cstr_altfuncAN() Gpio0" );
    try {
	CHECK( "spi0_SIO3",    Tx.cstr_altfuncAN( 0,  0 ) );
	CHECK( "dpi_PCLK",     Tx.cstr_altfuncAN( 1,  0 ) );
	CHECK( "u1_TX",        Tx.cstr_altfuncAN( 2,  0 ) );
	CHECK( "iic0_SDA",     Tx.cstr_altfuncAN( 3,  0 ) );
	CHECK( "--",           Tx.cstr_altfuncAN( 4,  0 ) );
	CHECK( "sRIO[0]",      Tx.cstr_altfuncAN( 5,  0 ) );
	CHECK( "pRIO[0]",      Tx.cstr_altfuncAN( 6,  0 ) );
	CHECK( "PIO[0]",       Tx.cstr_altfuncAN( 7,  0 ) );
	CHECK( "spi2_CSn0",    Tx.cstr_altfuncAN( 8,  0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "cstr_altfuncAN() Gpio18" );
    try {
	CHECK( "spi1_CSn0",    Tx.cstr_altfuncAN( 0, 18 ) );
	CHECK( "dpi_D14",      Tx.cstr_altfuncAN( 1, 18 ) );
	CHECK( "iis0_SCLK",    Tx.cstr_altfuncAN( 2, 18 ) );
	CHECK( "pwm0_CH2",     Tx.cstr_altfuncAN( 3, 18 ) );
	CHECK( "iis1_SCLK",    Tx.cstr_altfuncAN( 4, 18 ) );
	CHECK( "sRIO[18]",     Tx.cstr_altfuncAN( 5, 18 ) );
	CHECK( "pRIO[18]",     Tx.cstr_altfuncAN( 6, 18 ) );
	CHECK( "PIO[18]",      Tx.cstr_altfuncAN( 7, 18 ) );
	CHECK( "gp_CLK1",      Tx.cstr_altfuncAN( 8, 18 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

