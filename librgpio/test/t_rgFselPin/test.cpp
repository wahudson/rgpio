// 2018-01-01  William A. Hudson
//
// Testing:  rgFselPin  Pin Function Select class.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgIoPins.h"
#include "rgFselPin.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;
Bx.open_fake_mem();

rgIoPins		Px  ( &Bx );
rgFselPin		Fx  ( &Bx );


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgFselPin		tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "constructor, deprecated" );
    try {
	rgFselPin		tx  ( &Px );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "address" );
    try {
	CHECK( 1, Fx.FselReg[0].addr() == Px.Fsel0.addr() );
	CHECK( 1, Fx.FselReg[1].addr() == Px.Fsel1.addr() );
	CHECK( 1, Fx.FselReg[2].addr() == Px.Fsel2.addr() );
	CHECK( 1, Fx.FselReg[3].addr() == Px.Fsel3.addr() );
	CHECK( 1, Fx.FselReg[4].addr() == Px.Fsel4.addr() );
	CHECK( 1, Fx.FselReg[5].addr() == Px.Fsel5.addr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## read_Fsel_bit()  Direct Register access
//--------------------------------------------------------------------------

  CASE( "20a", "read_Fsel_bit() bit > 53" );
    try {
	Fx.read_Fsel_bit( 62 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::fselreg_bit():  out-of-range bit= 62",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "modify_Fsel_bit() bit > 53" );
    try {
	Fx.modify_Fsel_bit( 55, rgFselPin::f_Out );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::fselreg_bit():  out-of-range bit= 55",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "read_Fsel_bit()" );
    try {
	rgFselPin::rgFsel_enum	mode;
	mode = Fx.read_Fsel_bit( 3 );
	CHECK( rgFselPin::f_In, mode );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "modify_Fsel_bit()" );
    try {
	rgFselPin::rgFsel_enum	mode;
	Fx.modify_Fsel_bit( 3, rgFselPin::f_Out );
	mode = Fx.read_Fsel_bit( 3 );
	CHECK( rgFselPin::f_Out, mode );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21c", "modify_Fsel_bit()" );
    try {
	rgFselPin::rgFsel_enum	mode;
	Fx.modify_Fsel_bit( 3, rgFselPin::f_Alt2 );
	mode = Fx.read_Fsel_bit( 3 );
	CHECK( rgFselPin::f_Alt2, mode );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## fselreg_bit()  Register field position
//--------------------------------------------------------------------------

  CASE( "60", "fselreg_bit()" );
    try {
	int			loc;
	rgReg_rw		*rp;
	rp = Fx.fselreg_bit( 11, &loc );
	CHECK( 3, loc );
	CHECK( 1, rp->addr() - Px.get_base_addr() );
	// offset is Fsel register number
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61", "fselreg_bit() bit > 53" );
    try {
	int			loc;
	Fx.fselreg_bit( 54, &loc );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::fselreg_bit():  out-of-range bit= 54",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62", "fselreg_bit()" );
    try {
	int			loc;
	rgReg_rw		*rp;

	for ( int k=0;  k<=9;  k++ ) {
	    rp = Fx.fselreg_bit( k, &loc );
	    CHECK( k*3, loc );
	    CHECK( 0, rp->addr() - Px.get_base_addr() );
	}

	for ( int k=10;  k<=19;  k++ ) {
	    rp = Fx.fselreg_bit( k, &loc );
	    CHECK( (k-10)*3, loc );
	    CHECK( 1, rp->addr() - Px.get_base_addr() );
	}

	for ( int k=20;  k<=29;  k++ ) {
	    rp = Fx.fselreg_bit( k, &loc );
	    CHECK( (k-20)*3, loc );
	    CHECK( 2, rp->addr() - Px.get_base_addr() );
	}

	// .. 30, 40

	for ( int k=50;  k<=53;  k++ ) {
	    rp = Fx.fselreg_bit( k, &loc );
	    CHECK( (k-50)*3, loc );
	    CHECK( 5, rp->addr() - Px.get_base_addr() );
	}

    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgFsel_enum string conversion
//--------------------------------------------------------------------------

  CASE( "70a", "str_rgFsel_enum() object call" );
    try {
	const char		*v;
	v = Fx.str_rgFsel_enum( rgFselPin::f_Alt3 );
	CHECK( "Alt3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70b", "str_rgFsel_enum() class call" );
    try {
	const char		*v;
	v = rgFselPin::str_rgFsel_enum( rgFselPin::f_Alt3 );
	CHECK( "Alt3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//---------------------------------------
  CASE( "71a", "str_rgFsel_enum() cast an integer" );
    try {
	const char		*v;
	v = Fx.str_rgFsel_enum( (rgFselPin::rgFsel_enum) 3 );
	CHECK( "Alt4", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "str_rgFsel_enum() bad enum" );
    try {
	Fx.str_rgFsel_enum( (rgFselPin::rgFsel_enum) 8 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::str_rgFsel_enum():  bad enum= 8",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//---------------------------------------
  CASE( "72", "str_IoReg_enum() all enum" );
    try {
	CHECK(                              "In",
	    Fx.str_rgFsel_enum( rgFselPin::f_In   )
	);
	CHECK(                              "Out",
	    Fx.str_rgFsel_enum( rgFselPin::f_Out  )
	);
	CHECK(                              "Alt0",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt0 )
	);
	CHECK(                              "Alt1",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt1 )
	);
	CHECK(                              "Alt2",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt2 )
	);
	CHECK(                              "Alt3",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt3 )
	);
	CHECK(                              "Alt4",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt4 )
	);
	CHECK(                              "Alt5",
	    Fx.str_rgFsel_enum( rgFselPin::f_Alt5 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## find_rgFsel_enum()
//--------------------------------------------------------------------------

  CASE( "80a", "find_rgFsel_enum() object call" );
    try {
	rgFselPin::rgFsel_enum	v;
	v = Fx.find_rgFsel_enum( "Alt2" );
	CHECK( rgFselPin::f_Alt2, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "80b", "find_rgFsel_enum() class call" );
    try {
	rgFselPin::rgFsel_enum	v;
	v = rgFselPin::find_rgFsel_enum( "Alt2" );
	CHECK( rgFselPin::f_Alt2, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "81", "find_rgFsel_enum() null pointer" );
    try {
	rgFselPin::find_rgFsel_enum( NULL );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgFselPin::find_rgFsel_enum():  NULL string pointer",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "find_rgFsel_enum() null string" );
    try {
	rgFselPin::find_rgFsel_enum( "" );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::find_rgFsel_enum() no enum for string:  ",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "find_rgFsel_enum() bad string" );
    try {
	rgFselPin::find_rgFsel_enum( "bad_string" );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::find_rgFsel_enum() no enum for string:  bad_string",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "84", "find_rgFsel_enum() all strings" );
    try {
	CHECK(       rgFselPin::f_In,
	    Fx.find_rgFsel_enum( "In"   )
	);
	CHECK(       rgFselPin::f_Out,
	    Fx.find_rgFsel_enum( "Out"  )
	);
	CHECK(       rgFselPin::f_Alt0,
	    Fx.find_rgFsel_enum( "Alt0" )
	);
	CHECK(       rgFselPin::f_Alt1,
	    Fx.find_rgFsel_enum( "Alt1" )
	);
	CHECK(       rgFselPin::f_Alt2,
	    Fx.find_rgFsel_enum( "Alt2" )
	);
	CHECK(       rgFselPin::f_Alt3,
	    Fx.find_rgFsel_enum( "Alt3" )
	);
	CHECK(       rgFselPin::f_Alt4,
	    Fx.find_rgFsel_enum( "Alt4" )
	);
	CHECK(       rgFselPin::f_Alt5,
	    Fx.find_rgFsel_enum( "Alt5" )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

