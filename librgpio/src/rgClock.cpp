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
    BusyCount  = 0;
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
* Raw Write registers, no Busy check.
*    Is a no-op unless the password field reg[31:24] = 0x5a
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


/*
* Write registers, no Busy check, password applied.
*/
void
rgClock::write_CtlReg( uint32_t  vv )
{
    vv = (vv & 0x00ffffff) | 0x5a000000;
    *( addr_CtlReg() ) = vv;
}

void
rgClock::write_DivReg( uint32_t  vv )
{
    vv = (vv & 0x00ffffff) | 0x5a000000;
    *( addr_DivReg() ) = vv;
}


//--------------------------------------
/*
* Read Busy bit.
*/
uint32_t
rgClock::read_Busy_1()
{
    return  ( (read_CtlReg() >> Busy_pos) & 0x1 );
}


/*
* Read Enable bit.
*/
uint32_t
rgClock::read_Enable_1()
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
    write_CtlReg( cr );
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
    write_CtlReg( cr );
}


/*
* Disable the clock and wait until not Busy.
*    Uses an internal time-out sized so a normal clock should be stopped.
*    Intended as a simple combined operation.
*    Use disable_clock() and wait_while_Busy() for more control.
* Timeout - read_Busy_1() takes about 370 ns on RPi3, so 10,000 counts is
*    about 3.7 ms.  Sleep may not be worthwhile?
* call:
*    wait_disable()
* return:
*    () = busy status, 0= success clock stopped, 1= still busy
*    BusyCount = number of read_Busy_1() calls taken
*/
bool
rgClock::wait_disable()
{
    uint32_t		busy;
    int			i;

    disable_clock();

    for ( i=1;  i<=10000;  i++ )	//#!! tuning?
    {
	busy = read_Busy_1();
	if ( busy == 0 ) { break; }
    }
    BusyCount = i;		// save count in object

    // cerr << "wait_disable() loop i= " << i <<endl;

    return  busy;
}


/*
* Wait for not Busy.
*    Does not disable the clock or write the CtlReg.
*    Will time out if clock is still enabled.
*    Use with disable_clock() or raw_write_regs() to do your own timing.
* #!! Needs more complete definition.
* call:
*    wait_while_Busy( wait_ns )
*    wait_ns = number of nanoseconds to wait, < 1e9
* return:
*    () = busy status, 0= success clock stopped, 1= still busy
*/
bool
rgClock::wait_while_Busy(
    uint32_t		wait_ns,
    int			num_times
)
{
    uint32_t		busy;

    while ( (num_times--) > 0 )
    {
	busy = read_Busy_1();
	if ( busy == 0 ) { break; }
	//#!! nanosleep( wait_ns );
    }

    return  busy;
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
    cr |= ( 0x1 << Kill_pos );
    write_CtlReg( cr );
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
* Write control registers from the object.
*    May glitch or hang the clock generator, unless ENAB=0 and BUSY=0.
*    Password 0x5a is applied.
*/
void
rgClock::push_regs()
{
    write_CtlReg( CtlReg );
    write_DivReg( DivReg );
}


/*
* Safely write control registers from the object.
*    Password is always replaced with 0x5a.
*    First read the control register.
*    If not (ENAB=0 and BUSY=0), then set ENAB=0 and wait for BUSY=0.
*    Then write the registers with ENAB=0.
*    Then enable the clock if Enable=1.
* return:
*    () = Busy status, 0= ok, 1= still busy when registers were written, i.e.
*             wait_disable() timed out.
*/
bool
rgClock::apply_regs()
{
    uint32_t		cr;
    uint32_t		enab;
    bool		rv    = 0;

    cr = read_CtlReg();
    if ( cr & 0x00000090 ) {	// Busy=1 or Enable=1
	rv = wait_disable();
    }
    // Continue if still Busy
    //#!! What if still Busy?  Throw?  Kill?

    enab = get_Enable_1();	// save desired state

    put_Enable_1( 0 );
    put_PasswdCtl_8( 0x5a );
    put_PasswdDiv_8( 0x5a );

    raw_write_CtlReg( CtlReg );
    raw_write_DivReg( DivReg );

    if ( enab ) {		// enable only after reg update
	put_Enable_1( enab );		// restore enable
	raw_write_CtlReg( CtlReg );	// enable clock
    }

    return  rv;
}


//--------------------------------------------------------------------------
// Object Field Accessors
//--------------------------------------------------------------------------

/*
* Control Password field 8-bit value.  (Write-only)
*/
uint32_t
rgClock::get_PasswdCtl_8()
{
    return  ( (CtlReg >> Passwd_pos) & 0xff );
}

void
rgClock::put_PasswdCtl_8( uint32_t  bit8 )
{
    if ( bit8 > 0xff ) {
	std::ostringstream	css;
	css << "rgClock::put_PasswdCtl_8():  require 8-bit arg:  0x" << hex << bit8;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0xff << Passwd_pos );
    CtlReg |=  ( bit8 << Passwd_pos );
}


/*
* MASH field 2-bit value.
*/
uint32_t
rgClock::get_Mash_2()
{
    return  ( (CtlReg >> Mash_pos) & 0x3 );
}

void
rgClock::put_Mash_2( uint32_t  bit2 )
{
    if ( bit2 > 0x3 ) {
	std::ostringstream	css;
	css << "rgClock::put_Mash_2():  require 2-bit arg:  " << bit2;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x3  << Mash_pos );
    CtlReg |=  ( bit2 << Mash_pos );
}


/*
* Flip field 1-bit value.
*/
uint32_t
rgClock::get_Flip_1()
{
    return  ( (CtlReg >> Flip_pos) & 0x1 );
}

void
rgClock::put_Flip_1( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Flip_1():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Flip_pos );
    CtlReg |=  ( bit1 << Flip_pos );
}


