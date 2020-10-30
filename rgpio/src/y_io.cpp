// 2017-06-18  William A. Hudson
//
// rgpio io -- IO pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "Error.h"
#include "yOption.h"

#include "rgAddrMap.h"
#include "rgIoPins.h"	// object registers

#include "yUtil.h"
#include "y_io.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class io_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    // Beware namespace clash with 'hex' io manipulator.
    bool		bin;

    bool		w0;
    bool		w1;
    bool		fsel;
    bool		pud;
    bool		all;
    bool		raw;

    const char*		set;
    const char*		clr;
    const char*		mask;
    const char*		value;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// register args

    bool		PinLevel_w0;
    bool		PinLevel_w1;

    bool		PinSet_w0;
    bool		PinSet_w1;
    bool		PinClr_w0;
    bool		PinClr_w1;
    bool		PinRead_w0;
    bool		PinRead_w1;
    bool		EventStatus_w0;
    bool		EventStatus_w1;
    bool		DetectRise_w0;
    bool		DetectRise_w1;
    bool		DetectFall_w0;
    bool		DetectFall_w1;
    bool		DetectHigh_w0;
    bool		DetectHigh_w1;
    bool		DetectLow_w0;
    bool		DetectLow_w1;
    bool		DetectAsyncRise_w0;
    bool		DetectAsyncRise_w1;
    bool		DetectAsyncFall_w0;
    bool		DetectAsyncFall_w1;
    bool		PudProgMode;
    bool		PudProgClk_w0;
    bool		PudProgClk_w1;
    bool		Fsel0;
    bool		Fsel1;
    bool		Fsel2;
    bool		Fsel3;
    bool		Fsel4;
    bool		Fsel5;

  public:	// data values

    bool		modify;
    uint32_t		mask_n;
    uint32_t		value_n;

  public:
//    io_yOptLong( int argc,  char* argv[] );	// constructor
    io_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		do_rw(   const char* name,  rgReg_rw *rp );
    void		out_reg( const char* name,  uint32_t val );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    io_yOptLong	Opx  ( opx );
*/
io_yOptLong::io_yOptLong( yOption  *opx )
    : yOption( opx )
{
    bin         = 0;

    w0          = 0;
    w1          = 0;
    fsel        = 0;
    pud         = 0;
    all         = 0;
    raw         = 0;

    set         = "";
    clr         = "";
    mask        = "";
    value       = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
			// register args
    PinLevel_w0        = 0;
    PinLevel_w1        = 0;

    PinSet_w0          = 0;
    PinSet_w1          = 0;
    PinClr_w0          = 0;
    PinClr_w1          = 0;
    PinRead_w0         = 0;
    PinRead_w1         = 0;
    EventStatus_w0     = 0;
    EventStatus_w1     = 0;
    DetectRise_w0      = 0;
    DetectRise_w1      = 0;
    DetectFall_w0      = 0;
    DetectFall_w1      = 0;
    DetectHigh_w0      = 0;
    DetectHigh_w1      = 0;
    DetectLow_w0       = 0;
    DetectLow_w1       = 0;
    DetectAsyncRise_w0 = 0;
    DetectAsyncRise_w1 = 0;
    DetectAsyncFall_w0 = 0;
    DetectAsyncFall_w1 = 0;
    PudProgMode        = 0;
    PudProgClk_w0      = 0;
    PudProgClk_w1      = 0;
    Fsel0              = 0;
    Fsel1              = 0;
    Fsel2              = 0;
    Fsel3              = 0;
    Fsel4              = 0;
    Fsel5              = 0;

    modify      = 0;
    mask_n      = 0;
    value_n     = 0;
}


