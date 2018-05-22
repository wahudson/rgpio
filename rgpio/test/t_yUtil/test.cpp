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


  CASE( "99", "Done" );
}

