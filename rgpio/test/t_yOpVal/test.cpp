// 2024-09-14  William A. Hudson
//
// Testing:  yOpVal.cpp  Option Argument class.
//    10-19  Constructor
//    20-29  set() value
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <sstream>	// std::ostringstream
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"	// unit test library

#include "yOpVal.h"

using namespace std;


int main()
{

//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	yOpVal		tx;
	CHECK(           0, tx.Given );
	CHECKX( 0x00000000, tx.Val   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## set() value
//--------------------------------------------------------------------------

  CASE( "21", "set() hex" );
    try {
	yOpVal		tx;
	tx.set( "0xffffabba" );
	CHECK(            1, tx.Given );
	CHECKX(  0xffffabba, tx.Val   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "set() decimal" );
    try {
	yOpVal		tx;
	tx.set( "399" );
	CHECK(            1, tx.Given );
	CHECK(          399, tx.Val   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "set() octal" );
    try {
	yOpVal		tx;
	tx.set( "077" );
	CHECK(            1, tx.Given );
	CHECKX(  0x0000003f, tx.Val   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "set() bad number" );
    try {
	yOpVal		tx;
	tx.set( "0c3po" );
	FAIL( "no throw" );
    }
    catch ( std::invalid_argument& e ) {
	CHECK( "non-numeric option value:  0c3po", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "set() number -1" );
    try {
	yOpVal		tx;
	tx.set( "-1" );
	CHECK(            1, tx.Given );
	CHECKX(  0xffffffff, tx.Val   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "99", "Done" );
}