/*
* Parse options.
*/
void
io_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--bin"        )) { bin        = 1; }
	else if ( is( "--w0"         )) { w0         = 1; }
	else if ( is( "--w1"         )) { w1         = 1; }
	else if ( is( "--fsel"       )) { fsel       = 1; }
	else if ( is( "--pud"        )) { pud        = 1; }
	else if ( is( "--all"        )) { all        = 1; }
	else if ( is( "--raw"        )) { raw        = 1; }

	else if ( is( "--set="       )) { set        = this->val(); }
	else if ( is( "--clr="       )) { clr        = this->val(); }
	else if ( is( "--mask="      )) { mask       = this->val(); }
	else if ( is( "--value="     )) { value      = this->val(); }

	else if ( is( "--verbose"    )) { verbose    = 1; }
	else if ( is( "-v"           )) { verbose    = 1; }
	else if ( is( "--debug"      )) { debug      = 1; }
	else if ( is( "--TESTOP"     )) { TESTOP     = 1; }
	else if ( is( "--help"       )) { this->print_usage();  exit( 0 ); }
	else if ( is( "-"            )) {                break; }
	else if ( is( "--"           )) { this->next();  break; }
	else {
	    Error::msg( "unknown option:  " ) << this->current_option() <<endl;
	}
    }

    if ( !( (get_argc() > 0) || w0 || w1 || fsel || pud || all ) ) {
	// no reg specified
	w0 = 1;
	w1 = 1;
    }

    if ( all ) {	// show all register groups
	w0   =1;
	w1   =1;
	fsel =1;
	pud  =1;
    }

    if ( *set ) {
	mask_n = strtoul( set, NULL, 0 );
    }

    if ( *clr ) {
	mask_n = strtoul( clr, NULL, 0 );
    }

    if ( *mask ) {
	mask_n = strtoul( mask, NULL, 0 );
    }

    if ( *value ) {
	value_n = strtoul( value, NULL, 0 );
    }

    if ( *mask || *value ) {
	if ( ! (*mask && *value) ) {
	    Error::msg( "modify requires --mask --value\n" );
	}
    }

    if ( *set ) {
	if ( *clr || *mask || *value ) {
	    Error::msg( "--set invalid with --clr or --mask and --value\n" );
	}
    }

    if ( *clr ) {
	if ( *set || *mask || *value ) {
	    Error::msg( "--clr invalid with --set or --mask and --value\n" );
	}
    }

    modify = ( *clr || *set || *mask || *value );

    if ( modify ) {
	if ( w0 || w1 || fsel || pud || all ) {
	    Error::msg( "modification invalid with --w0 --w1 --fsel --pud --all\n" );
	}
	// To avoid accidental broad changes.
    }

    if ( w0 ) {
	if ( raw ) {
	    PinSet_w0           = 1;
	    PinClr_w0           = 1;
	    PinRead_w0          = 1;
	}
	else {
	    PinLevel_w0         = 1;
	}
	EventStatus_w0      = 1;
	DetectRise_w0       = 1;
	DetectFall_w0       = 1;
	DetectHigh_w0       = 1;
	DetectLow_w0        = 1;
	DetectAsyncRise_w0  = 1;
	DetectAsyncFall_w0  = 1;
    }

    if ( w1 ) {
	if ( raw ) {
	    PinSet_w1           = 1;
	    PinClr_w1           = 1;
	    PinRead_w1          = 1;
	}
	else {
	    PinLevel_w1         = 1;
	}
	EventStatus_w1      = 1;
	DetectRise_w1       = 1;
	DetectFall_w1       = 1;
	DetectHigh_w1       = 1;
	DetectLow_w1        = 1;
	DetectAsyncRise_w1  = 1;
	DetectAsyncFall_w1  = 1;
    }

    if ( pud ) {
	PudProgMode         = 1;
	PudProgClk_w0       = 1;
	PudProgClk_w1       = 1;
    }

    if ( fsel ) {
	Fsel0               = 1;
	Fsel1               = 1;
	Fsel2               = 1;
	Fsel3               = 1;
	Fsel4               = 1;
	Fsel5               = 1;
    }

    while ( get_argc() > 0 )		// Register arguments
    {
	     if ( is( "PinLevel_w0"        )) { PinLevel_w0        = 1; }
	else if ( is( "PinLevel_w1"        )) { PinLevel_w1        = 1; }
	else if ( is( "PinSet_w0"          )) { PinSet_w0          = 1; }
	else if ( is( "PinSet_w1"          )) { PinSet_w1          = 1; }
	else if ( is( "PinClr_w0"          )) { PinClr_w0          = 1; }
	else if ( is( "PinClr_w1"          )) { PinClr_w1          = 1; }
	else if ( is( "PinRead_w0"         )) { PinRead_w0         = 1; }
	else if ( is( "PinRead_w1"         )) { PinRead_w1         = 1; }
	else if ( is( "EventStatus_w0"     )) { EventStatus_w0     = 1; }
	else if ( is( "EventStatus_w1"     )) { EventStatus_w1     = 1; }
	else if ( is( "DetectRise_w0"      )) { DetectRise_w0      = 1; }
	else if ( is( "DetectRise_w1"      )) { DetectRise_w1      = 1; }
	else if ( is( "DetectFall_w0"      )) { DetectFall_w0      = 1; }
	else if ( is( "DetectFall_w1"      )) { DetectFall_w1      = 1; }
	else if ( is( "DetectHigh_w0"      )) { DetectHigh_w0      = 1; }
	else if ( is( "DetectHigh_w1"      )) { DetectHigh_w1      = 1; }
	else if ( is( "DetectLow_w0"       )) { DetectLow_w0       = 1; }
	else if ( is( "DetectLow_w1"       )) { DetectLow_w1       = 1; }
	else if ( is( "DetectAsyncRise_w0" )) { DetectAsyncRise_w0 = 1; }
	else if ( is( "DetectAsyncRise_w1" )) { DetectAsyncRise_w1 = 1; }
	else if ( is( "DetectAsyncFall_w0" )) { DetectAsyncFall_w0 = 1; }
	else if ( is( "DetectAsyncFall_w1" )) { DetectAsyncFall_w1 = 1; }
	else if ( is( "PudProgMode"        )) { PudProgMode        = 1; }
	else if ( is( "PudProgClk_w0"      )) { PudProgClk_w0      = 1; }
	else if ( is( "PudProgClk_w1"      )) { PudProgClk_w1      = 1; }
	else if ( is( "Fsel0"              )) { Fsel0              = 1; }
	else if ( is( "Fsel1"              )) { Fsel1              = 1; }
	else if ( is( "Fsel2"              )) { Fsel2              = 1; }
	else if ( is( "Fsel3"              )) { Fsel3              = 1; }
	else if ( is( "Fsel4"              )) { Fsel4              = 1; }
	else if ( is( "Fsel5"              )) { Fsel5              = 1; }
	else {
	    Error::msg( "unknown register:  " ) << this->current_option() <<endl;
	}

	next_arg();
    }

    if ( modify ) {
	if (                          PinSet_w0      && !(*set) ) {
	    Error::msg(   "set only:  PinSet_w0"      ) <<endl;
	}
	if (                          PinSet_w1      && !(*set) ) {
	    Error::msg(   "set only:  PinSet_w1"      ) <<endl;
	}

	if (                          PinClr_w0      && !(*clr) ) {
	    Error::msg( "clear only:  PinClr_w0"      ) <<endl;
	}
	if (                          PinClr_w1      && !(*clr) ) {
	    Error::msg( "clear only:  PinClr_w1"      ) <<endl;
	}

	if (                          PinRead_w0                ) {
	    Error::msg(  "read only:  PinRead_w0"     ) <<endl;
	}
	if (                          PinRead_w1                ) {
	    Error::msg(  "read only:  PinRead_w1"     ) <<endl;
	}

	if (                          EventStatus_w0 && !(*clr) ) {
	    Error::msg( "clear only:  EventStatus_w0" ) <<endl;
	}
	if (                          EventStatus_w1 && !(*clr) ) {
	    Error::msg( "clear only:  EventStatus_w1" ) <<endl;
	}
    }
}