/*
* Busy field 1-bit value.  (Read-only)
*/
uint32_t
rgClock::get_Busy_1()
{
    return  ( (CtlReg >> Busy_pos) & 0x1 );
}

void
rgClock::put_Busy_1( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Busy_1():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Busy_pos );
    CtlReg |=  ( bit1 << Busy_pos );
}


/*
* Kill field 1-bit value.
*/
uint32_t
rgClock::get_Kill_1()
{
    return  ( (CtlReg >> Kill_pos) & 0x1 );
}

void
rgClock::put_Kill_1( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Kill_1():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Kill_pos );
    CtlReg |=  ( bit1 << Kill_pos );
}


/*
* Enable field 1-bit value.
*/
uint32_t
rgClock::get_Enable_1()
{
    return  ( (CtlReg >> Enable_pos) & 0x1 );
}

void
rgClock::put_Enable_1( uint32_t  bit1 )
{
    if ( bit1 > 0x1 ) {
	std::ostringstream	css;
	css << "rgClock::put_Enable_1():  require 1-bit arg:  " << bit1;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0x1  << Enable_pos );
    CtlReg |=  ( bit1 << Enable_pos );
}


/*
* Clock Source field 4-bit value.
*/
uint32_t
rgClock::get_Source_4()
{
    return  ( (CtlReg >> Source_pos) & 0xf );
}

void
rgClock::put_Source_4( uint32_t  bit4 )
{
    if ( bit4 > 0xf ) {
	std::ostringstream	css;
	css << "rgClock::put_Source_4():  require 4-bit arg:  0x" <<hex << bit4;
	throw std::range_error ( css.str() );
    }

    CtlReg &= ~( 0xf  << Source_pos );
    CtlReg |=  ( bit4 << Source_pos );
}


/*
* Divider Password field 8-bit value.  (Write-only)
*/
uint32_t
rgClock::get_PasswdDiv_8()
{
    return  ( (DivReg >> Passwd_pos) & 0xff );
}

void
rgClock::put_PasswdDiv_8( uint32_t  bit8 )
{
    if ( bit8 > 0xff ) {
	std::ostringstream	css;
	css << "rgClock::put_PasswdDiv_8():  require 8-bit arg:  0x" << hex << bit8;
	throw std::range_error ( css.str() );
    }

    DivReg &= ~( 0xff << Passwd_pos );
    DivReg |=  ( bit8 << Passwd_pos );
}


/*
* DivI field, Integer part of divisor, 12-bit value.
*     The MASH filter limits the minimum value of DivI, to ensure it is not
*     exposed to frequencies higher than 25MHz.
*     See p.105.
*/
uint32_t
rgClock::get_DivI_12()
{
    return  ( (DivReg >> DivI_pos) & 0xfff );
}

void
rgClock::put_DivI_12( uint32_t  bit12 )
{
    if ( bit12 > 0xfff ) {
	std::ostringstream	css;
	css << "rgClock::put_DivI_12():  require 12-bit arg:  0x" <<hex << bit12;
	throw std::range_error ( css.str() );
    }

    DivReg &= ~( 0xfff << DivI_pos );
    DivReg |=  ( bit12 << DivI_pos );
}


/*
* DivF field, Fractional part of divisor, 12-bit value.
*/
uint32_t
rgClock::get_DivF_12()
{
    return  ( (DivReg >> DivF_pos) & 0xfff );
}

void
rgClock::put_DivF_12( uint32_t  bit12 )
{
    if ( bit12 > 0xfff ) {
	std::ostringstream	css;
	css << "rgClock::put_DivF_12():  require 12-bit arg:  0x" <<hex << bit12;
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

