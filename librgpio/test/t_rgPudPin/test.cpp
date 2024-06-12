// 2020-09-05  William A. Hudson
//
// Testing:  rgPudPin IO Pin Pull Up/Down
//    10-19  Constructor, get_bcm_address(), rgReg_* constructors
//    20-29  Address of registers  addr()
//    30-39  Register read(), write()
//    40-49  Clk Register set(), clr(), modify()
//    50-59  Mode Register get(), put(), grab(), push()
//    60-68  Register Field Accessors  get_(), put_()
//    70-78  Enum Conversions - rgPud_enum
//    80-89  Program Pull Resistors - program_pud_w0()
//    90-99  Program Pull Resistors - program_pud_bit()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgPudPin.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2837 );	// RPi3

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	CHECKX( 0x7e000000, Bx.config_DocBase() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgPudPin		Tx   ( &Bx );		// test object

//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "rgPudPin constructor" );
    try {
	rgPudPin	tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "rgPudPin constructor RPi0" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2835 );
	rgPudPin	tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "rgPudPin domain_error RPi4" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2711 );
	rgPudPin	tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( std::domain_error& e ) {
	CHECK( "rgPudPin:  require RPi3 (soc_BCM2837) or earlier", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "rgPudPin domain_error RPi5" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );
	rgPudPin	tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( std::domain_error& e ) {
	CHECK( "rgPudPin:  require RPi3 (soc_BCM2837) or earlier", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e200000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Stand-alone Register constructor init has NULL address.

  CASE( "12", "rgPudPin_Mode constructor init" );
    try {
	rgPudPin_Mode	tx;
	CHECK( 1, tx.addr() == NULL );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "rgPudPin_Clk constructor init" );
    try {
	rgPudPin_Clk	tx;
	CHECK( 1, tx.addr() == NULL );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor copy initialization, all classes.

  CASE( "16", "rgPudPin_Mode copy constructor" );
    try {
	rgPudPin_Mode	rx  ( Tx.PudProgMode );
	CHECKX( 0x94, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "17a", "rgPudPin_Clk copy constructor" );
    try {
	rgPudPin_Clk	rx  ( Tx.PudProgClk_w0 );
	CHECKX( 0x98, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18b", "rgPudPin_Clk copy constructor" );
    try {
	rgPudPin_Clk	rx  ( Tx.PudProgClk_w1 );
	CHECKX( 0x9c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "PudProgMode.addr()" );
    try {
	CHECKX( 0x94, (Tx.PudProgMode.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "PudProgClk_w0.addr()" );
    try {
	CHECKX( 0x98, (Tx.PudProgClk_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x9c, (Tx.PudProgClk_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition write(), read()" );
    try {
	Tx.PudProgMode.write(     0xffffffff );
	Tx.PudProgClk_w0.write(   0xffffffff );
	Tx.PudProgClk_w1.write(   0xffffffff );
	CHECKX(                   0xffffffff, Tx.PudProgMode.read()     );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "PudProgMode.read()" );
    try {
	Tx.PudProgMode.write(     0x11111111 );
	CHECKX(                   0x11111111, Tx.PudProgMode.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "PudProgClk_w0.read()" );
    try {
	Tx.PudProgClk_w0.write(   0x22222222 );
	CHECKX(                   0x22222222, Tx.PudProgClk_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "PudProgClk_w1.read()" );
    try {
	Tx.PudProgClk_w1.write(   0x33333333 );
	CHECKX(                   0x33333333, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Clk Register set(), clr(), modify()
//--------------------------------------------------------------------------

  CASE( "41a", "rgPudPin_Clk  PudProgClk_w0.set()" );
    try {
	Tx.PudProgClk_w0.write(   0xffff0000 );
	Tx.PudProgClk_w0.set(     0x00330033 );
	CHECKX(                   0xffff0033, Tx.PudProgClk_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "rgPudPin_Clk  PudProgClk_w1.set()" );
    try {
	Tx.PudProgClk_w1.write(   0xffff0000 );
	Tx.PudProgClk_w1.set(     0x00330033 );
	CHECKX(                   0xffff0033, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "rgPudPin_Clk  PudProgClk_w0.clr()" );
    try {
	Tx.PudProgClk_w0.write(   0xffff0000 );
	Tx.PudProgClk_w0.clr(     0x00330033 );
	CHECKX(                   0xffcc0000, Tx.PudProgClk_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42b", "rgPudPin_Clk  PudProgClk_w1.clr()" );
    try {
	Tx.PudProgClk_w1.write(   0xffff0000 );
	Tx.PudProgClk_w1.clr(     0x00330033 );
	CHECKX(                   0xffcc0000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "rgPudPin_Clk  PudProgClk_w0.modify()" );
    try {
	Tx.PudProgClk_w0.write(   0x00ff00ff );
	Tx.PudProgClk_w0.modify(  0x0000ffff, 0x33333333 );
	CHECKX(                   0x00ff3333, Tx.PudProgClk_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43b", "rgPudPin_Clk  PudProgClk_w1.modify()" );
    try {
	Tx.PudProgClk_w1.write(   0x00ff00ff );
	Tx.PudProgClk_w1.modify(  0x0000ffff, 0x33333333 );
	CHECKX(                   0x00ff3333, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Mode Register get(), put(), grab(), push()
//--------------------------------------------------------------------------

  CASE( "50", "condition registers" );
    try {
	Tx.PudProgMode.write(     0x00000000 );
	Tx.PudProgClk_w0.write(   0x00000000 );
	Tx.PudProgClk_w1.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgMode.read()     );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "51a", "PudProgMode.grab(), get()" );
    try {
	Tx.PudProgMode.write(     0xccc33ccc );
	Tx.PudProgMode.put(       0x77444477 );
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.read()     );
	CHECKX(                   0x77444477, Tx.PudProgMode.get()      );
	Tx.PudProgMode.grab();
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.read()     );
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51b", "PudProgMode.put(), push()" );
    try {
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.read()     );
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.get()      );
	Tx.PudProgMode.put(       0xbeef0303 );
	CHECKX(                   0xccc33ccc, Tx.PudProgMode.read()     );
	CHECKX(                   0xbeef0303, Tx.PudProgMode.get()      );
	Tx.PudProgMode.push();
	CHECKX(                   0xbeef0303, Tx.PudProgMode.read()     );
	CHECKX(                   0xbeef0303, Tx.PudProgMode.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

//--------------------------------------
// Example heavy-weight bit-field test.

  CASE( "61a", "get_UpDown_2()" );
    try {
	Tx.PudProgMode.put(   0x00000000 );
	CHECKX(               0x00000000, Tx.PudProgMode.get() );
	Tx.PudProgMode.put_UpDown_2(   3 );
	CHECKX(               0x00000003, Tx.PudProgMode.get() );
	CHECK(                         3, Tx.PudProgMode.get_UpDown_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "get_UpDown_2()" );
    try {
	Tx.PudProgMode.put(   0xffffffff );
	CHECKX(               0xffffffff, Tx.PudProgMode.get() );
	Tx.PudProgMode.put_UpDown_2(   0 );
	CHECKX(               0xfffffffc, Tx.PudProgMode.get() );
	CHECK(                         0, Tx.PudProgMode.get_UpDown_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "put_UpDown_2() bad value" );
    try {
	Tx.PudProgMode.put_UpDown_2(   4 );
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

//--------------------------------------------------------------------------
//## Enum Conversions - rgPud_enum
//--------------------------------------------------------------------------

  CASE( "71", "rgPudPin::int2pud_enum() class call" );
    try {
	rgPudPin::rgPud_enum		y;
	y = rgPudPin::int2pud_enum( 0 );
	CHECK( rgPudPin::pd_Off, y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "72a", "int2pud_enum( 0 ) object call" );
    try {
	rgPudPin::rgPud_enum		y;
	y = Tx.int2pud_enum( 0 );
	CHECK( rgPudPin::pd_Off, y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "int2pud_enum( 1 )" );
    try {
	CHECK( rgPudPin::pd_Down, Tx.int2pud_enum( 1 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72c", "int2pud_enum( 2 )" );
    try {
	CHECK( rgPudPin::pd_Up, Tx.int2pud_enum( 2 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "73a", "int2pud_enum( 3 ) out of range" );
    try {
	Tx.int2pud_enum( 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPudPin::int2pud_enum() int out of range:  3",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "int2pud_enum( -1 ) out of range" );
    try {
	Tx.int2pud_enum( -1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPudPin::int2pud_enum() int out of range:  -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "75", "rgPudPin::pud_enum2cstr() class call" );
    try {
	const char		*cst;
	cst = rgPudPin::pud_enum2cstr( rgPudPin::pd_Off );
	CHECK( "Off", cst );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "76a", "Tx.pud_enum2cstr() object call" );
    try {
	const char		*cst;
	cst = Tx.pud_enum2cstr( rgPudPin::pd_Off );
	CHECK( "Off", cst );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "pud_enum2cstr()" );
    try {
	CHECK( "Down", Tx.pud_enum2cstr( rgPudPin::pd_Down ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76c", "pud_enum2cstr()" );
    try {
	CHECK( "Up",   Tx.pud_enum2cstr( rgPudPin::pd_Up   ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Program Pull Resistors - program_pud_w0()
//--------------------------------------------------------------------------
// Test result is not observable, since program_pud() resets registers.
// (Can manual test by hacking the program_pud() function.)

  CASE( "80", "condition write(), read()" );
    try {
	Tx.PudProgMode.write(     0xffffffff );
	Tx.PudProgClk_w0.write(   0xffffffff );
	Tx.PudProgClk_w1.write(   0xffffffff );
	CHECKX(                   0xffffffff, Tx.PudProgMode.read()     );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81", "program_pud_w0()" );
    try {
	bool		rv;
	rv = Tx.program_pud_w0( rgPudPin::pd_Up, 0xbeeb7447 );
	CHECK( 0, rv );
	CHECKX(                   0xfffffffe, Tx.PudProgMode.read()     );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "program_pud_w1()" );
    try {
	Tx.PudProgMode.write(     0xffffffff );
	Tx.PudProgClk_w0.write(   0xffffffff );
	Tx.PudProgClk_w1.write(   0xffffffff );
	CHECKX(                   0xffffffff, Tx.PudProgMode.read()     );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w1.read()   );
	//
	CHECK( 0, Tx.program_pud_w1( rgPudPin::pd_Down, 0xabba3113 ) );
	CHECKX(                   0xfffffffd, Tx.PudProgMode.read()     );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Program Pull Resistors - program_pud_bit()
//--------------------------------------------------------------------------
// Poor testability, results not observable.

  CASE( "90", "program_pud_bit()" );
    try {
	Tx.program_pud_bit( rgPudPin::pd_Up, 54 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPudPin::program_pud_bit() require bit number {53..0}:  54",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "91", "program_pud_bit()" );
    try {
	bool		rv;
	rv = Tx.program_pud_bit( rgPudPin::pd_Up, 53);
	CHECK( 0, rv );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "92", "program_pud_bit() in w0" );
    try {
	Tx.PudProgMode.write(     0xffffffff );
	Tx.PudProgClk_w0.write(   0xffffffff );
	Tx.PudProgClk_w1.write(   0xffffffff );
	CHECKX(                   0xffffffff, Tx.PudProgMode.read()     );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w1.read()   );
	//
	CHECK( 0, Tx.program_pud_bit( rgPudPin::pd_Down,  7 ) );
	CHECKX(                   0xfffffffd, Tx.PudProgMode.read()     );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "93", "program_pud_bit() in w1" );
    try {
	Tx.PudProgMode.write(     0xffffffff );
	Tx.PudProgClk_w0.write(   0xffffffff );
	Tx.PudProgClk_w1.write(   0xffffffff );
	CHECKX(                   0xffffffff, Tx.PudProgMode.read()     );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0xffffffff, Tx.PudProgClk_w1.read()   );
	//
	CHECK( 0, Tx.program_pud_bit( rgPudPin::pd_Down, 32 ) );
	CHECKX(                   0xfffffffd, Tx.PudProgMode.read()     );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read()   );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

