// 2019-11-02  William A. Hudson
//
// Testing:  Live rgSpi0  SPI0 function with GPIO pins on RPi
//    10-19  Explore Idle state pin levels (RunActive_1=0)
//    20-29  Explore RunActive_1  CS (Chip Select) pins
//    30-39  .
//    50-59  .
//    60-98  .

// Provide external configuration:
//    % rgpio fsel --mode=Alt0  11 10 9 8 7
//    Gpio[11:7] == {SCLK, MOSI, MISO, CE0_n, CE1_n}
//    All other GPIO pins unconnected.  Loopback is optional.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgIoPins.h"
#include "rgSpi0.h"

using namespace std;

#define SPI0_G		0x00000f80	// GPIO[11:7] SPI0 mask

//--------------------------------------------------------------------------
//## Helper function
//--------------------------------------------------------------------------

/*
* Read and format GPIO[11:7] (Spi0 pins) into a character string.
* call:
*    CHECK( "0 00 00", cstr_spi0_gpio( &Gx ) );
*/
const char*
cstr_spi0_gpio( rgIoPins  *gx )
{
    static char		cr[12];
    uint32_t		reg;

    reg = gx->PinLevel_w0.read() & SPI0_G;

    cr[0] = (reg & 0x0800) ? '1' : '0';		// SCLK
    cr[1] = ' ';
    cr[2] = (reg & 0x0400) ? '1' : '0';		// MOSI
    cr[3] = (reg & 0x0200) ? '1' : '0';		// MISO
    cr[4] = ' ';
    cr[5] = (reg & 0x0100) ? '1' : '0';		// CE0_n
    cr[6] = (reg & 0x0080) ? '1' : '0';		// CE1_n
    cr[7] = '\0';

    return  cr;
}


//--------------------------------------------------------------------------

