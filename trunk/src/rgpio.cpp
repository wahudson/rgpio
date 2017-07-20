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

#include "y_io.h"

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
    dev         = "";
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
	    Error::err( "unknown option:  ", this->current_option() );
	}
    }

    if ( this->get_argc() > 0 ) {
	feature = this->current_option();	// no advance for next level
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
    "usage:  " << ProgName << " [common_opts..]  feature  [options..]\n"
    "  feature:\n"
    "    io           General Purpose IO pins\n"
    "    clock        Clock generator\n"
    "  common options:\n"
    "    --dev=m|g|f         device file type\n"
    "    --ro                read only\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
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
	    return ( Error::err() ? 1 : 0 );
	}

	if ( Error::err() )  return 1;

	rgAddrMap		Amx;	// constructor
	//#!! better name?

	//#!! --dev=  --ro
	Amx.use_dev_gpiomem();
//	Amx.use_fake_mem();

	if (      Opx.feature == "io"       ) {
	    cout << "do io" << endl;

	    y_io		iox  ( &Opx, &Amx );	// constructor
	    return  iox.doit();
	}
	else if ( Opx.feature == "clock"    ) {
	    cout << "do clock" << endl;
	    return( 0 );
	}
	else if ( Opx.feature == ""         ) {
	    cout << "do nothing" << endl;
	    return( 0 );
	}
	else {
	    Error::err( "unknown feature:  ", Opx.feature.c_str() );
	    return( 1 );
	}

    }
    catch ( std::exception& e ) {
	Error::err( "exception caught:  ", e.what() );
    }
    catch (...) {
	Error::err( "unexpected exception" );
    }

    return ( Error::err() ? 1 : 0 );
}

