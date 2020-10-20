// 2020-09-18  William A. Hudson
//
// Testing:  Live rgPudPin IO Pin Pull Up/Down programming sequence.
//    10-19  Simply set Pull to Up and Down
//    20-29  Not a Transparent latch
//    30-39  Is a Rising Edge latch
//    40-49  Toggle Pull direction
//    50-59  Gpio[27:0] program_pud_bit(), program_pud_w0()
//    60-89  .
//
// Provide external configuration:
//    % rgpio fsel --mode=In  27 26 25 24 23 22 21 20 19 18 17 .. 3 2 1 0
//    % rgpio fsel --mode=In  9
//      All pins should be unconnected.
//      Observe Gpio[9] on an oscilloscope.
//
// All tests have assertions on IO pin level, and all should pass.
// Note:  System is also programming pull pins, and I have seen a clash.
//--------------------------------------------------------------------------

#include <iostream>	// std::cerr
#include <stdexcept>	// std::stdexcept

#include "utLib1.h"		// unit test library

#include "rgAddrMap.h"
#include "rgIoPins.h"
#include "rgPudPin.h"

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
	Bx.open_dev_gpiomem();
	PASS( "Common object" );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

rgIoPins		Px   ( &Bx );
rgPudPin		Tx   ( &Bx );	// test object

