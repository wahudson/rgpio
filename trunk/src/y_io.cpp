// 2017-06-18  William A. Hudson
//
// rgpio io -- IO pins - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>

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
    const char*		value;
    const char*		mask;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    int			reg_n;
    uint32_t		mask_n;

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
    value       = "";
    mask        = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    reg_n       = 0;
    mask_n      = 0;
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
	else if ( is( "--value="     )) { value      = this->val(); }
	else if ( is( "--mask="      )) { mask       = this->val(); }

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

    string	mask_s  ( mask );

    if ( mask_s.length() ) {
	mask_n = stoi( mask_s );
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

    cout << "--hex         = " << hex          << endl;
    cout << "--bin         = " << bin          << endl;
    cout << "--reg         = " << reg          << endl;
    cout << "--value       = " << value        << endl;
    cout << "--mask        = " << mask         << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "mask_n        = " << mask_n       << endl;
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
    "  output forms:  (default is --col)\n"
    "    -c,--col            show one bit per line\n"
    "    --hex               word format hexadecimal\n"
    "    --bin               word format binary\n"
    "  modify:\n"
    "    --reg=R             register to access, R= byte offset or enum\n"
    "    --value=0x00        bit value, 32-bits\n"
    "    --mask=0xff         mask select bits to change, 32-bits\n"
    "  options:\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
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
	volatile uint32_t	*vp;

	Gpx.init_addr( AddrMap );

	cout.fill( '0' );

	cout << "rgEventStatus_w0 enum= 0x"
	    <<hex << rgIoPin::rgEventStatus_w0 <<endl;

	cout << "addr_reg()" <<endl;
	vp = Gpx.addr_reg( rgIoPin::rgEventStatus_w0 );
//	cout << "rgEventStatus_w0= 0x" <<hex <<setw(8) << (void*)vp <<endl;
	cout << "rgEventStatus_w0= " << (void*)vp <<endl;

	cout << "read_reg()" <<endl;
	val = Gpx.read_reg( rgIoPin::rgEventStatus_w0 );
	cout << "rgEventStatus_w0= 0x" <<hex <<setw(8) << val <<endl;

	cout << "read_reg()" <<endl;
	val = Gpx.read_reg( rgIoPin::rgPinLevel_w0 );
	cout << "rgPinLevel_w0= 0x" <<hex <<setw(8) << val <<endl;

	cout << "mod_reg()" <<endl;
	Gpx.mod_reg( rgIoPin::rgEventStatus_w0, 0xcccc3333, 0x00ffff00 );

	cout << "read_reg()" <<endl;
	val = Gpx.read_reg( rgIoPin::rgEventStatus_w0 );
	cout << "rgEventStatus_w0= 0x" <<hex <<setw(8) << val <<endl;

	if ( Opx.debug ) {
//	    cout << "    gpio_read= " << (unsigned *)gpio_read << endl;
	}

    }
    catch ( std::exception& e ) {
//	cerr << "Error:  exception:  " << e.what() <<endl;
	Error::err( "exception caught:  ", e.what() );
    }
    catch (...) {
	Error::err( "unexpected exception" );
    }

    return ( Error::err() ? 1 : 0 );
}

