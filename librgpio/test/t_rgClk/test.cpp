// 2019-12-17  William A. Hudson
//
// Testing:  rgClk  Clock Manager enumerated class.
//    10-19  Constructor
//    20-29  Addresses
//    30-39  Direct register access  read(), write()
//    40-49  Full register get(), put(), grab(), push()
//    50-59  Special functions  wait_while_busy()
//    60-69  Special functions  apply_nicely()
//    70-79  rgClk_enum - get_Cntl_offset(), cstr_clk_enum(), int2clk_enum()
//    80-89  Cntl Field Accessors  get_(), put_()
//    90-99  Divr Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgClk.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;

  CASE( "00", "AddrMap object" );
    try {
	Bx.open_fake_mem();
	PASS( "AddrMap object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgClk			Tx   ( &Bx, rgClk::cm_Clk0 );	// test object
rgClk			Tx2  ( &Bx, rgClk::cm_Clk2 );	// test object, Clk2


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor Clk0" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk0 );
	CHECK(  0,      tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11", "constructor Clk1" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk1 );
	CHECK(  1,      tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "12", "constructor Clk2" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk2 );
	CHECK(  2,      tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "constructor ClkPcm" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_ClkPcm );
	CHECK(  3,      tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "constructor ClkPwm" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_ClkPwm );
	CHECK(  4,      tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register Class constructors - be sure they exist.

  CASE( "17a", "rgClk_Cntl class" );
    try {
	rgClk_Cntl	rx  ( Tx.Cntl );
	CHECKX( 0x70,  (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "17b", "rgClk_Divr class" );
    try {
	rgClk_Divr	rx  ( Tx.Divr );
	CHECKX( 0x74,  (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "19", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e101000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Addresses
//--------------------------------------------------------------------------

  CASE( "20", "Clk0 addr()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk0 );
	CHECKX( 0x70,     (tx.Cntl.addr() - tx.get_base_addr())*4 );
	CHECKX( 0x74,     (tx.Divr.addr() - tx.get_base_addr())*4 );
	CHECK(  0,         tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "Clk1 addr()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk1 );
	CHECKX( 0x78,     (tx.Cntl.addr() - tx.get_base_addr())*4 );
	CHECKX( 0x7c,     (tx.Divr.addr() - tx.get_base_addr())*4 );
	CHECK(  1,         tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "Clk2 addr()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk2 );
	CHECKX( 0x80,     (tx.Cntl.addr() - tx.get_base_addr())*4 );
	CHECKX( 0x84,     (tx.Divr.addr() - tx.get_base_addr())*4 );
	CHECK(  2,         tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "ClkPcm addr()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_ClkPcm );
	CHECKX( 0x98,     (tx.Cntl.addr() - tx.get_base_addr())*4 );
	CHECKX( 0x9c,     (tx.Divr.addr() - tx.get_base_addr())*4 );
	CHECK(  rgClk::cm_ClkPcm, tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "ClkPwm addr()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_ClkPwm );
	CHECKX( 0xa0,     (tx.Cntl.addr() - tx.get_base_addr())*4 );
	CHECKX( 0xa4,     (tx.Divr.addr() - tx.get_base_addr())*4 );
	CHECK(  rgClk::cm_ClkPwm, tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "27a", "Cntl.addr() Clk0" );
    try {
	volatile uint32_t	*vp;	// Example address variable
	vp = Tx.Cntl.addr();
	CHECKX( 0x70, (vp - Tx.get_base_addr())*4 );
	CHECK(  0,          Tx.get_clk_enum() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read(), write()
//--------------------------------------------------------------------------

  CASE( "30", "condition write(), read()" );
    try {
	Tx.Cntl.write(     0xffffffff );
	Tx.Divr.write(     0xffffffff );
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "Cntl.read()" );
    try {
	Tx.Cntl.write(     0x222dd222 );
	CHECKX(            0x222dd222, Tx.Cntl.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Divr.read()" );
    try {
	Tx.Divr.write(     0x111ee111 );
	CHECKX(            0x111ee111, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "check final register pattern" );
    try {
	CHECKX(            0x222dd222, Tx.Cntl.read()     );
	CHECKX(            0x111ee111, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Full register get(), put(), grab(), push()
//--------------------------------------------------------------------------

  CASE( "40", "condition registers" );
    try {
	Tx.Cntl.write(     0x00000000 );
	Tx.Divr.write(     0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "41a", "Cntl.grab(), get()" );
    try {
	Tx.Cntl.write(     0xccc33ccc );
	Tx.Cntl.put(       0x77444477 );
	CHECKX(            0xccc33ccc, Tx.Cntl.read()     );
	CHECKX(            0x77444477, Tx.Cntl.get()      );
	Tx.Cntl.grab();
	CHECKX(            0xccc33ccc, Tx.Cntl.read()     );
	CHECKX(            0xccc33ccc, Tx.Cntl.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "Cntl.put(), push()" );
    try {
	CHECKX(            0xccc33ccc, Tx.Cntl.read()     );
	CHECKX(            0xccc33ccc, Tx.Cntl.get()      );
	Tx.Cntl.put(       0xbeeffeeb );
	CHECKX(            0xccc33ccc, Tx.Cntl.read()     );
	CHECKX(            0xbeeffeeb, Tx.Cntl.get()      );
	Tx.Cntl.push();
	CHECKX(            0xbeeffeeb, Tx.Cntl.read()     );
	CHECKX(            0xbeeffeeb, Tx.Cntl.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "Divr.grab(), get()" );
    try {
	Tx.Divr.write(     0x55aaaa55 );
	Tx.Divr.put(       0x11111111 );
	CHECKX(            0x55aaaa55, Tx.Divr.read()     );
	CHECKX(            0x11111111, Tx.Divr.get()      );
	Tx.Divr.grab();
	CHECKX(            0x55aaaa55, Tx.Divr.read()     );
	CHECKX(            0x55aaaa55, Tx.Divr.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "Divr.put(), push()" );
    try {
	CHECKX(            0x55aaaa55, Tx.Divr.read()     );
	CHECKX(            0x55aaaa55, Tx.Divr.get()      );
	Tx.Divr.put(       0xccc33ccc );
	CHECKX(            0x55aaaa55, Tx.Divr.read()     );
	CHECKX(            0xccc33ccc, Tx.Divr.get()      );
	Tx.Divr.push();
	CHECKX(            0xccc33ccc, Tx.Divr.read()     );
	CHECKX(            0xccc33ccc, Tx.Divr.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "45", "init_put_reset()" );
    try {
	Tx.Cntl.write(     0xffffffff );
	Tx.Divr.write(     0xffffffff );
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Divr.read()     );
	Tx.Cntl.put(       0xffffffff );
	Tx.Divr.put(       0xffffffff );
	CHECKX(            0xffffffff, Tx.Cntl.get()      );
	CHECKX(            0xffffffff, Tx.Divr.get()      );
	Tx.init_put_reset();
	CHECKX(            0x00000000, Tx.Cntl.get()      );
	CHECKX(            0x00000000, Tx.Divr.get()      );
	//
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "46", "grab_regs()" );
    try {
	Tx.Cntl.write(     0x11111111 );
	Tx.Divr.write(     0x22222222 );
	//
	Tx.Cntl.put(       0xaaaaaaaa );
	Tx.Divr.put(       0xbbbbbbbb );
	Tx.grab_regs();
	CHECKX(            0x11111111, Tx.Cntl.get()      );
	CHECKX(            0x22222222, Tx.Divr.get()      );
	//
	CHECKX(            0x11111111, Tx.Cntl.read()     );
	CHECKX(            0x22222222, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "47", "push_regs()" );
    try {
	Tx.Cntl.write(     0x33333333 );
	Tx.Divr.write(     0x44444444 );
	//
	Tx.Cntl.put(       0xcccccccc );
	Tx.Divr.put(       0xdddddddd );
	Tx.push_regs();
	CHECKX(            0xcccccccc, Tx.Cntl.get()      );
	CHECKX(            0xdddddddd, Tx.Divr.get()      );
	//
	CHECKX(            0xcccccccc, Tx.Cntl.read()     );
	CHECKX(            0xdddddddd, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "48", "apply_regs() sets passwd" );
    try {
	Tx.Cntl.write(     0x33333333 );
	Tx.Divr.write(     0x44444444 );
	//
	Tx.Cntl.put(       0xcccccccc );
	Tx.Divr.put(       0xdddddddd );
	Tx.apply_regs();
	CHECKX(            0x5acccccc, Tx.Cntl.get()      );
	CHECKX(            0x5adddddd, Tx.Divr.get()      );
	//
	CHECKX(            0x5acccccc, Tx.Cntl.read()     );
	CHECKX(            0x5adddddd, Tx.Divr.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Special functions  wait_while_busy()
//--------------------------------------------------------------------------

  CASE( "50a", "default wait_time_ns()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk0 );
	CHECK(       0, tx.wait_time_ns() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "default wait_count_n()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk0 );
	CHECK(   10000, tx.wait_count_n() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50c", "default get_busy_count()" );
    try {
	rgClk		tx  ( &Bx, rgClk::cm_Clk0 );
	CHECK(  0,      tx.get_busy_count() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53", "set/get wait_time_ns()" );
    try {
	Tx.wait_time_ns( 5012 );
	CHECK(  5012,       Tx.wait_time_ns() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "54", "set/get wait_count_n()" );
    try {
	Tx.wait_count_n( 42 );
	CHECK(  42,         Tx.wait_count_n() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "55", "wait_while_busy() Busy_1=1" );
    try {
	Tx.wait_count_n(            3 );
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Busy_1(         1 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0x00000000 );
	CHECK(                      1, Tx.wait_while_busy() );	// DUT
	CHECK(                      3, Tx.get_busy_count() );
	CHECKX(            0x00000000, Tx.Cntl.get()      );	// unchanged
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "56", "wait_while_busy() Busy_1=0" );
    try {
	Tx.wait_count_n(            3 );
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Busy_1(         0 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0x00000000 );
	CHECK(                      0, Tx.wait_while_busy() );	// DUT
	CHECK(                      0, Tx.get_busy_count() );
	CHECKX(            0x00000000, Tx.Cntl.get()      );	// unchanged
	CHECKX(            0xffffff7f, Tx.Cntl.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "57", "wait_while_busy() Busy_1=1, no wait" );
    try {
	Tx.wait_count_n(            0 );
	Tx.Cntl.put_Busy_1(         1 );
	Tx.Cntl.push();
	CHECK(                      1, Tx.wait_while_busy() );	// DUT
	CHECK(                      0, Tx.get_busy_count() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Special functions  apply_nicely()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "60a", "apply_nicely() running, Enable=1" );
    try {
	Tx.wait_time_ns(         2000 );
	Tx.wait_count_n(            3 );
	Tx.Divr.write(     0xff333ccc );
	Tx.Divr.put(       0x00ddd555 );
	//
	Tx.Cntl.put(       0x00000000 );
	Tx.Cntl.put_Busy_1(         1 );
	Tx.Cntl.put_Enable_1(       1 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Enable_1(       1 );
	CHECKX(            0xff333ccc, Tx.Divr.read()     );
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x00000090, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Cntl.get()      );
	//
	CHECK(                      1, Tx.apply_nicely() );	// DUT
	CHECK(                      3, Tx.get_busy_count() );
	//
	CHECKX(            0xff333ccc, Tx.Divr.read()     );	// no write
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x5a000080, Tx.Cntl.read()     );	// disabled
	CHECKX(            0xffffffff, Tx.Cntl.get()      );	// unchanged
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60b", "apply_nicely() running, Enable=0" );
    try {
	Tx.wait_time_ns(         2000 );
	Tx.wait_count_n(            3 );
	Tx.Divr.write(     0xff333ccc );
	Tx.Divr.put(       0x00ddd555 );
	//
	Tx.Cntl.put(       0x00000000 );
	Tx.Cntl.put_Busy_1(         1 );
	Tx.Cntl.put_Enable_1(       1 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Enable_1(       0 );
	CHECKX(            0xff333ccc, Tx.Divr.read()     );
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x00000090, Tx.Cntl.read()     );
	CHECKX(            0xffffffef, Tx.Cntl.get()      );
	//
	CHECK(                      1, Tx.apply_nicely() );	// DUT
	CHECK(                      3, Tx.get_busy_count() );
	//
	CHECKX(            0xff333ccc, Tx.Divr.read()     );	// no write
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x5a000080, Tx.Cntl.read()     );	// disabled
	CHECKX(            0xffffffef, Tx.Cntl.get()      );	// unchanged
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "61a", "apply_nicely() stopped, Enable=1" );
    try {
	Tx.wait_time_ns(         2000 );
	Tx.wait_count_n(            3 );
	Tx.Divr.write(     0xff333ccc );
	Tx.Divr.put(       0x00ddd555 );
	//
	Tx.Cntl.put(       0x00000000 );
	Tx.Cntl.put_Busy_1(         0 );
	Tx.Cntl.put_Enable_1(       0 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Enable_1(       1 );
	CHECKX(            0xff333ccc, Tx.Divr.read()     );
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Cntl.get()      );
	//
	CHECK(                      0, Tx.apply_nicely() );	// DUT
	CHECK(                      0, Tx.get_busy_count() );
	//
	CHECKX(            0x5addd555, Tx.Divr.read()     );
	CHECKX(            0x5addd555, Tx.Divr.get()      );
	CHECKX(            0x5affffff, Tx.Cntl.read()     );
	CHECKX(            0x5affffff, Tx.Cntl.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "apply_nicely() stopped, Enable=0" );
    try {
	Tx.wait_time_ns(         2000 );
	Tx.wait_count_n(            3 );
	Tx.Divr.write(     0xff333ccc );
	Tx.Divr.put(       0x00ddd555 );
	//
	Tx.Cntl.put(       0x00000000 );
	Tx.Cntl.put_Busy_1(         0 );
	Tx.Cntl.put_Enable_1(       0 );
	Tx.Cntl.push();
	Tx.Cntl.put(       0xffffffff );
	Tx.Cntl.put_Enable_1(       0 );
	CHECKX(            0xff333ccc, Tx.Divr.read()     );
	CHECKX(            0x00ddd555, Tx.Divr.get()      );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0xffffffef, Tx.Cntl.get()      );
	//
	CHECK(                      0, Tx.apply_nicely() );	// DUT
	CHECK(                      0, Tx.get_busy_count() );
	//
	CHECKX(            0x5addd555, Tx.Divr.read()     );
	CHECKX(            0x5addd555, Tx.Divr.get()      );
	CHECKX(            0x5affffef, Tx.Cntl.read()     );
	CHECKX(            0x5affffef, Tx.Cntl.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgClk_enum - get_Cntl_offset(), cstr_clk_enum(), int2clk_enum()
//--------------------------------------------------------------------------

  CASE( "70", "rgClk_enum values" );
    try {
	CHECK(  0, rgClk::cm_Clk0     );
	CHECK(  1, rgClk::cm_Clk1     );
	CHECK(  2, rgClk::cm_Clk2     );
	CHECK(  3, rgClk::cm_ClkPcm   );
	CHECK(  4, rgClk::cm_ClkPwm   );
	CHECK(  4, rgClk::cm_MaxEnum  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "71a", "get_Cntl_offset() all values" );
    try {
	CHECKX( 0x00000070, 4 * Tx.get_Cntl_offset( rgClk::cm_Clk0   ) );
	CHECKX( 0x00000078, 4 * Tx.get_Cntl_offset( rgClk::cm_Clk1   ) );
	CHECKX( 0x00000080, 4 * Tx.get_Cntl_offset( rgClk::cm_Clk2   ) );
	CHECKX( 0x00000098, 4 * Tx.get_Cntl_offset( rgClk::cm_ClkPcm ) );
	CHECKX( 0x000000a0, 4 * Tx.get_Cntl_offset( rgClk::cm_ClkPwm ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "get_Cntl_offset() bad enum" );
    try {
	CHECKX( 0x0000001c, Tx.get_Cntl_offset( (rgClk::rgClk_enum) -1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "72a", "cstr_clk_enum() all names" );
    try {
	CHECK( "Clk0",   Tx.cstr_clk_enum( rgClk::cm_Clk0   ) );
	CHECK( "Clk1",   Tx.cstr_clk_enum( rgClk::cm_Clk1   ) );
	CHECK( "Clk2",   Tx.cstr_clk_enum( rgClk::cm_Clk2   ) );
	CHECK( "ClkPcm", Tx.cstr_clk_enum( rgClk::cm_ClkPcm ) );
	CHECK( "ClkPwm", Tx.cstr_clk_enum( rgClk::cm_ClkPwm ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "cstr_clk_enum() bad enum" );
    try {
	CHECK(  "Clk0", Tx.cstr_clk_enum( (rgClk::rgClk_enum) -1 ) );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClk::cstr_clk_enum() invalid rgClk_enum:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "75a", "int2clk_enum()" );
    try {
	rgClk::rgClk_enum	ev;	// verify return type
	ev = Tx.int2clk_enum( 4 );
	CHECK(  4, ev );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75b", "int2clk_enum() bad int" );
    try {
	Tx.int2clk_enum( -1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClk::int2clk_enum() int out of range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75c", "int2clk_enum() bad int" );
    try {
	Tx.int2clk_enum( 5 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgClk::int2clk_enum() int out of range:  5",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Cntl Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

  CASE( "80", "clear registers" );	// to avoid any wayward 1s
    try {
	Tx.Cntl.write(     0x00000000 );
	Tx.Divr.write(     0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Divr.read()     );
	Tx.Cntl.put(       0x00000000 );
	Tx.Divr.put(       0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.get()      );
	CHECKX(            0x00000000, Tx.Divr.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "81a", "Cntl.get_Passwd_8()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Passwd_8(    0xff );
	CHECKX(            0xff000000, Tx.Cntl.get()     );
	CHECKX(                  0xff, Tx.Cntl.get_Passwd_8() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81c", "Cntl.get_Passwd_8() bad value" );
    try {
	Tx.Cntl.put_Passwd_8(   0x100 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xff:  0x100",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "82a", "Cntl.get_Mash_2()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Mash_2(         3 );
	CHECKX(            0x00000600, Tx.Cntl.get()     );
	CHECKX(                     3, Tx.Cntl.get_Mash_2() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82c", "Cntl.get_Mash_2() bad value" );
    try {
	Tx.Cntl.put_Mash_2(         4 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x3:  0x4",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "83a", "Cntl.get_Flip_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Flip_1(         1 );
	CHECKX(            0x00000100, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Flip_1() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83c", "Cntl.get_Flip_1() bad value" );
    try {
	Tx.Cntl.put_Flip_1(         2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "84a", "Cntl.get_Busy_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Busy_1(         1 );
	CHECKX(            0x00000080, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Busy_1() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "84c", "Cntl.get_Busy_1() bad value" );
    try {
	Tx.Cntl.put_Busy_1(         2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "85a", "Cntl.get_Kill_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Kill_1(         1 );
	CHECKX(            0x00000020, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Kill_1() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "85c", "Cntl.get_Kill_1() bad value" );
    try {
	Tx.Cntl.put_Kill_1(         2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "86a", "Cntl.get_Enable_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Enable_1(       1 );
	CHECKX(            0x00000010, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Enable_1() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "86c", "Cntl.get_Enable_1() bad value" );
    try {
	Tx.Cntl.put_Enable_1(       2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "87a", "Cntl.get_Source_4()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Source_4(     0xf );
	CHECKX(            0x0000000f, Tx.Cntl.get()     );
	CHECKX(                   0xf, Tx.Cntl.get_Source_4() );
	Tx.Cntl.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "87c", "Cntl.get_Source_4() bad value" );
    try {
	Tx.Cntl.put_Source_4(    0x10 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xf:  0x10",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "89", "check registers" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Divr.read()     );
	//
	CHECKX(            0x00000000, Tx.Cntl.get()      );
	CHECKX(            0x00000000, Tx.Divr.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Divr Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

  CASE( "90", "clear registers" );	// to avoid any wayward 1s
    try {
	Tx.Cntl.write(     0x00000000 );
	Tx.Divr.write(     0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Divr.read()     );
	Tx.Cntl.put(       0x00000000 );
	Tx.Divr.put(       0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.get()      );
	CHECKX(            0x00000000, Tx.Divr.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "91a", "Divr.get_Passwd_8()" );
    try {
	CHECKX(            0x00000000, Tx.Divr.get()     );
	Tx.Divr.put_Passwd_8(    0xff );
	CHECKX(            0xff000000, Tx.Divr.get()     );
	CHECKX(                  0xff, Tx.Divr.get_Passwd_8() );
	Tx.Divr.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "91c", "Divr.get_Passwd_8() bad value" );
    try {
	Tx.Divr.put_Passwd_8(   0x100 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xff:  0x100",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "92a", "Divr.get_DivI_12()" );
    try {
	CHECKX(            0x00000000, Tx.Divr.get()     );
	Tx.Divr.put_DivI_12(    0xfff );
	CHECKX(            0x00fff000, Tx.Divr.get()     );
	CHECKX(                 0xfff, Tx.Divr.get_DivI_12() );
	Tx.Divr.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92c", "Divr.get_DivI_12() bad value" );
    try {
	Tx.Divr.put_DivI_12(   0x1000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xfff:  0x1000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "93a", "Divr.get_DivF_12()" );
    try {
	CHECKX(            0x00000000, Tx.Divr.get()     );
	Tx.Divr.put_DivF_12(    0xfff );
	CHECKX(            0x00000fff, Tx.Divr.get()     );
	CHECKX(                 0xfff, Tx.Divr.get_DivF_12() );
	Tx.Divr.put(       0x00000000 );        // restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "93c", "Divr.get_DivF_12() bad value" );
    try {
	Tx.Divr.put_DivF_12(   0x1000 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0xfff:  0x1000",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "99", "Done" );
}

