// 2019-10-21  William A. Hudson
//
// Testing:  rgSpi0  SPI0 Master class.  Include RPi4.
//    10-19  Constructor, get_bcm_address()
//    20-29  Address of registers  addr()
//    30-39  Direct register access  read(), write()
//    50-59  Object State registers  init_put_reset(), grab_regs(), push_regs()
//    60-98  Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgSpi0.h"

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

rgSpi0			Tx   ( &Bx );		// test object

rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2837 );  // RPi3

//--------------------------------------------------------------------------
//## Constructor, get_bcm_address()
//--------------------------------------------------------------------------

  CASE( "10a", "constructor spi0 default" );
    try {
	rgSpi0		tx  ( &Bx );
	CHECKX( 0x7e204000, tx.get_bcm_address() );
	CHECK(  0,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "constructor spi0" );
    try {
	rgSpi0		tx  ( &Bx, 0 );
	CHECKX( 0x7e204000, tx.get_bcm_address() );
	CHECK(  0,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "constructor bad spi number" );
    try {
	rgSpi0		tx  ( &Bx, 3 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0:  constructor invalid spi number for BCM2837:  3",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "rgSpi0 domain_error RPi5" );
    try {
	rgRpiRev::simulate_SocEnum( rgRpiRev::soc_BCM2712 );
	rgSpi0		tx  ( &Bx );
	FAIL( "no throw" );
    }
    catch ( std::domain_error& e ) {
	CHECK( "rgSpi0:  require RPi4 (soc_BCM2711) or earlier", e.what() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
rgRpiRev::Global.SocEnum.override( rgRpiRev::soc_BCM2711 );	// RPi4

  CASE( "11a", "RPi4 constructor spi0" );
    try {
	rgSpi0		tx  ( &Bx, 0 );
	CHECKX( 0x7e204000, tx.get_bcm_address() );
	CHECK(  0,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11b", "RPi4 constructor NOT spi1" );
    try {
	rgSpi0		tx  ( &Bx, 1 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0:  constructor requires spi number {0,3,4,5,6}:  1",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11c", "RPi4 constructor NOT spi2" );
    try {
	rgSpi0		tx  ( &Bx, 2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgSpi0:  constructor requires spi number {0,3,4,5,6}:  2",
	    e.what()
	);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11d", "RPi4 constructor spi3" );
    try {
	rgSpi0		tx  ( &Bx, 3 );
	CHECKX( 0x7e204600, tx.get_bcm_address() );
	CHECK(  3,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11e", "RPi4 constructor spi4" );
    try {
	rgSpi0		tx  ( &Bx, 4 );
	CHECKX( 0x7e204800, tx.get_bcm_address() );
	CHECK(  4,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11f", "RPi4 constructor spi5" );
    try {
	rgSpi0		tx  ( &Bx, 5 );
	CHECKX( 0x7e204a00, tx.get_bcm_address() );
	CHECK(  5,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "11g", "RPi4 constructor spi6" );
    try {
	rgSpi0		tx  ( &Bx, 6 );
	CHECKX( 0x7e204c00, tx.get_bcm_address() );
	CHECK(  6,          tx.get_unit_num() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Address of registers  addr()
//--------------------------------------------------------------------------

  CASE( "21", "CntlStat.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.CntlStat.addr();
	CHECKX( 0x00000000, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "22", "Fifo.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Fifo.addr();
	CHECKX( 0x00000004, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "23", "ClkDiv.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.ClkDiv.addr();
	CHECKX( 0x00000008, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "24", "DmaLen.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.DmaLen.addr();
	CHECKX( 0x0000000c, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "25", "Lossi.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.Lossi.addr();
	CHECKX( 0x00000010, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "26", "DmaReq.addr()" );
    try {
	volatile uint32_t*	vp;
	vp = Tx.DmaReq.addr();
	CHECKX( 0x00000014, (vp - Tx.get_base_addr())*4 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Sample final register on extended spi numbers.

  CASE( "27", "RPi4 spi*.DmaReq.addr()" );
    try {
	rgSpi0		tx3  ( &Bx, 3 );
	rgSpi0		tx4  ( &Bx, 4 );
	rgSpi0		tx5  ( &Bx, 5 );
	rgSpi0		tx6  ( &Bx, 6 );
	CHECKX( 0x0014, (tx3.DmaReq.addr() - tx3.get_base_addr())*4 );
	CHECKX( 0x0014, (tx4.DmaReq.addr() - tx4.get_base_addr())*4 );
	CHECKX( 0x0014, (tx5.DmaReq.addr() - tx5.get_base_addr())*4 );
	CHECKX( 0x0014, (tx6.DmaReq.addr() - tx6.get_base_addr())*4 );
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
	Tx.CntlStat.write( 0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.ClkDiv.write(   0xffffffff );
	Tx.DmaLen.write(   0xffffffff );
	Tx.Lossi.write(    0xffffffff );
	Tx.DmaReq.write(   0xffffffff );
	CHECKX(            0xffffffff, Tx.CntlStat.read() );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.read()   );
	CHECKX(            0xffffffff, Tx.DmaLen.read()   );
	CHECKX(            0xffffffff, Tx.Lossi.read()    );
	CHECKX(            0xffffffff, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31", "CntlStat.read()" );
    try {
	Tx.CntlStat.write( 0x33cccc33 );
	CHECKX(            0x33cccc33, Tx.CntlStat.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32", "Fifo.read()" );
    try {
	Tx.Fifo.write(     0x11eeee11 );
	CHECKX(            0x11eeee11, Tx.Fifo.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "33", "ClkDiv.read()" );
    try {
	Tx.ClkDiv.write(   0x22dddd22 );
	CHECKX(            0x22dddd22, Tx.ClkDiv.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "34", "DmaLen.read()" );
    try {
	Tx.DmaLen.write(   0x44bbbb44 );
	CHECKX(            0x44bbbb44, Tx.DmaLen.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "35", "Lossi.read()" );
    try {
	Tx.Lossi.write(    0x55aaaa55 );
	CHECKX(            0x55aaaa55, Tx.Lossi.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "36", "DmaReq.read()" );
    try {
	Tx.DmaReq.write(   0x66999966 );
	CHECKX(            0x66999966, Tx.DmaReq.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "39", "check final register pattern" );
    try {
	CHECKX(            0x33cccc33, Tx.CntlStat.read() );
	CHECKX(            0x11eeee11, Tx.Fifo.read()     );
	CHECKX(            0x22dddd22, Tx.ClkDiv.read()   );
	CHECKX(            0x44bbbb44, Tx.DmaLen.read()   );
	CHECKX(            0x55aaaa55, Tx.Lossi.read()    );
	CHECKX(            0x66999966, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object State  init_put_reset(), grab_regs(), push_regs()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "50a", "init_put_reset() 0xffffffff" );
    try {
	Tx.CntlStat.write( 0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.ClkDiv.write(   0xffffffff );
	Tx.DmaLen.write(   0xffffffff );
	Tx.Lossi.write(    0xffffffff );
	Tx.DmaReq.write(   0xffffffff );
	//
	Tx.CntlStat.put(   0xffffffff );
	Tx.Fifo.put(       0xffffffff );
	Tx.ClkDiv.put(     0xffffffff );
	Tx.DmaLen.put(     0xffffffff );
	Tx.Lossi.put(      0xffffffff );
	Tx.DmaReq.put(     0xffffffff );
	Tx.init_put_reset();
	CHECKX(            0xffffffff, Tx.CntlStat.read() );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.read()   );
	CHECKX(            0xffffffff, Tx.DmaLen.read()   );
	CHECKX(            0xffffffff, Tx.Lossi.read()    );
	CHECKX(            0xffffffff, Tx.DmaReq.read()   );
	//
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "50b", "init_put_reset() 0x00000000" );
    try {
	Tx.CntlStat.write( 0x00000000 );
	Tx.Fifo.write(     0x00000000 );
	Tx.ClkDiv.write(   0x00000000 );
	Tx.DmaLen.write(   0x00000000 );
	Tx.Lossi.write(    0x00000000 );
	Tx.DmaReq.write(   0x00000000 );
	//
	Tx.CntlStat.put(   0x00000000 );
	Tx.Fifo.put(       0x00000000 );
	Tx.ClkDiv.put(     0x00000000 );
	Tx.DmaLen.put(     0x00000000 );
	Tx.Lossi.put(      0x00000000 );
	Tx.DmaReq.put(     0x00000000 );
	Tx.init_put_reset();
	CHECKX(            0x00000000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.Fifo.read()     );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000000, Tx.Lossi.read()    );
	CHECKX(            0x00000000, Tx.DmaReq.read()   );
	//
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.Fifo.get()     );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "grab_regs(), not Fifo" );
    try {
	Tx.CntlStat.write( 0x11111111 );
	Tx.Fifo.write(     0x22222222 );
	Tx.ClkDiv.write(   0x33333333 );
	Tx.DmaLen.write(   0x44444444 );
	Tx.Lossi.write(    0x55555555 );
	Tx.DmaReq.write(   0x66666666 );
	//
	Tx.CntlStat.put(   0xeeeeeeee );
	Tx.Fifo.put(       0xdddddddd );
	Tx.ClkDiv.put(     0xcccccccc );
	Tx.DmaLen.put(     0xbbbbbbbb );
	Tx.Lossi.put(      0xaaaaaaaa );
	Tx.DmaReq.put(     0x99999999 );
	Tx.grab_regs();
	CHECKX(            0x11111111, Tx.CntlStat.read() );
	CHECKX(            0x22222222, Tx.Fifo.read()     );
	CHECKX(            0x33333333, Tx.ClkDiv.read()   );
	CHECKX(            0x44444444, Tx.DmaLen.read()   );
	CHECKX(            0x55555555, Tx.Lossi.read()    );
	CHECKX(            0x66666666, Tx.DmaReq.read()   );
	//
	CHECKX(            0x11111111, Tx.CntlStat.get() );
	CHECKX(            0xdddddddd, Tx.Fifo.get()     );	// not Fifo
	CHECKX(            0x33333333, Tx.ClkDiv.get()   );
	CHECKX(            0x44444444, Tx.DmaLen.get()   );
	CHECKX(            0x55555555, Tx.Lossi.get()    );
	CHECKX(            0x66666666, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "53a", "push_regs(), not Fifo" );
    try {
	Tx.CntlStat.write( 0x11111111 );
	Tx.Fifo.write(     0x22222222 );
	Tx.ClkDiv.write(   0x33333333 );
	Tx.DmaLen.write(   0x44444444 );
	Tx.Lossi.write(    0x55555555 );
	Tx.DmaReq.write(   0x66666666 );
	//
	Tx.CntlStat.put(   0xeeeeeeee );
	Tx.Fifo.put(       0xdddddddd );
	Tx.ClkDiv.put(     0xcccccccc );
	Tx.DmaLen.put(     0xbbbbbbbb );
	Tx.Lossi.put(      0xaaaaaaaa );
	Tx.DmaReq.put(     0x99999999 );
	Tx.push_regs();
	CHECKX(            0xeeeeeeee, Tx.CntlStat.read() );
	CHECKX(            0x22222222, Tx.Fifo.read()     );	// not Fifo
	CHECKX(            0xcccccccc, Tx.ClkDiv.read()   );
	CHECKX(            0xbbbbbbbb, Tx.DmaLen.read()   );
	CHECKX(            0xaaaaaaaa, Tx.Lossi.read()    );
	CHECKX(            0x99999999, Tx.DmaReq.read()   );
	//
	CHECKX(            0xeeeeeeee, Tx.CntlStat.get() );
	CHECKX(            0xdddddddd, Tx.Fifo.get()     );
	CHECKX(            0xcccccccc, Tx.ClkDiv.get()   );
	CHECKX(            0xbbbbbbbb, Tx.DmaLen.get()   );
	CHECKX(            0xaaaaaaaa, Tx.Lossi.get()    );
	CHECKX(            0x99999999, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------------------------------------------
//## Object Field Accessors  get_(), put_()
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "60", "condition hardware/object registers" );	// avoid any wayward 0s
    try {
	Tx.CntlStat.write( 0xffffffff );
	Tx.Fifo.write(     0xffffffff );
	Tx.ClkDiv.write(   0xffffffff );
	Tx.DmaLen.write(   0xffffffff );
	Tx.Lossi.write(    0xffffffff );
	Tx.DmaReq.write(   0xffffffff );
	//
	Tx.CntlStat.put(   0xffffffff );
	Tx.Fifo.put(       0xffffffff );
	Tx.ClkDiv.put(     0xffffffff );
	Tx.DmaLen.put(     0xffffffff );
	Tx.Lossi.put(      0xffffffff );
	Tx.DmaReq.put(     0xffffffff );
	CHECKX(            0xffffffff, Tx.CntlStat.read() );
	CHECKX(            0xffffffff, Tx.Fifo.read()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.read()   );
	CHECKX(            0xffffffff, Tx.DmaLen.read()   );
	CHECKX(            0xffffffff, Tx.Lossi.read()    );
	CHECKX(            0xffffffff, Tx.DmaReq.read()   );
	//
	CHECKX(            0xffffffff, Tx.CntlStat.get() );
	CHECKX(            0xffffffff, Tx.Fifo.get()     );
	CHECKX(            0xffffffff, Tx.ClkDiv.get()   );
	CHECKX(            0xffffffff, Tx.DmaLen.get()   );
	CHECKX(            0xffffffff, Tx.Lossi.get()    );
	CHECKX(            0xffffffff, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Example heavy-weight bit-field test.

  CASE( "61a", "CntlStat.get_LossiWord_1()" );
    try {
	Tx.CntlStat.put(     0x00000000 );
	CHECKX(              0x00000000, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiWord_1(  1 );
	CHECKX(              0x02000000, Tx.CntlStat.get() );
	CHECKX(                       1, Tx.CntlStat.get_LossiWord_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61b", "CntlStat.get_LossiWord_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiWord_1(  0 );
	CHECKX(              0xfdffffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_LossiWord_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "61c", "CntlStat.put_LossiWord_1() bad value" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiWord_1(  2 );
	FAIL( "no throw" );
    }
    catch ( range_error& e ) {
	CHECK( "rgRegister::put_field():  value exceeds 0x1:  0x2",
	    e.what()
	);
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
// Light-weight bit-field test.

  CASE( "62b", "CntlStat.get_LossiDmaEn_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiDmaEn_1( 0 );
	CHECKX(              0xfeffffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_LossiDmaEn_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "63b", "CntlStat.get_CsPolarity_3()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_3( 0 );
	CHECKX(              0xff1fffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_CsPolarity_3() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "64b", "CntlStat.get_RxFullStop_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_RxFullStop_1( 0 );
	CHECKX(              0xffefffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_RxFullStop_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "65b", "CntlStat.get_RxHalf_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_RxHalf_1(     0 );
	CHECKX(              0xfff7ffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_RxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "66b", "CntlStat.get_TxHasSpace_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_TxHasSpace_1( 0 );
	CHECKX(              0xfffbffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_TxHasSpace_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "67b", "CntlStat.get_RxHasData_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_RxHasData_1(  0 );
	CHECKX(              0xfffdffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_RxHasData_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "68b", "CntlStat.get_TxEmpty_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_TxEmpty_1(    0 );
	CHECKX(              0xfffeffff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_TxEmpty_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "69b", "CntlStat.get_LossiEnable_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_LossiEnable_1( 0 );
	CHECKX(              0xffffdfff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_LossiEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "70b", "CntlStat.get_ReadEnable_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_ReadEnable_1( 0 );
	CHECKX(              0xffffefff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_ReadEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "71b", "CntlStat.get_DmaEndCs_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_DmaEndCs_1(   0 );
	CHECKX(              0xfffff7ff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_DmaEndCs_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "72b", "CntlStat.get_IrqRxHalf_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_IrqRxHalf_1(  0 );
	CHECKX(              0xfffffbff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_IrqRxHalf_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "73b", "CntlStat.get_IrqTxEmpty_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_IrqTxEmpty_1( 0 );
	CHECKX(              0xfffffdff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_IrqTxEmpty_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "74b", "CntlStat.get_DmaEnable_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_DmaEnable_1(  0 );
	CHECKX(              0xfffffeff, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_DmaEnable_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "75b", "CntlStat.get_RunActive_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(  0 );
	CHECKX(              0xffffff7f, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_RunActive_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "76b", "CntlStat.get_CsPolarity_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_1( 0 );
	CHECKX(              0xffffffbf, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_CsPolarity_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "77b", "CntlStat.get_ClearRxTxFifo_2()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_ClearRxTxFifo_2( 0 );
	CHECKX(              0xffffffcf, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_ClearRxTxFifo_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "78b", "CntlStat.get_ClockPolarity_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_ClockPolarity_1( 0 );
	CHECKX(              0xfffffff7, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_ClockPolarity_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "79b", "CntlStat.get_ClockPhase_1()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_ClockPhase_1( 0 );
	CHECKX(              0xfffffffb, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_ClockPhase_1() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "80b", "CntlStat.get_ChipSelectN_2()" );
    try {
	Tx.CntlStat.put(     0xffffffff );
	CHECKX(              0xffffffff, Tx.CntlStat.get() );
	Tx.CntlStat.put_ChipSelectN_2( 0 );
	CHECKX(              0xfffffffc, Tx.CntlStat.get() );
	CHECKX(                       0, Tx.CntlStat.get_ChipSelectN_2() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "81b", "ClkDiv.get_ClockDiv_16()" );
    try {
	Tx.ClkDiv.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.ClkDiv.get() );
	Tx.ClkDiv.put_ClockDiv_16( 0x0000 );
	CHECKX(              0xffff0000, Tx.ClkDiv.get() );
	CHECKX(                       0, Tx.ClkDiv.get_ClockDiv_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "82b", "DmaLen.get_DmaDataLen_16()" );
    try {
	Tx.DmaLen.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.DmaLen.get() );
	Tx.DmaLen.put_DmaDataLen_16( 0x0000 );
	CHECKX(              0xffff0000, Tx.DmaLen.get() );
	CHECKX(                       0, Tx.DmaLen.get_DmaDataLen_16() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "83b", "Lossi.get_LossiHoldDly_4()" );
    try {
	Tx.Lossi.put(        0xffffffff );
	CHECKX(              0xffffffff, Tx.Lossi.get() );
	Tx.Lossi.put_LossiHoldDly_4( 0x0 );
	CHECKX(              0xfffffff0, Tx.Lossi.get() );
	CHECKX(                       0, Tx.Lossi.get_LossiHoldDly_4() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "84b", "DmaReq.get_DmaRxPanicLev_8()" );
    try {
	Tx.DmaReq.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaRxPanicLev_8( 0x00 );
	CHECKX(              0x00ffffff, Tx.DmaReq.get() );
	CHECKX(                       0, Tx.DmaReq.get_DmaRxPanicLev_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "85b", "DmaReq.get_DmaRxReqLev_8()" );
    try {
	Tx.DmaReq.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaRxReqLev_8(  0x00 );
	CHECKX(              0xff00ffff, Tx.DmaReq.get() );
	CHECKX(                       0, Tx.DmaReq.get_DmaRxReqLev_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "86b", "DmaReq.get_DmaTxPanicLev_8()" );
    try {
	Tx.DmaReq.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaTxPanicLev_8( 0x00 );
	CHECKX(              0xffff00ff, Tx.DmaReq.get() );
	CHECKX(                       0, Tx.DmaReq.get_DmaTxPanicLev_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "87b", "DmaReq.get_DmaTxReqLev_8()" );
    try {
	Tx.DmaReq.put(       0xffffffff );
	CHECKX(              0xffffffff, Tx.DmaReq.get() );
	Tx.DmaReq.put_DmaTxReqLev_8(  0x00 );
	CHECKX(              0xffffff00, Tx.DmaReq.get() );
	CHECKX(                       0, Tx.DmaReq.get_DmaTxReqLev_8() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "98", "verify object put() results" );
    try {
	CHECKX(            0xfffffffc, Tx.CntlStat.get() );
	CHECKX(            0xffffffff, Tx.Fifo.get()     );
	CHECKX(            0xffff0000, Tx.ClkDiv.get()   );
	CHECKX(            0xffff0000, Tx.DmaLen.get()   );
	CHECKX(            0xfffffff0, Tx.Lossi.get()    );
	CHECKX(            0xffffff00, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

