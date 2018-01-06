// 2018-01-04  William A. Hudson
//
// rgpio fsel -- Function Select pins - RaspberryPi GPIO utility program.
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
#include "rgFselPin.h"

#include "rgAltFuncName.h"

#include "y_fsel.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class fsel_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		bin;
    bool		col;

    bool		w0;
    bool		w1;

    const char*		mode;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    rgFselPin::rgFsel_enum	mode_e;

  public:
    fsel_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

    int			doit();
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    fsel_yOptLong	Opx  ( opx );
*/
fsel_yOptLong::fsel_yOptLong( yOption  *opx )
    : yOption( opx )
{
    bin         = 0;
    col         = 0;

    w0          = 0;
    w1          = 0;

    mode        = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    mode_e      = rgFselPin::f_In;
}


/*
* Parse options.
*/
void
fsel_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--bin"        )) { bin        = 1; }
	else if ( is( "--col"        )) { col        = 1; }
	else if ( is( "-c"           )) { col        = 1; }

	else if ( is( "--w0"         )) { w0         = 1; }
	else if ( is( "--w1"         )) { w1         = 1; }

	else if ( is( "--mode="      )) { mode       = this->val(); }

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

    if ( !( (get_argc() > 0) || w0 || w1 ) ) {	// no bit specified
	w0 = 1;
    }

    if ( *mode ) {
	try {
	    mode_e = rgFselPin::find_rgFsel_enum( mode );
	}
	catch ( range_error& e ) {
	    Error::msg( "unknown Fsel mode:  --fsel=" ) << mode <<endl
		<< "    " <<  e.what() << endl;
	}
    }
}


/*
* Show option flags.
*/
void
fsel_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--bin         = " << bin          << endl;
    cout << "--col         = " << col          << endl;
    cout << "--w0          = " << w0           << endl;
    cout << "--w1          = " << w1           << endl;
    cout << "--mode        = " << mode         << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "mode_e        = " << rgFselPin::str_rgFsel_enum( mode_e ) << endl;
}


/*
* Show usage.
*/
void
fsel_yOptLong::print_usage()
{
    cout <<
    "    Function Select pin operations\n"
    "usage:  " << ProgName << " fsel [options..]  [N..]\n"
    "    N                   bit number 0..53\n"
    "  output:  (one of)\n"
//  "    --bin               show one line binary\n"
    "    -c,--col            show one bit per line\n"
//  "    --show_alts         show show alternate function table\n"
    "  bit number groups:  (accumulate)\n"
    "    --w0                register word 0 (default)\n"
    "    --w1                register word 1\n"
    "  modify:\n"
    "    --mode=F            set mode {In, Out, Alt0, .. Alt5}\n"
//  " #  --reset             reset registers to power-up value\n"
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

y_fsel::y_fsel(
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
y_fsel::doit()
{

    try {
	fsel_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgIoPin			Gpx  ( AddrMap );	// constructor
	rgFselPin		Fpx  ( &Gpx );		// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout <<setw(8) << (void*) Gpx.get_base_addr()
		 << "  GpioBase" <<endl;
	    cout <<setw(8) << (void*) Gpx.addr_reg( rgIoPin::rgPinRead_w0 )
		 << "  addr rgPinRead_w0" <<endl;
	}

	const int		BitLimit = 64;
	int			bitarg[BitLimit];	// bit numbers
	int			bitcnt = 0;
	char			*arg;

    // Register groups

	if ( Opx.w0 ) {
	    for ( int k=0;  k<=31;  k++ )
	    {
		bitarg[bitcnt++] = k;
	    }
	}

	if ( Opx.w1 ) {
	    for ( int k=32;  k<=53;  k++ )
	    {
		bitarg[bitcnt++] = k;
	    }
	}

    // Argument bit list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;

	    n = strtol( arg, NULL, 0 );
	    if ( (n < 0) || (n > 53) ) {
		Error::msg( " bit arg out-of-range:  " ) << n <<endl;
		continue;
	    }

	    bitarg[bitcnt] = n;
	    bitcnt++;

	    if ( bitcnt > BitLimit ) {
		Error::msg( "max bit args:  " ) << BitLimit <<endl;
		break;
	    }
	}
//	cout << "bitcnt=" << bitcnt <<endl;
	if ( Error::has_err() )  return 1;

    // Process Fsel bits
	if ( *Opx.mode ) {	// Modify
	    cout << "Modify:" << endl;
	}

	cout << "Bit  Mode  Function" <<endl;

	for ( int ii=0;  ii<bitcnt;  ii++ )
	{
	    int				bit;
	    rgFselPin::rgFsel_enum	mode;

	    bit = bitarg[ii];

	    if ( *Opx.mode ) {	// Modify
		Fpx.modify_Fsel_bit( bit, Opx.mode_e );
	    }

	    mode = Fpx.read_Fsel_bit( bit );

	    cout.fill(' ');
	    cout <<dec;
	    cout << " "  <<setw(2) <<right << bit
		 << "  " <<setw(4) <<left  << Fpx.str_rgFsel_enum( mode )
		 << "  " << rgAltFuncName::str_altfunc_bit( mode, bit )
		 <<endl;
	}

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

