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

    bool		hex;
    bool		bin;

    bool		fsel;
    bool		w0;
    bool		w1;
    bool		pud;
    bool		all;

    const char*		set;
    const char*		clr;
    const char*		mask;
    const char*		value;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

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
    hex         = 0;
    bin         = 0;

    fsel        = 0;
    w0          = 0;
    w1          = 0;
    pud         = 0;
    all         = 0;

    set         = "";
    clr         = "";
    mask        = "";
    value       = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

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
	     if ( is( "--hex"        )) { hex        = 1; }
	else if ( is( "--bin"        )) { bin        = 1; }

	else if ( is( "--fsel"       )) { fsel       = 1; }
	else if ( is( "--w0"         )) { w0         = 1; }
	else if ( is( "--w1"         )) { w1         = 1; }
	else if ( is( "--pud"        )) { pud        = 1; }
	else if ( is( "--all"        )) { all        = 1; }

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
    }

    if ( all ) {	// show all register groups
	fsel =1;
	w0   =1;
	w1   =1;
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
}


/*
* Show option flags.
*/
void
io_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--hex         = " << hex          << endl;
    cout << "--bin         = " << bin          << endl;
    cout << "--fsel        = " << fsel         << endl;
    cout << "--w0          = " << w0           << endl;
    cout << "--w1          = " << w1           << endl;
    cout << "--pud         = " << pud          << endl;
    cout << "--all         = " << all          << endl;
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
    "    reg                 register enum name\n"
