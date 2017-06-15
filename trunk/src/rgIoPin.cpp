// 2017-06-10  William A. Hudson

// GPIO IO Pin control class.
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.89-101  6.  General Purpose I/O (GPIO)
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

//#include "Error.h"
#include "rgAddrMap.h"
#include "rgIoPin.h"


/*
* Constructor.
*/
rgIoPin::rgIoPin()
{
    GpioBase   = NULL;

    FselReg[0] = 0;
    FselReg[1] = 0;
    FselReg[2] = 0;
    FselReg[3] = 0;
    FselReg[4] = 0;
    FselReg[5] = 0;
}


//--------------------------------------------------------------------------
// GpioBase address
//--------------------------------------------------------------------------

/*
* Init GpioBase address.
*/
void
rgIoPin::init_addr(
    rgAddrMap		*xx
)
{
    GpioBase = xx->get_mem_block( 0x7e200000 );
}

// inline volatile uint32_t*   get_base_addr()


//--------------------------------------------------------------------------
// Generic Register access
//--------------------------------------------------------------------------

/*
* Get raw register address.
*/
volatile uint32_t*
rgIoPin::addr_reg(
    rgIoReg_enum	reg
)
{
    if ( NULL == GpioBase ) {
	std::ostringstream	css;
	css << "rgIoPin:: not initialized in:  addr_reg( 0x"
	    <<hex << (reg*4) << "/4 )";
	throw std::logic_error ( css.str() );
    }

    return  (GpioBase + reg);
}


/*
* Read raw register value.
*    No copy in the object.
*    No check for write-only registers.
*/
uint32_t
rgIoPin::read_reg(
    rgIoReg_enum	reg
)
{
    return  *(GpioBase + reg);
}


/*
* Modify register.
*    Does read/modify/write.
*    No copy in the object.
*    Not applicable for write-only registers.
*/
void
rgIoPin::mod_reg(
    rgIoReg_enum	reg,
    uint32_t		value,
    uint32_t		mask
)
{
    uint32_t		x;

    x = *(GpioBase + reg);
    x = (x & (~ mask) ) | (value & mask);
    *(GpioBase + reg) = x;
}

//--------------------------------------------------------------------------
// IO Register read/write
//--------------------------------------------------------------------------

