// 2023-02-05  William A. Hudson
//
// rgpio header -- Header pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgFselPin.h"

#include "rgHeaderPin.h"
#include "rgAltFuncName.h"

#include "yUtil.h"
#include "y_header.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class header_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		gpio  = 0;
    bool		power = 0;
    bool		row   = 0;

    const char*		mode;

    bool		show  = 0;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    rgFselPin::rgFsel_enum	mode_e;

    bool		ModifyBits = 0;

  public:
    header_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    header_yOptLong	Opx  ( opx );
*/
header_yOptLong::header_yOptLong( yOption  *opx )
    : yOption( opx )
{
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
header_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--show"       )) { show       = 1; }

	else if ( is( "--gpio"       )) { gpio       = 1; }
	else if ( is( "--power"      )) { power      = 1; }
	else if ( is( "--row"        )) { row        = 1; }

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

    if ( get_argc() > 0 ) {
	if ( gpio || power || row ) {
	    Error::msg( "argv not valid with --gpio --power --row" ) <<endl;
	}
    }

    if ( row ) {
	if ( gpio || power ) {
	    Error::msg( "--row not valid with --gpio --power" ) <<endl;
	}
    }

    if ( *mode ) {
	ModifyBits = 1;
	try {
	    mode_e = rgFselPin::find_rgFsel_enum( mode );
	}
	catch ( range_error& e ) {
	    Error::msg( "unknown pin function select:  --mode=" ) << mode <<endl
		<< "    " <<  e.what() << endl;
	}

	if ( power ) {
	    Error::msg( "--mode not valid with --power" ) <<endl;
	}

	if ( row ) {
	    Error::msg( "--mode not valid with --row" ) <<endl;
	}

	if ( show ) {
	    Error::msg( "--mode not valid with --show" ) <<endl;
	}
    }
}


/*
* Show option flags.
*/
void
header_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--gpio        = " << gpio         << endl;
    cout << "--power       = " << power        << endl;
    cout << "--row         = " << row          << endl;

    cout << "--mode        = " << mode         << endl;
    cout << "--show        = " << show         << endl;

    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "mode_e        = " << rgFselPin::str_rgFsel_enum( mode_e ) << endl;
    cout << "ModifyBits    = " << ModifyBits   << endl;
}


