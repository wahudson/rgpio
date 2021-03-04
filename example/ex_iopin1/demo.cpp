// 2021-02-25  William A. Hudson
//
// Example IO Pin - rgIoPins toggling Gpio[8]
// External setup:
//    Set Gpio[8] to 'Out', optional to drive an LED.
//    % rgpio fsel --mode=Out 8
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
//#include <stdlib.h>

using namespace std;

#include "rgAddrMap.h"
#include "rgIoPins.h"


int
main( int argc,  char *argv[] )
{

    cout << "hello" <<endl;

    try {
	rgAddrMap		Amx;	// constructor

	cout << "BaseAddr= 0x" <<hex << Amx.config_BaseAddr() <<endl;

	Amx.config_Debug( 1 );
	Amx.config_FakeNoPi( 1 );	// when not on RPi
	Amx.open_dev_gpiomem();
//	Amx.open_dev_mem();

	cout << "BaseAddr= 0x" <<hex << Amx.config_BaseAddr() <<endl;

	if ( Amx.is_fake_mem() ) {
	    cout << "Using Fake Memory" <<endl;
	}

	rgIoPins		Gx  ( &Amx );	// constructor

	// Read the Gpio pins, toggling Gpio[8]
	cout.fill('0');
	cout <<hex;
	cout << "PinLevel_w0= 0x" <<setw(8) << Gx.PinLevel_w0.read() <<endl;

	Gx.PinLevel_w0.clr( 0x00000100 );
	cout << "PinLevel_w0= 0x" <<setw(8) << Gx.PinLevel_w0.read() <<endl;

	Gx.PinLevel_w0.set( 0x00000100 );
	cout << "PinLevel_w0= 0x" <<setw(8) << Gx.PinLevel_w0.read() <<endl;

	Gx.PinLevel_w0.clr( 0x00000100 );
	cout << "PinLevel_w0= 0x" <<setw(8) << Gx.PinLevel_w0.read() <<endl;

	cout <<dec;

	// Loop blinking Gpio[8]
	for ( int j=10;  j>0;  j-- ) {
	    cout << "j= " << j <<endl;

	    Gx.PinClr_w0.clr( 0x00000100 );
	    for ( volatile int i=10000000;  i>0;  i-- ) {}

	    Gx.PinSet_w0.set( 0x00000100 );
	    for ( volatile int i=10000000;  i>0;  i-- ) {}
	}

	cout.fill('0');
	cout <<hex;
	cout << "PinLevel_w0= 0x" <<setw(8) << Gx.PinLevel_w0.read() <<endl;
    }
    catch ( std::exception& e ) {
	cerr << "Error:  exception caught:  " << e.what() <<endl;
    }
    catch (...) {
	cerr << "Error:  unexpected exception" <<endl;
    }

    return  0;
}

