// 2017-06-16  William A. Hudson
//
// RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>

using namespace std;

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgVersion.h"

#include "y_clk.h"
#include "y_fsel.h"
#include "y_header.h"
#include "y_iic.h"
#include "y_info.h"
#include "y_io.h"
#include "y_man.h"
#include "y_pads.h"
#include "y_pud.h"
#include "y_pull.h"
#include "y_pwm.h"
#include "y_spi0.h"
#include "y_timer.h"
#include "y_uspi.h"

#include "Error.h"
#include "yOption.h"
#include "yVersion.h"


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
    bool		rpi3;
    bool		rpi4;

    bool		version;
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
    rpi3        = 0;
    rpi4        = 0;

    version     = 0;
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
	else if ( is( "--rpi3"       )) { rpi3       = 1; }
	else if ( is( "--rpi4"       )) { rpi4       = 1; }

	else if ( is( "--version"    )) { version    = 1; }
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

    if ( !( ((*dev == 'm') || (*dev == 'g') || (*dev == 'f')) &&
	    (*(dev+1) == '\0') ) ) {
	Error::msg( "require --dev=m|g|f" ) <<endl;
    }

    if ( rpi3 && rpi4 ) {
	Error::msg( "require only one:  --rpi3 --rpi4" ) <<endl;
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
    "    fsel         Pin Function Select, by Gpio bit number\n"
    "    header       Pin Function, by pin number on 40-pin header\n"
    "    clk          Clock generator\n"
    "    iic          I2C Master\n"
    "    info         RPi Revision Information\n"
    "    man          man pager\n"
    "    pads         Pads Control\n"
    "    pud          Pin Pull-Up/Down - RPi3 and earlier\n"
    "    pull         Pin Pull-Up/Down - RPi4\n"
    "    pwm          PWM Pulse Width Modulator\n"
    "    spi0         SPI0 Master\n"
    "    timer        System Timer\n"
    "    uspi         Universal SPI Master, Spi1, Spi2\n"
    "  main options:\n"
    "    --dev=m|g|f         device file type, m= /dev/mem (default),\n"
    "                                          g= /dev/gpiomem, f= fake\n"
//  "  # --ro                read only\n"
    "    --rpi3              act like RPi3 or earlier\n"
    "    --rpi4              act like RPi4 or later\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output, show if fake memory\n"
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

	if ( Opx.version ) {
	    cout << "rgpio version " <<  yVersion::get_Version() <<
		"  (librgpio "       << rgVersion::get_Version() << ")"
		<<endl;

	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	// Put rgAddrMap first to help validate default BaseAddr.
	rgAddrMap		Amx;	// constructor

	Amx.config_FakeNoPi( 1 );	// when not on RPi
	Amx.config_Debug( Opx.debug );

	if ( Opx.rpi3 ) {
	    rgRpiRev::Config.BaseAddr.find();	// config first
	    rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2837 );
	}

	if ( Opx.rpi4 ) {
	    rgRpiRev::Config.BaseAddr.find();	// config first
	    rgRpiRev::Config.SocEnum.put( rgRpiRev::soc_BCM2711 );
	}

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex
		<< "+ rgRpiRev::Config.SocEnum  = soc_" <<
		    rgRpiRev::soc_enum2cstr( rgRpiRev::find_SocEnum() ) <<endl
		<< "+ rgRpiRev::Config.BaseAddr = 0x" <<setw(8) <<
		    rgRpiRev::find_BaseAddr() <<endl
		;
	    cout.fill(' ');
	    cout <<dec;
	}

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

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout << "+ AddrMap.config_BaseAddr() = 0x" <<hex <<setw(8) <<
		    Amx.config_BaseAddr() <<endl;
	    cout.fill(' ');
	    cout <<dec;
	}

	// Note:  Device file cannot be closed until each feature has
	// mapped its memory block.  Fine to leave it open.

	int			retv = 0;	// return value

	if (      Opx.feature == "io"       ) {
	    y_io		iox  ( &Opx, &Amx );	// constructor
	    retv = iox.doit();
	}
	else if ( Opx.feature == "fsel"    ) {
	    y_fsel		fx  ( &Opx, &Amx );	// constructor
	    retv = fx.doit();
	}
	else if ( Opx.feature == "header"    ) {
	    y_header		fx  ( &Opx, &Amx );	// constructor
	    retv = fx.doit();
	}
	else if ( Opx.feature == "clk"     ) {
	    y_clk		cx  ( &Opx, &Amx );	// constructor
	    retv = cx.doit();
	}
	else if ( Opx.feature == "iic"     ) {
	    y_iic		icx  ( &Opx, &Amx );	// constructor
	    retv = icx.doit();
	}
	else if ( Opx.feature == "info"     ) {
	    y_info		inx  ( &Opx, &Amx );	// constructor
	    retv = inx.doit();
	}
	else if ( Opx.feature == "man"     ) {
	    y_man		fx  ( &Opx, &Amx );	// constructor
	    retv = fx.doit();
	}
	else if ( Opx.feature == "pads"     ) {
	    y_pads		pax  ( &Opx, &Amx );	// constructor
	    retv = pax.doit();
	}
	else if ( Opx.feature == "pud"     ) {
	    y_pud		pux  ( &Opx, &Amx );	// constructor
	    retv = pux.doit();
	}
	else if ( Opx.feature == "pull"     ) {
	    y_pull		pux  ( &Opx, &Amx );	// constructor
	    retv = pux.doit();
	}
	else if ( Opx.feature == "pwm"     ) {
	    y_pwm		pwx  ( &Opx, &Amx );	// constructor
	    retv = pwx.doit();
	}
	else if ( Opx.feature == "spi0"    ) {
	    y_spi0		usx  ( &Opx, &Amx );	// constructor
	    retv = usx.doit();
	}
	else if ( Opx.feature == "timer"    ) {
	    y_timer		usx  ( &Opx, &Amx );	// constructor
	    retv = usx.doit();
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