int main()
{

//--------------------------------------------------------------------------
//## Shared object
//--------------------------------------------------------------------------

rgAddrMap		Bx;

  CASE( "00", "Address map object" );
    try {
	Bx.open_dev_mem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgSpi0			Tx   ( &Bx );		// test object
rgIoPins		Gx   ( &Bx );		// GPIO pins


//--------------------------------------------------------------------------
//## Explore Idle state pin levels (RunActive_1=0)
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "10a", "init_put_reset(), clear Fifo" );
    try {
	Tx.init_put_reset();
	Tx.CntlStat.put_ClearRxTxFifo_2( 0x3 );	// one-shot, reads as 0
	CHECKX(            0x00041030, Tx.CntlStat.get() );
	CHECKX(            0x00000000, Tx.ClkDiv.get()   );
	CHECKX(            0x00000000, Tx.DmaLen.get()   );
	CHECKX(            0x00000001, Tx.Lossi.get()    );
	CHECKX(            0x30201020, Tx.DmaReq.get()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10b", "push_regs() reset state" );
    try {
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, Tx.ClkDiv.read()   );
	CHECKX(            0x00000000, Tx.DmaLen.read()   );
	CHECKX(            0x00000001, Tx.Lossi.read()    );
	CHECKX(            0x30201020, Tx.DmaReq.read()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10c", "CntlStat grab_regs()" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      1, Tx.CntlStat.get_ReadEnable_1()    );
	CHECK(                      0, Tx.CntlStat.get_RunActive_1()     );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "10d", "Spi0 pin levels" );
    try {
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
	//                   ^    ^^   idle state with reset config
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "11", "set ClockPolarity_1=1  CntlStat[3]" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_ClockPolarity_1( 1 );
	CHECKX(            0x00041008, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00041008, Tx.CntlStat.read() );
	CHECKX(            0x00000980, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "1 00 11", cstr_spi0_gpio( &Gx ) );
	//                   ^         SCLK idle state is 1
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "12", "set CsPolarity_1=1  CntlStat[6]  no effect" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_1(    1 );
	CHECKX(            0x00041040, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00041040, Tx.CntlStat.read() );
	CHECKX(            0x00000180, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
	//                        ^^   no change in CE0_n or CE1_n
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "13", "set CsPolarity_3=1  CntlStat[23:21]" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_3(    1 );
	CHECKX(            0x00241000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00241000, Tx.CntlStat.read() );
	CHECKX(            0x00000080, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 01", cstr_spi0_gpio( &Gx ) );
	//                        ^    CE0_n idle low
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "14", "set CsPolarity_3=2  CntlStat[23:21]" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_3(    2 );
	CHECKX(            0x00441000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00441000, Tx.CntlStat.read() );
	CHECKX(            0x00000100, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 10", cstr_spi0_gpio( &Gx ) );
	//                         ^   CE1_n idle low
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "15", "set CsPolarity_3=4  CntlStat[23:21]" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_3(    4 );
	CHECKX(            0x00841000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00841000, Tx.CntlStat.read() );
	CHECKX(            0x00000180, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
	//                        ^^   there is no CE2_n, no effect
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "16", "set CsPolarity_3=3  CntlStat[23:21]" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_CsPolarity_3(    3 );
	CHECKX(            0x00641000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00641000, Tx.CntlStat.read() );
	CHECKX(            0x00000000, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 00", cstr_spi0_gpio( &Gx ) );
	//                        ^^   both CE0_n, CE1_n idle low
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
//## Explore RunActive_1  CS (Chip Select) pins
//--------------------------------------------------------------------------

//--------------------------------------
  CASE( "20a", "set RunActive_1=0  CntlStat[7] baseline" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(     0 );
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00041000, Tx.CntlStat.read() );
	CHECKX(            0x00000180, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20b", "set RunActive_1=1  see CE0_n active" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(     1 );
	CHECKX(            0x00041080, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00051080, Tx.CntlStat.read() );
	CHECKX(            0x00000080, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 01", cstr_spi0_gpio( &Gx ) );
	//                        ^    CE0_n is active
	Tx.grab_regs();
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      1, Tx.CntlStat.get_ReadEnable_1()    );  // reset
	CHECK(                      0, Tx.CntlStat.get_ChipSelectN_2()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "20c", "put back RunActive_1=0  see CE0_n return to idle" );
    try {
	Tx.grab_regs();
	CHECKX(            0x00051080, Tx.CntlStat.get() );
	Tx.CntlStat.put_RunActive_1(     0 );
	Tx.push_regs();
	Tx.grab_regs();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	CHECKX(            0x00000180, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
	//                        ^    CE0_n is idle
	Tx.grab_regs();
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      0, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      1, Tx.CntlStat.get_ReadEnable_1()    );
	CHECK(                      0, Tx.CntlStat.get_ChipSelectN_2()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "21a", "set ChipSelectN_2=1" );
    try {
	Tx.init_put_reset();
	CHECKX(            0x00041000, Tx.CntlStat.get() );
	Tx.CntlStat.put_ChipSelectN_2(   1 );
	CHECKX(            0x00041001, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00041001, Tx.CntlStat.read() );
	CHECKX(            0x00000180, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 11", cstr_spi0_gpio( &Gx ) );
	//                        ^^   both CE0_n, CE1_n idle
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "21b", "set RunActive_1=1  see CE1_n active" );
    try {
	Tx.CntlStat.put_RunActive_1(     1 );
	CHECKX(            0x00041081, Tx.CntlStat.get() );
	Tx.push_regs();
	CHECKX(            0x00051081, Tx.CntlStat.read() );
	CHECKX(            0x00000100, (Gx.PinLevel_w0.read() & SPI0_G) );
	CHECK(              "0 00 10", cstr_spi0_gpio( &Gx ) );
	//                         ^   CE1_n is active
	Tx.grab_regs();
	CHECK(                      0, Tx.CntlStat.get_RxFullStop_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHalf_1()        );
	CHECK(                      1, Tx.CntlStat.get_TxHasSpace_1()    );
	CHECK(                      0, Tx.CntlStat.get_RxHasData_1()     );
	CHECK(                      1, Tx.CntlStat.get_TxEmpty_1()       );
	CHECK(                      1, Tx.CntlStat.get_ReadEnable_1()    );  // reset
	CHECK(                      1, Tx.CntlStat.get_ChipSelectN_2()   );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }


//--------------------------------------------------------------------------
  CASE( "99", "Done" );
}

