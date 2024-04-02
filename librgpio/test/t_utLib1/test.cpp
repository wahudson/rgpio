/*
* Test utLib1 test library itself.
*
* Note:  Tests fail to show capability.
*/

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

    utLib1::ut_CASE( "10", "bare functions" );
    try {
	utLib1::ut_FAIL( __LINE__, "test fail" );
	utLib1::ut_PASS( "pass2" );
    }
    catch (...) {
	cerr << "caught something" << endl;
    }

    CASE( "11", "simple fail" );
    try {
	FAIL( "test fail" );
    }
    catch (...) {
	cerr << "caught something" << endl;
    }

    CASE( "12", "no check" );

    CASE( "13", "pass" );
    try {
	PASS( "test pass" );
    }
    catch (...) {
	cerr << "caught something" << endl;
    }

    CASE( "14", "fail on exception" );
    try {
	throw  runtime_error  ("oops");
	FAIL( "throw not happen" );	// not reached
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

    CASE( "15", "pass on exception" );
    try {
	throw  runtime_error  ("oops");
    }
    catch (...) {
	PASS( "expected exception" );
    }

    CASE( "16", "pass check" );
    try {
	CHECK( 42, 42 );
    }
    catch (...) {
    }

    CASE( "16b", "fail int check" );
    try {
	CHECK( 42, 43 );
    }
    catch (...) {
    }

    CASE( "17", "pass hex check" );
    try {
	uint32_t	jj = 0xaaaa5555;
	CHECKX( 0xaaaa5555, jj );
    }
    catch (...) {
    }

    CASE( "17b", "fail hex check" );
    try {
	int32_t		jj = -1;	// sign extend
	CHECKX( 0x00000001, jj );
    }
    catch (...) {
    }

    CASE( "18", "pass str check" );
    try {
	CHECK( "hello", "hello" );
    }
    catch (...) {
    }

    CASE( "19", "fail str check" );
    try {
	CHECK( "hello", "jello" );
	const char *exp = "now was the time";
	CHECK( "now is the time",
	    exp
	);
    }
    catch (...) {
    }

    CASE( "20", "pass on exception" );
    try {
	throw  runtime_error  ("oops");
	FAIL( "expected throw" );	// not reached
    }
    catch ( runtime_error& e ) {
	CHECK( "oops", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

    CASE( "21", "pass ambiguous overload CHECK()" );
    try {
	CHECK( 0, 0 );	// ambiguous call (long int) and (const char*)
    }
    catch (...) {
    }

    utLib1::ut_CASE( "99", "Done" );
}

