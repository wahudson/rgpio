// 2019-10-21  William A. Hudson
//
// Testing:  rgPwm  PWM Pulse Width Modulator class
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers  addr()
//    30-39  Direct register access  read(), write()
//    40-49  Full register get(), put(), grab(), push()
//    50-59  Object State  init_put_reset(), grab_regs(), push_regs()
//    60-98  Field Accessors  get(), put()  #!! incomplete
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgPwm.h"

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

rgPwm			Tx   ( &Bx );		// test object


//--------------------------------------------------------------------------
//## Constructor
//--------------------------------------------------------------------------

  CASE( "10", "constructor" );
    try {
	rgPwm		tx  ( &Bx );
	PASS( "constructor" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "get_bcm_address() Feature Address" );
    try {
	CHECKX( 0x7e20c000, Tx.get_bcm_address() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Constructor initialization, all reg classes

  CASE( "14a", "reg constructor rgPwm_Cntl" );
    try {
	rgPwm_Cntl	rx  ( Tx.Cntl );
	CHECKX( 0x00, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14b", "reg constructor rgPwm_Stat" );
    try {
	rgPwm_Stat	rx  ( Tx.Stat );
	CHECKX( 0x04, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14c", "reg constructor rgPwm_DmaConf" );
    try {
	rgPwm_DmaConf	rx  ( Tx.DmaConf );
	CHECKX( 0x08, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14d", "reg constructor rgPwm_Fifo" );
    try {
	rgPwm_Fifo	rx  ( Tx.Fifo );
	CHECKX( 0x18, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14e", "reg constructor rgPwm_Ch1Range" );
    try {
	rgPwm_Ch1Range	rx  ( Tx.Ch1Range );
	CHECKX( 0x10, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14f", "reg constructor rgPwm_Ch1Data" );
    try {
	rgPwm_Ch1Data	rx  ( Tx.Ch1Data );
	CHECKX( 0x14, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14g", "reg constructor rgPwm_Ch2Range" );
    try {
	rgPwm_Ch2Range	rx  ( Tx.Ch2Range );
	CHECKX( 0x20, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14h", "reg constructor rgPwm_Ch2Data" );
    try {
	rgPwm_Ch2Data	rx  ( Tx.Ch2Data );
	CHECKX( 0x24, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Register object copy, example class

  CASE( "15d", "reg constructor rgPwm_Fifo" );
    try {
	rgPwm_Fifo	rx;		// uninitialized
	CHECKX( 0x00, (rx.addr() - ((volatile uint32_t*) NULL) ) );
	rx = Tx.Fifo;
	CHECKX( 0x18, (rx.addr() - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "Cntl.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Cntl.addr();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "Stat.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Stat.addr();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "DmaConf.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.DmaConf.addr();
	CHECKX( 0x00000008, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "Fifo.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Fifo.addr();
	CHECKX( 0x00000018, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "Ch1Range.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Ch1Range.addr();
	CHECKX( 0x00000010, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26", "Ch1Data.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Ch1Data.addr();
	CHECKX( 0x00000014, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "27", "Ch2Range.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Ch2Range.addr();
	CHECKX( 0x00000020, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "28", "Ch2Data.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Ch2Data.addr();
	CHECKX( 0x00000024, (vp - Tx.get_base_addr())*4 );
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
	Tx.Cntl.write(     0xffffffff );
	Tx.Stat.write(     0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.DmaConf.write(  0xffffffff );
	Tx.Ch1Range.write( 0xffffffff );
	Tx.Ch1Data.write(  0xffffffff );
	Tx.Ch2Range.write( 0xffffffff );
	Tx.Ch2Data.write(  0xffffffff );
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Stat.read()     );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.DmaConf.read()  );
	CHECKX(            0xffffffff, Tx.Ch1Range.read() );
	CHECKX(            0xffffffff, Tx.Ch1Data.read()  );
	CHECKX(            0xffffffff, Tx.Ch2Range.read() );
	CHECKX(            0xffffffff, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "Cntl.read()" );
    try {
	Tx.Cntl.write(     0x33cccc33 );
	CHECKX(            0x33cccc33, Tx.Cntl.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Stat.read()" );
    try {
	Tx.Stat.write(     0x11eeee11 );
	CHECKX(            0x11eeee11, Tx.Stat.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "DmaConf.read()" );
    try {
	Tx.DmaConf.write(  0x22dddd22 );
	CHECKX(            0x22dddd22, Tx.DmaConf.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "Fifo.read()" );
    try {
	Tx.Fifo.write(     0x44bbbb44 );
	CHECKX(            0x44bbbb44, Tx.Fifo.read()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "Ch1Range.read()" );
    try {
	Tx.Ch1Range.write( 0x55aaaa55 );
	CHECKX(            0x55aaaa55, Tx.Ch1Range.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "Ch1Data.read()" );
    try {
	Tx.Ch1Data.write(  0x66999966 );
	CHECKX(            0x66999966, Tx.Ch1Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "37", "Ch2Range.read()" );
    try {
	Tx.Ch2Range.write( 0x77444477 );
	CHECKX(            0x77444477, Tx.Ch2Range.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "38", "Ch2Data.read()" );
    try {
	Tx.Ch2Data.write(  0x88333388 );
	CHECKX(            0x88333388, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "39", "check final register pattern" );
    try {
	CHECKX(            0x33cccc33, Tx.Cntl.read()     );
	CHECKX(            0x11eeee11, Tx.Stat.read()     );
	CHECKX(            0x22dddd22, Tx.DmaConf.read()  );
	CHECKX(            0x44bbbb44, Tx.Fifo.read()     );
	CHECKX(            0x55aaaa55, Tx.Ch1Range.read() );
	CHECKX(            0x66999966, Tx.Ch1Data.read()  );
	CHECKX(            0x77444477, Tx.Ch2Range.read() );
	CHECKX(            0x88333388, Tx.Ch2Data.read()  );
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
	Tx.Cntl.write(     0x00000000 );
	Tx.Stat.write(     0x00000000 );
	Tx.Fifo.write(     0x00000000 );
	Tx.DmaConf.write(  0x00000000 );
	Tx.Ch1Range.write( 0x00000000 );
	Tx.Ch1Data.write(  0x00000000 );
	Tx.Ch2Range.write( 0x00000000 );
	Tx.Ch2Data.write(  0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Stat.read()     );
	CHECKX(            0x00000000, Tx.Fifo.read()     );
	CHECKX(            0x00000000, Tx.DmaConf.read()  );
	CHECKX(            0x00000000, Tx.Ch1Range.read() );
	CHECKX(            0x00000000, Tx.Ch1Data.read()  );
	CHECKX(            0x00000000, Tx.Ch2Range.read() );
	CHECKX(            0x00000000, Tx.Ch2Data.read()  );
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
	Tx.Cntl.put(       0xbeef0303 );
	CHECKX(            0xccc33ccc, Tx.Cntl.read()     );
	CHECKX(            0xbeef0303, Tx.Cntl.get()      );
	Tx.Cntl.push();
	CHECKX(            0xbeef0303, Tx.Cntl.read()     );
	CHECKX(            0xbeef0303, Tx.Cntl.get()      );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State  init_put_reset(), grab_regs(), push_regs()
//--------------------------------------------------------------------------

  CASE( "50", "condition registers" );
    try {
	Tx.Cntl.write(     0xffffffff );
	Tx.Stat.write(     0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.DmaConf.write(  0xffffffff );
	Tx.Ch1Range.write( 0xffffffff );
	Tx.Ch1Data.write(  0xffffffff );
	Tx.Ch2Range.write( 0xffffffff );
	Tx.Ch2Data.write(  0xffffffff );
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Stat.read()     );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.DmaConf.read()  );
	CHECKX(            0xffffffff, Tx.Ch1Range.read() );
	CHECKX(            0xffffffff, Tx.Ch1Data.read()  );
	CHECKX(            0xffffffff, Tx.Ch2Range.read() );
	CHECKX(            0xffffffff, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "51a", "init_put_reset() 0xffffffff" );
    try {
	Tx.Cntl.put(       0xffffffff );
	Tx.Stat.put(       0xffffffff );
	Tx.Fifo.put(       0xffffffff );
	Tx.DmaConf.put(    0xffffffff );
	Tx.Ch1Range.put(   0xffffffff );
	Tx.Ch1Data.put(    0xffffffff );
	Tx.Ch2Range.put(   0xffffffff );
	Tx.Ch2Data.put(    0xffffffff );
	Tx.init_put_reset();
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	CHECKX(            0x00000002, Tx.Stat.get()     );
	CHECKX(            0x00000000, Tx.Fifo.get()     );	// included
	CHECKX(            0x00000707, Tx.DmaConf.get()  );
	CHECKX(            0x00000020, Tx.Ch1Range.get() );
	CHECKX(            0x00000000, Tx.Ch1Data.get()  );
	CHECKX(            0x00000020, Tx.Ch2Range.get() );
	CHECKX(            0x00000000, Tx.Ch2Data.get()  );
	//
	CHECKX(            0xffffffff, Tx.Cntl.read()     );
	CHECKX(            0xffffffff, Tx.Stat.read()     );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.DmaConf.read()  );
	CHECKX(            0xffffffff, Tx.Ch1Range.read() );
	CHECKX(            0xffffffff, Tx.Ch1Data.read()  );
	CHECKX(            0xffffffff, Tx.Ch2Range.read() );
	CHECKX(            0xffffffff, Tx.Ch2Data.read()  );
	//
	CHECK(   1, Tx.Stat.get_FifoEmpty_1()       );
	CHECK(   7, Tx.DmaConf.get_DmaPanicLev_8()  );
	CHECK(   7, Tx.DmaConf.get_DmaReqLev_8()    );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52", "grab_regs(), not Fifo" );
    try {
	Tx.Cntl.write(     0x11111111 );
	Tx.Stat.write(     0x22222222 );
	Tx.Fifo.write(     0x33333333 );
	Tx.DmaConf.write(  0x44444444 );
	Tx.Ch1Range.write( 0x55555555 );
	Tx.Ch1Data.write(  0x66666666 );
	Tx.Ch2Range.write( 0x77777777 );
	Tx.Ch2Data.write(  0x88888888 );
	//
	Tx.Cntl.put(       0xaaaaaaaa );
	Tx.Stat.put(       0xbbbbbbbb );
	Tx.Fifo.put(       0xcccccccc );
	Tx.DmaConf.put(    0xdddddddd );
	Tx.Ch1Range.put(   0xeeeeeeee );
	Tx.Ch1Data.put(    0x99999999 );
	Tx.Ch2Range.put(   0xabbaabba );
	Tx.Ch2Data.put(    0xcaaccaac );
	Tx.grab_regs();
	CHECKX(            0x11111111, Tx.Cntl.get()     );
	CHECKX(            0x22222222, Tx.Stat.get()     );
	CHECKX(            0xcccccccc, Tx.Fifo.get()     );	// not change
	CHECKX(            0x44444444, Tx.DmaConf.get()  );
	CHECKX(            0x55555555, Tx.Ch1Range.get() );
	CHECKX(            0x66666666, Tx.Ch1Data.get()  );
	CHECKX(            0x77777777, Tx.Ch2Range.get() );
	CHECKX(            0x88888888, Tx.Ch2Data.get()  );
	//
	CHECKX(            0x11111111, Tx.Cntl.read()     );
	CHECKX(            0x22222222, Tx.Stat.read()     );
	CHECKX(            0x33333333, Tx.Fifo.read()     );
	CHECKX(            0x44444444, Tx.DmaConf.read()  );
	CHECKX(            0x55555555, Tx.Ch1Range.read() );
	CHECKX(            0x66666666, Tx.Ch1Data.read()  );
	CHECKX(            0x77777777, Tx.Ch2Range.read() );
	CHECKX(            0x88888888, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "53", "push_regs(), not Fifo" );
    try {
	Tx.Cntl.write(     0x11111111 );
	Tx.Stat.write(     0x22222222 );
	Tx.Fifo.write(     0x33333333 );
	Tx.DmaConf.write(  0x44444444 );
	Tx.Ch1Range.write( 0x55555555 );
	Tx.Ch1Data.write(  0x66666666 );
	Tx.Ch2Range.write( 0x77777777 );
	Tx.Ch2Data.write(  0x88888888 );
	//
	Tx.Cntl.put(       0xaaaaaaaa );
	Tx.Stat.put(       0xbbbbbbbb );
	Tx.Fifo.put(       0xcccccccc );
	Tx.DmaConf.put(    0xdddddddd );
	Tx.Ch1Range.put(   0xeeeeeeee );
	Tx.Ch1Data.put(    0x99999999 );
	Tx.Ch2Range.put(   0xabbaabba );
	Tx.Ch2Data.put(    0xcaaccaac );
	Tx.push_regs();
	CHECKX(            0xaaaaaaaa, Tx.Cntl.get()     );
	CHECKX(            0xbbbbbbbb, Tx.Stat.get()     );
	CHECKX(            0xcccccccc, Tx.Fifo.get()     );
	CHECKX(            0xdddddddd, Tx.DmaConf.get()  );
	CHECKX(            0xeeeeeeee, Tx.Ch1Range.get() );
	CHECKX(            0x99999999, Tx.Ch1Data.get()  );
	CHECKX(            0xabbaabba, Tx.Ch2Range.get() );
	CHECKX(            0xcaaccaac, Tx.Ch2Data.get()  );
	//
	CHECKX(            0xaaaaaaaa, Tx.Cntl.read()     );
	CHECKX(            0xbbbbbbbb, Tx.Stat.read()     );
	CHECKX(            0x33333333, Tx.Fifo.read()     );	// not change
	CHECKX(            0xdddddddd, Tx.DmaConf.read()  );
	CHECKX(            0xeeeeeeee, Tx.Ch1Range.read() );
	CHECKX(            0x99999999, Tx.Ch1Data.read()  );
	CHECKX(            0xabbaabba, Tx.Ch2Range.read() );
	CHECKX(            0xcaaccaac, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Field Accessors  get(), put()
//--------------------------------------------------------------------------
// Gray box - check field position and width.
// Bit pattern is correct from inherited rgRegister.

//#!! Incomplete!

  CASE( "60a", "clear hardware regs" );	// to avoid any wayward 1s
    try {
	Tx.Cntl.write(     0x00000000 );
	Tx.Stat.write(     0x00000000 );
	Tx.Fifo.write(     0x00000000 );
	Tx.DmaConf.write(  0x00000000 );
	Tx.Ch1Range.write( 0x00000000 );
	Tx.Ch1Data.write(  0x00000000 );
	Tx.Ch2Range.write( 0x00000000 );
	Tx.Ch2Data.write(  0x00000000 );
	CHECKX(            0x00000000, Tx.Cntl.read()     );
	CHECKX(            0x00000000, Tx.Stat.read()     );
	CHECKX(            0x00000000, Tx.Fifo.read()     );
	CHECKX(            0x00000000, Tx.DmaConf.read()  );
	CHECKX(            0x00000000, Tx.Ch1Range.read() );
	CHECKX(            0x00000000, Tx.Ch1Data.read()  );
	CHECKX(            0x00000000, Tx.Ch2Range.read() );
	CHECKX(            0x00000000, Tx.Ch2Data.read()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "60b", "clear object regs" );	// to avoid any wayward 1s
    try {
	Tx.Cntl.put(       0x00000000 );
	Tx.Stat.put(       0x00000000 );
	Tx.Fifo.put(       0x00000000 );
	Tx.DmaConf.put(    0x00000000 );
	Tx.Ch1Range.put(   0x00000000 );
	Tx.Ch1Data.put(    0x00000000 );
	Tx.Ch2Range.put(   0x00000000 );
	Tx.Ch2Data.put(    0x00000000 );
	Tx.grab_regs();
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	CHECKX(            0x00000000, Tx.Stat.get()     );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.DmaConf.get()  );
	CHECKX(            0x00000000, Tx.Ch1Range.get() );
	CHECKX(            0x00000000, Tx.Ch1Data.get()  );
	CHECKX(            0x00000000, Tx.Ch2Range.get() );
	CHECKX(            0x00000000, Tx.Ch2Data.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "61a", "Cntl.get_Ch2_MsEnable_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_MsEnable_1( 1 );
	CHECKX(            0x00008000, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_MsEnable_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "Cntl.put_Ch2_MsEnable_1() bad value" );
    try {
	Tx.Cntl.put_Ch2_MsEnable_1( 2 );
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
  CASE( "62a", "Cntl.get_Ch2_UseFifo_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_UseFifo_1(  1 );
	CHECKX(            0x00002000, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_UseFifo_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "62c", "Cntl.put_Ch2_UseFifo_1() bad value" );
    try {
	Tx.Cntl.put_Ch2_UseFifo_1( 2 );
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
  CASE( "63a", "Cntl.get_Ch2_Invert_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_Invert_1(   1 );
	CHECKX(            0x00001000, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_Invert_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "64a", "Cntl.get_Ch2_IdleBit_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_IdleBit_1(  1 );
	CHECKX(            0x00000800, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_IdleBit_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "65a", "Cntl.get_Ch2_Repeat_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_Repeat_1(   1 );
	CHECKX(            0x00000400, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_Repeat_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "66a", "Cntl.get_Ch2_SerMode_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_SerMode_1(  1 );
	CHECKX(            0x00000200, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_SerMode_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "67a", "Cntl.get_Ch2_Enable_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_Ch2_Enable_1(   1 );
	CHECKX(            0x00000100, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_Ch2_Enable_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "68a", "Cntl.get_ClearFifo_1()" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	Tx.Cntl.put_ClearFifo_1(    1 );
	CHECKX(            0x00000040, Tx.Cntl.get()     );
	CHECKX(                     1, Tx.Cntl.get_ClearFifo_1() );
	Tx.Cntl.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }



//--------------------------------------
  CASE( "80a", "Stat.get_Ch2_Active_1()" );
    try {
	CHECKX(            0x00000000, Tx.Stat.get()     );
	Tx.Stat.put_Ch2_Active_1(   1 );
	CHECKX(            0x00000400, Tx.Stat.get()     );
	CHECKX(                     1, Tx.Stat.get_Ch2_Active_1() );
	Tx.Stat.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "90a", "Stat.get_FifoFull_1()" );
    try {
	CHECKX(            0x00000000, Tx.Stat.get()     );
	Tx.Stat.put_FifoFull_1(     1 );
	CHECKX(            0x00000001, Tx.Stat.get()     );
	CHECKX(                     1, Tx.Stat.get_FifoFull_1() );
	Tx.Stat.put(       0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
  CASE( "94a", "DmaConf.get_DmaEnable_1()" );
    try {
	CHECKX(            0x00000000, Tx.DmaConf.get()  );
	Tx.DmaConf.put_DmaEnable_1( 1 );
	CHECKX(            0x80000000, Tx.DmaConf.get()  );
	CHECKX(                     1, Tx.DmaConf.get_DmaEnable_1() );
	Tx.DmaConf.put(    0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "95a", "DmaConf.get_DmaPanicLev_8()" );
    try {
	CHECKX(            0x00000000, Tx.DmaConf.get()  );
	Tx.DmaConf.put_DmaPanicLev_8( 0xff );
	CHECKX(            0x0000ff00, Tx.DmaConf.get()  );
	CHECKX(                  0xff, Tx.DmaConf.get_DmaPanicLev_8() );
	Tx.DmaConf.put(    0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "96a", "DmaConf.get_DmaReqLev_8()" );
    try {
	CHECKX(            0x00000000, Tx.DmaConf.get()  );
	Tx.DmaConf.put_DmaReqLev_8( 0xff );
	CHECKX(            0x000000ff, Tx.DmaConf.get()  );
	CHECKX(                  0xff, Tx.DmaConf.get_DmaReqLev_8() );
	Tx.DmaConf.put(    0x00000000 );	// restore
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "98", "verify object regs all 0" );
    try {
	CHECKX(            0x00000000, Tx.Cntl.get()     );
	CHECKX(            0x00000000, Tx.Stat.get()     );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.DmaConf.get()  );
	CHECKX(            0x00000000, Tx.Ch1Range.get() );
	CHECKX(            0x00000000, Tx.Ch1Data.get()  );
	CHECKX(            0x00000000, Tx.Ch2Range.get() );
	CHECKX(            0x00000000, Tx.Ch2Data.get()  );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

