// 2023-02-05  William A. Hudson
//
// rGPIO Header Pin Names (40-pin header only)
//
// See:  Raspberry Pi Pinout  https://pinout.xyz/#
//     Raspberry Pi hardware - GPIO and the 40-pin Header
//     https://www.raspberrypi.com/documentation/computers/raspberry-pi.html
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "rgHeaderPin.h"

//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

rgHeaderPin::rgHeaderPin()
{
}

//--------------------------------------------------------------------------
// Class Data
//--------------------------------------------------------------------------

/*
* Gpio bit number for each header pin number.  (private)
*
* GpioBit[ pin ]	Gpio bit number, -1 otherwise
*     pin  = header pin number, {1..MaxPin}
*/
const int		rgHeaderPin::GpioBit[rgHeaderPin::MaxPin + 1] = {
     // gpio       pin
	-1,	//  0  zero does not exist
	-1,	//  1
	-1,	//  2
	 2,	//  3
	-1,	//  4
	 3,	//  5
	-1,	//  6
	 4,	//  7
	14,	//  8
	-1,	//  9

	15,	// 10
	17,	// 11
	18,	// 12
	27,	// 13
	-1,	// 14
	22,	// 15
	23,	// 16
	-1,	// 17
	24,	// 18
	10,	// 19

	-1,	// 20
	 9,	// 21
	25,	// 22
	11,	// 23
	 8,	// 24
	-1,	// 25
	 7,	// 26
	 0,	// 27
	 1,	// 28
	 5,	// 29

	-1,	// 30
	 6,	// 31
	12,	// 32
	13,	// 33
	-1,	// 34
	19,	// 35
	16,	// 36
	26,	// 37
	20,	// 38
	-1,	// 39

	21	// 40
};


/*
* Power pin name for each header pin number.  (private)
*
* PowerName[ pin ]	Power name string, "" otherwise
*     pin  = header pin number, {1..MaxPin}
*/
const char*		rgHeaderPin::PowerName[rgHeaderPin::MaxPin + 1] = {
     // power		   pin
	"",		//  0  zero does not exist
	"+3.3V",	//  1
	"+5V",		//  2
	"",		//  3
	"+5V",		//  4
	"",		//  5
	"GND",		//  6
	"",		//  7
	"",		//  8
	"GND",		//  9

	"",		// 10
	"",		// 11
	"",		// 12
	"",		// 13
	"GND",		// 14
	"",		// 15
	"",		// 16
	"+3.3V",	// 17
	"",		// 18
	"",		// 19

	"GND",		// 20
	"",		// 21
	"",		// 22
	"",		// 23
	"",		// 24
	"GND",		// 25
	"",		// 26
	"",		// 27
	"",		// 28
	"",		// 29

	"GND",		// 30
	"",		// 31
	"",		// 32
	"",		// 33
	"GND",		// 34
	"",		// 35
	"",		// 36
	"",		// 37
	"",		// 38
	"GND",		// 39

	""		// 40
};

//--------------------------------------------------------------------------
// Accessor Functions
//--------------------------------------------------------------------------

/*
* Lookup gpio bit number given a header pin number.
* call:  (class or object)
*    pin2gpio( pin )
*    pin = header pin number {1..40}
* return:
*    () = gpio bit number, -1 otherwise
*/
const int
rgHeaderPin::pin2gpio(
    int		pin
)
{
    if ( (pin <= 0) || (pin > 40) ) {
	std::ostringstream      css;
	css << "pin2gpio():  pin out-of-range:  " << pin;
	throw std::range_error ( css.str() );
    }

    return  GpioBit[ pin ];
}


/*
* Lookup power pin name given a header pin number.
* call:  (class or object)
*    pin2power_name( pin )
*    pin = header pin number {1..40}
* return:
*    () = power name string, null string "" otherwise
*/
const char*
rgHeaderPin::pin2power_name(
    int		pin
)
{
    if ( (pin <= 0) || (pin > 40) ) {
	std::ostringstream      css;
	css << "pin2power_name():  pin out-of-range:  " << pin;
	throw std::range_error ( css.str() );
    }

    return  PowerName[ pin ];
}

