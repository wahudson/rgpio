// 2017-05-11  William A. Hudson

// rGPIO General Purpose Clock class.
//
// See:  BCM2835 ARM Peripherals (2012)
//	p.105-108  6.3 General Purpose GPIO Clocks
//
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdexcept>

using namespace std;

#include "rgAddrMap.h"
#include "rgClock.h"


/*
* Constructor.
* Optional address map initialization.
*    rgAddrMap	amx;		// address map object
*    amx.open_dev_gpiomem();	// select and open device file
* call:  (Deprecated!)
*    rgIoPin	gpx  ( 2 );		// no GpioBase init, use seperate init
*    gpx.init_addr( 2, &amx );
* call:
*    rgIoPin	gpx  ( 2, &amx );	// constructor with address map
*/
rgClock::rgClock(
    uint32_t		clknum,	// clock number {0,1,2}
    rgAddrMap		*xx	// default NULL
)
{
    GpioBase   = NULL;
    if ( xx ) {
	this->init_addr( xx );
    }

    if ( clknum > 2 ) {
	std::ostringstream	css;
	css << "rgClock:  require clock number {0,1,2}:  " << clknum;
	throw std::range_error ( css.str() );
    }

    ClkNum   = clknum;
    CtlReg   = 0;
    DivReg   = 0;
}


//--------------------------------------------------------------------------
// GpioBase address
//--------------------------------------------------------------------------

/*
* Init GpioBase address.
*    The class knows its BCM documentation base address.
*/
void
rgClock::init_addr(
    rgAddrMap		*xx
)
{
    GpioBase = xx->get_mem_block( FeatureAddr );
}

// inline volatile uint32_t*   get_base_addr()


//--------------------------------------------------------------------------
// Direct low-level access
//--------------------------------------------------------------------------
// No modification of object.

/*
* Control register address.
*/
volatile uint32_t*
rgClock::addr_CtlReg()
{
    if ( GpioBase == NULL ) {
	throw std::logic_error ( "rgClock:  not initialized" );
    }
    return  (GpioBase + Ctl_offset + (ClkNum * 2));	// word offsets
}

/*
* Divider register address.
*/
volatile uint32_t*
rgClock::addr_DivReg()
{
    if ( GpioBase == NULL ) {
	throw std::logic_error ( "rgClock:  not initialized" );
    }
    return  (GpioBase + Div_offset + (ClkNum * 2));	// word offsets
}


/*
* Read Control register.
*/
uint32_t
rgClock::read_CtlReg()
{
    return  *( addr_CtlReg() );
}

/*
* Read Divisor register.
*/
uint32_t
rgClock::read_DivReg()
{
    return  *( addr_DivReg() );
}


/*
* Write Control register, no Busy check.
*    Intended for internal use and Test.
*/
void
rgClock::raw_write_CtlReg( uint32_t  vv )
{
    *( addr_CtlReg() ) = vv;
}

void
rgClock::raw_write_DivReg( uint32_t  vv )
{
    *( addr_DivReg() ) = vv;
}


//--------------------------------------
/*
* Read Busy bit.
*/
uint32_t
rgClock::read_Busy()
{
    return  ( (read_CtlReg() >> Busy_pos) & 0x1 );
}


/*
* Read Enable bit.
*/
uint32_t
rgClock::read_Enable()
{
    return  ( (read_CtlReg() >> Enable_pos) & 0x1 );
}


//--------------------------------------------------------------------------
// Direct control:  (modify register fields)
//--------------------------------------------------------------------------

/*
* Enable the clock.
*/
void
rgClock::enable_clock()
{
    uint32_t		cr;

    cr = read_CtlReg();
    cr |= ( 0x1 << Enable_pos );
    raw_write_CtlReg( cr );
}


/*
* Disable the clock.
*/
void
rgClock::disable_clock()
{
    uint32_t		cr;

    cr = read_CtlReg();
    cr &= ~( 0x1 << Enable_pos );
    raw_write_CtlReg( cr );
}


/*
* Disable the clock and wait until not Busy.
*    Uses an internal time-out.
* call:
*    wait_disable()
* return:
*    () = status, 0= success, 1= failure
*/
bool
rgClock::wait_disable()
{
    disable_clock();

    for ( int i=10;  i>0;  i-- )
    {
	if ( read_Busy() == 0 ) { break; }
    }
    //#!! sleep
    return( 0 );
}


/*
* Wait for not Busy.
* call:
*    wait_while_Busy( wait_ns )
*    wait_ns = number of nanoseconds to wait, < 1e9
* return:
*    () = status, 0= success, 1= failure
*/
bool
rgClock::wait_while_Busy()
{
    uint32_t		busy;

    busy = read_Busy();

    if ( read_Enable() == 1 ) {
	return 1;
	// error, not disabled
    }

    if ( busy ) {
	return 1;
    }
    //#!! sleep

    return 0;
}


/*
* Kill the clock generator.
*    Stop and reset the clock generator.  May glitch the output.
*    The Kill bit is left set, user must clear.
*    Intended for test/debug, and possible reset from a bad state.
*/
void
rgClock::kill_generator()
{
    uint32_t		cr;

    cr = read_CtlReg();
    cr |= ( 0x1 << Enable_pos );
    raw_write_CtlReg( cr );
}


//--------------------------------------------------------------------------
// Object state opterations
//--------------------------------------------------------------------------