/*
* Show option flags.
*/
void
io_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--w0          = " << w0           << endl;
    cout << "--w1          = " << w1           << endl;
    cout << "--fsel        = " << fsel         << endl;
    cout << "--pud         = " << pud          << endl;
    cout << "--all         = " << all          << endl;
    cout << "--raw         = " << raw          << endl;
    cout << "--set         = " << set          << endl;
    cout << "--clr         = " << clr          << endl;
    cout << "--mask        = " << mask         << endl;
    cout << "--value       = " << value        << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "modify        = " << modify       << endl;

    cout.fill('0');
    cout <<std::hex;

    cout << "mask_n        = 0x" <<setw(8) << mask_n       << endl;
    cout << "value_n       = 0x" <<setw(8) << value_n      << endl;
    cout <<std::dec;
}


/*
* Show usage.
*/
void
io_yOptLong::print_usage()
{
    cout <<
    "    IO pin operations\n"
    "usage:  " << ProgName << " io [options..]  [reg..]\n"
    "    reg                 register name, as shown with --all\n"
//  "  output format:  (one of)\n"
//  "    --hex               word format hexadecimal (default)\n"
//  " #  --bin               word format binary\n"
    "  register groups:  (accumulate)\n"
    "    --w0                pin Level, Event, Detect word 0 (default)\n"
    "    --w1                pin Level, Event, Detect word 1 (default)\n"
    "    --fsel              Fsel function select registers\n"
    "    --pud               pin Pull-Up/Down program registers\n"
    "    --all               all registers above\n"
    "  register group modifiers on --w0 --w1:\n"
    "    --raw               show real instead of virtual PinLevel*\n"
    "  modify:  (32-bit values)\n"
    "    --set=0xff..        set mask bits\n"
    "    --clr=0xff..        clear mask bits\n"
    "    --mask=0xff..       mask to select bits of --value\n"
    "    --value=0x00..      bit values selected by --mask\n"
//  " #  --reset             reset registers to power-up value\n"
    "  options:\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output, show registers in binary\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


/*
* Apply modification and output object register.
*/
void
io_yOptLong::do_rw( const char* name, rgReg_rw *rp )
{
    if (      *(this->set) ) {
	rp->set( this->mask_n );
    }
    else if ( *(this->clr) ) {
	rp->clr( this->mask_n );
    }
    else if ( this->mask_n ) {
	rp->modify( this->mask_n, this->value_n );
    }

    out_reg( name, rp->read() );
}

/*
* Output register value in hex and binary.
*/
void
io_yOptLong::out_reg( const char* name,  uint32_t vv )
{
    cout.fill('0');
    cout << "0x" <<std::hex <<setw(8)  << vv;

    cout.fill(' ');
    if ( this->verbose ) {
	cout << "  " <<left <<setw(18) << name;
	cout << "  "                   << cstr_bits32( vv ) <<endl;
    }
    else {
	cout << "  "                   << name <<endl;
    }

    cout <<std::dec <<right;	// restore defaults
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_io::y_io(
    yOption		*opx,
    rgAddrMap		*rgx
) {
    AddrMap = rgx;
    MainOpx = opx;
}


//--------------------------------------------------------------------------
// Main body
//--------------------------------------------------------------------------

int
y_io::doit()
{

    try {
	io_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgIoPins		Gpx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout <<setw(8) << (void*) Gpx.get_base_addr()
		 << "  GpioBase" <<endl;
	    cout <<setw(8) << (void*) Gpx.PinRead_w0.addr()
		 << "  PinRead_w0.addr()" <<endl;
	}

	if ( Opx.verbose ) {
	    cout << "IO Pin Registers:                 28   24   20   16   12    8    4    0" << endl;
	}

#define APPLY( X, Y ) if ( Opx.X ) { Opx.do_rw( Y, &(Gpx.X) ); }

    // Word 0
	if ( Opx.PinLevel_w0 ) {
	    if (      *(Opx.set) ) { Gpx.PinLevel_w0.set( Opx.mask_n ); }
	    else if ( *(Opx.clr) ) { Gpx.PinLevel_w0.clr( Opx.mask_n ); }
	    else if ( Opx.mask_n ) {
		Gpx.PinLevel_w0.modify( Opx.mask_n, Opx.value_n );
	    }
	    Opx.out_reg( "PinLevel_w0", Gpx.PinLevel_w0.read() );
	}

	if ( Opx.PinSet_w0 ) {
	    if ( *(Opx.set) ) {       Gpx.PinSet_w0.set( Opx.mask_n ); }
	    Opx.out_reg( "PinSet_w0", Gpx.PinSet_w0.read() );
	}

	if ( Opx.PinClr_w0 ) {
	    if ( *(Opx.clr) ) {       Gpx.PinClr_w0.clr( Opx.mask_n ); }
	    Opx.out_reg( "PinClr_w0", Gpx.PinClr_w0.read() );
	}

	if ( Opx.PinRead_w0 ) {
	    Opx.out_reg( "PinRead_w0", Gpx.PinRead_w0.read() );
	}

	if ( Opx.EventStatus_w0 ) {
	    if ( *(Opx.clr) ) {         Gpx.EventStatus_w0.clr( Opx.mask_n ); }
	    Opx.out_reg( "EventStatus_w0", Gpx.EventStatus_w0.read() );
	}

	APPLY( DetectRise_w0,      "DetectRise_w0"      )
	APPLY( DetectFall_w0,      "DetectFall_w0"      )
	APPLY( DetectHigh_w0,      "DetectHigh_w0"      )
	APPLY( DetectLow_w0,       "DetectLow_w0"       )
	APPLY( DetectAsyncRise_w0, "DetectAsyncRise_w0" )
	APPLY( DetectAsyncFall_w0, "DetectAsyncFall_w0" )

    // Word 1
	if ( Opx.w1 && (Opx.w0) ) { cout <<endl; }

	if ( Opx.PinLevel_w1 ) {
	    if (      *(Opx.set) ) { Gpx.PinLevel_w1.set( Opx.mask_n ); }
	    else if ( *(Opx.clr) ) { Gpx.PinLevel_w1.clr( Opx.mask_n ); }
	    else if ( Opx.mask_n ) {
		Gpx.PinLevel_w1.modify( Opx.mask_n, Opx.value_n );
	    }
	    Opx.out_reg( "PinLevel_w1", Gpx.PinLevel_w1.read() );
	}

	if ( Opx.PinSet_w1 ) {
	    if ( *(Opx.set) ) {       Gpx.PinSet_w1.set( Opx.mask_n ); }
	    Opx.out_reg( "PinSet_w1", Gpx.PinSet_w1.read() );
	}

	if ( Opx.PinClr_w1 ) {
	    if ( *(Opx.clr) ) {       Gpx.PinClr_w1.clr( Opx.mask_n ); }
	    Opx.out_reg( "PinClr_w1", Gpx.PinClr_w1.read() );
	}

	if ( Opx.PinRead_w1 ) {
	    Opx.out_reg( "PinRead_w1", Gpx.PinRead_w1.read() );
	}

	if ( Opx.EventStatus_w1 ) {
	    if ( *(Opx.clr) ) {         Gpx.EventStatus_w1.clr( Opx.mask_n ); }
	    Opx.out_reg( "EventStatus_w1", Gpx.EventStatus_w1.read() );
	}

	APPLY( DetectRise_w1,      "DetectRise_w1"      )
	APPLY( DetectFall_w1,      "DetectFall_w1"      )
	APPLY( DetectHigh_w1,      "DetectHigh_w1"      )
	APPLY( DetectLow_w1,       "DetectLow_w1"       )
	APPLY( DetectAsyncRise_w1, "DetectAsyncRise_w1" )
	APPLY( DetectAsyncFall_w1, "DetectAsyncFall_w1" )

    // Pud
	if ( Opx.pud && (Opx.w0 || Opx.w1) ) { cout <<endl; }
	APPLY( PudProgMode,        "PudProgMode"        )
	APPLY( PudProgClk_w0,      "PudProgClk_w0"      )
	APPLY( PudProgClk_w1,      "PudProgClk_w1"      )

    // Fsel
	if ( Opx.fsel && (Opx.w0 || Opx.w1 || Opx.pud) ) { cout <<endl; }
	APPLY( Fsel0,              "Fsel0"              )
	APPLY( Fsel1,              "Fsel1"              )
	APPLY( Fsel2,              "Fsel2"              )
	APPLY( Fsel3,              "Fsel3"              )
	APPLY( Fsel4,              "Fsel4"              )
	APPLY( Fsel5,              "Fsel5"              )

    }
    catch ( std::exception& e ) {
	Error::msg( "exception caught:  " ) << e.what() <<endl;
    }
    catch (...) {
	Error::msg( "unexpected exception\n" );
    }

    return ( Error::has_err() ? 1 : 0 );
    //#!! return value?
}

