// 2021-05-29  William A. Hudson
//
// Testing:  rgFselPin  Pin Function Select class.
//    10-19  Constructor, get_bcm_address(), rgReg_* constructors, REG.addr()
//    20-29  Register read(), write()
//    30-39  Register set(), clr(), modify()
//    40-49  rgFselPin_reg class - modify_field(), read_field()
//    50-59  rgFselPin_reg class - modify_mask(), read_mask()
//    60-69  Top read_Fsel_bit(), modify_Fsel_bit()
//    70-79  Top read_Fsel_w0(), modify_Fsel_w0(), .. _w1()
//    80-89  rgFsel_enum string conversion
//    90-99  find_rgFsel_enum()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <iomanip>
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
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

rgIoPins		Px  ( &Bx );
rgFselPin		Tx  ( &Bx );


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgFselPin		tx  ( &Bx );
	PASS( "constructor" );
	CHECKX( 0x7e200000, tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "constructor, deprecated" );
    try {
	rgFselPin		tx  ( &Px );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "rgFselPin domain_error RPi5" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );
	rgFselPin       tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( std::domain_error& e ) {
	CHECK( "rgFselPin:  require RPi4 (soc_BCM2711) or earlier", e.what() );
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

  CASE( "14", "rgFselPin_reg constructor init" );
    try {
	rgFselPin::rgFselPin_reg        tx;
	CHECK( 1, tx.addr() == NULL );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "rgFselPin_reg copy constructor" );
    try {
	rgFselPin::rgFselPin_reg        rx  ( Tx.Fsel2 );
	CHECKX( 0x08, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Address of registers  addr()

  CASE( "17", "Fsel*.addr()" );
    try {
	CHECKX( 0x00, (Tx.Fsel0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x04, (Tx.Fsel1.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x08, (Tx.Fsel2.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x0c, (Tx.Fsel3.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x10, (Tx.Fsel4.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x14, (Tx.Fsel5.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "18", "register address compared to rgIoPins" );
    try {
	CHECK( 1, Tx.Fsel0.addr() == Px.Fsel0.addr() );
	CHECK( 1, Tx.Fsel1.addr() == Px.Fsel1.addr() );
	CHECK( 1, Tx.Fsel2.addr() == Px.Fsel2.addr() );
	CHECK( 1, Tx.Fsel3.addr() == Px.Fsel3.addr() );
	CHECK( 1, Tx.Fsel4.addr() == Px.Fsel4.addr() );
	CHECK( 1, Tx.Fsel5.addr() == Px.Fsel5.addr() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Enum value is the Fsel register field value.

  CASE( "19", "rgFsel_enum values" );
    try {
	CHECK( 0, rgFselPin::f_In   );
	CHECK( 1, rgFselPin::f_Out  );
	CHECK( 4, rgFselPin::f_Alt0 );
	CHECK( 5, rgFselPin::f_Alt1 );
	CHECK( 6, rgFselPin::f_Alt2 );
	CHECK( 7, rgFselPin::f_Alt3 );
	CHECK( 3, rgFselPin::f_Alt4 );
	CHECK( 2, rgFselPin::f_Alt5 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "19b", "rgFsel_enum values" );
    try {
	CHECK( 0, Tx.f_In );
	CHECK( 1, Tx.f_Out  );
	CHECK( 4, Tx.f_Alt0 );
	CHECK( 5, Tx.f_Alt1 );
	CHECK( 6, Tx.f_Alt2 );
	CHECK( 7, Tx.f_Alt3 );
	CHECK( 3, Tx.f_Alt4 );
	CHECK( 2, Tx.f_Alt5 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "20", "condition write(), read()" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	Tx.Fsel1.write(     0xffffffff );
	Tx.Fsel2.write(     0xffffffff );
	Tx.Fsel3.write(     0xffffffff );
	Tx.Fsel4.write(     0xffffffff );
	Tx.Fsel5.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	CHECKX(             0xffffffff, Tx.Fsel1.read() );
	CHECKX(             0xffffffff, Tx.Fsel2.read() );
	CHECKX(             0xffffffff, Tx.Fsel3.read() );
	CHECKX(             0xffffffff, Tx.Fsel4.read() );
	CHECKX(             0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21", "Fsel0.read()" );
    try {
	Tx.Fsel0.write(     0x11111111 );
	CHECKX(             0x11111111, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "Fsel1.read()" );
    try {
	Tx.Fsel1.write(     0x22222222 );
	CHECKX(             0x22222222, Tx.Fsel1.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "Fsel2.read()" );
    try {
	Tx.Fsel2.write(     0x33333333 );
	CHECKX(             0x33333333, Tx.Fsel2.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "Fsel3.read()" );
    try {
	Tx.Fsel3.write(     0x44444444 );
	CHECKX(             0x44444444, Tx.Fsel3.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "Fsel4.read()" );
    try {
	Tx.Fsel4.write(     0x55555555 );
	CHECKX(             0x55555555, Tx.Fsel4.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26", "Fsel5.read()" );
    try {
	Tx.Fsel5.write(     0x66666666 );
	CHECKX(             0x66666666, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "29", "check register result" );
    try {
	CHECKX(             0x11111111, Tx.Fsel0.read() );
	CHECKX(             0x22222222, Tx.Fsel1.read() );
	CHECKX(             0x33333333, Tx.Fsel2.read() );
	CHECKX(             0x44444444, Tx.Fsel3.read() );
	CHECKX(             0x55555555, Tx.Fsel4.read() );
	CHECKX(             0x66666666, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register set(), clr(), modify()
//--------------------------------------------------------------------------
// Gray box - sample one register.

  CASE( "31a", "Fsel0.set()" );
    try {
	Tx.Fsel0.write(     0xffff0000 );
	Tx.Fsel0.set(       0x00330033 );
	CHECKX(             0xffff0033, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "32a", "Fsel0.clr()" );
    try {
	Tx.Fsel0.write(     0xffff0000 );
	Tx.Fsel0.clr(       0x00330033 );
	CHECKX(             0xffcc0000, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "33a", "Fsel0.modify()" );
    try {
	Tx.Fsel0.write(     0x00ff00ff );
	Tx.Fsel0.modify(    0x0000ffff, 0x33333333 );
	CHECKX(             0x00ff3333, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgFselPin_reg class - modify_field(), read_field()
//--------------------------------------------------------------------------
// Gray box - sample one register.

  CASE( "40", "read_field() verify return type" );
    try {
	rgFselPin::rgFsel_enum		y;
	Tx.Fsel0.write(     0xfffffff8 );
	CHECKX(             0xfffffff8, Tx.Fsel0.read() );
	y = Tx.Fsel0.read_field(  0 );
	CHECK( rgFselPin::f_In,   y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41", "read_field( 10 ) field out-of-range" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.read_field(        10 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin_reg::read_field():  field out-of-range:  10",
	    e.what()
	);
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "modify_field( 10 ) field out-of-range" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(      10, rgFselPin::f_Out );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin_reg::modify_field():  field out-of-range:  10",
	    e.what()
	);
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "43a", "modify_field( 9 )" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       9, rgFselPin::f_Alt5 );
	CHECKX(             0xd7ffffff, Tx.Fsel0.read() );
	CHECK(                       2,    Tx.Fsel0.read_field( 9 ) );
	CHECK( "Alt5", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 9 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43b", "modify_field( 0 )" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       0, rgFselPin::f_Alt5 );
	CHECKX(             0xfffffffa, Tx.Fsel0.read() );
	CHECK(                       2,    Tx.Fsel0.read_field( 0 ) );
	CHECK( "Alt5", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 0 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "44a", "modify_field() values" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       3, rgFselPin::f_In );
	Tx.Fsel0.modify_field(       8, rgFselPin::f_Out );
	CHECKX(             0xf9fff1ff, Tx.Fsel0.read() );
	CHECK(                       0,    Tx.Fsel0.read_field( 3 ) );
	CHECK(                       1,    Tx.Fsel0.read_field( 8 ) );
	CHECK( "In",   Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 3 ) ) );
	CHECK( "Out",  Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 8 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44b", "modify_field() values" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       3, rgFselPin::f_Alt0 );
	Tx.Fsel0.modify_field(       8, rgFselPin::f_Alt1 );
	CHECKX(             0xfdfff9ff, Tx.Fsel0.read() );
	CHECK(                       4,    Tx.Fsel0.read_field( 3 ) );
	CHECK(                       5,    Tx.Fsel0.read_field( 8 ) );
	CHECK( "Alt0", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 3 ) ) );
	CHECK( "Alt1", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 8 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44c", "modify_field() values" );
    try {
	Tx.Fsel0.write(     0x00000000 );
	CHECKX(             0x00000000, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       3, rgFselPin::f_Alt2 );
	Tx.Fsel0.modify_field(       8, rgFselPin::f_Alt3 );
	CHECKX(             0x07000c00, Tx.Fsel0.read() );
	CHECK(                       6,    Tx.Fsel0.read_field( 3 ) );
	CHECK(                       7,    Tx.Fsel0.read_field( 8 ) );
	CHECK( "Alt2", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 3 ) ) );
	CHECK( "Alt3", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 8 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "44d", "modify_field() values" );
    try {
	Tx.Fsel0.write(     0xffff0000 );
	CHECKX(             0xffff0000, Tx.Fsel0.read() );
	Tx.Fsel0.modify_field(       3, rgFselPin::f_Alt4 );
	Tx.Fsel0.modify_field(       8, rgFselPin::f_Alt5 );
	CHECKX(             0xfaff0600, Tx.Fsel0.read() );
	CHECK(                       3,    Tx.Fsel0.read_field( 3 ) );
	CHECK(                       2,    Tx.Fsel0.read_field( 8 ) );
	CHECK( "Alt4", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 3 ) ) );
	CHECK( "Alt5", Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 8 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "46", "modify_field() loop all" );
    try {
	uint32_t	mask = 0x3;	// f_Alt4
	for ( int k=0;  k<=9;  k++ ) {
	    Tx.Fsel0.write(   0x00000000 );
	    Tx.Fsel0.modify_field(     k, Tx.f_Alt4 );
	    CHECK(                     3, Tx.Fsel0.read_field( k ) );
	    CHECKX(                 mask, Tx.Fsel0.read() );
	    cout.fill('0');
	    cout << "k= "       <<dec           << k
		<< "  mask= 0x" <<hex <<setw(8) << mask
		<< "  read= 0o" <<oct <<setw(10)<< Tx.Fsel0.read() <<dec <<endl;
	    mask = mask << 3;
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgFselPin_reg class - modify_mask(), read_mask()
//--------------------------------------------------------------------------
// Gray box - sample one register.

  CASE( "50a", "read_mask() all 1" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt2 ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "read_mask() all 0" );
    try {
	Tx.Fsel0.write(       0x00000000 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt2 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50c", "read_mask() mix" );
    try {
	Tx.Fsel0.write(     005701456732 );
	CHECKX(             005701456732, Tx.Fsel0.read() );
	CHECKX(               0x00000080, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000040, Tx.Fsel0.read_mask( Tx.f_Out  ) );
	CHECKX(               0x00000020, Tx.Fsel0.read_mask( Tx.f_Alt0 ) );
	CHECKX(               0x00000210, Tx.Fsel0.read_mask( Tx.f_Alt1 ) );
	CHECKX(               0x00000008, Tx.Fsel0.read_mask( Tx.f_Alt2 ) );
	CHECKX(               0x00000104, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	CHECKX(               0x00000002, Tx.Fsel0.read_mask( Tx.f_Alt4 ) );
	CHECKX(               0x00000001, Tx.Fsel0.read_mask( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "51", "modify_mask() oversize mask" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	Tx.Fsel0.modify_mask( 0xffffffff, rgFselPin::f_In );
	CHECKX(               0xc0000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "modify_mask() all 10 mask bits" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	Tx.Fsel0.modify_mask( 0x000003ff, rgFselPin::f_In );
	CHECKX(               0xc0000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52b", "modify_mask() no mask bits" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	Tx.Fsel0.modify_mask( 0xfffffc00, rgFselPin::f_In );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "modify_mask() all 10 mask bits" );
    try {
	Tx.Fsel0.write(       0x00000000 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	Tx.Fsel0.modify_mask( 0x000003ff, rgFselPin::f_Alt3 );
	CHECKX(               0x3fffffff, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53b", "modify_mask() no mask bits" );
    try {
	Tx.Fsel0.write(       0x00000000 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	Tx.Fsel0.modify_mask( 0xfffffc00, rgFselPin::f_Alt3 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	CHECKX(               0x000003ff, Tx.Fsel0.read_mask( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "54", "modify_mask() bit 3" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	Tx.Fsel0.modify_mask( 0x00000008, rgFselPin::f_Alt5 );
	CHECKX(               0xfffff5ff, Tx.Fsel0.read() );
	CHECKX(               0x00000008, Tx.Fsel0.read_mask( Tx.f_Alt5 ) );
	CHECK(                         2, Tx.Fsel0.read_field( 3 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "55", "modify_mask() bit 8" );
    try {
	Tx.Fsel0.write(       0x00000000 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	Tx.Fsel0.modify_mask( 0x00000100, rgFselPin::f_Alt3 );
	CHECKX(               0x07000000, Tx.Fsel0.read() );
	CHECKX(               0x00000100, Tx.Fsel0.read_mask( Tx.f_Alt3 ) );
	CHECK(                         7, Tx.Fsel0.read_field( 8 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "56", "modify_mask() loop all" );
    try {
	uint32_t	mask = 0x1;
	for ( int k=0;  k<=9;  k++ ) {
	    Tx.Fsel0.write(   0xffffffff );
	    Tx.Fsel0.modify_mask(   mask, Tx.f_Alt0 );
	    CHECKX(                 mask, Tx.Fsel0.read_mask( Tx.f_Alt0 ) );
	    CHECK(                     4, Tx.Fsel0.read_field( k ) );
	    cout.fill('0');
	    cout << "k= "       <<dec           << k
		<< "  mask= 0x" <<hex <<setw(8) << mask
		<< "  read= 0o" <<oct <<setw(10)<< Tx.Fsel0.read() <<dec <<endl;
	    mask = mask << 1;
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Top read_Fsel_bit(), modify_Fsel_bit()
//--------------------------------------------------------------------------

  CASE( "60", "condition write(), read()" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0xffffffff, Tx.Fsel1.read() );
	CHECKX(               0xffffffff, Tx.Fsel2.read() );
	CHECKX(               0xffffffff, Tx.Fsel3.read() );
	CHECKX(               0xffffffff, Tx.Fsel4.read() );
	CHECKX(               0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61", "read_Fsel_bit() verify return type" );
    try {
	rgFselPin::rgFsel_enum		y;
	Tx.Fsel0.write(     0xfffffff8 );
	CHECKX(             0xfffffff8, Tx.Fsel0.read() );
	y = Tx.Fsel0.read_field(  0 );
	CHECK( rgFselPin::f_In,   y );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "62a", "read_Fsel_bit() bit out-of-range RPi4" );
    try {
	Tx.read_Fsel_bit( 58 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::read_Fsel_bit():  out-of-range bit= 58",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62b", "modify_Fsel_bit() bit out-of-range RPi4" );
    try {
	Tx.modify_Fsel_bit( 58, rgFselPin::f_Out );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::modify_Fsel_bit():  out-of-range bit= 58",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Exercise bits at both ends of each register.
  CASE( "63a", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel0.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel0.read() );
	Tx.modify_Fsel_bit(          0, rgFselPin::f_In );
	Tx.modify_Fsel_bit(          9, rgFselPin::f_Out );
	CHECKX(           031777777770, Tx.Fsel0.read() );
	CHECK(                       0,    Tx.read_Fsel_bit(  0 ) );
	CHECK(                       1,    Tx.read_Fsel_bit(  9 ) );
	CHECK(                       0,    Tx.Fsel0.read_field( 0 ) );
	CHECK(                       1,    Tx.Fsel0.read_field( 9 ) );
	CHECK( "In",   Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 0 ) ) );
	CHECK( "Out",  Tx.rgFsel_enum2cstr( Tx.Fsel0.read_field( 9 ) ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel1.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel1.read() );
	Tx.modify_Fsel_bit(         10, rgFselPin::f_Out );
	Tx.modify_Fsel_bit(         19, rgFselPin::f_In );
	CHECKX(           030777777771, Tx.Fsel1.read() );
	CHECK(                       1, Tx.read_Fsel_bit( 10 ) );
	CHECK(                       0, Tx.read_Fsel_bit( 19 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63c", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel2.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel2.read() );
	Tx.modify_Fsel_bit(         20, rgFselPin::f_Alt0 );
	Tx.modify_Fsel_bit(         29, rgFselPin::f_Alt1 );
	CHECKX(           035777777774, Tx.Fsel2.read() );
	CHECK(                       4, Tx.read_Fsel_bit( 20 ) );
	CHECK(                       5, Tx.read_Fsel_bit( 29 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63d", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel3.write(     0xffffffff );
	CHECKX(             0xffffffff, Tx.Fsel3.read() );
	Tx.modify_Fsel_bit(         30, rgFselPin::f_Alt4 );
	Tx.modify_Fsel_bit(         39, rgFselPin::f_Alt5 );
	CHECKX(           032777777773, Tx.Fsel3.read() );
	CHECK(                       3, Tx.read_Fsel_bit( 30 ) );
	CHECK(                       2, Tx.read_Fsel_bit( 39 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63e", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel4.write(     0x00000000 );
	CHECKX(             0x00000000, Tx.Fsel4.read() );
	Tx.modify_Fsel_bit(         40, rgFselPin::f_Alt2 );
	Tx.modify_Fsel_bit(         49, rgFselPin::f_Alt3 );
	CHECKX(           007000000006, Tx.Fsel4.read() );
	CHECK(                       6, Tx.read_Fsel_bit( 40 ) );
	CHECK(                       7, Tx.read_Fsel_bit( 49 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63f", "modify_Fsel_bit() values" );
    try {
	Tx.Fsel5.write(     0x00000000 );
	CHECKX(             0x00000000, Tx.Fsel5.read() );
	Tx.modify_Fsel_bit(         50, rgFselPin::f_Alt3 );
	Tx.modify_Fsel_bit(         57, rgFselPin::f_Alt2 );	// max bit
	CHECKX(           000060000007, Tx.Fsel5.read() );
	CHECK(                       7, Tx.read_Fsel_bit( 50 ) );
	CHECK(                       6, Tx.read_Fsel_bit( 57 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63x", "check final registers" );
    try {
	CHECKX(           031777777770, Tx.Fsel0.read() );
	CHECKX(           030777777771, Tx.Fsel1.read() );
	CHECKX(           035777777774, Tx.Fsel2.read() );
	CHECKX(           032777777773, Tx.Fsel3.read() );
	CHECKX(           007000000006, Tx.Fsel4.read() );
	CHECKX(           000060000007, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "modify_Fsel_bit() loop all" );
    try {
	for ( int k=0;  k<=57;  k++ ) {
	    Tx.Fsel0.write(   0xffffffff );
	    Tx.Fsel1.write(   0xffffffff );
	    Tx.Fsel2.write(   0xffffffff );
	    Tx.Fsel3.write(   0xffffffff );
	    Tx.Fsel4.write(   0xffffffff );
	    Tx.Fsel5.write(   0xffffffff );
	    Tx.modify_Fsel_bit(        k, Tx.f_Alt5 );
	    CHECK(                     2, Tx.read_Fsel_bit( k ) );
//	    CHECKX(           0xffffffff, Tx.Fsel0.read() );
	    cout << "k= "       <<dec << k <<endl;
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Top read_Fsel_w0(), modify_Fsel_w0(), .. _w1()
//--------------------------------------------------------------------------

  CASE( "70", "setup all 1" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0xffffffff, Tx.Fsel1.read() );
	CHECKX(               0xffffffff, Tx.Fsel2.read() );
	CHECKX(               0xffffffff, Tx.Fsel3.read() );
	CHECKX(               0xffffffff, Tx.Fsel4.read() );
	CHECKX(               0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70a", "read_Fsel_w0()" );
    try {
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt2 ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70b", "read_Fsel_w1()" );
    try {
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt2 ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "71", "setup all 0" );
    try {
	Tx.Fsel0.write(       0x00000000 );
	Tx.Fsel1.write(       0x00000000 );
	Tx.Fsel2.write(       0x00000000 );
	Tx.Fsel3.write(       0x00000000 );
	Tx.Fsel4.write(       0x00000000 );
	Tx.Fsel5.write(       0x00000000 );
	CHECKX(               0x00000000, Tx.Fsel0.read() );
	CHECKX(               0x00000000, Tx.Fsel1.read() );
	CHECKX(               0x00000000, Tx.Fsel2.read() );
	CHECKX(               0x00000000, Tx.Fsel3.read() );
	CHECKX(               0x00000000, Tx.Fsel4.read() );
	CHECKX(               0x00000000, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71a", "read_Fsel_w0()" );
    try {
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt2 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "read_Fsel_w1()" );
    try {
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt2 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt5 ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "72a", "modify_Fsel_w0() all bits" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	Tx.modify_Fsel_w0(    0xffffffff, Tx.f_In   );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0xc0000000, Tx.Fsel0.read() );
	CHECKX(               0xc0000000, Tx.Fsel1.read() );
	CHECKX(               0xc0000000, Tx.Fsel2.read() );
	CHECKX(               0xffffffc0, Tx.Fsel3.read() );
	CHECKX(               0xffffffff, Tx.Fsel4.read() );
	CHECKX(               0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "modify_Fsel_w1() all bits" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	Tx.modify_Fsel_w1(    0xffffffff, Tx.f_In   );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0xffffffff, Tx.Fsel1.read() );
	CHECKX(               0xffffffff, Tx.Fsel2.read() );
	CHECKX(               0xc000003f, Tx.Fsel3.read() );
	CHECKX(               0xc0000000, Tx.Fsel4.read() );
	CHECKX(               0xc0000000, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "73a", "modify_Fsel_w0() No change mask=0" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	Tx.modify_Fsel_w0(    0x00000000, Tx.f_In   );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	//
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0xffffffff, Tx.Fsel1.read() );
	CHECKX(               0xffffffff, Tx.Fsel2.read() );
	CHECKX(               0xffffffff, Tx.Fsel3.read() );
	CHECKX(               0xffffffff, Tx.Fsel4.read() );
	CHECKX(               0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "modify_Fsel_w1() No change mask=0" );
    try {
	Tx.Fsel0.write(       0xffffffff );
	Tx.Fsel1.write(       0xffffffff );
	Tx.Fsel2.write(       0xffffffff );
	Tx.Fsel3.write(       0xffffffff );
	Tx.Fsel4.write(       0xffffffff );
	Tx.Fsel5.write(       0xffffffff );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	Tx.modify_Fsel_w1(    0x00000000, Tx.f_In   );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	//
	CHECKX(               0xffffffff, Tx.Fsel0.read() );
	CHECKX(               0xffffffff, Tx.Fsel1.read() );
	CHECKX(               0xffffffff, Tx.Fsel2.read() );
	CHECKX(               0xffffffff, Tx.Fsel3.read() );
	CHECKX(               0xffffffff, Tx.Fsel4.read() );
	CHECKX(               0xffffffff, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "74a", "modify_Fsel_w0() mix" );
    try {
	Tx.Fsel0.write(     000000000000 );
	Tx.Fsel1.write(     000000000000 );
	Tx.Fsel2.write(     000000000000 );
	Tx.Fsel3.write(     000000000000 );
	Tx.Fsel4.write(     000000000000 );
	Tx.Fsel5.write(     000000000000 );
	CHECKX(               0xffffffff, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt2 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w0( Tx.f_Alt5 ) );
	Tx.modify_Fsel_w0(    0x00000007, Tx.f_In   );	// 0
	Tx.modify_Fsel_w0(    0x00000070, Tx.f_Out  );	// 1
	Tx.modify_Fsel_w0(    0x00000700, Tx.f_Alt0 );	// 4
	Tx.modify_Fsel_w0(    0x00007000, Tx.f_Alt1 );	// 5
	Tx.modify_Fsel_w0(    0x00070000, Tx.f_Alt2 );	// 6
	Tx.modify_Fsel_w0(    0x00700000, Tx.f_Alt3 );	// 7
	Tx.modify_Fsel_w0(    0x07000000, Tx.f_Alt4 );	// 3
	Tx.modify_Fsel_w0(    0x70000000, Tx.f_Alt5 );	// 2
	CHECKX(               0x8888888f, Tx.read_Fsel_w0( Tx.f_In   ) );
	CHECKX(               0x00000070, Tx.read_Fsel_w0( Tx.f_Out  ) );
	CHECKX(               0x00000700, Tx.read_Fsel_w0( Tx.f_Alt0 ) );
	CHECKX(               0x00007000, Tx.read_Fsel_w0( Tx.f_Alt1 ) );
	CHECKX(               0x00070000, Tx.read_Fsel_w0( Tx.f_Alt2 ) );
	CHECKX(               0x00700000, Tx.read_Fsel_w0( Tx.f_Alt3 ) );
	CHECKX(               0x07000000, Tx.read_Fsel_w0( Tx.f_Alt4 ) );
	CHECKX(               0x70000000, Tx.read_Fsel_w0( Tx.f_Alt5 ) );
	//
	CHECKX(             004401110000, Tx.Fsel0.read() );
	CHECKX(             000666055504, Tx.Fsel1.read() );
	CHECKX(             002203330777, Tx.Fsel2.read() );
	CHECKX(             000000000002, Tx.Fsel3.read() );
	CHECKX(             000000000000, Tx.Fsel4.read() );
	CHECKX(             000000000000, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74b", "modify_Fsel_w1() mix" );
    try {
	Tx.Fsel0.write(     000000000000 );
	Tx.Fsel1.write(     000000000000 );
	Tx.Fsel2.write(     000000000000 );
	Tx.Fsel3.write(     000000000000 );
	Tx.Fsel4.write(     000000000000 );
	Tx.Fsel5.write(     000000000000 );
	CHECKX(               0x0fffffff, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Out  ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt0 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt1 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt2 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt4 ) );
	CHECKX(               0x00000000, Tx.read_Fsel_w1( Tx.f_Alt5 ) );
	Tx.modify_Fsel_w1(    0x00000007, Tx.f_Out  );	// 1
	Tx.modify_Fsel_w1(    0x00000070, Tx.f_Alt0 );	// 4
	Tx.modify_Fsel_w1(    0x00000700, Tx.f_Alt1 );	// 5
	Tx.modify_Fsel_w1(    0x00007000, Tx.f_Alt2 );	// 6
	Tx.modify_Fsel_w1(    0x00070000, Tx.f_Alt3 );	// 7
	Tx.modify_Fsel_w1(    0x00700000, Tx.f_Alt4 );	// 3
	Tx.modify_Fsel_w1(    0x07000000, Tx.f_Alt5 );	// 2
	Tx.modify_Fsel_w1(    0x70000000, Tx.f_In   );	// 0  no effect
	CHECKX(               0x08888888, Tx.read_Fsel_w1( Tx.f_In   ) );
	CHECKX(               0x00000007, Tx.read_Fsel_w1( Tx.f_Out  ) );
	CHECKX(               0x00000070, Tx.read_Fsel_w1( Tx.f_Alt0 ) );
	CHECKX(               0x00000700, Tx.read_Fsel_w1( Tx.f_Alt1 ) );
	CHECKX(               0x00007000, Tx.read_Fsel_w1( Tx.f_Alt2 ) );
	CHECKX(               0x00070000, Tx.read_Fsel_w1( Tx.f_Alt3 ) );
	CHECKX(               0x00700000, Tx.read_Fsel_w1( Tx.f_Alt4 ) );
	CHECKX(               0x07000000, Tx.read_Fsel_w1( Tx.f_Alt5 ) );
	//
	CHECKX(             000000000000, Tx.Fsel0.read() );
	CHECKX(             000000000000, Tx.Fsel1.read() );
	CHECKX(             000000000000, Tx.Fsel2.read() );
	CHECKX(             000444011100, Tx.Fsel3.read() );
	CHECKX(             007706660555, Tx.Fsel4.read() );
	CHECKX(             000222033307, Tx.Fsel5.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "78a", "modify_Fsel_w0() loop all" );
    try {
	uint32_t	mask = 0x1;
	for ( int k=0;  k<=31;  k++ ) {
	    Tx.Fsel0.write(   0xffffffff );
	    Tx.Fsel1.write(   0xffffffff );
	    Tx.Fsel2.write(   0xffffffff );
	    Tx.Fsel3.write(   0xffffffff );
	    Tx.Fsel4.write(   0xffffffff );
	    Tx.Fsel5.write(   0xffffffff );
	    Tx.modify_Fsel_w0( mask, Tx.f_In   );
	    CHECKX(                 mask, Tx.read_Fsel_w0( Tx.f_In   ) );
	    CHECK(                     0, Tx.read_Fsel_bit( k ) );
	    cout.fill('0');
	    cout << "k= "       <<dec <<setw(2) << k
		<< "  mask= 0x" <<hex <<setw(8) << mask <<dec <<endl;
	    mask = mask << 1;
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "78b", "modify_Fsel_w1() loop all" );
    try {
	uint32_t	mask = 0x1;
	for ( int k=32;  k<=57;  k++ ) {
	    Tx.Fsel0.write(   0xffffffff );
	    Tx.Fsel1.write(   0xffffffff );
	    Tx.Fsel2.write(   0xffffffff );
	    Tx.Fsel3.write(   0xffffffff );
	    Tx.Fsel4.write(   0xffffffff );
	    Tx.Fsel5.write(   0xffffffff );
	    Tx.modify_Fsel_w1( mask, Tx.f_In   );
	    CHECKX(                 mask, Tx.read_Fsel_w1( Tx.f_In   ) );
	    CHECK(                     0, Tx.read_Fsel_bit( k ) );
	    cout.fill('0');
	    cout << "k= "       <<dec <<setw(2) << k
		<< "  mask= 0x" <<hex <<setw(8) << mask <<dec <<endl;
	    mask = mask << 1;
	}
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## rgFsel_enum string conversion
//--------------------------------------------------------------------------

  CASE( "80a", "rgFsel_enum2cstr() object call" );
    try {
	const char		*v;
	v = Tx.rgFsel_enum2cstr( rgFselPin::f_Alt3 );
	CHECK( "Alt3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "80b", "rgFsel_enum2cstr() class call" );
    try {
	const char		*v;
	v = rgFselPin::rgFsel_enum2cstr( rgFselPin::f_Alt3 );
	CHECK( "Alt3", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//---------------------------------------
  CASE( "81a", "rgFsel_enum2cstr() cast an integer" );
    try {
	const char		*v;
	v = Tx.rgFsel_enum2cstr( (rgFselPin::rgFsel_enum) 3 );
	CHECK( "Alt4", v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "81b", "rgFsel_enum2cstr() bad enum" );
    try {
	Tx.rgFsel_enum2cstr( (rgFselPin::rgFsel_enum) 8 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgFselPin::rgFsel_enum2cstr():  bad enum= 8",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//---------------------------------------
  CASE( "82", "str_IoReg_enum() all enum" );
    try {
	CHECK(                              "In",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_In   )
	);
	CHECK(                              "Out",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Out  )
	);
	CHECK(                              "Alt0",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt0 )
	);
	CHECK(                              "Alt1",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt1 )
	);
	CHECK(                              "Alt2",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt2 )
	);
	CHECK(                              "Alt3",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt3 )
	);
	CHECK(                              "Alt4",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt4 )
	);
	CHECK(                              "Alt5",
	    Tx.rgFsel_enum2cstr( rgFselPin::f_Alt5 )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## find_rgFsel_enum()
//--------------------------------------------------------------------------

  CASE( "90a", "find_rgFsel_enum() object call" );
    try {
	rgFselPin::rgFsel_enum	v;
	v = Tx.find_rgFsel_enum( "Alt2" );
	CHECK( rgFselPin::f_Alt2, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "90b", "find_rgFsel_enum() class call" );
    try {
	rgFselPin::rgFsel_enum	v;
	v = rgFselPin::find_rgFsel_enum( "Alt2" );
	CHECK( rgFselPin::f_Alt2, v );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "91", "find_rgFsel_enum() null pointer" );
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

  CASE( "92", "find_rgFsel_enum() null string" );
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

  CASE( "93", "find_rgFsel_enum() bad string" );
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
  CASE( "94", "find_rgFsel_enum() all strings" );
    try {
	CHECK(       rgFselPin::f_In,
	    Tx.find_rgFsel_enum( "In"   )
	);
	CHECK(       rgFselPin::f_Out,
	    Tx.find_rgFsel_enum( "Out"  )
	);
	CHECK(       rgFselPin::f_Alt0,
	    Tx.find_rgFsel_enum( "Alt0" )
	);
	CHECK(       rgFselPin::f_Alt1,
	    Tx.find_rgFsel_enum( "Alt1" )
	);
	CHECK(       rgFselPin::f_Alt2,
	    Tx.find_rgFsel_enum( "Alt2" )
	);
	CHECK(       rgFselPin::f_Alt3,
	    Tx.find_rgFsel_enum( "Alt3" )
	);
	CHECK(       rgFselPin::f_Alt4,
	    Tx.find_rgFsel_enum( "Alt4" )
	);
	CHECK(       rgFselPin::f_Alt5,
	    Tx.find_rgFsel_enum( "Alt5" )
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


  CASE( "99", "Done" );
}

