// 2023-02-05  William A. Hudson
//
// rGPIO Header Pin Names (40-pin header only)
//
// See also:
//   Raspberry Pi Pinout
//     https://pinout.xyz/#
//   Raspberry Pi hardware - GPIO and the 40-pin Header
//     https://www.raspberrypi.com/documentation/computers/raspberry-pi.html
//
// Header pin numbers should only appear in this class, and no other rg* class.
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
* Header Pin map to Gpio number and pin name.  (private)
*
* PinMap[ pin ]		pinRec structure indexed by pin number
*     pin  = header pin number, {1..MaxPin}
* pinRec structure:
*     Gnum    = gpio bit number, otherwise -1= GND, -2= +5.0V, -3= +3.3V
*     Name[8] = pin name string
*/
rgHeaderPin::pinRec	rgHeaderPin::PinMap[rgHeaderPin::MaxPin + 1] = {
  // Gnum Name[8]          pin
    { -1, ""       },	//  0  pin zero does not exist

    { -3, "+3.3V"  },	//  1
    { -2, "+5V"    },	//  2
    {  2, "Gpio2"  },	//  3
    { -2, "+5V"    },	//  4
    {  3, "Gpio3"  },	//  5
    { -1, "GND"    },	//  6
    {  4, "Gpio4"  },	//  7
    { 14, "Gpio14" },	//  8
    { -1, "GND"    },	//  9
    { 15, "Gpio15" },	// 10

    { 17, "Gpio17" },	// 11
    { 18, "Gpio18" },	// 12
    { 27, "Gpio27" },	// 13
    { -1, "GND"    },	// 14
    { 22, "Gpio22" },	// 15
    { 23, "Gpio23" },	// 16
    { -3, "+3.3V"  },	// 17
    { 24, "Gpio24" },	// 18
    { 10, "Gpio10" },	// 19
    { -1, "GND"    },	// 20

    {  9, "Gpio9"  },	// 21
    { 25, "Gpio25" },	// 22
    { 11, "Gpio11" },	// 23
    {  8, "Gpio8"  },	// 24
    { -1, "GND"    },	// 25
    {  7, "Gpio7"  },	// 26
    {  0, "Gpio0"  },	// 27
    {  1, "Gpio1"  },	// 28
    {  5, "Gpio5"  },	// 29
    { -1, "GND"    },	// 30

    {  6, "Gpio6"  },	// 31
    { 12, "Gpio12" },	// 32
    { 13, "Gpio13" },	// 33
    { -1, "GND"    },	// 34
    { 19, "Gpio19" },	// 35
    { 16, "Gpio16" },	// 36
    { 26, "Gpio26" },	// 37
    { 20, "Gpio20" },	// 38
    { -1, "GND"    },	// 39
    { 21, "Gpio21" },	// 40
};

//--------------------------------------------------------------------------
// Accessor Functions
//--------------------------------------------------------------------------

/*
* Lookup gpio bit number given a header pin number.
* call:  (class or object)
*    pin2gpio_int( pin )
*    pin = header pin number {1..40}
* return:
*    () = gpio bit number, otherwise -1= GND, -2= +5.0V, -3= +3.3V
* exception:
*    Throw range_error if pin number is invalid.
*/
const int
rgHeaderPin::pin2gpio_int(
    int		pin
)
{
    if ( (pin <= 0) || (pin > MaxPin) ) {
	std::ostringstream      css;
	css << "pin2gpio_int():  header pin out-of-range:  " << pin;
	throw std::range_error ( css.str() );
    }

    return  PinMap[pin].Gnum;
}


/*
* Lookup pin name given a header pin number.
* call:  (class or object)
*    pin2name_cstr( pin )
*    pin = header pin number {1..40}
* return:
*    () = pin name string
* exception:
*    Throw range_error if pin number is invalid.
*/
const char*
rgHeaderPin::pin2name_cstr(
    int		pin
)
{
    if ( (pin <= 0) || (pin > MaxPin) ) {
	std::ostringstream      css;
	css << "pin2name_cstr():  header pin out-of-range:  " << pin;
	throw std::range_error ( css.str() );
    }

    return  PinMap[pin].Name;
}

