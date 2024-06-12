// 2020-04-25  William A. Hudson
//
// Testing:  rgSysTimer  System Timer
//    10-19  Constructor, get_bcm_address(), rgReg_* constructors
//    20-29  Address of registers  addr()
//    30-39  Direct register access  read(), write()
//    40-49  Full register get(), put(), grab(), push()
//    50-59  Virtual register rgSysTimer_TimeDw:: class
//    60-98  .
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgSysTimer.h"

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

rgSysTimer		Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "rgSysTimer constructor" );
    try {
	rgSysTimer	tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e003000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12", "rgSysTimer_Stat constructor init" );
    try {
	rgSysTimer_Stat		tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "13", "rgSysTimer_TimeW0 constructor init" );
    try {
	rgSysTimer_TimeW0	tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "rgSysTimer_TimeW1 constructor init" );
    try {
	rgSysTimer_TimeW1	tx;
	CHECK( 1, tx.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "rgSysTimer_TimeDw constructor init" );
    try {
	rgSysTimer_TimeDw	tx;
	CHECK( 1, tx.W0.addr()  == NULL );
	CHECK( 1, tx.W1.addr()  == NULL );
	CHECK( 1, tx.W1B.addr() == NULL );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor copy initialization, all classes.

  CASE( "16a", "rgSysTimer_Stat copy constructor" );
    try {
	rgSysTimer_Stat		rx  ( Tx.Stat );
	CHECKX( 0x00, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16b", "rgSysTimer_TimeW0 copy constructor" );
    try {
	rgSysTimer_TimeW0	rx  ( Tx.TimeW0 );
	CHECKX( 0x04, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16c", "rgSysTimer_TimeW1 copy constructor" );
    try {
	rgSysTimer_TimeW1	rx  ( Tx.TimeW1 );
	CHECKX( 0x08, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16d", "rgSysTimer_TimeDw copy constructor" );
    try {
	rgSysTimer_TimeDw	rx  ( Tx.TimeDw );
	CHECKX( 0x04, (rx.W0.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16e", "rgRegister copy constructor" );
    try {
	rgRegister		rx  ( Tx.Cmp0 );
	CHECKX( 0x0c, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "Stat.addr()" );
    try {
	CHECKX( 0x00, (Tx.Stat.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "TimeW0.addr()" );
    try {
	CHECKX( 0x04, (Tx.TimeW0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x08, (Tx.TimeW1.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "TimeDw.addr() virtual register" );
    try {
	CHECKX( 0x04, (Tx.TimeDw.W0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x08, (Tx.TimeDw.W1.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x08, (Tx.TimeDw.W1B.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "Cmp0.addr()" );
    try {
	CHECKX( 0x0c, (Tx.Cmp0.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x10, (Tx.Cmp1.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x14, (Tx.Cmp2.addr() - Tx.get_base_addr())*4 );
	CHECKX( 0x18, (Tx.Cmp3.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Direct register access  read(), write()
//--------------------------------------------------------------------------
// Test write/read to fake memory.

  CASE( "30", "condition write(), read()" );
    try {
	Tx.Stat.write(    0xffffffff );
	Tx.TimeW0.write(  0xffffffff );
	Tx.TimeW1.write(  0xffffffff );
	Tx.Cmp0.write(    0xffffffff );
	Tx.Cmp1.write(    0xffffffff );
	Tx.Cmp2.write(    0xffffffff );
	Tx.Cmp3.write(    0xffffffff );
	CHECKX(           0xffffffff, Tx.Stat.read()    );
	CHECKX(           0xffffffff, Tx.TimeW0.read()  );
	CHECKX(           0xffffffff, Tx.TimeW1.read()  );
	CHECKX(           0xffffffff, Tx.Cmp0.read()    );
	CHECKX(           0xffffffff, Tx.Cmp1.read()    );
	CHECKX(           0xffffffff, Tx.Cmp2.read()    );
	CHECKX(           0xffffffff, Tx.Cmp3.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "Stat.read()" );
    try {
	Tx.Stat.write(    0x44bbbb44 );
	CHECKX(           0x44bbbb44, Tx.Stat.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "TimeW0.read()" );
    try {
	Tx.TimeW0.write(  0x55aaaa55 );
	CHECKX(           0x55aaaa55, Tx.TimeW0.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "TimeW1.read()" );
    try {
	Tx.TimeW1.write(  0x66999966 );
	CHECKX(           0x66999966, Tx.TimeW1.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "Cmp0.read()" );
    try {
	Tx.Cmp0.write(    0x00ffff00 );
	CHECKX(           0x00ffff00, Tx.Cmp0.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "Cmp1.read()" );
    try {
	Tx.Cmp1.write(    0x11eeee11 );
	CHECKX(           0x11eeee11, Tx.Cmp1.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "Cmp2.read()" );
    try {
	Tx.Cmp2.write(    0x22dddd22 );
	CHECKX(           0x22dddd22, Tx.Cmp2.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "37", "Cmp3.read()" );
    try {
	Tx.Cmp3.write(    0x33cccc33 );
	CHECKX(           0x33cccc33, Tx.Cmp3.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "39", "check final register pattern" );
    try {
	CHECKX(           0x44bbbb44, Tx.Stat.read()    );
	CHECKX(           0x55aaaa55, Tx.TimeW0.read()  );
	CHECKX(           0x66999966, Tx.TimeW1.read()  );
	CHECKX(           0x00ffff00, Tx.Cmp0.read()    );
	CHECKX(           0x11eeee11, Tx.Cmp1.read()    );
	CHECKX(           0x22dddd22, Tx.Cmp2.read()    );
	CHECKX(           0x33cccc33, Tx.Cmp3.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Full register get(), put(), grab(), push()
//--------------------------------------------------------------------------
// Gray box sanity check just one register.

  CASE( "40", "condition registers" );
    try {
	Tx.Stat.write(    0x00000000 );
	Tx.TimeW0.write(  0x00000000 );
	Tx.TimeW1.write(  0x00000000 );
	Tx.Cmp0.write(    0x00000000 );
	Tx.Cmp1.write(    0x00000000 );
	Tx.Cmp2.write(    0x00000000 );
	Tx.Cmp3.write(    0x00000000 );
	CHECKX(           0x00000000, Tx.Stat.read()    );
	CHECKX(           0x00000000, Tx.TimeW0.read()  );
	CHECKX(           0x00000000, Tx.TimeW1.read()  );
	CHECKX(           0x00000000, Tx.Cmp0.read()    );
	CHECKX(           0x00000000, Tx.Cmp1.read()    );
	CHECKX(           0x00000000, Tx.Cmp2.read()    );
	CHECKX(           0x00000000, Tx.Cmp3.read()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41a", "TimeW0.grab(), get()" );
    try {
	Tx.TimeW0.write(  0xccc33ccc );
	Tx.TimeW0.put(    0x77444477 );
	CHECKX(           0xccc33ccc, Tx.TimeW0.read()  );
	CHECKX(           0x77444477, Tx.TimeW0.get()   );
	Tx.TimeW0.grab();
	CHECKX(           0xccc33ccc, Tx.TimeW0.read()  );
	CHECKX(           0xccc33ccc, Tx.TimeW0.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "41b", "TimeW0.put(), push()" );
    try {
	CHECKX(           0xccc33ccc, Tx.TimeW0.read()  );
	CHECKX(           0xccc33ccc, Tx.TimeW0.get()   );
	Tx.TimeW0.put(    0xbeef0303 );
	CHECKX(           0xccc33ccc, Tx.TimeW0.read()  );
	CHECKX(           0xbeef0303, Tx.TimeW0.get()   );
	Tx.TimeW0.push();
	CHECKX(           0xbeef0303, Tx.TimeW0.read()  );
	CHECKX(           0xbeef0303, Tx.TimeW0.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Virtual register rgSysTimer_TimeDw:: class
//--------------------------------------------------------------------------

  CASE( "50", "init TimeDw" );
    try {
	Tx.TimeW0.write(  0x22222222 );
	Tx.TimeW1.write(  0x77777777 );
	CHECKX(           0x22222222, Tx.TimeDw.W0.read()  );
	CHECKX(           0x77777777, Tx.TimeDw.W1.read()  );
	CHECKX(           0x77777777, Tx.TimeDw.W1B.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51", "TimeDw.grab64()" );
    try {
	Tx.TimeW0.write(  0x22222222 );
	Tx.TimeW1.write(  0x77777777 );
	CHECK( 1, 0x7777777722222222 == Tx.TimeDw.grab64() );
	CHECK( 0, 0x7777777722222221 == Tx.TimeDw.grab64() );  // not equal
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "TimeDw.get64() rollover before" );
    try {
	Tx.TimeDw.W0.put(    0x0000ffff );
	Tx.TimeDw.W1.put(    0x00000001 );
	Tx.TimeDw.W1B.put(   0x00000002 );
	CHECKX(              0x0000ffff, Tx.TimeDw.W0.get()  );
	CHECKX(              0x00000001, Tx.TimeDw.W1.get()  );
	CHECKX(              0x00000002, Tx.TimeDw.W1B.get() );
	CHECK( 1, 0x000000020000ffff == Tx.TimeDw.get64() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53b", "TimeDw.get64() rollover after" );
    try {
	Tx.TimeDw.W0.put(    0xff000000 );
	Tx.TimeDw.W1.put(    0x00000001 );
	Tx.TimeDw.W1B.put(   0x00000002 );
	CHECKX(              0xff000000, Tx.TimeDw.W0.get()  );
	CHECKX(              0x00000001, Tx.TimeDw.W1.get()  );
	CHECKX(              0x00000002, Tx.TimeDw.W1B.get() );
	CHECK( 1, 0x00000001ff000000 == Tx.TimeDw.get64() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53c", "TimeDw.get64() no rollover" );
    try {
	Tx.TimeDw.W0.put(    0x0000ffff );
	Tx.TimeDw.W1.put(    0x00000003 );
	Tx.TimeDw.W1B.put(   0x00000003 );
	CHECKX(              0x0000ffff, Tx.TimeDw.W0.get()  );
	CHECKX(              0x00000003, Tx.TimeDw.W1.get()  );
	CHECKX(              0x00000003, Tx.TimeDw.W1B.get() );
	CHECK( 1, 0x000000030000ffff == Tx.TimeDw.get64() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

