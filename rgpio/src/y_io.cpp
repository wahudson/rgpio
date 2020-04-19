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
#include "rgIoPin.h"

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
    bool		rgPinSet_w0;
    bool		rgPinSet_w1;
    bool		rgPinClr_w0;
    bool		rgPinClr_w1;
    bool		rgPinRead_w0;
    bool		rgPinRead_w1;
    bool		rgEventStatus_w0;
    bool		rgEventStatus_w1;
    bool		rgDetectRising_w0;
    bool		rgDetectRising_w1;
    bool		rgDetectFalling_w0;
    bool		rgDetectFalling_w1;
    bool		rgDetectHigh_w0;
    bool		rgDetectHigh_w1;
    bool		rgDetectLow_w0;
    bool		rgDetectLow_w1;
    bool		rgDetectAsyncRising_w0;
    bool		rgDetectAsyncRising_w1;
    bool		rgDetectAsyncFalling_w0;
    bool		rgDetectAsyncFalling_w1;
    bool		rgPullUpDown;
    bool		rgPullUpDownClk_w0;
    bool		rgPullUpDownClk_w1;
    bool		rgFsel0;
    bool		rgFsel1;
    bool		rgFsel2;
    bool		rgFsel3;
    bool		rgFsel4;
    bool		rgFsel5;

  public:	// data values

    bool		modify;
    uint32_t		mask_n;
    uint32_t		value_n;

    rgIoPin		*Gpxx;		// for register operations

  public:
