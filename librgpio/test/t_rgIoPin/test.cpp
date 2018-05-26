// 2017-06-12  William A. Hudson
//
// Testing:  rgIoPin  GPIO IO Pin control class
//    10-19  Constructor, init_addr(), get_base_addr()
//    20-29  Generic access {addr, read, write, modify, set, clr}_reg()
//    30-39  Addresses addr_reg()
//    40-49  Special access registers.  PinLevel, EventStatus
//    50-59  .
//    60-69  .
//    70-79  rgIoReg_enum string conversion
//    80-89  find_IoReg_enum()
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
	Bx.open_fake_mem();
	Tx.init_addr( &Bx );
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

// All tests should leave fake memory registers zero 0x00000000.

//--------------------------------------------------------------------------
//## Constructor, init_addr(), get_base_addr()
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
	bx.open_fake_mem();
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
	bx.open_fake_mem();
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
  CASE( "18", "addr_reg() uninitialized" );
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
//## Generic access {addr, read, write, modify, set, clr}_reg()
//--------------------------------------------------------------------------
// Exercise single register as an example.

  CASE( "20", "addr_reg()" );
    try {
	volatile uint32_t	*vp;
	vp = Tx.addr_reg( rgIoPin::rgPinRead_w0 );
	CHECKX( 0x34,
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
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgPinRead_w0 );
	*aa   = 0x12345678;
	v     = Tx.read_reg(rgIoPin::rgPinRead_w0 );
	CHECKX( 0x12345678, v );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "write_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgPinRead_w0 );
	CHECKX( 0x00000000, *aa );
	Tx.write_reg(   rgIoPin::rgPinRead_w0, 0x11223344 );
	CHECKX( 0x11223344, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23a", "modify_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	CHECKX( 0x00000000, *aa );
	Tx.modify_reg( rgIoPin::rgDetectHigh_w0, 0x00ffff00, 0x55554444 );
	CHECKX( 0x00554400, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23b", "modify_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	*aa   = 0xffffffff;
	CHECKX( 0xffffffff, *aa );
	Tx.modify_reg( rgIoPin::rgDetectHigh_w0, 0x00ffff00, 0x55554444 );
	CHECKX( 0xff5544ff, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "set_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	CHECKX( 0x00000000, *aa );
	Tx.set_reg( rgIoPin::rgDetectHigh_w0, 0x71111117 );
	CHECKX( 0x71111117, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "set_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	*aa   = 0xffffffff;
	CHECKX( 0xffffffff, *aa );
	Tx.set_reg( rgIoPin::rgDetectHigh_w0, 0x71111117 );
	CHECKX( 0xffffffff, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "clr_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	CHECKX( 0x00000000, *aa );
	Tx.clr_reg( rgIoPin::rgDetectHigh_w0, 0x71111117 );
	CHECKX( 0x00000000, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "clr_reg()" );
    try {
	volatile uint32_t*	aa= Tx.addr_reg( rgIoPin::rgDetectHigh_w0 );
	*aa   = 0xffffffff;
	CHECKX( 0xffffffff, *aa );
	Tx.clr_reg( rgIoPin::rgDetectHigh_w0, 0x71111117 );
	CHECKX( 0x8eeeeee8, *aa );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Generic access Exceptions on non-normal read/write registers.
//--------------------------------------------------------------------------
// Mix modify_reg(), set_reg(), and clr_reg() -- all are read/modify/write.

//--------------------------------------
  CASE( "25a", "set_reg( rgPinSet_w0 ) write-only reg" );
    try {
	Tx.set_reg( rgIoPin::rgPinSet_w0, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinSet_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25b", "clr_reg( rgPinSet_w1 ) write-only reg" );
    try {
	Tx.clr_reg( rgIoPin::rgPinSet_w1, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinSet_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "26a", "set_reg( rgPinClr_w0 ) write-only reg" );
    try {
	Tx.set_reg( rgIoPin::rgPinClr_w0, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinClr_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26b", "clr_reg( rgPinClr_w1 ) write-only reg" );
    try {
	Tx.set_reg( rgIoPin::rgPinClr_w1, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinClr_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "27a", "modify_reg( rgPinRead_w0 ) read-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinRead_w0, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinRead_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27b", "modify_reg( rgPinRead_w1 ) read-only reg" );
    try {
	Tx.modify_reg( rgIoPin::rgPinRead_w1, 0x000ff000, 0x55554444 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgPinRead_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "28a", "set_reg( rgEventStatus_w0 ) read-clear reg" );
    try {
	Tx.set_reg( rgIoPin::rgEventStatus_w0, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgEventStatus_w0",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "28b", "clr_reg( rgEventStatus_w1 ) read-clear reg" );
    try {
	Tx.clr_reg( rgIoPin::rgEventStatus_w1, 0x000ff000 );
	FAIL( "no throw" );
    }
    catch ( logic_error& e ) {
	CHECK( "inappropriate register in rgIoPin::modify_reg():  rgEventStatus_w1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Addresses addr_reg()
//--------------------------------------------------------------------------

  CASE( "30", "addr_reg()" );
    try {
      volatile uint32_t		*ba = Tx.get_base_addr();

      CHECKX( 0x00, (Tx.addr_reg( rgIoPin::rgFsel0                 ) - ba)*4 );
      CHECKX( 0x04, (Tx.addr_reg( rgIoPin::rgFsel1                 ) - ba)*4 );
      CHECKX( 0x08, (Tx.addr_reg( rgIoPin::rgFsel2                 ) - ba)*4 );
      CHECKX( 0x0c, (Tx.addr_reg( rgIoPin::rgFsel3                 ) - ba)*4 );
      CHECKX( 0x10, (Tx.addr_reg( rgIoPin::rgFsel4                 ) - ba)*4 );
      CHECKX( 0x14, (Tx.addr_reg( rgIoPin::rgFsel5                 ) - ba)*4 );

      CHECKX( 0x1c, (Tx.addr_reg( rgIoPin::rgPinSet_w0             ) - ba)*4 );
      CHECKX( 0x20, (Tx.addr_reg( rgIoPin::rgPinSet_w1             ) - ba)*4 );
      CHECKX( 0x28, (Tx.addr_reg( rgIoPin::rgPinClr_w0             ) - ba)*4 );
      CHECKX( 0x2c, (Tx.addr_reg( rgIoPin::rgPinClr_w1             ) - ba)*4 );
      CHECKX( 0x34, (Tx.addr_reg( rgIoPin::rgPinRead_w0            ) - ba)*4 );
      CHECKX( 0x38, (Tx.addr_reg( rgIoPin::rgPinRead_w1            ) - ba)*4 );

      CHECKX( 0x40, (Tx.addr_reg( rgIoPin::rgEventStatus_w0        ) - ba)*4 );
      CHECKX( 0x44, (Tx.addr_reg( rgIoPin::rgEventStatus_w1        ) - ba)*4 );

      CHECKX( 0x4c, (Tx.addr_reg( rgIoPin::rgDetectRising_w0       ) - ba)*4 );
      CHECKX( 0x50, (Tx.addr_reg( rgIoPin::rgDetectRising_w1       ) - ba)*4 );
      CHECKX( 0x58, (Tx.addr_reg( rgIoPin::rgDetectFalling_w0      ) - ba)*4 );
      CHECKX( 0x5c, (Tx.addr_reg( rgIoPin::rgDetectFalling_w1      ) - ba)*4 );
      CHECKX( 0x64, (Tx.addr_reg( rgIoPin::rgDetectHigh_w0         ) - ba)*4 );
      CHECKX( 0x68, (Tx.addr_reg( rgIoPin::rgDetectHigh_w1         ) - ba)*4 );
      CHECKX( 0x70, (Tx.addr_reg( rgIoPin::rgDetectLow_w0          ) - ba)*4 );
      CHECKX( 0x74, (Tx.addr_reg( rgIoPin::rgDetectLow_w1          ) - ba)*4 );
      CHECKX( 0x7c, (Tx.addr_reg( rgIoPin::rgDetectAsyncRising_w0  ) - ba)*4 );
      CHECKX( 0x80, (Tx.addr_reg( rgIoPin::rgDetectAsyncRising_w1  ) - ba)*4 );
      CHECKX( 0x88, (Tx.addr_reg( rgIoPin::rgDetectAsyncFalling_w0 ) - ba)*4 );
      CHECKX( 0x8c, (Tx.addr_reg( rgIoPin::rgDetectAsyncFalling_w1 ) - ba)*4 );

      CHECKX( 0x94, (Tx.addr_reg( rgIoPin::rgPullUpDown            ) - ba)*4 );
      CHECKX( 0x98, (Tx.addr_reg( rgIoPin::rgPullUpDownClk_w0      ) - ba)*4 );
      CHECKX( 0x9c, (Tx.addr_reg( rgIoPin::rgPullUpDownClk_w1      ) - ba)*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "addr_PinRead_w0()" );
    try {
	CHECK( 0,           Tx.addr_PinRead_w0() -
	    Tx.addr_reg( rgIoPin::rgPinRead_w0 )
	);
	CHECK( 0,           Tx.addr_PinRead_w1() -
	    Tx.addr_reg( rgIoPin::rgPinRead_w1 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "addr_PinSet_w0()" );
    try {
	CHECK( 0,           Tx.addr_PinSet_w0() -
	    Tx.addr_reg( rgIoPin::rgPinSet_w0 )
	);
	CHECK( 0,           Tx.addr_PinSet_w1() -
	    Tx.addr_reg( rgIoPin::rgPinSet_w1 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "addr_PinClr_w0()" );
    try {
	CHECK( 0,           Tx.addr_PinClr_w0() -
	    Tx.addr_reg( rgIoPin::rgPinClr_w0 )
	);
	CHECK( 0,           Tx.addr_PinClr_w1() -
	    Tx.addr_reg( rgIoPin::rgPinClr_w1 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "addr_EventStatus_w0()" );
    try {
	CHECK( 0,           Tx.addr_EventStatus_w0() -
	    Tx.addr_reg( rgIoPin::rgEventStatus_w0 )
	);
	CHECK( 0,           Tx.addr_EventStatus_w1() -
	    Tx.addr_reg( rgIoPin::rgEventStatus_w1 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Special access registers.  PinLevel, EventStatus
//--------------------------------------------------------------------------
// These test the functions, not the actual hardware behavior.

  CASE( "40", "clear many registers" );
    try {
	*( Tx.addr_reg( rgIoPin::rgPinSet_w0             ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgPinClr_w0             ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgPinRead_w0            ) ) = 0x00000000;

	*( Tx.addr_reg( rgIoPin::rgEventStatus_w0        ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectRising_w0       ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectFalling_w0      ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectHigh_w0         ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectLow_w0          ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectAsyncRising_w0  ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgDetectAsyncFalling_w0 ) ) = 0x00000000;

	*( Tx.addr_reg( rgIoPin::rgPullUpDown            ) ) = 0x00000000;
	*( Tx.addr_reg( rgIoPin::rgPullUpDownClk_w0      ) ) = 0x00000000;
	CHECKX( 0x00000000, Tx.read_reg( rgIoPin::rgPinRead_w0 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "rgPinRead_w0, read_PinLevel_w0()" );
    try {
	volatile uint32_t*
			aa= Tx.addr_reg( rgIoPin::rgPinRead_w0 );
	CHECKX( 0x00000000, Tx.read_reg( rgIoPin::rgPinRead_w0 ) );
	CHECKX( 0x00000000, Tx.read_PinLevel_w0() );
	*aa   = 0xc5555553;
	CHECKX( 0xc5555553, Tx.read_reg( rgIoPin::rgPinRead_w0 ) );
	CHECKX( 0xc5555553, Tx.read_PinLevel_w0() );
	Tx.write_reg(                    rgIoPin::rgPinRead_w0, 0x3aaaaaac );
	CHECKX( 0x3aaaaaac, Tx.read_reg( rgIoPin::rgPinRead_w0 ) );
	CHECKX( 0x3aaaaaac, Tx.read_PinLevel_w0() );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "rgPinSet_w0, set_PinLevel_w0()" );
    try {
	volatile uint32_t*
			aa= Tx.addr_reg( rgIoPin::rgPinSet_w0 );
	CHECKX( 0x00000000, Tx.read_reg( rgIoPin::rgPinSet_w0 ) );
	*aa   = 0xc5555553;
	CHECKX( 0xc5555553, Tx.read_reg( rgIoPin::rgPinSet_w0 ) );
	Tx.write_reg(                    rgIoPin::rgPinSet_w0, 0x3aaaaaac );
	CHECKX( 0x3aaaaaac, Tx.read_reg( rgIoPin::rgPinSet_w0 ) );
	Tx.set_PinLevel_w0( 0xf1111117 );
	CHECKX( 0xf1111117, Tx.read_reg( rgIoPin::rgPinSet_w0 ) );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "rgPinClr_w0, set_PinLevel_w0()" );
    try {
	volatile uint32_t*
			aa= Tx.addr_reg( rgIoPin::rgPinClr_w0 );
	CHECKX( 0x00000000, Tx.read_reg( rgIoPin::rgPinClr_w0 ) );
	*aa   = 0xa333333c;
	CHECKX( 0xa333333c, Tx.read_reg( rgIoPin::rgPinClr_w0 ) );
	Tx.write_reg(                    rgIoPin::rgPinClr_w0, 0x5cccccc3 );
	CHECKX( 0x5cccccc3, Tx.read_reg( rgIoPin::rgPinClr_w0 ) );
	Tx.clr_PinLevel_w0( 0xf2222227 );
	CHECKX( 0xf2222227, Tx.read_reg( rgIoPin::rgPinClr_w0 ) );
	*aa   = 0x00000000;
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "44a", "rgEventStatus_w0, read_EventStatus_w0()" );
    try {
	volatile uint32_t*
			aa= Tx.addr_reg( rgIoPin::rgEventStatus_w0 );
	CHECKX( 0x00000000, Tx.read_reg( rgIoPin::rgEventStatus_w0 ) );
	CHECKX( 0x00000000,                 Tx.read_EventStatus_w0() );
	*aa   = 0xf4444443;
	CHECKX( 0xf4444443, Tx.read_reg( rgIoPin::rgEventStatus_w0 ) );
	CHECKX( 0xf4444443, Tx.read_EventStatus_w0() );
	Tx.write_reg(                    rgIoPin::rgEventStatus_w0, 0x7bbbbbb1 );
	CHECKX( 0x7bbbbbb1, Tx.read_reg( rgIoPin::rgEventStatus_w0 ) );
	CHECKX( 0x7bbbbbb1,                 Tx.read_EventStatus_w0() );
	Tx.clr_EventStatus_w0( 0x1444444e );
	CHECKX( 0x1444444e, Tx.read_reg( rgIoPin::rgEventStatus_w0 ) );
	CHECKX( 0x1444444e,                 Tx.read_EventStatus_w0() );
	*aa   = 0x00000000;
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

