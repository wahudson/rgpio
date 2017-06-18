// 2017-06-16  William A. Hudson
//
// RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>

using namespace std;

#include "rgAddrMap.h"
#include "rgIoPin.h"

#include "Error.h"
#include "yOption.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    const char*		npix;
    const char*		prefix;
    bool		hex;
    bool		raw;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    int			npix_n;			// number of pixels

  public:
    yOptLong( int argc,  char* argv[] );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

};


/*
* Constructor.  Init options with default values.
*    Pass in the main() argc and argv parameters.
* call:
*    yOptLong	Opx  ( argc, argv );
*    yOptLong	Opx = yOptLong::yOptLong( argc, argv );
*/
yOptLong::yOptLong( int argc,  char* argv[] )
    : yOption( argc, argv )
{
    npix        = "";
    prefix      = "";
    hex         = 0;
    raw         = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    npix_n      = 64;
}


/*
* Parse options.
*/
void
yOptLong::parse_options()
{
    while ( this->next() )
    {
	if      ( is( "--npix="      )) { npix       = this->val(); }
	else if ( is( "--prefix="    )) { prefix     = this->val(); }
	else if ( is( "--hex"        )) { hex        = 1; }
	else if ( is( "--raw"        )) { raw        = 1; }

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

    string	npix_s    ( npix );

    if ( npix_s.length() ) {
	npix_n = stoi( npix_s );
    }

    if ( get_argc() > 0 ) {
	Error::err( "extra arguments:  ", next_arg() );
    }
}


/*
* Show option flags.
*/
void
yOptLong::print_option_flags()
{

    cout << "--npix        = " << npix         << endl;
    cout << "--prefix      = " << prefix       << endl;
    cout << "--hex         = " << hex          << endl;
    cout << "--raw         = " << raw          << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "npix_n        = " << npix_n       << endl;
}


/*
* Show usage.
*/
void
yOptLong::print_usage()
{
    cout <<
    "    Raspberry Pi GPIO utility\n"
    "usage:  " << ProgName << " [options]\n"
    "  output forms:  (default is none)\n"
    "    --hex               hex data dump\n"
    "    --raw               raw hex data\n"
    "  options:\n"
    "    --npix=N            number of pixel to collect\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


//--------------------------------------------------------------------------
// yMain object class
//--------------------------------------------------------------------------

class yMain {
  private:

  public:
    yOptLong		*opx;

  public:
    yMain();					// constructor
    yMain( yOptLong *optx );			// constructor
};


/*
* Constructor.
*/
yMain::yMain()
{
}


/*
* Constructor.
*    Pass in reference to option object.
*    !! Probably refactor this class to build the option object.
*/
yMain::yMain( yOptLong *optx )
{
    opx          = optx;

}


//--------------------------------------------------------------------------
// Main program
//--------------------------------------------------------------------------

int
main( int	argc,
      char	*argv[]
) {

    try {
	yOptLong		Opx  ( argc, argv );	// constructor

	Opx.parse_options();

//	yMain			Mnx  ( &Opx );	// constructor

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::err() ? 1 : 0 );
	}

	if ( Error::err() )  return 1;

	rgAddrMap		Amx;	// constructor
	//#!! better name?

//	Amx.use_dev_gpiomem();
	Amx.use_fake_mem();

	rgIoPin			Gpx;	// constructor

	uint32_t		val;
	volatile uint32_t	*vp;

	Gpx.init_addr( &Amx );

	cout.fill( '0' );

	vp = Gpx.addr_reg( rgIoPin::rgEventStatus_w0 );
//	cout << "rgEventStatus_w0= 0x" <<hex <<setw(8) << (void*)vp <<endl;
	cout << "rgEventStatus_w0= " << (void*)vp <<endl;

	Gpx.mod_reg( rgIoPin::rgEventStatus_w0, 0xcccc3333, 0x00ffff00 );

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

