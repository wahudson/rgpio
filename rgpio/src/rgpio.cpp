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

#include "y_clock.h"
#include "y_fsel.h"
#include "y_io.h"
#include "y_uspi.h"

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

    const char*		dev;
    bool		ro;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:	// data values

    std::string		feature;

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
    dev         = "m";
    ro          = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    feature     = "";
}


/*
* Parse options.
*/
void
yOptLong::parse_options()
{
    while ( this->next() )
    {
	if      ( is( "--dev="       )) { dev        = this->val(); }
	else if ( is( "--ro"         )) { ro         = 1; }

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

    if ( this->get_argc() > 0 ) {
	feature = this->current_option();	// no advance for next level
    }

    if ( !((*dev == 'm') || (*dev == 'g') || (*dev == 'f')) ) {
	Error::msg( "require --dev=m|g|f" ) <<endl;
    }
}


/*
* Show option flags.
*/
void
yOptLong::print_option_flags()
{

    cout << "--dev         = " << dev          << endl;
    cout << "--ro          = " << ro           << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;
    cout << "feature       = " << feature      << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

}


/*
* Show usage.
*/
void
yOptLong::print_usage()
{
    cout <<
    "    Raspberry Pi GPIO utility\n"
    "usage:  " << ProgName << " [main_options..]  feature  [options..]\n"
    "  feature:\n"
    "    io           General Purpose IO pins\n"
    "    fsel         Pin Function Select\n"
    "    clock        Clock generator\n"
    "    uspi         Universal SPI Master, Spi1, Spi2\n"
    "  main options:\n"
    "    --dev=m|g|f         device file type, m= /dev/mem (default),\n"
    "                                          g= /dev/gpiomem, f= fake\n"
//  "  # --ro                read only\n"
    "    --help              show this usage\n"
//  "    -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
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

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgAddrMap		Amx;	// constructor

	Amx.config_FakeNoPi( 1 );	// when not on RPi
	Amx.config_Debug( Opx.debug );

	//#!! --ro
	if (      *Opx.dev == 'g' ) {
	    Amx.open_dev_gpiomem();
	}
	else if ( *Opx.dev == 'm' ) {
	    Amx.open_dev_mem();
	}
	else if ( *Opx.dev == 'f' ) {
	    Amx.open_fake_mem();
	}

	if ( Amx.is_fake_mem() && (Opx.verbose || Opx.debug) ) {
	    cout << "Using Fake memory" <<endl;
	}

	//#!! close_dev()

	int			retv = 0;	// return value

	if (      Opx.feature == "io"       ) {
	    y_io		iox  ( &Opx, &Amx );	// constructor
	    retv = iox.doit();
	}
	else if ( Opx.feature == "fsel"    ) {
	    y_fsel		fx  ( &Opx, &Amx );	// constructor
	    retv = fx.doit();
	}
	else if ( Opx.feature == "clock"    ) {
	    y_clock		cx  ( &Opx, &Amx );	// constructor
	    retv = cx.doit();
	}
	else if ( Opx.feature == "uspi"    ) {
	    y_uspi		usx  ( &Opx, &Amx );	// constructor
	    retv = usx.doit();
	}
	else if ( Opx.feature == ""         ) {
	    cout << "Do nothing.  Try '" << Opx.ProgName << " --help'" << endl;
	    // Useful to verify device file access.
	    retv = 0;
	}
	else {
	    Error::msg( "unknown feature:  " ) << Opx.feature.c_str() <<endl;
	    retv = 1;
	}

	return  retv;
    }
    catch ( std::exception& e ) {
	Error::msg( "exception caught:  " ) << e.what() <<endl;
    }
    catch (...) {
	Error::msg( "unexpected exception\n" );
    }

    return ( Error::has_err() ? 1 : 0 );
}

