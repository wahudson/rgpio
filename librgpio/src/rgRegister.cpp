// 2019-11-12  William A. Hudson

// rGPIO Register base class.
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgRegister.h"


/*
* Constructor.
* call:
*    rgRegister	 rx;
*/
rgRegister::rgRegister()
{
    Addr    = 0;
    RegVal  = 0;
}


//--------------------------------------------------------------------------
// Generic Field accessors:  (private)
//--------------------------------------------------------------------------
/*
* Register bits are numbered [31:0], left to right.
* Field position is the bit number of the field LSB.
* Mask is a contiguous bit-field of ones, and is the maximum field value.
* These are protected because safety relies on proper arguments.
* Intended only for use by derived registers.
*/

/*
* Get field value of Register.
*    Extract field value out of a 32-bit register value.
* call:
*    value = get_field( 20, 0xfff )	// bits [31:20]
*/
uint32_t
rgRegister::get_field(
    const uint32_t		pos,	// field bit position of LSB
    const uint32_t		mask	// mask field size
)
{
    return  ( (RegVal >> pos) & mask );
}


/*
* Insert field value into Register.
*    Value is range checked.
* call:
*    put_field( 16, 0xff, value )	// bits [23:16]
* exception:
*    range_error  if ( value > mask )
* #!! Want better identification of where the error occured?  Too anonymous.
*/
void
rgRegister::put_field(
    const uint32_t		pos,	// field bit position
    const uint32_t		mask,	// mask field size
    const uint32_t		value	// value to insert
)
{
    if ( value > mask ) {
	std::ostringstream	css;
	css << "rgRegister::put_field():  value exceeds 0x" <<hex << mask
	    << ":  0x" <<hex << value;
	throw std::range_error ( css.str() );
    }

    RegVal = (RegVal & ~(mask << pos)) | (value << pos);
}