/*
* Show usage.
*/
void
header_yOptLong::print_usage()
{
    cout <<
    "    Header Pin Functions, by pin number on 40-pin header\n"
    "usage:  " << ProgName << " header [options..]  [N..]\n"
    "  header pin number list:  (default all by pin number)\n"
    "    N                   header pin number 1..40\n"
    "    --gpio              gpio header pins\n"
    "    --power             power header pins\n"
    "    --row               all by row, odd pins first then even\n"
    "  modify:\n"
    "    --mode=In           set function mode {In, Out, Alt0, .. Alt5}\n"
    "  options:\n"
    "    --show              show all alternate functions\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


/*
* Output a trace message.
*    This function provides custom configuration of:
*    Option for conditional output.  Output stream used.
*    Argument text should have no prefix or suffix new-line.
* call:
*    trace_msg( "text" );
*/
void
header_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_header::y_header(
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
y_header::doit()
{

    try {
	header_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgFselPin		Fpx  ( AddrMap );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex;
	    cout << "+ FeatureAddr  = 0x"
		 <<setw(8) << Fpx.get_bcm_address() <<endl;
	    cout << "+ GpioBase     = "
		 <<setw(8) << (void*) Fpx.get_base_addr() <<endl;
	    cout <<dec;
	}

	const int		PinLimit = 64;
	int			pinlist[PinLimit + 1];	// pin numbers
	int			pincnt = 0;
	char			*arg;

    // Pin groups
	if ( Opx.get_argc() == 0 ) {

	    if ( Opx.gpio ) {
		for ( int k=1;  k<=40;  k++ )
		{
		    if ( rgHeaderPin::pin2gpio_int( k ) >= 0 ) {
			pinlist[pincnt++] = k;
		    }
		}
	    }

	    if ( Opx.power ) {
		for ( int k=1;  k<=40;  k++ )
		{
		    if ( rgHeaderPin::pin2gpio_int( k ) < 0 ) {
			pinlist[pincnt++] = k;
		    }
		}
	    }

	    if ( Opx.row && (pincnt == 0) ) {
		for ( int k=1;  k<=40;  k+=2 )	// odd
		{
		    pinlist[pincnt++] = k;
		}

		for ( int k=2;  k<=40;  k+=2 )	// even
		{
		    pinlist[pincnt++] = k;
		}
	    }

	    if ( pincnt == 0 ) {	// default, none of above
		for ( int k=1;  k<=40;  k++ )	// all pins
		{
		    pinlist[pincnt++] = k;
		}
	    }
	    // PinLimit implied by loop count logic
	}

    // Argument pin list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;

	    n = strtol( arg, NULL, 0 );
	    if ( (n < 1) || (n > 40) ) {
		Error::msg( "pin arg out-of-range {1..40}:  " ) << n <<endl;
		continue;
	    }

	    pinlist[pincnt] = n;
	    pincnt++;

	    if ( pincnt > PinLimit ) {
		Error::msg( "max pin args:  " ) << PinLimit <<endl;
		break;
	    }
	}

	if ( Error::has_err() )  return 1;

    // Show all alternate functions
	if ( Opx.show ) {

	    const rgFselPin::rgFsel_enum	mode_tab[] = {
		rgFselPin::f_Alt0,
		rgFselPin::f_Alt1,
		rgFselPin::f_Alt2,
		rgFselPin::f_Alt3,
		rgFselPin::f_Alt4,
		rgFselPin::f_Alt5,
	    };

	    cout <<dec << " Pin  Gpio   ";
	    for ( int jj=0;  jj<=5;  jj++ )		// heading
	    {
		cout << " " << setw(10) <<left
		     << Fpx.str_rgFsel_enum( mode_tab[jj] );
	    }
	    cout <<endl;

	    for ( int ii=0;  ii<pincnt;  ii++ )		// each pin
	    {
		int			pin;
		int			bit;
		rgFselPin::rgFsel_enum	mode;
		const char*		gpio_str;

		pin = pinlist[ii];
		bit = rgHeaderPin::pin2gpio_int( pin );

		gpio_str = rgHeaderPin::pin2name_cstr( pin );

		int pw = (pin & 0x1) ? 2 : 4;	// indent even pin numbers

		cout << " "  <<setw(pw) <<right << pin <<setw(4 - pw) << "";
		cout << " "  <<setw(7)  <<left  << gpio_str;
		for ( int jj=0;  jj<=5;  jj++ )		// each alt mode
		{
		    mode = mode_tab[jj];
		    cout << " " << setw(10) <<left
			 << (( bit < 0 ) ? "--" :
				rgAltFuncName::str_altfunc_bit( mode, bit ));
		}
		cout <<endl;
	    }

	    return 0;
	}

    // Modify Pins
	if ( Opx.ModifyBits ) {
	    int				bit;

	    Opx.trace_msg( "Modify pins" );
	    for ( int ii=0;  ii<pincnt;  ii++ )
	    {
		bit = rgHeaderPin::pin2gpio_int( pinlist[ii] );
		if ( bit >= 0 ) {
		    Fpx.modify_Fsel_bit( bit, Opx.mode_e );
		}
	    }
	}

    // Output Pins
	if ( pincnt > 0 ) {
	    cout << " Pin   Gpio    Mode  Function" <<endl;
	}

	for ( int ii=0;  ii<pincnt;  ii++ )
	{
	    int				pin;
	    int				bit;
	    rgFselPin::rgFsel_enum	mode;
	    const char*			gpio_str;
	    const char*			mode_str;
	    const char*			func_str;

	    pin  = pinlist[ii];
	    bit  = rgHeaderPin::pin2gpio_int( pin );

	    if ( bit >= 0 ) {
		mode = Fpx.read_Fsel_bit( bit );
		gpio_str = rgHeaderPin::pin2name_cstr( pin );
		mode_str = Fpx.str_rgFsel_enum( mode );
		func_str = rgAltFuncName::str_altfunc_bit( mode, bit );
	    } else {
		gpio_str = "--";
		mode_str = "--";
		func_str = rgHeaderPin::pin2name_cstr( pin );
	    }

	    int pw = (pin & 0x1) ? 2 : 4;	// indent even pin numbers

	    cout.fill(' ');
	    cout <<dec;
	    cout << " "  <<setw(pw) <<right << pin <<setw(4 - pw) << ""
		 << "  " <<setw(6)  <<left  << gpio_str
		 << "  " <<setw(4)  <<left  << mode_str
		 << "  "                    << func_str
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
}

