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
    const char*		reg;
    const char*		mask;
    const char*		value;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    rgIoPin::rgIoReg_enum	reg_e;
    uint32_t		mask_n;
    uint32_t		value_n;

  public:
//    io_yOptLong( int argc,  char* argv[] );	// constructor
    io_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

    int			doit();
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
    reg         = "";
    mask        = "";
    value       = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    reg_e       = rgIoPin::rgPinLevel_w0;
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
	else if ( is( "--reg="       )) { reg        = this->val(); }
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
	    Error::err( "unknown option:  ", this->current_option() );
	}
    }

    if ( *reg ) {
	try {
	    reg_e = rgIoPin::find_IoReg_enum( reg );
	}
	catch ( range_error& e ) {
	    Error::err( "unknown register in --reg=", reg,
		"\n    ",  e.what() );
	}
    }

    if ( *mask ) {
	mask_n = strtoul( mask, NULL, 0 );
    }

    if ( *value ) {
	value_n = strtoul( value, NULL, 0 );
    }

    if ( *mask || *value ) {
	if ( ! (*mask && *value && *reg) ) {
	    Error::err( "modify requires --reg --mask --value" );
	}
    }

    if ( get_argc() > 0 ) {
	Error::err( "extra arguments:  ", next_arg() );
    }
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
    cout << "--reg         = " << reg          << endl;
    cout << "--mask        = " << mask         << endl;
    cout << "--value       = " << value        << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "reg_e         = " << rgIoPin::str_IoReg_enum( reg_e ) << endl;

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
    "usage:  " << ProgName << " io [options..]\n"
    "  output format:  (default is --col)\n"
    " #  -c,--col            show one bit per line\n"
    "    --hex               word format hexadecimal\n"
    " #  --bin               word format binary\n"
    "  selection:\n"
    "    --reg=R             register to access, R= enum\n"
    "  modify:  --reg\n"
    "    --mask=0xff         mask select bits to change, 32-bits\n"
    "    --value=0x00        bit value, 32-bits\n"
    "  options:\n"
    "    --help              show this usage\n"
    " #  -v, --verbose       verbose output\n"
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
	    return ( Error::err() ? 1 : 0 );
	}

	if ( Error::err() )  return 1;

	rgIoPin			Gpx;	// constructor

	uint32_t		val;

	Gpx.init_addr( AddrMap );

	if ( Opx.debug ) {
	}

	rgIoPin::rgIoReg_enum	list_w0[] = {
	    rgIoPin::rgPinLevel_w0,
	    rgIoPin::rgEventStatus_w0,
	    rgIoPin::rgDetectRising_w0,
	    rgIoPin::rgDetectFalling_w0,
	    rgIoPin::rgDetectHigh_w0,
	    rgIoPin::rgDetectLow_w0,
	    rgIoPin::rgDetectAsyncRising_w0,
	    rgIoPin::rgDetectAsyncFalling_w0,
	};

	// Set register value
	if ( *Opx.mask && *Opx.value && *Opx.reg ) {
	    cout << "Modify:" << endl;
	    Gpx.mod_reg( Opx.reg_e, Opx.value_n, Opx.mask_n );
	}

	if ( *Opx.reg ) {
	    cout.fill('0');
	    cout <<hex;
	    cout << "0x" <<setw(8) << Gpx.read_reg( Opx.reg_e )
		 << "  "           << Gpx.str_IoReg_enum( Opx.reg_e ) <<endl;
	}

	// Show status of all registers
	if ( ! *Opx.reg ) {
	    cout.fill('0');
	    cout <<hex;

	    int	list_w0_N = sizeof( list_w0 ) / sizeof( rgIoPin::rgIoReg_enum);
//	    cout << "list_w0_N= " << list_w0_N << endl;

	    for ( int k=0;  k < list_w0_N;  k++ )
	    {
		rgIoPin::rgIoReg_enum	rr;
		rr = list_w0[k];
		cout << "0x" <<setw(8) << Gpx.read_reg( rr )
		     << "  0x"         << rr*4
		     << "  "           << Gpx.str_IoReg_enum( rr ) <<endl;
	    }

	}

    }
    catch ( std::exception& e ) {
	Error::err( "exception caught:  ", e.what() );
    }
    catch (...) {
	Error::err( "unexpected exception" );
    }

    return ( Error::err() ? 1 : 0 );
    //#!! return value?
}

