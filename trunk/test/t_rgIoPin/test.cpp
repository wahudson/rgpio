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

  CASE( "16", "device not open" );
    try {
	rgAddrMap		bx;
	rgIoPin			tx;
	tx.init_addr( &bx );
    }
    catch ( runtime_error& e ) {
        CHECK( "get_mem_block() device not open",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "19", "get_base_addr()" );
    try {
//	volatile uint32_t*	aa;
//	aa = Tx.get_base_addr();
//	cout << "GpioBase=0x" <<hex << (long int)aa <<endl;
	PASS( "get_base_addr()" );
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
	vp = Tx.addr_reg( rgIoPin::rgPinLevel_w0 );
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
	v = Tx.read_reg( rgIoPin::rgPinLevel_w0 );
        CHECK( 0, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "mod_reg()" );
    try {
	uint32_t		v;
	Tx.mod_reg( rgIoPin::rgPinLevel_w0, 0x55554444, 0x000ff000 );
	v = Tx.read_reg( rgIoPin::rgPinLevel_w0 );
        CHECK( 0x00054000, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "addr_reg() uninitialized" );
    try {
	rgIoPin			tx;
	volatile uint32_t	*vp;
	vp = tx.addr_reg( rgIoPin::rgPinLevel_w0 );
	cout << "0x" <<hex << (vp - Tx.get_base_addr())*4 <<endl;
    }
    catch ( logic_error& e ) {
        CHECK( "rgIoPin:: not initialized in:  addr_reg( 0x34/4 )",
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

        CHECK(                               "rgOutSet_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgOutSet_w0 )
	);
        CHECK(                               "rgOutSet_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgOutSet_w1 )
	);

        CHECK(                               "rgOutClr_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgOutClr_w0 )
	);
        CHECK(                               "rgOutClr_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgOutClr_w1 )
	);

        CHECK(                               "rgPinLevel_w0",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinLevel_w0 )
	);
        CHECK(                               "rgPinLevel_w1",
	    rgIoPin::str_IoReg_enum( rgIoPin::rgPinLevel_w1 )
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

  CASE( "99", "Done" );
}

