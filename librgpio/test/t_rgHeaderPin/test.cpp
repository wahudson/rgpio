// 2023-02-07  William A. Hudson
//
// Testing:  rgHeaderPin  Header Pin Names class.
//    10-19  Constructor
//    20-29  pin2gpio_int()
//    30-39  pin2name_cstr()
//    40-49  .
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgHeaderPin.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgHeaderPin		Tx;	// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgHeaderPin	tx;
	CHECK(  40,     tx.MaxPin );
	CHECK(  27,     tx.MaxGpio );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "static MaxPin, MaxGpio" );
    try {
	CHECK(  40,     rgHeaderPin::MaxPin );
	CHECK(  27,     rgHeaderPin::MaxGpio );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## pin2gpio_int()
//--------------------------------------------------------------------------

  CASE( "20", "pin2gpio_int( 1..10)" );
    try {
	CHECK(  -3,  Tx.pin2gpio_int(  1 ) );
	CHECK(  -2,  Tx.pin2gpio_int(  2 ) );
	CHECK(   2,  Tx.pin2gpio_int(  3 ) );
	CHECK(  -2,  Tx.pin2gpio_int(  4 ) );
	CHECK(   3,  Tx.pin2gpio_int(  5 ) );
	CHECK(  -1,  Tx.pin2gpio_int(  6 ) );
	CHECK(   4,  Tx.pin2gpio_int(  7 ) );
	CHECK(  14,  Tx.pin2gpio_int(  8 ) );
	CHECK(  -1,  Tx.pin2gpio_int(  9 ) );
	CHECK(  15,  Tx.pin2gpio_int( 10 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "pin2gpio_int( 11..20)" );
    try {
	CHECK(  17,  Tx.pin2gpio_int( 11 ) );
	CHECK(  18,  Tx.pin2gpio_int( 12 ) );
	CHECK(  27,  Tx.pin2gpio_int( 13 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 14 ) );
	CHECK(  22,  Tx.pin2gpio_int( 15 ) );
	CHECK(  23,  Tx.pin2gpio_int( 16 ) );
	CHECK(  -3,  Tx.pin2gpio_int( 17 ) );
	CHECK(  24,  Tx.pin2gpio_int( 18 ) );
	CHECK(  10,  Tx.pin2gpio_int( 19 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 20 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "pin2gpio_int( 21..30)" );
    try {
	CHECK(   9,  Tx.pin2gpio_int( 21 ) );
	CHECK(  25,  Tx.pin2gpio_int( 22 ) );
	CHECK(  11,  Tx.pin2gpio_int( 23 ) );
	CHECK(   8,  Tx.pin2gpio_int( 24 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 25 ) );
	CHECK(   7,  Tx.pin2gpio_int( 26 ) );
	CHECK(   0,  Tx.pin2gpio_int( 27 ) );
	CHECK(   1,  Tx.pin2gpio_int( 28 ) );
	CHECK(   5,  Tx.pin2gpio_int( 29 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 30 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "pin2gpio_int( 31..40)" );
    try {
	CHECK(   6,  Tx.pin2gpio_int( 31 ) );
	CHECK(  12,  Tx.pin2gpio_int( 32 ) );
	CHECK(  13,  Tx.pin2gpio_int( 33 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 34 ) );
	CHECK(  19,  Tx.pin2gpio_int( 35 ) );
	CHECK(  16,  Tx.pin2gpio_int( 36 ) );
	CHECK(  26,  Tx.pin2gpio_int( 37 ) );
	CHECK(  20,  Tx.pin2gpio_int( 38 ) );
	CHECK(  -1,  Tx.pin2gpio_int( 39 ) );
	CHECK(  21,  Tx.pin2gpio_int( 40 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27", "pin2gpio_int(0) bad" );
    try {
	Tx.pin2gpio_int( 0 );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "pin2gpio_int():  header pin out-of-range:  0", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "28", "pin2gpio_int(41) bad" );
    try {
	Tx.pin2gpio_int( 41 );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "pin2gpio_int():  header pin out-of-range:  41", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "29", "class call pin2gpio_int()" );
    try {
	CHECK(  21,  rgHeaderPin::pin2gpio_int( 40 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## pin2name_cstr()
//--------------------------------------------------------------------------

  CASE( "30", "pin2name_cstr(1..10)" );
    try {
	CHECK( "+3.3V",  Tx.pin2name_cstr(  1 ) );
	CHECK( "+5V",    Tx.pin2name_cstr(  2 ) );
	CHECK( "Gpio2",  Tx.pin2name_cstr(  3 ) );
	CHECK( "+5V",    Tx.pin2name_cstr(  4 ) );
	CHECK( "Gpio3",  Tx.pin2name_cstr(  5 ) );
	CHECK( "GND",    Tx.pin2name_cstr(  6 ) );
	CHECK( "Gpio4",  Tx.pin2name_cstr(  7 ) );
	CHECK( "Gpio14", Tx.pin2name_cstr(  8 ) );
	CHECK( "GND",    Tx.pin2name_cstr(  9 ) );
	CHECK( "Gpio15", Tx.pin2name_cstr( 10 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "pin2name_cstr(11..20)" );
    try {
	CHECK( "Gpio17", Tx.pin2name_cstr( 11 ) );
	CHECK( "Gpio18", Tx.pin2name_cstr( 12 ) );
	CHECK( "Gpio27", Tx.pin2name_cstr( 13 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 14 ) );
	CHECK( "Gpio22", Tx.pin2name_cstr( 15 ) );
	CHECK( "Gpio23", Tx.pin2name_cstr( 16 ) );
	CHECK( "+3.3V",  Tx.pin2name_cstr( 17 ) );
	CHECK( "Gpio24", Tx.pin2name_cstr( 18 ) );
	CHECK( "Gpio10", Tx.pin2name_cstr( 19 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 20 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "pin2name_cstr(21..30)" );
    try {
	CHECK( "Gpio9",  Tx.pin2name_cstr( 21 ) );
	CHECK( "Gpio25", Tx.pin2name_cstr( 22 ) );
	CHECK( "Gpio11", Tx.pin2name_cstr( 23 ) );
	CHECK( "Gpio8",  Tx.pin2name_cstr( 24 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 25 ) );
	CHECK( "Gpio7",  Tx.pin2name_cstr( 26 ) );
	CHECK( "Gpio0",  Tx.pin2name_cstr( 27 ) );
	CHECK( "Gpio1",  Tx.pin2name_cstr( 28 ) );
	CHECK( "Gpio5",  Tx.pin2name_cstr( 29 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 30 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "pin2name_cstr(31..40)" );
    try {
	CHECK( "Gpio6",  Tx.pin2name_cstr( 31 ) );
	CHECK( "Gpio12", Tx.pin2name_cstr( 32 ) );
	CHECK( "Gpio13", Tx.pin2name_cstr( 33 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 34 ) );
	CHECK( "Gpio19", Tx.pin2name_cstr( 35 ) );
	CHECK( "Gpio16", Tx.pin2name_cstr( 36 ) );
	CHECK( "Gpio26", Tx.pin2name_cstr( 37 ) );
	CHECK( "Gpio20", Tx.pin2name_cstr( 38 ) );
	CHECK( "GND",    Tx.pin2name_cstr( 39 ) );
	CHECK( "Gpio21", Tx.pin2name_cstr( 40 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "37", "pin2name_cstr(0) bad" );
    try {
	Tx.pin2name_cstr( 0 );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "pin2name_cstr():  header pin out-of-range:  0", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38", "pin2name_cstr(41) bad" );
    try {
	Tx.pin2name_cstr( 41 );
	FAIL( "no throw" );
    }
    catch ( std::runtime_error& e ) {
	CHECK( "pin2name_cstr():  header pin out-of-range:  41", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "class call pin2name_cstr()" );
    try {
	CHECK( "GND",    rgHeaderPin::pin2name_cstr( 39 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