//    io_yOptLong( int argc,  char* argv[] );	// constructor
    io_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		do_reg(  rgIoPin::rgIoReg_enum reg );
    void		out_reg( rgIoPin::rgIoReg_enum reg );
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
    PinLevel_w0             = 0;
    PinLevel_w1             = 0;
    rgPinSet_w0             = 0;
    rgPinSet_w1             = 0;
    rgPinClr_w0             = 0;
    rgPinClr_w1             = 0;
    rgPinRead_w0            = 0;
    rgPinRead_w1            = 0;
    rgEventStatus_w0        = 0;
    rgEventStatus_w1        = 0;
    rgDetectRising_w0       = 0;
    rgDetectRising_w1       = 0;
    rgDetectFalling_w0      = 0;
    rgDetectFalling_w1      = 0;
    rgDetectHigh_w0         = 0;
    rgDetectHigh_w1         = 0;
    rgDetectLow_w0          = 0;
    rgDetectLow_w1          = 0;
    rgDetectAsyncRising_w0  = 0;
    rgDetectAsyncRising_w1  = 0;
    rgDetectAsyncFalling_w0 = 0;
    rgDetectAsyncFalling_w1 = 0;
    rgPullUpDown            = 0;
    rgPullUpDownClk_w0      = 0;
    rgPullUpDownClk_w1      = 0;
    rgFsel0                 = 0;
    rgFsel1                 = 0;
    rgFsel2                 = 0;
    rgFsel3                 = 0;
    rgFsel4                 = 0;
    rgFsel5                 = 0;

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
	    rgPinSet_w0             = 1;
	    rgPinClr_w0             = 1;
	    rgPinRead_w0            = 1;
	}
	else {
	    PinLevel_w0             = 1;
	}
	rgEventStatus_w0        = 1;
	rgDetectRising_w0       = 1;
	rgDetectFalling_w0      = 1;
	rgDetectHigh_w0         = 1;
	rgDetectLow_w0          = 1;
	rgDetectAsyncRising_w0  = 1;
	rgDetectAsyncFalling_w0 = 1;
    }

    if ( w1 ) {
	if ( raw ) {
	    rgPinSet_w1             = 1;
	    rgPinClr_w1             = 1;
	    rgPinRead_w1            = 1;
	}
	else {
	    PinLevel_w1             = 1;
	}
	rgEventStatus_w1        = 1;
	rgDetectRising_w1       = 1;
	rgDetectFalling_w1      = 1;
	rgDetectHigh_w1         = 1;
	rgDetectLow_w1          = 1;
	rgDetectAsyncRising_w1  = 1;
	rgDetectAsyncFalling_w1 = 1;
    }

    if ( pud ) {
	rgPullUpDown            = 1;
	rgPullUpDownClk_w0      = 1;
	rgPullUpDownClk_w1      = 1;
    }

    if ( fsel ) {
	rgFsel0                 = 1;
	rgFsel1                 = 1;
	rgFsel2                 = 1;
	rgFsel3                 = 1;
	rgFsel4                 = 1;
	rgFsel5                 = 1;
    }

    while ( get_argc() > 0 )		// Register arguments
    {
	     if ( is( "PinLevel_w0"           )) { PinLevel_w0           = 1; }
	else if ( is( "PinLevel_w1"           )) { PinLevel_w1           = 1; }
	else if ( is( "rgPinSet_w0"           )) { rgPinSet_w0           = 1; }
	else if ( is( "rgPinSet_w1"           )) { rgPinSet_w1           = 1; }
	else if ( is( "rgPinClr_w0"           )) { rgPinClr_w0           = 1; }
	else if ( is( "rgPinClr_w1"           )) { rgPinClr_w1           = 1; }
	else if ( is( "rgPinRead_w0"          )) { rgPinRead_w0          = 1; }
	else if ( is( "rgPinRead_w1"          )) { rgPinRead_w1          = 1; }
	else if ( is( "rgEventStatus_w0"      )) { rgEventStatus_w0      = 1; }
	else if ( is( "rgEventStatus_w1"      )) { rgEventStatus_w1      = 1; }
	else if ( is( "rgDetectRising_w0"     )) { rgDetectRising_w0     = 1; }
	else if ( is( "rgDetectRising_w1"     )) { rgDetectRising_w1     = 1; }
	else if ( is( "rgDetectFalling_w0"    )) { rgDetectFalling_w0    = 1; }
	else if ( is( "rgDetectFalling_w1"    )) { rgDetectFalling_w1    = 1; }
	else if ( is( "rgDetectHigh_w0"       )) { rgDetectHigh_w0       = 1; }
	else if ( is( "rgDetectHigh_w1"       )) { rgDetectHigh_w1       = 1; }
	else if ( is( "rgDetectLow_w0"        )) { rgDetectLow_w0        = 1; }
	else if ( is( "rgDetectLow_w1"        )) { rgDetectLow_w1        = 1; }
	else if ( is( "rgDetectAsyncRising_w0")) { rgDetectAsyncRising_w0= 1; }
	else if ( is( "rgDetectAsyncRising_w1")) { rgDetectAsyncRising_w1= 1; }
	else if ( is( "rgDetectAsyncFalling_w0")) { rgDetectAsyncFalling_w0= 1;}
	else if ( is( "rgDetectAsyncFalling_w1")) { rgDetectAsyncFalling_w1= 1;}
	else if ( is( "rgPullUpDown"          )) { rgPullUpDown          = 1; }
	else if ( is( "rgPullUpDownClk_w0"    )) { rgPullUpDownClk_w0    = 1; }
	else if ( is( "rgPullUpDownClk_w1"    )) { rgPullUpDownClk_w1    = 1; }
	else if ( is( "rgFsel0"               )) { rgFsel0               = 1; }
	else if ( is( "rgFsel1"               )) { rgFsel1               = 1; }
	else if ( is( "rgFsel2"               )) { rgFsel2               = 1; }
	else if ( is( "rgFsel3"               )) { rgFsel3               = 1; }
	else if ( is( "rgFsel4"               )) { rgFsel4               = 1; }
	else if ( is( "rgFsel5"               )) { rgFsel5               = 1; }
	else {
	    Error::msg( "unknown register:  " ) << this->current_option() <<endl;
	}

	next_arg();
    }

    if ( modify ) {
	if (                          rgPinSet_w0      && !(*set) ) {
	    Error::msg(   "set only:  rgPinSet_w0"      ) <<endl;
	}
	if (                          rgPinSet_w1      && !(*set) ) {
	    Error::msg(   "set only:  rgPinSet_w1"      ) <<endl;
	}

	if (                          rgPinClr_w0      && !(*clr) ) {
	    Error::msg( "clear only:  rgPinClr_w0"      ) <<endl;
	}
	if (                          rgPinClr_w1      && !(*clr) ) {
	    Error::msg( "clear only:  rgPinClr_w1"      ) <<endl;
	}

	if (                          rgPinRead_w0                ) {
	    Error::msg(  "read only:  rgPinRead_w0"     ) <<endl;
	}
	if (                          rgPinRead_w1                ) {
	    Error::msg(  "read only:  rgPinRead_w1"     ) <<endl;
	}

	if (                          rgEventStatus_w0 && !(*clr) ) {
	    Error::msg( "clear only:  rgEventStatus_w0" ) <<endl;
	}
	if (                          rgEventStatus_w1 && !(*clr) ) {
	    Error::msg( "clear only:  rgEventStatus_w1" ) <<endl;
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
    "    --pud               pin PullUpDown registers\n"
    "    --all               all registers above\n"
    "  register group modifiers on --w0 --w1:\n"
    "    --raw               show rgPin* instead of virtual PinLevel*\n"
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
* Apply modification and output enum register.
*/
void
io_yOptLong::do_reg( rgIoPin::rgIoReg_enum reg )
{
    if (      *(this->set) ) {
	this->Gpxx->set_reg( reg, this->mask_n );
    }
    else if ( *(this->clr) ) {
	this->Gpxx->clr_reg( reg, this->mask_n );
    }
    else if ( this->mask_n ) {
	this->Gpxx->modify_reg( reg, this->mask_n, this->value_n );
    }

    out_reg( reg );
}

/*
* Output enum register.
*/
void
io_yOptLong::out_reg( rgIoPin::rgIoReg_enum reg )
{
    out_reg( rgIoPin::str_IoReg_enum( reg ), this->Gpxx->read_reg( reg ) );
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
	cout << "  " <<left <<setw(23) << name;
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

	rgIoPin			Gpx  ( AddrMap );	// constructor

	Opx.Gpxx = &Gpx;	// for register operations

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout <<setw(8) << (void*) Gpx.get_base_addr()
		 << "  GpioBase" <<endl;
	    cout <<setw(8) << (void*) Gpx.addr_reg( rgIoPin::rgPinRead_w0 )
		 << "  addr rgPinRead_w0" <<endl;
	}

	if ( Opx.verbose ) {
	    cout << "IO Pin Registers:                      28   24   20   16   12    8    4    0" << endl;
	}

#define APPLX( X ) if ( Opx.X ) { Opx.do_reg( rgIoPin::X ); }

    // Word 0
	if ( Opx.PinLevel_w0 ) {
	    if (      *(Opx.set) ) { Gpx.set_PinLevel_w0( Opx.mask_n ); }
	    else if ( *(Opx.clr) ) { Gpx.clr_PinLevel_w0( Opx.mask_n ); }
	    else if ( Opx.mask_n ) {
		Gpx.set_PinLevel_w0( Opx.mask_n &    Opx.value_n  );
		Gpx.clr_PinLevel_w0( Opx.mask_n & (~ Opx.value_n) );
	    }
	    Opx.out_reg( "PinLevel_w0", Gpx.read_PinLevel_w0() );
	}

	if ( Opx.rgPinSet_w0 ) {
	    if ( *(Opx.set) ) { Gpx.set_PinLevel_w0( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgPinSet_w0 );
	}

	if ( Opx.rgPinClr_w0 ) {
	    if ( *(Opx.clr) ) { Gpx.clr_PinLevel_w0( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgPinClr_w0 );
	}

	if ( Opx.rgPinRead_w0 ) {
	    Opx.out_reg( rgIoPin::rgPinRead_w0 );
	}

	if ( Opx.rgEventStatus_w0 ) {
	    if ( *(Opx.clr) ) { Gpx.clr_EventStatus_w0( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgEventStatus_w0 );
	}

	APPLX( rgDetectRising_w0       )
	APPLX( rgDetectFalling_w0      )
	APPLX( rgDetectHigh_w0         )
	APPLX( rgDetectLow_w0          )
	APPLX( rgDetectAsyncRising_w0  )
	APPLX( rgDetectAsyncFalling_w0 )

    // Word 1
	if ( Opx.w1 && (Opx.w0) ) { cout <<endl; }

	if ( Opx.PinLevel_w1 ) {
	    if (      *(Opx.set) ) { Gpx.set_PinLevel_w1( Opx.mask_n ); }
	    else if ( *(Opx.clr) ) { Gpx.clr_PinLevel_w1( Opx.mask_n ); }
	    else if ( Opx.mask_n ) {
		Gpx.set_PinLevel_w1( Opx.mask_n &    Opx.value_n  );
		Gpx.clr_PinLevel_w1( Opx.mask_n & (~ Opx.value_n) );
	    }
	    Opx.out_reg( "PinLevel_w1", Gpx.read_PinLevel_w1() );
	}

	if ( Opx.rgPinSet_w1 ) {
	    if ( *(Opx.set) ) { Gpx.set_PinLevel_w1( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgPinSet_w1 );
	}

	if ( Opx.rgPinClr_w1 ) {
	    if ( *(Opx.clr) ) { Gpx.clr_PinLevel_w1( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgPinClr_w1 );
	}

	if ( Opx.rgPinRead_w1 ) {
	    Opx.out_reg( rgIoPin::rgPinRead_w1 );
	}

	if ( Opx.rgEventStatus_w1 ) {
	    if ( *(Opx.clr) ) { Gpx.clr_EventStatus_w1( Opx.mask_n ); }
	    Opx.out_reg( rgIoPin::rgEventStatus_w1 );
	}

	APPLX( rgDetectRising_w1       )
	APPLX( rgDetectFalling_w1      )
	APPLX( rgDetectHigh_w1         )
	APPLX( rgDetectLow_w1          )
	APPLX( rgDetectAsyncRising_w1  )
	APPLX( rgDetectAsyncFalling_w1 )

    // Pud
	if ( Opx.pud && (Opx.w0 || Opx.w1) ) { cout <<endl; }
	APPLX( rgPullUpDown            )
	APPLX( rgPullUpDownClk_w0      )
	APPLX( rgPullUpDownClk_w1      )

    // Fsel
	if ( Opx.fsel && (Opx.w0 || Opx.w1 || Opx.pud) ) { cout <<endl; }
	APPLX( rgFsel0                 )
	APPLX( rgFsel1                 )
	APPLX( rgFsel2                 )
	APPLX( rgFsel3                 )
	APPLX( rgFsel4                 )
	APPLX( rgFsel5                 )

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

