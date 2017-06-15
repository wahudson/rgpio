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


  CASE( "99", "Done" );
}

