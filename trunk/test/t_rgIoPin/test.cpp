// 2017-06-12  William A. Hudson
//
// Testing:  rgIoPin  rGPIO Address Map class for Raspberry Pi.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgIoPin.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;
rgIoPin			Tx;


  CASE( "00", "Common object" );
    try {
	Bx.use_fake_mem();
	Tx.init_addr( &Bx );
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgIoPin			tx;
	PASS( "bare constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "constructor with rgAddrMap" );
    try {
	rgIoPin			tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "constructor uninit addr map" );
    try {
	rgAddrMap		bx;
	rgIoPin			tx  ( &bx );
	FAIL( "no throw" );
    }
    catch ( exception& e ) {
	CHECK( "get_mem_block() device not open",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "constructor with rgAddrMap" );
    try {
	rgAddrMap		bx;
	bx.use_fake_mem();
	rgIoPin			tx  ( &bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "15", "init_addr()" );
    try {
	rgAddrMap		bx;
	rgIoPin			tx;
	bx.use_fake_mem();
	tx.init_addr( &bx );
	PASS( "init_addr()" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16", "init_addr() device not open" );
    try {
	rgAddrMap		bx;
	rgIoPin			tx;
	tx.init_addr( &bx );
	FAIL( "no throw" );
    }
    catch ( runtime_error& e ) {
	CHECK( "get_mem_block() device not open",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
//--------------------------------------
  CASE( "19a", "get_base_addr()" );
    try {
	volatile uint32_t*	aa;
	aa = Tx.get_base_addr();
//	cout << "GpioBase=0x" <<hex << (long int)aa <<endl;
	if ( aa ) { PASS( "not null" ); } else { FAIL( "NULL" ); }
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "19b", "get_base_addr()" );
    try {
	rgIoPin			tx;
	volatile uint32_t*	aa;
	aa = tx.get_base_addr();
	if ( aa ) { FAIL( "not null" ); } else { PASS( "NULL" ); }
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic Register access
//--------------------------------------------------------------------------

  CASE( "20", "addr_reg()" );
    try {
	volatile uint32_t	*vp;
	vp = Tx.addr_reg( rgIoPin::rgPinRead_w0 );
	CHECK( 0x34,
	    (vp - Tx.get_base_addr())*4
	);
//	cout << "0x" <<hex << (vp - Tx.get_base_addr())*4 <<endl;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "read_reg()" );
    try {
	uint32_t		v;
	v = Tx.read_reg( rgIoPin::rgPinRead_w0 );
	CHECK( 0, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "modify_reg()" );
    try {
	uint32_t		v;
	Tx.modify_reg( rgIoPin::rgPinRead_w0, 0x000ff000, 0x55554444 );
	v = Tx.read_reg( rgIoPin::rgPinRead_w0 );
	CHECK( 0x00054000, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    //#!! Not sensitive to mask/value swap since x=0.

  CASE( "23", "addr_reg() uninitialized" );
    try {
	rgIoPin			tx;
	tx.addr_reg( rgIoPin::rgPinRead_w0 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgIoPin:: not initialized in:  addr_reg( 0x34/4 )",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
//## modify_reg() write-only registers

  CASE( "24a", "modify_reg() write-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinSet_w0, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "write-only register in rgIoPin::modify_reg():  rgPinSet_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "modify_reg() write-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinSet_w1, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "write-only register in rgIoPin::modify_reg():  rgPinSet_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24c", "modify_reg() write-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinClr_w0, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "write-only register in rgIoPin::modify_reg():  rgPinClr_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24c", "modify_reg() write-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinClr_w1, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "write-only register in rgIoPin::modify_reg():  rgPinClr_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgIoReg_enum string conversion
//--------------------------------------------------------------------------

  CASE( "70a", "str_IoReg_enum() object call" );
    try {
	const char		*v;
	v = Tx.str_IoReg_enum( rgIoPin::rgFsel2 );
	CHECK( "rgFsel2", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70b", "str_IoReg_enum() class call" );
    try {
	const char		*v;
	v = rgIoPin::str_IoReg_enum( rgIoPin::rgFsel3 );
	CHECK( "rgFsel3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70c", "str_IoReg_enum() cast integer" );
    try {
	const char		*v;
	v = rgIoPin::str_IoReg_enum( (rgIoPin::rgIoReg_enum) 3 );
	CHECK( "rgFsel3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71", "str_IoReg_enum() bad enum" );
    try {
	rgIoPin::str_IoReg_enum( (rgIoPin::rgIoReg_enum) 6 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgIoPin::  bad rgIoReg_enum in:  str_IoReg_enum()",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72", "str_IoReg_enum() all enum" );
    try {
	CHECK(                               "rgFsel0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel0 )
	);
	CHECK(                               "rgFsel1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel1 )
	);
	CHECK(                               "rgFsel2",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel2 )
	);
	CHECK(                               "rgFsel3",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel3 )
	);
	CHECK(                               "rgFsel4",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel4 )
	);
	CHECK(                               "rgFsel5",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgFsel5 )
	);

	CHECK(                               "rgPinSet_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinSet_w0 )
	);
	CHECK(                               "rgPinSet_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinSet_w1 )
	);

	CHECK(                               "rgPinClr_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinClr_w0 )
	);
	CHECK(                               "rgPinClr_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinClr_w1 )
	);

	CHECK(                               "rgPinRead_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinRead_w0 )
	);
	CHECK(                               "rgPinRead_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinRead_w1 )
	);

	CHECK(                               "rgEventStatus_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgEventStatus_w0 )
	);
	CHECK(                               "rgEventStatus_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgEventStatus_w1 )
	);

	CHECK(                               "rgDetectRising_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectRising_w0 )
	);
	CHECK(                               "rgDetectRising_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectRising_w1 )
	);

	CHECK(                               "rgDetectFalling_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectFalling_w0 )
	);
	CHECK(                               "rgDetectFalling_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectFalling_w1 )
	);

	CHECK(                               "rgDetectHigh_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectHigh_w0 )
	);
	CHECK(                               "rgDetectHigh_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectHigh_w1 )
	);

	CHECK(                               "rgDetectLow_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectLow_w0 )
	);
	CHECK(                               "rgDetectLow_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectLow_w1 )
	);

	CHECK(                               "rgDetectAsyncRising_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectAsyncRising_w0 )
	);
	CHECK(                               "rgDetectAsyncRising_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectAsyncRising_w1 )
	);

	CHECK(                               "rgDetectAsyncFalling_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectAsyncFalling_w0 )
	);
	CHECK(                               "rgDetectAsyncFalling_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgDetectAsyncFalling_w1 )
	);

	CHECK(                               "rgPullUpDown",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPullUpDown )
	);

	CHECK(                               "rgPullUpDownClk_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPullUpDownClk_w0 )
	);
	CHECK(                               "rgPullUpDownClk_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPullUpDownClk_w1 )
	);

    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## find_IoReg_enum()
//--------------------------------------------------------------------------

  CASE( "80a", "find_IoReg_enum() object call" );
    try {
	rgIoPin::rgIoReg_enum	v;
	v = Tx.find_IoReg_enum( "rgFsel3" );
	CHECK( rgIoPin::rgFsel3, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "80b", "find_IoReg_enum() class call" );
    try {
	rgIoPin::rgIoReg_enum	v;
	v = rgIoPin::find_IoReg_enum( "rgFsel3" );
	CHECK( rgIoPin::rgFsel3, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81", "find_IoReg_enum() null string" );
    try {
	rgIoPin::find_IoReg_enum( "" );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgIoPin::  str_IoReg_enum() no enum for string:  ",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "find_IoReg_enum() bad string" );
    try {
	rgIoPin::find_IoReg_enum( "bad_string" );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgIoPin::  str_IoReg_enum() no enum for string:  bad_string",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "find_IoReg_enum() NULL pointer" );
    try {
	rgIoPin::find_IoReg_enum( NULL );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "rgIoPin::  NULL string pointer in:  find_IoReg_enum()",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "84", "find_IoReg_enum() all strings" );
    try {

	CHECK(                rgIoPin::rgFsel0,
	    rgIoPin::find_IoReg_enum( "rgFsel0" )
	);
	CHECK(                rgIoPin::rgFsel1,
	    rgIoPin::find_IoReg_enum( "rgFsel1" )
	);
	CHECK(                rgIoPin::rgFsel2,
	    rgIoPin::find_IoReg_enum( "rgFsel2" )
	);
	CHECK(                rgIoPin::rgFsel3,
	    rgIoPin::find_IoReg_enum( "rgFsel3" )
	);
	CHECK(                rgIoPin::rgFsel4,
	    rgIoPin::find_IoReg_enum( "rgFsel4" )
	);
	CHECK(                rgIoPin::rgFsel5,
	    rgIoPin::find_IoReg_enum( "rgFsel5" )
	);

	CHECK(                rgIoPin::rgPinSet_w0,
	    rgIoPin::find_IoReg_enum( "rgPinSet_w0" )
	);
	CHECK(                rgIoPin::rgPinSet_w1,
	    rgIoPin::find_IoReg_enum( "rgPinSet_w1" )
	);

	CHECK(                rgIoPin::rgPinClr_w0,
	    rgIoPin::find_IoReg_enum( "rgPinClr_w0" )
	);
	CHECK(                rgIoPin::rgPinClr_w1,
	    rgIoPin::find_IoReg_enum( "rgPinClr_w1" )
	);

	CHECK(                rgIoPin::rgPinRead_w0,
	    rgIoPin::find_IoReg_enum( "rgPinRead_w0" )
	);
	CHECK(                rgIoPin::rgPinRead_w1,
	    rgIoPin::find_IoReg_enum( "rgPinRead_w1" )
	);

	CHECK(                rgIoPin::rgEventStatus_w0,
	    rgIoPin::find_IoReg_enum( "rgEventStatus_w0" )
	);
	CHECK(                rgIoPin::rgEventStatus_w1,
	    rgIoPin::find_IoReg_enum( "rgEventStatus_w1" )
	);

	CHECK(                rgIoPin::rgDetectRising_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectRising_w0" )
	);
	CHECK(                rgIoPin::rgDetectRising_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectRising_w1" )
	);

	CHECK(                rgIoPin::rgDetectFalling_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectFalling_w0" )
	);
	CHECK(                rgIoPin::rgDetectFalling_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectFalling_w1" )
	);

	CHECK(                rgIoPin::rgDetectHigh_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectHigh_w0" )
	);
	CHECK(                rgIoPin::rgDetectHigh_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectHigh_w1" )
	);

	CHECK(                rgIoPin::rgDetectLow_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectLow_w0" )
	);
	CHECK(                rgIoPin::rgDetectLow_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectLow_w1" )
	);

	CHECK(                rgIoPin::rgDetectAsyncRising_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectAsyncRising_w0" )
	);
	CHECK(                rgIoPin::rgDetectAsyncRising_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectAsyncRising_w1" )
	);

	CHECK(                rgIoPin::rgDetectAsyncFalling_w0,
	    rgIoPin::find_IoReg_enum( "rgDetectAsyncFalling_w0" )
	);
	CHECK(                rgIoPin::rgDetectAsyncFalling_w1,
	    rgIoPin::find_IoReg_enum( "rgDetectAsyncFalling_w1" )
	);

	CHECK(                rgIoPin::rgPullUpDown,
	    rgIoPin::find_IoReg_enum( "rgPullUpDown" )
	);

	CHECK(                rgIoPin::rgPullUpDownClk_w0,
	    rgIoPin::find_IoReg_enum( "rgPullUpDownClk_w0" )
	);
	CHECK(                rgIoPin::rgPullUpDownClk_w1,
	    rgIoPin::find_IoReg_enum( "rgPullUpDownClk_w1" )
	);

    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "99", "Done" );
}