/*
* Read control registers into the object.
*/
void
rgClock::grab_regs()
{
    CtlReg = read_CtlReg();
    DivReg = read_DivReg();
}


/*
* Raw write control registers from the object.
*    May glitch or hang the clock generator, unless ENAB=0 and BUSY=0.
*/
void
rgClock::raw_write_regs()
{
    raw_write_CtlReg( CtlReg );
    raw_write_DivReg( DivReg );
}


/*
* Safely write control registers from the object.
*    First read the control register.
*    If not (ENAB=0 and BUSY=0), then set ENAB=0 and wait for BUSY=0.
*    Then write the registers with ENAB=0.
*    Then enable the clock if Enable=1.
* #!! NOT done
*/
void
rgClock::apply_regs()
{
//    uint32_t		cr;
//    uint32_t		busy;
    uint32_t		enab;

//    cr = read_CtlReg();
//    if ( cr & (BUSY | ENABLE) ) {
//	wait_disable();
//    }

    enab = get_Enable();	// save desired state

    put_Enable( 0 );

    raw_write_regs();

    if ( enab ) {		// enable only after reg update
	enable_clock();
    }
}


//--------------------------------------------------------------------------
// Object Field Accessors
//--------------------------------------------------------------------------

/*
* Get the MASH field 2-bit value.
*/
uint32_t
rgClock::get_Mash()
{
    return  ( (CtlReg >> Mash_pos) & 0x3 );
}

/*
* Set the MASH field 2-bit value.
*/
void
rgClock::put_Mash( uint32_t  bit2 )
{
    if ( bit2 > 0x3 ) {
	std::ostringstream	css;
	css << "rgClock::put_Mash():  require 2-bit arg:  " << bit2;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x3  << Mash_pos );
    CtlReg |=  ( bit2 << Mash_pos );
}


/*
* Flip field 1-bit value.
*/
uint32_t
rgClock::get_Flip()
{
    return  ( (CtlReg >> Flip_pos) & 0x1 );
}

void
rgClock::put_Flip( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Flip():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Flip_pos );
    CtlReg |=  ( bit1 << Flip_pos );
}


/*
* Busy field 1-bit value.  (Read-only)
*/
uint32_t
rgClock::get_Busy()
{
    return  ( (CtlReg >> Busy_pos) & 0x1 );
}

void
rgClock::put_Busy( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Busy():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Busy_pos );
    CtlReg |=  ( bit1 << Busy_pos );
}


/*
* Kill field 1-bit value.
*/
uint32_t
rgClock::get_Kill()
{
    return  ( (CtlReg >> Kill_pos) & 0x1 );
}

void
rgClock::put_Kill( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Kill():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Kill_pos );
    CtlReg |=  ( bit1 << Kill_pos );
}


/*
* Enable field 1-bit value.
*/
uint32_t
rgClock::get_Enable()
{
    return  ( (CtlReg >> Enable_pos) & 0x1 );
}

void
rgClock::put_Enable( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Enable():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Enable_pos );
    CtlReg |=  ( bit1 << Enable_pos );
}


/*
* Clock Source field 4-bit value.
*/
uint32_t
rgClock::get_Source()
{
    return  ( (CtlReg >> Source_pos) & 0xf );
}

void
rgClock::put_Source( uint32_t  bit4 )
{
    if ( bit4 > 0xf ) {
	std::ostringstream	css;
	css << "rgClock::put_Source():  require 4-bit arg:  " << bit4;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0xf  << Source_pos );
    CtlReg |=  ( bit4 << Source_pos );
}


/*
* DivI field, Integer part of divisor, 12-bit value.
*     The MASH filter limits the minimum value of DivI, to ensure it is not
*     exposed to frequencies higher than 25MHz.
*     See p.105.
*/
uint32_t
rgClock::get_DivI()
{
    return  ( (DivReg >> DivI_pos) & 0xfff );
}

void
rgClock::put_DivI( uint32_t  bit12 )
{
    if ( bit12 > 0xfff ) {
	std::ostringstream	css;
	css << "rgClock::put_DivI():  require 12-bit arg:  0x" <<hex << bit12;
	throw std::range_error ( css.str() );
    }

    DivReg &= ~( 0xfff << DivI_pos );
    DivReg |=  ( bit12 << DivI_pos );
}


/*
* DivF field, Fractional part of divisor, 12-bit value.
*/
uint32_t
rgClock::get_DivF()
{
    return  ( (DivReg >> DivF_pos) & 0xfff );
}

void
rgClock::put_DivF( uint32_t  bit12 )
{
    if ( bit12 > 0xfff ) {
	std::ostringstream	css;
	css << "rgClock::put_DivF():  require 12-bit arg:  0x" <<hex << bit12;
	throw std::range_error ( css.str() );
    }

    DivReg &= ~( 0xfff << DivF_pos );
    DivReg |=  ( bit12 << DivF_pos );
}


//--------------------------------------------------------------------------
// Debug
//--------------------------------------------------------------------------

/*
* Show debug output.
* call:
*    self.show_debug()
*/
void
rgClock::show_debug( std::ostream& sout )
{
    sout << "ClkNum= "      <<dec << ClkNum
	 << "  CtlReg= 0x"  <<hex << CtlReg
	 << "  DivReg= 0x"  <<hex << DivReg
	 << endl;
    sout <<dec;
}

