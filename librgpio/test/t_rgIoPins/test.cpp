// 2019-10-21  William A. Hudson
//
// Testing:  rgIoPins  IO Pin Registers (register objects)
//    10-19  Constructor, get_bcm_address(), rgReg_* constructors
//    20-29  Address of registers  addr()
//    30-39  Indirect rgReg::read(), write()
//    40-49  Register set()
//    50-59  Register clr()
//    60-98  Register modify()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgIoPins.h"

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

rgIoPins		Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "rgIoPins constructor" );
    try {
	rgIoPins	tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "rgIoPins domain_error RPi5" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );
	rgIoPins       tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( std::domain_error& e ) {
	CHECK( "rgIoPins:  require RPi4 (soc_BCM2711) or earlier", e.what() );
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
  CASE( "12", "rgReg constructor init" );
    try {
	rgReg		tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "rgReg_set constructor init" );
    try {
	rgReg_set	tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "rgReg_clr constructor init" );
    try {
	rgReg_clr	tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "rgReg_rw constructor init" );
    try {
	rgReg_rw	tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16", "rgReg_PinLevel constructor init" );
    try {
	rgReg_PinLevel	tx;
	CHECK( 1, tx.addr()     == NULL );
	CHECK( 1, tx.addr_set() == NULL );
	CHECK( 1, tx.addr_clr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor copy initialization, all classes.

  CASE( "16a", "rgReg_set copy constructor" );
    try {
	rgReg_set	rx  ( Tx.PinSet_w0 );
	CHECKX( 0x1c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16b", "rgReg_clr copy constructor" );
    try {
	rgReg_clr	rx  ( Tx.EventStatus_w0 );
	CHECKX( 0x40, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16c", "rgReg_rw copy constructor" );
    try {
	rgReg_rw	rx  ( Tx.DetectRise_w0 );
	CHECKX( 0x4c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16d", "rgReg_PinLevel copy constructor" );
    try {
	rgReg_PinLevel	rx  ( Tx.PinLevel_w0 );
	CHECKX( 0x34, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "Fsel0.addr()" );
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

//--------------------------------------
  CASE( "22a", "PinSet_w0.addr()" );
    try {
	CHECKX( 0x1c, (Tx.PinSet_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x20, (Tx.PinSet_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22b", "PinClr_w0.addr()" );
    try {
	CHECKX( 0x28, (Tx.PinClr_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x2c, (Tx.PinClr_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22c", "PinRead_w0.addr()" );
    try {
	CHECKX( 0x34, (Tx.PinRead_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x38, (Tx.PinRead_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "23", "EventStatus_w0.addr()" );
    try {
	CHECKX( 0x40, (Tx.EventStatus_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x44, (Tx.EventStatus_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "24a", "DetectRise_w0.addr()" );
    try {
	CHECKX( 0x4c, (Tx.DetectRise_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x50, (Tx.DetectRise_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24b", "DetectFall_w0.addr()" );
    try {
	CHECKX( 0x58, (Tx.DetectFall_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x5c, (Tx.DetectFall_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24c", "DetectHigh_w0.addr()" );
    try {
	CHECKX( 0x64, (Tx.DetectHigh_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x68, (Tx.DetectHigh_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24d", "DetectLow_w0.addr()" );
    try {
	CHECKX( 0x70, (Tx.DetectLow_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x74, (Tx.DetectLow_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24e", "DetectAsyncRise_w0.addr()" );
    try {
	CHECKX( 0x7c, (Tx.DetectAsyncRise_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x80, (Tx.DetectAsyncRise_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24f", "DetectAsyncFall_w0.addr()" );
    try {
	CHECKX( 0x88, (Tx.DetectAsyncFall_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x8c, (Tx.DetectAsyncFall_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "25", "PudProgMode.addr()" );
    try {
	CHECKX( 0x94, (Tx.PudProgMode.addr()   - Tx.get_base_addr())*4 );
	CHECKX( 0x98, (Tx.PudProgClk_w0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x9c, (Tx.PudProgClk_w1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "26", "PullSel0.addr()" );
    try {
	CHECKX( 0xe4, (Tx.PullSel0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xe8, (Tx.PullSel1.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xec, (Tx.PullSel2.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0xf0, (Tx.PullSel3.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "27a", "PinLevel_w0.addr()" );
    try {
	CHECKX( 0x34, (Tx.PinLevel_w0.addr()     - Tx.get_base_addr())*4 );
	CHECKX( 0x1c, (Tx.PinLevel_w0.addr_set() - Tx.get_base_addr())*4 );
	CHECKX( 0x28, (Tx.PinLevel_w0.addr_clr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27b", "PinLevel_w1.addr()" );
    try {
	CHECKX( 0x38, (Tx.PinLevel_w1.addr()     - Tx.get_base_addr())*4 );
	CHECKX( 0x20, (Tx.PinLevel_w1.addr_set() - Tx.get_base_addr())*4 );
	CHECKX( 0x2c, (Tx.PinLevel_w1.addr_clr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Indirect rgReg::read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.
// Gray-box testing, only one example register in each class.

  CASE( "30", "condition write(), read()" );
    try {
	Tx.Fsel0.write(              0xffffffff );
	Tx.PinSet_w0.write(          0xffffffff );
	Tx.PinClr_w0.write(          0xffffffff );
	Tx.PinRead_w0.write(         0xffffffff );
	Tx.EventStatus_w0.write(     0xffffffff );
	Tx.DetectRise_w0.write(      0xffffffff );
	Tx.DetectFall_w0.write(      0xffffffff );
	Tx.DetectHigh_w0.write(      0xffffffff );
	Tx.DetectLow_w0.write(       0xffffffff );
	Tx.DetectAsyncRise_w0.write( 0xffffffff );
	Tx.DetectAsyncFall_w0.write( 0xffffffff );
	Tx.PudProgMode.write(        0xffffffff );
	Tx.PudProgClk_w0.write(      0xffffffff );
	Tx.PullSel0.write(           0xffffffff );
	CHECKX(                      0xffffffff, Tx.Fsel0.read()             );
	CHECKX(                      0xffffffff, Tx.PinSet_w0.read()         );
	CHECKX(                      0xffffffff, Tx.PinClr_w0.read()         );
	CHECKX(                      0xffffffff, Tx.PinRead_w0.read()        );
	CHECKX(                      0xffffffff, Tx.EventStatus_w0.read()    );
	CHECKX(                      0xffffffff, Tx.DetectRise_w0.read()     );
	CHECKX(                      0xffffffff, Tx.DetectFall_w0.read()     );
	CHECKX(                      0xffffffff, Tx.DetectHigh_w0.read()     );
	CHECKX(                      0xffffffff, Tx.DetectLow_w0.read()      );
	CHECKX(                      0xffffffff, Tx.DetectAsyncRise_w0.read());
	CHECKX(                      0xffffffff, Tx.DetectAsyncFall_w0.read());
	CHECKX(                      0xffffffff, Tx.PudProgMode.read()       );
	CHECKX(                      0xffffffff, Tx.PudProgClk_w0.read()     );
	CHECKX(                      0xffffffff, Tx.PullSel0.read()          );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "rgReg_rw  Fsel0.read()" );
    try {
	Tx.Fsel0.write(              0x11111111 );
	CHECKX(                      0x11111111, Tx.Fsel0.read()             );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register set()
//--------------------------------------------------------------------------
// Fake memory - does not model hardware set/clear.

  CASE( "41", "rgReg_set  PinSet_w0.set()" );
    try {
	Tx.PinSet_w0.write(          0xffff0000 );
	Tx.PinSet_w0.set(            0x00330033 );
	CHECKX(                      0x00330033, Tx.PinSet_w0.read()         );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "42", "rgReg_rw  Fsel0.set()" );
    try {
	Tx.Fsel0.write(              0xffff0000 );
	Tx.Fsel0.set(                0x00330033 );
	CHECKX(                      0xffff0033, Tx.Fsel0.read()             );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "43", "rgReg_PinLevel  PinLevel_w0.set() virtual reg" );
    try {
	Tx.PinLevel_w0.write(        0xffff0000 );
	Tx.PinRead_w0.write(         0xffff0000 );
	Tx.PinSet_w0.write(          0xffff0000 );
	Tx.PinClr_w0.write(          0xffff0000 );
	Tx.PinLevel_w0.set(          0x00330033 );
	CHECKX(                      0xffff0000, Tx.PinLevel_w0.read() );
	CHECKX(                      0xffff0000, Tx.PinRead_w0.read()  );
	CHECKX(                      0x00330033, Tx.PinSet_w0.read()   );
	CHECKX(                      0xffff0000, Tx.PinClr_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register clr()
//--------------------------------------------------------------------------

  CASE( "51", "rgReg_clr  PinClr_w0.set()" );
    try {
	Tx.PinClr_w0.write(          0xffff0000 );
	Tx.PinClr_w0.clr(            0x00330033 );
	CHECKX(                      0x00330033, Tx.PinClr_w0.read()         );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52", "rgReg_rw  Fsel0.clr()" );
    try {
	Tx.Fsel0.write(              0xffff0000 );
	Tx.Fsel0.clr(                0x00330033 );
	CHECKX(                      0xffcc0000, Tx.Fsel0.read()             );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53", "rgReg_PinLevel  PinLevel_w0.clr() virtual reg" );
    try {
	Tx.PinLevel_w0.write(        0xffff0000 );
	Tx.PinRead_w0.write(         0xffff0000 );
	Tx.PinSet_w0.write(          0xffff0000 );
	Tx.PinClr_w0.write(          0xffff0000 );
	Tx.PinLevel_w0.clr(          0x00330033 );
	CHECKX(                      0xffff0000, Tx.PinLevel_w0.read() );
	CHECKX(                      0xffff0000, Tx.PinRead_w0.read()  );
	CHECKX(                      0xffff0000, Tx.PinSet_w0.read()   );
	CHECKX(                      0x00330033, Tx.PinClr_w0.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Register modify()
//--------------------------------------------------------------------------

  CASE( "62", "rgReg_rw  Fsel0.modify()" );
    try {
	Tx.Fsel0.write(              0x00ff00ff );
	Tx.Fsel0.modify( 0x0000ffff, 0x33333333 );
	CHECKX(                      0x00ff3333, Tx.Fsel0.read()             );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63", "rgReg_PinLevel  PinLevel_w0.modify()" );
    try {
	Tx.PinLevel_w0.write(              0x00ff00ff );
	Tx.PinRead_w0.write(               0x00ff00ff );
	Tx.PinSet_w0.write(                0x00ff00ff );
	Tx.PinClr_w0.write(                0x00ff00ff );
	Tx.PinLevel_w0.modify( 0x0000ffff, 0x33333333 );
	CHECKX(                            0x00ff00ff, Tx.PinLevel_w0.read() );
	CHECKX(                            0x00ff00ff, Tx.PinRead_w0.read()  );
	CHECKX(                            0x00003333, Tx.PinSet_w0.read() );
	CHECKX(                            0x0000cccc, Tx.PinClr_w0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------
  CASE( "99", "Done" );
}