//--------------------------------------------------------------------------
//## Simply set Pull to Up and Down
//--------------------------------------------------------------------------

  CASE( "10", "Set Gpio[9] as pull-up" );
    try {
	Tx.PudProgClk_w0.write(   0x00000000 );
	Tx.PudProgClk_w1.write(   0x00000000 );
	Tx.PudProgMode.write(     0x00000002 );	// 2= up
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000200, Px.PinLevel_w0.read() & 0x0200 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Does pull-up.

  CASE( "11", "Set Gpio[9] as pull-down" );
    try {
	Tx.PudProgClk_w0.write(   0x00000000 );
	Tx.PudProgClk_w1.write(   0x00000000 );
	Tx.PudProgMode.write(     0x00000001 );	// 1= down
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Does pull-down.

//--------------------------------------------------------------------------
//## Not a Transparent latch
//--------------------------------------------------------------------------
// Experiment to see if the pull storage node is a transparent latch.
// Essence is to enable the Clk, then toggle the Mode between Up and Down.
// Observe pin on oscilloscope.

if ( 1 ) {
  CASE( "21", "Toggle Mode up/down on Gpio[9] with Clk high" );
    try {
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	//
	Tx.PudProgMode.write(     0x00000002 );	// 2= up
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgMode.write(     0x00000001 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgMode.write(     0x00000002 );	// 2= up
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgMode.write(     0x00000001 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgMode.write(     0x00000002 );	// 2= up
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgMode.write(     0x00000001 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	//
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Pull resistor does not toggle, even with long wait.
    // Thus is not a transparent latch.
}

//--------------------------------------------------------------------------
//## Is a Rising Edge latch
//--------------------------------------------------------------------------

if ( 1 ) {
//--------------------------------------
  CASE( "31a", "Setup Gpio[9] as pull-down" );	// and already down
    try {
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200 );
	Tx.PudProgClk_w0.write(   0x00000000 );
	Tx.PudProgClk_w1.write(   0x00000000 );
	Tx.PudProgMode.write(     0x00000001 );	// 1= down
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "31b", "Clk falling edge on Gpio[9]" );
    try {
	// setup clk high, pull dir already Down
	Tx.PudProgMode.write(     0x00000001 );	// 1= down
	Tx.PudProgClk_w0.write(   0x00000200 );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
	// clk falling edge with new mode
	Tx.PudProgMode.write(     0x00000002 );
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	// see pin while clk is low
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200);
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Clk fall makes no change.

//--------------------------------------
  CASE( "32a", "Setup Gpio[9] as pull-down" );	// and already down
    try {
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200 );
	Tx.PudProgClk_w0.write(   0x00000000 );
	Tx.PudProgClk_w1.write(   0x00000000 );
	Tx.PudProgMode.write(     0x00000001 );	// 1= down
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x0200 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "32b", "Clk rising edge on Gpio[9]" );
    try {
	// setup clk already low, pull dir already Down
	Tx.PudProgMode.write(     0x00000001 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	// clk rising edge with new mode
	Tx.PudProgMode.write(     0x00000002 );
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	// see pin while clk is high
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000200, Px.PinLevel_w0.read() & 0x0200);
	// restore clk
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Clk rise is the active edge.
}

//--------------------------------------------------------------------------
//## Toggle Pull direction
//--------------------------------------------------------------------------

if ( 1 ) {
  CASE( "40", "Toggle Pull direction on Gpio[9]" );
    try {
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	for ( int i=4;  i>0;  i-- ) {
	    Tx.PudProgMode.write(     0x00000001 );
//	    CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	    Tx.PudProgClk_w0.write(   0x00000200 );
//	    CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	    Tx.PudProgClk_w0.write(   0x00000000 );
//	    CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	    for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	    //
	    Tx.PudProgMode.write(     0x00000002 );
//	    CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	    Tx.PudProgClk_w0.write(   0x00000200 );
//	    CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	    Tx.PudProgClk_w0.write(   0x00000000 );
//	    CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	    for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	}
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000200, Px.PinLevel_w0.read() & 0x0200);
	// restore pull-down
	Tx.PudProgMode.write(     0x00000001 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	Tx.PudProgClk_w0.write(   0x00000200 );
	CHECKX(                   0x00000200, Tx.PudProgClk_w0.read() );
	Tx.PudProgClk_w0.write(   0x00000000 );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Toggle quickly, wait for RC charge of IO pin.
    // Pin voltage has about 3 us rise/fall times, to ~90% (oscilloscope).
    // No delay is needed to program the pull resistor.
}

//--------------------------------------------------------------------------
//## Gpio[27:0] program_pud_bit(), program_pud_w0()
//--------------------------------------------------------------------------

  CASE( "51a", "pull-up Gpio[12]" );
    try {
	Tx.program_pud_bit( rgPudPin::pd_Up, 12 );
	CHECKX(                   0x00000002, Tx.PudProgMode.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00001000, Px.PinLevel_w0.read() & 0x1000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "51b", "pull-down Gpio[12]" );
    try {
	Tx.program_pud_bit( rgPudPin::pd_Down, 12 );
	CHECKX(                   0x00000001, Tx.PudProgMode.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(                   0x00000000, Tx.PudProgClk_w1.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(                   0x00000000, Px.PinLevel_w0.read() & 0x1000 );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "52a", "Gpio[27:0] as pull-up" );
    try {
	Tx.program_pud_w0( rgPudPin::pd_Up, 0x0fffffff );
	CHECKX(             0x00000002, Tx.PudProgMode.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w1.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(             0x0fffffff, Px.PinLevel_w0.read() & 0x0fffffff );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

  CASE( "52b", "Gpio[27:0] as pull-down" );
    try {
	Tx.program_pud_w0( rgPudPin::pd_Down, 0x0fffffff );
	CHECKX(             0x00000001, Tx.PudProgMode.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w1.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(             0x0000000c, Px.PinLevel_w0.read() & 0x0fffffff );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }
    // Note:  Gpio[3:2] have 1.8 kohm pullups

//--------------------------------------
  CASE( "59", "Restore default Gpio[27:0] pull" );
    try {
	Tx.program_pud_w0( rgPudPin::pd_Down, 0x0ffffe00 );
	Tx.program_pud_w0( rgPudPin::pd_Up,   0x000001ff );
	CHECKX(             0x00000002, Tx.PudProgMode.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w0.read() );
	CHECKX(             0x00000000, Tx.PudProgClk_w1.read() );
	for ( volatile int i=500;  i>0;  i-- ) {}   // wait
	CHECKX(             0x000001ff, Px.PinLevel_w0.read() & 0x0fffffff );
    }
    catch (...) {
	FAIL( "unexpected exception" );
    }

//--------------------------------------
  CASE( "99", "Done" );
}

