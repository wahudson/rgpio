// 2018-05-18  William A. Hudson
//
// Testing:  yUtil.cpp  Utility functions.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <sstream>	// std::ostringstream
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"	// unit test library

#include "yUtil.h"

using namespace std;


int main()
{

//--------------------------------------------------------------------------
//## cstr_bits32()
//--------------------------------------------------------------------------

  CASE( "11", "cstr_bits32()" );
    try {
	CHECK(
	    "0001 0010 0011 0100 0101 0110 0111 1000",
	    cstr_bits32( 0x12345678 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "cstr_bits32()" );
    try {
	CHECK(
	    "1000 1001 1010 1011 1100 1101 1110 1111",
	    cstr_bits32( 0x89abcdef )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## hex2char()
//--------------------------------------------------------------------------
// Check comparison repors integer value of character, not ideal.

  CASE( "21a", "hex2char() '0'" );
    try {
	CHECK( '0', hex2char( 0x00 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "hex2char() '9'" );
    try {
	CHECK( '9', hex2char( 0x09 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "hex2char() 'a'" );
    try {
	CHECK( 'a', hex2char( 0x0a ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21d", "hex2char() 'f'" );
    try {
	CHECK( 'f', hex2char( 0x0f ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "22a", "hex2char() mask 0xfffffff0" );
    try {
	CHECK( '0', hex2char( 0xfffffff0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "hex2char() mask 0xfffffffc" );
    try {
	CHECK( 'c', hex2char( 0xfffffffc ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