//  "  output format:  (one of)\n"
//  "    --hex               word format hexadecimal (default)\n"
//  " #  --bin               word format binary\n"
    "  register groups:  (accumulate)\n"
    "    --w0                word 0 registers (default)\n"
    "    --w1                word 1 registers\n"
    "    --fsel              Fsel function select registers\n"
    "    --pud               pin PullUpDown registers\n"
    "    --all               all registers plus PinSet, PinClr\n"
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

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout <<setw(8) << (void*) Gpx.get_base_addr()
		 << "  GpioBase" <<endl;
	    cout <<setw(8) << (void*) Gpx.addr_reg( rgIoPin::rgPinRead_w0 )
		 << "  addr rgPinRead_w0" <<endl;
	}

	const int		RegLimit = 30;
	rgIoPin::rgIoReg_enum	regarg[RegLimit];	// arg registers
	int			regcnt = 0;
	char			*arg;

    // Register groups
	if ( Opx.fsel ) {
	    regarg[regcnt++] = rgIoPin::rgFsel0;
	    regarg[regcnt++] = rgIoPin::rgFsel1;
	    regarg[regcnt++] = rgIoPin::rgFsel2;
	    regarg[regcnt++] = rgIoPin::rgFsel3;
	    regarg[regcnt++] = rgIoPin::rgFsel4;
	    regarg[regcnt++] = rgIoPin::rgFsel5;
	}

	if ( Opx.w0 ) {
	    regarg[regcnt++] = rgIoPin::rgPinRead_w0;
	    regarg[regcnt++] = rgIoPin::rgEventStatus_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectRising_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectFalling_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectHigh_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectLow_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectAsyncRising_w0;
	    regarg[regcnt++] = rgIoPin::rgDetectAsyncFalling_w0;
	}

	if ( Opx.w1 ) {
	    regarg[regcnt++] = rgIoPin::rgPinRead_w1;
	    regarg[regcnt++] = rgIoPin::rgEventStatus_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectRising_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectFalling_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectHigh_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectLow_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectAsyncRising_w1;
	    regarg[regcnt++] = rgIoPin::rgDetectAsyncFalling_w1;
	}

	if ( Opx.pud ) {
	    regarg[regcnt++] = rgIoPin::rgPullUpDown;
	    regarg[regcnt++] = rgIoPin::rgPullUpDownClk_w0;
	    regarg[regcnt++] = rgIoPin::rgPullUpDownClk_w1;
	}

	if ( Opx.all ) {
	    regarg[regcnt++] = rgIoPin::rgPinSet_w0;
	    regarg[regcnt++] = rgIoPin::rgPinSet_w1;
	    regarg[regcnt++] = rgIoPin::rgPinClr_w0;
	    regarg[regcnt++] = rgIoPin::rgPinClr_w1;
	}

    // Argument register list
	while ( (arg = Opx.next_arg()) )
	{
	    try {
		regarg[regcnt] = rgIoPin::find_IoReg_enum( arg );
		regcnt++;
	    }
	    catch ( range_error& e ) {
		Error::msg( "unknown register:  " ) << arg <<endl
		    << "    " <<  e.what() << endl;
	    }

	    if ( regcnt > RegLimit ) {
		Error::msg( "max register args:  " ) << RegLimit <<endl;
		break;
	    }
	}
	if ( Error::has_err() )  return 1;

	if ( Opx.verbose ) {
	    cout << "IO Pin Registers:                      28   24   20   16   12    8    4    0" << endl;
	}

    // Process registers
	if ( Opx.modify ) {
	    cout << "Modify:" << endl;
	}

	for ( int ii=0;  ii<regcnt;  ii++ )
	{
	    rgIoPin::rgIoReg_enum	reg = regarg[ii];
	    bool			ok = 0;

	    if ( Opx.modify ) {

		if (      (reg == rgIoPin::rgPinSet_w0) && *Opx.set ) {
				   Gpx.set_PinLevel_w0( Opx.mask_n );
		    ok = 1;
		}
		else if ( (reg == rgIoPin::rgPinSet_w1) && *Opx.set ) {
				   Gpx.set_PinLevel_w1( Opx.mask_n );
		    ok = 1;
		}
		else if ( (reg == rgIoPin::rgPinClr_w0) && *Opx.clr ) {
				   Gpx.clr_PinLevel_w0( Opx.mask_n );
		    ok = 1;
		}
		else if ( (reg == rgIoPin::rgPinClr_w1) && *Opx.clr ) {
				   Gpx.clr_PinLevel_w1( Opx.mask_n );
		    ok = 1;
		}
		else if ( (reg == rgIoPin::rgPinRead_w0) ||
			  (reg == rgIoPin::rgPinRead_w1) ) {
		    ok = 0;	// read-only
		}
		else if ( (reg == rgIoPin::rgEventStatus_w0) && *Opx.clr ) {
				     Gpx.clr_EventStatus_w0( Opx.mask_n );
		    ok = 1;
		}
		else if ( (reg == rgIoPin::rgEventStatus_w1) && *Opx.clr ) {
				     Gpx.clr_EventStatus_w1( Opx.mask_n );
		    ok = 1;
		}
		else {		// generic register
		    if ( *Opx.set ) {
			Gpx.set_reg( reg, Opx.mask_n );
		    }
		    else if ( *Opx.clr ) {
			Gpx.clr_reg( reg, Opx.mask_n );
		    }
		    else {
			Gpx.modify_reg( reg, Opx.mask_n, Opx.value_n );
		    }
		    ok = 1;
		}

		if ( ! ok ) {
		    Error::msg( "inappropriate operation on register:  " )
			<< Gpx.str_IoReg_enum( reg ) <<endl;
		    continue;
		}
	    }

	    uint32_t	vv = Gpx.read_reg( reg );

	    if ( Opx.verbose ) {
		cout <<setfill('0')
		     << "0x" <<setw(8)  <<hex  << vv
		     <<setfill(' ')
		     << "  " <<setw(23) <<left << Gpx.str_IoReg_enum( reg )
		     << "  "                   << cstr_bits32( vv )
		     <<endl;
	    }
	    else {
		cout <<setfill('0')
		     << "0x" <<setw(8)  <<hex  << vv
		     << "  "                   << Gpx.str_IoReg_enum( reg )
		     <<endl;
	    }
	}
	cout.fill(' ');

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

