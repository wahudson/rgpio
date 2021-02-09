// 2020-09-05  William A. Hudson
//
// Testing:  rgPullPin IO Pin Pull Up/Down for RPi4
//    10-19  Constructor, get_bcm_address(), rgReg_* constructors
//    20-29  Address of registers  addr()
//    30-39  Register read(), write()
//    40-49  Register set(), clr(), modify()
//    50-59  pullreg_bit()  Register field position
//    60-69  Read/Modify bit
//    70-78  .
//    80-88  rgPull_enum - pull_enum2cstr()
//    90-99  rgPull_enum - int2pull_enum()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgPullPin.h"

using namespace std;

//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_fake_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgPullPin		Tx   ( &Bx );		// test object

//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "rgPullPin constructor" );
    try {
	rgPullPin	tx  ( &Bx );
	PASS( "constructor" );
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
// Stand-alone Register constructor.

  CASE( "16", "rgPullPin_reg constructor init" );
    try {
	rgPullPin::rgPullPin_reg	tx;
	CHECK( 1, tx.addr() == NULL );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "17", "rgPullPin_reg copy constructor" );
    try {
	rgPullPin::rgPullPin_reg	rx  ( Tx.PullSel0 );
	CHECKX( 0xe4, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "20", "PullSel[].addr() compare with PullSel0.addr()" );
    try {
	CHECK( 1, (Tx.PullSel[0]->addr() == Tx.PullSel0.addr()) );
	CHECK( 1, (Tx.PullSel[1]->addr() == Tx.PullSel1.addr()) );
	CHECK( 1, (Tx.PullSel[2]->addr() == Tx.PullSel2.addr()) );
	CHECK( 1, (Tx.PullSel[3]->addr() == Tx.PullSel3.addr()) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "PullSel*.addr()" );
    try {
	CHECKX( 0xe4, (Tx.PullSel0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xe8, (Tx.PullSel1.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xec, (Tx.PullSel2.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xf0, (Tx.PullSel3.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "PullSel[].addr()" );
    try {
	CHECKX( 0xe4, (Tx.PullSel[0]->addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xe8, (Tx.PullSel[1]->addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xec, (Tx.PullSel[2]->addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xf0, (Tx.PullSel[3]->addr() - Tx.get_base_addr())*4 );
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
	Tx.PullSel0.write(     0xffffffff );
	Tx.PullSel1.write(     0xffffffff );
	Tx.PullSel2.write(     0xffffffff );
	Tx.PullSel3.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	CHECKX(                0xffffffff, Tx.PullSel1.read() );
	CHECKX(                0xffffffff, Tx.PullSel2.read() );
	CHECKX(                0xffffffff, Tx.PullSel3.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "PullSel0.read()" );
    try {
	Tx.PullSel0.write(     0x11111111 );
	CHECKX(                0x11111111, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "PullSel1.read()" );
    try {
	Tx.PullSel1.write(     0x22222222 );
	CHECKX(                0x22222222, Tx.PullSel1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "PullSel2.read()" );
    try {
	Tx.PullSel2.write(     0x33333333 );
	CHECKX(                0x33333333, Tx.PullSel2.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "PullSel3.read()" );
    try {
	Tx.PullSel3.write(     0x44444444 );
	CHECKX(                0x44444444, Tx.PullSel3.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register set(), clr(), modify()
//--------------------------------------------------------------------------

  CASE( "41a", "PullSel0.set()" );
    try {
	Tx.PullSel0.write(     0xffff0000 );
	Tx.PullSel0.set(       0x00330033 );
	CHECKX(                0xffff0033, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "42a", "PullSel0.clr()" );
    try {
	Tx.PullSel0.write(     0xffff0000 );
	Tx.PullSel0.clr(       0x00330033 );
	CHECKX(                0xffcc0000, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "PullSel0.modify()" );
    try {
	Tx.PullSel0.write(     0x00ff00ff );
	Tx.PullSel0.modify(    0x0000ffff, 0x33333333 );
	CHECKX(                0x00ff3333, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## pullreg_bit()  Register field position
//--------------------------------------------------------------------------

  CASE( "50", "pullreg_bit() one sample" );
    try {
	int                     loc;
	rgReg_rw                *rp;
	rp = Tx.pullreg_bit( 11, &loc );
	CHECK(    22, loc );
	CHECK(     1, &Tx.PullSel0 == rp );
	CHECKX( 0xe4, (rp->addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "51a", "pullreg_bit() bit < 0" );
    try {
	int                     loc;
	Tx.pullreg_bit( -1, &loc );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::pullreg_bit():  out-of-range bit= -1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51b", "pullreg_bit() bit > 57" );
    try {
	int                     loc;
	Tx.pullreg_bit( 58, &loc );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::pullreg_bit():  out-of-range bit= 58",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52", "pullreg_bit() all bits" );
    try {
	int                     loc;
	rgReg_rw                *rp;

	for ( int k=0;  k<=15;  k++ ) {
	    rp = Tx.pullreg_bit( k, &loc );
	    CHECK( (k-0)*2, loc );
	    CHECK( 1, &Tx.PullSel0 == rp );
	}
	for ( int k=16;  k<=31;  k++ ) {
	    rp = Tx.pullreg_bit( k, &loc );
	    CHECK( (k-16)*2, loc );
	    CHECK( 1, &Tx.PullSel1 == rp );
	}
	for ( int k=32;  k<=47;  k++ ) {
	    rp = Tx.pullreg_bit( k, &loc );
	    CHECK( (k-32)*2, loc );
	    CHECK( 1, &Tx.PullSel2 == rp );
	}
	for ( int k=48;  k<=57;  k++ ) {
	    rp = Tx.pullreg_bit( k, &loc );
	    CHECK( (k-48)*2, loc );
	    CHECK( 1, &Tx.PullSel3 == rp );
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Read/Modify bit read_Pull_bit(), modify_Pull_bit()
//--------------------------------------------------------------------------

  CASE( "60", "condition write(), read()" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	Tx.PullSel1.write(     0xffffffff );
	Tx.PullSel2.write(     0xffffffff );
	Tx.PullSel3.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	CHECKX(                0xffffffff, Tx.PullSel1.read() );
	CHECKX(                0xffffffff, Tx.PullSel2.read() );
	CHECKX(                0xffffffff, Tx.PullSel3.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61", "read_Pull_bit() verify return type" );
    try {
	rgPullPin::rgPull_enum		y;
	y = Tx.read_Pull_bit( 4 );
	CHECK( rgPullPin::pd_Unknown, y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62", "read_Pull_bit() bit > 57" );
    try {
	Tx.read_Pull_bit( 58 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::read_Pull_bit():  bit out-of-range:  58",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63", "read_Pull_bit()" );
    try {
	Tx.PullSel0.write(     0xbff3ff7f );
	CHECKX(                0xbff3ff7f, Tx.PullSel0.read() );
	CHECK( rgPullPin::pd_Unknown,      Tx.read_Pull_bit(  0 ) );
	CHECK( rgPullPin::pd_Up,           Tx.read_Pull_bit(  3 ) );
	CHECK( rgPullPin::pd_Off,          Tx.read_Pull_bit(  9 ) );
	CHECK( rgPullPin::pd_Down,         Tx.read_Pull_bit( 15 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "64a", "read_Pull_w0()" );
    try {
	Tx.PullSel0.write( 0xffaa5500 );
	Tx.PullSel1.write( 0xaaff0055 );
	CHECKX(            0xffaa5500, Tx.PullSel0.read() );
	CHECKX(            0xaaff0055, Tx.PullSel1.read() );
	CHECKX(            0x000f00f0, Tx.read_Pull_w0( rgPullPin::pd_Up ));
	CHECKX(            0xf0000f00, Tx.read_Pull_w0( rgPullPin::pd_Down ));
	CHECKX(            0x00f0000f, Tx.read_Pull_w0( rgPullPin::pd_Off ));
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65", "modify_Pull_bit() pd_Unknown error" );
    try {
	Tx.modify_Pull_bit( 58, rgPullPin::pd_Unknown );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::modify_Pull_bit() invalid direction:  pd_Unknown",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66", "modify_Pull_bit() bit > 57" );
    try {
	Tx.modify_Pull_bit( 58, rgPullPin::pd_Up );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::modify_Pull_bit():  bit out-of-range:  58",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67", "modify_Pull_bit()" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	Tx.modify_Pull_bit(             5, rgPullPin::pd_Up );
	CHECKX(                0xfffff7ff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "68a", "modify_Pull_w0()" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	Tx.PullSel1.write(     0x0000ffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	CHECKX(                0x0000ffff, Tx.PullSel1.read() );
	Tx.modify_Pull_w0(     0x330fcc33, rgPullPin::pd_Up );
	CHECKX(                0x5f5ff5f5, Tx.PullSel0.read() );
	CHECKX(                0x0505ff55, Tx.PullSel1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "69", "modify_Pull_w0()" );
    try {
	Tx.PullSel0.write(     0xffff0000 );
	Tx.PullSel1.write(     0x0000ffff );
	CHECKX(                0xffff0000, Tx.PullSel0.read() );
	CHECKX(                0x0000ffff, Tx.PullSel1.read() );
	Tx.modify_Pull_w0(     0x330fcc33, rgPullPin::pd_Unknown );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::modify_Pull_w0() invalid direction:  pd_Unknown",
	    e.what()
	);
	CHECKX(                0xffff0000, Tx.PullSel0.read() );
	CHECKX(                0x0000ffff, Tx.PullSel1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgPullPin_reg class - modify_field()
//--------------------------------------------------------------------------

  CASE( "71", "modify_field()" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_field(       5, rgPullPin::pd_Up );
	CHECKX(                0xfffff7ff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72", "modify_field() numbers" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_field(       0, rgPullPin::pd_Down );
	Tx.PullSel0.modify_field(       5, rgPullPin::pd_Off  );
	Tx.PullSel0.modify_field(       9, rgPullPin::pd_Down );
	Tx.PullSel0.modify_field(      15, rgPullPin::pd_Up   );
	CHECKX(                0x7ffbf3fe, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73", "modify_field() pd_Unknown allowed" );
    try {
	Tx.PullSel0.write(     0x00000000 );
	CHECKX(                0x00000000, Tx.PullSel0.read() );
	Tx.PullSel0.modify_field(       5, rgPullPin::pd_Unknown );
	CHECKX(                0x00000c00, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75", "modify_field() field out-of-range" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_field(      16, rgPullPin::pd_Up );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin_reg::modify_field():  field out-of-range:  16",
	    e.what()
	);
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "76a", "modify_mask()" );
    try {
	Tx.PullSel0.write(       0xffffffff );
	CHECKX(                  0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_mask( 0x0000fc3f, rgPullPin::pd_Up   );
	CHECKX(                  0x555ff555, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "modify_mask()" );
    try {
	Tx.PullSel0.write(       0xffffffff );
	CHECKX(                  0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_mask( 0x0000fc3f, rgPullPin::pd_Down );
	CHECKX(                  0xaaaffaaa, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76c", "modify_mask()" );
    try {
	Tx.PullSel0.write(       0xffffffff );
	CHECKX(                  0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_mask( 0x0000fc3f, rgPullPin::pd_Off  );
	CHECKX(                  0x000ff000, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76d", "modify_mask() pd_Unknown" );
    try {
	Tx.PullSel0.write(       0x000f0000 );
	CHECKX(                  0x000f0000, Tx.PullSel0.read() );
	Tx.PullSel0.modify_mask( 0x0000fc3f, rgPullPin::pd_Unknown );
	CHECKX(                  0xffff0fff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "77", "modify_mask() no bits" );
    try {
	Tx.PullSel0.write(       0xffffffff );
	CHECKX(                  0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.modify_mask( 0xffff0000, rgPullPin::pd_Up   );
	CHECKX(                  0xffffffff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgPullPin_reg class - read_field()
//--------------------------------------------------------------------------

  CASE( "80", "read_field() verify return type" );
    try {
	rgPullPin::rgPull_enum		y;
	Tx.PullSel0.write(     0xfffffffc );
	CHECKX(                0xfffffffc, Tx.PullSel0.read() );
	y = Tx.PullSel0.read_field(  0 );
	CHECK( rgPullPin::pd_Off,    y );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "81", "read_field()" );
    try {
	Tx.PullSel0.write(     0xfffffff3 );
	CHECKX(                0xfffffff3, Tx.PullSel0.read() );
        CHECK(  rgPullPin::pd_Off,         Tx.PullSel0.read_field(  1 ) );
	CHECKX(                0xfffffff3, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "read_field()" );
    try {
	Tx.PullSel0.write(     0x00000100 );
	CHECKX(                0x00000100, Tx.PullSel0.read() );
        CHECK(  rgPullPin::pd_Up,          Tx.PullSel0.read_field(  4 ) );
	CHECKX(                0x00000100, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "read_field() field out-of-range" );
    try {
	Tx.PullSel0.write(     0xffffffff );
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
	Tx.PullSel0.read_field(        16 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin_reg::read_field():  field out-of-range:  16",
	    e.what()
	);
	CHECKX(                0xffffffff, Tx.PullSel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "80", "read_mask() verify return type" );
    try {
	uint32_t		y;
	Tx.PullSel0.write(     0x55551111 );
	CHECKX(                0x55551111, Tx.PullSel0.read() );
	y = Tx.PullSel0.read_mask( rgPullPin::pd_Up );
	CHECKX(                0x0000ff55, y );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "81", "read_mask()" );
    try {
	Tx.PullSel0.write( 0xcccc00ff );
	CHECKX(            0xcccc00ff, Tx.PullSel0.read() );
	CHECKX(            0x000055f0, Tx.PullSel0.read_mask( rgPullPin::pd_Off ));
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "82", "read_mask()" );
    try {
	Tx.PullSel0.write( 0xffaa5500 );
	CHECKX(            0xffaa5500, Tx.PullSel0.read() );
	CHECKX(            0x0000000f, Tx.PullSel0.read_mask( rgPullPin::pd_Off     ));
	CHECKX(            0x000000f0, Tx.PullSel0.read_mask( rgPullPin::pd_Up      ));
	CHECKX(            0x00000f00, Tx.PullSel0.read_mask( rgPullPin::pd_Down    ));
	CHECKX(            0x0000f000, Tx.PullSel0.read_mask( rgPullPin::pd_Unknown ));
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgPull_enum - pull_enum2cstr()
//--------------------------------------------------------------------------

  CASE( "80", "rgPull_enum values" );
    try {
	CHECK(  0, rgPullPin::pd_Off     );
	CHECK(  1, rgPullPin::pd_Up      );
	CHECK(  2, rgPullPin::pd_Down    );
	CHECK(  3, rgPullPin::pd_Unknown );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81", "Tx.pull_enum2cstr() verify return type" );
    try {
	const char		*cst;
	cst = Tx.pull_enum2cstr( rgPullPin::pd_Off );
	CHECK( "Off", cst );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "82", "pull_enum2cstr() object call" );
    try {
	CHECK( "Off",     Tx.pull_enum2cstr( rgPullPin::pd_Off     ) );
	CHECK( "Up",      Tx.pull_enum2cstr( rgPullPin::pd_Up      ) );
	CHECK( "Down",    Tx.pull_enum2cstr( rgPullPin::pd_Down    ) );
	CHECK( "Unknown", Tx.pull_enum2cstr( rgPullPin::pd_Unknown ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "83", "pull_enum2cstr() class call" );
    try {
	CHECK( "Off",     rgPullPin::pull_enum2cstr( rgPullPin::pd_Off     ) );
	CHECK( "Up",      rgPullPin::pull_enum2cstr( rgPullPin::pd_Up      ) );
	CHECK( "Down",    rgPullPin::pull_enum2cstr( rgPullPin::pd_Down    ) );
	CHECK( "Unknown", rgPullPin::pull_enum2cstr( rgPullPin::pd_Unknown ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "84", "pull_enum2cstr() bad enum" );	// cast to a bad value
    try {
	Tx.pull_enum2cstr( (rgPullPin::rgPull_enum) 99 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgPullPin::pull_enum2cstr() invalid rgPull_enum:  99",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgPull_enum - int2pull_enum()
//--------------------------------------------------------------------------

  CASE( "90", "int2pull_enum() verify return type" );
    try {
        CHECK( "Off", Tx.pull_enum2cstr( Tx.int2pull_enum( 0 ) ) );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "91", "int2pull_enum() object call" );
    try {
        CHECK( rgPullPin::pd_Off,     Tx.int2pull_enum( 0 ) );
        CHECK( rgPullPin::pd_Up,      Tx.int2pull_enum( 1 ) );
        CHECK( rgPullPin::pd_Down,    Tx.int2pull_enum( 2 ) );
        CHECK( rgPullPin::pd_Unknown, Tx.int2pull_enum( 3 ) );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "92", "int2pull_enum() class call" );
    try {
        CHECK( rgPullPin::pd_Off,     rgPullPin::int2pull_enum( 0 ) );
        CHECK( rgPullPin::pd_Up,      rgPullPin::int2pull_enum( 1 ) );
        CHECK( rgPullPin::pd_Down,    rgPullPin::int2pull_enum( 2 ) );
        CHECK( rgPullPin::pd_Unknown, rgPullPin::int2pull_enum( 3 ) );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "93", "Tx.int2pull_enum() verify return type" );
    try {
	rgPullPin::rgPull_enum		y;
	y = Tx.int2pull_enum( 2 );
	CHECK( rgPullPin::pd_Down,   y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "94a", "int2pull_enum() bad int" );
    try {
        Tx.int2pull_enum( -1 );
        FAIL( "no throw" );
    }
    catch ( range_error& e ) {
        CHECK( "rgPullPin::int2pull_enum() int out of range:  -1",
            e.what()
        );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

  CASE( "94b", "int2pull_enum() bad int" );
    try {
        Tx.int2pull_enum( 4 );
        FAIL( "no throw" );
    }
    catch ( range_error& e ) {
        CHECK( "rgPullPin::int2pull_enum() int out of range:  4",
            e.what()
        );
    }
    catch (...) {
        FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

