// 2017-06-16  William A. Hudson
//
// RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <unistd.h>	// getpagesize()

using namespace std;

#include "rgRpiRev.h"
#include "rgAddrMap.h"
#include "rgVersion.h"

#include "y_clk.h"
#include "y_fsel.h"
#include "y_fsel5.h"
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

#include "y_rio.h"
#include "y_rcon.h"
#include "y_rpad.h"

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
    bool		rpi3;
    bool		rpi4;
    bool		rpi5;
    bool		sim;

    bool		version;
    bool		verbose;
    bool		debug;
    bool		TESTOP;
    bool		TESTMODE;

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
    rpi3        = 0;
    rpi4        = 0;
    rpi5        = 0;
    sim         = 0;

    version     = 0;
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
    TESTMODE    = 0;

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
	else if ( is( "--rpi3"       )) { rpi3       = 1; }
	else if ( is( "--rpi4"       )) { rpi4       = 1; }
	else if ( is( "--rpi5"       )) { rpi5       = 1; }
	else if ( is( "--sim"        )) { sim        = 1; }
	else if ( is( "-n"           )) { sim        = 1; }

	else if ( is( "--version"    )) { version    = 1; }
	else if ( is( "--verbose"    )) { verbose    = 1; }
	else if ( is( "-v"           )) { verbose    = 1; }
	else if ( is( "--debug"      )) { debug      = 1; }
	else if ( is( "--TESTOP"     )) { TESTOP     = 1; }
	else if ( is( "--TESTMODE"   )) { TESTMODE   = 1; }
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

    if ( (rpi3 + rpi4 + rpi5) > 1 ) {
	Error::msg( "require only one:  --rpi3 --rpi4 --rpi5" ) <<endl;
    }
}


/*
* Show option flags.
*/
void
yOptLong::print_option_flags()
{

    cout << "--dev         = " << dev          << endl;
    cout << "--sim         = " << sim          << endl;
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
    "  common:\n"
    "    info         RPi Revision Information\n"
    "    man          reference manuals\n"
    "    header       Pin Function, by pin number on 40-pin header\n"
    "  RPi4 and earlier:\n"
    "    fsel         IO Function Select\n"
    "    io           General Purpose IO pins\n"
    "    clk          Clock generator\n"
    "    iic          I2C Master\n"
    "    pwm          PWM Pulse Width Modulator\n"
    "    spi0         SPI0 Master\n"
    "    timer        System Timer\n"
    "    uspi         Universal SPI Master, Spi1, Spi2\n"
    "    pads         Pads Control\n"
    "    pud          Pin Pull-Up/Down - RPi3 and earlier\n"
    "    pull         Pin Pull-Up/Down - RPi4 only\n"
    "  RPi5 only:\n"
    "    fsel5        IO Function Select\n"
    "    rio          Register IO\n"
    "    rcon         IO Control/Status\n"
    "    rpad         IO Pad control\n"
    "  main options:\n"
    "    --dev=m|g|f         device file type, m= /dev/mem (default),\n"
    "                                          g= /dev/gpiomem, f= fake\n"
    "    --rpi3              simulate RPi3 or earlier\n"
    "    --rpi4              simulate RPi4\n"
    "    --rpi5              simulate RPi5\n"
    "    -n, --sim           simulate appropriate platform (dry run)\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output, show if fake memory\n"
    "    --debug             debug output\n"
    "    --version           show version info\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
//       --TESTMODE     test mode
}


//--------------------------------------------------------------------------
// Main program
//--------------------------------------------------------------------------

// Define functions for each sub-command

#define DEFUNC( F, C )  int	 F( yOptLong *optx, rgAddrMap *amap ) {\
    C		rrx  ( optx, amap );\
    return  rrx.doit(); }

	int	rf_io( yOptLong *optx, rgAddrMap *amap ) {
	    y_io		rrx  ( optx, amap );
	    return  rrx.doit();
	}

      //DEFUNC( rf_io,         y_io         )
	DEFUNC( rf_fsel,       y_fsel       )
	DEFUNC( rf_header,     y_header     )
	DEFUNC( rf_clk,        y_clk        )
	DEFUNC( rf_iic,        y_iic        )
	DEFUNC( rf_info,       y_info       )
	DEFUNC( rf_man,        y_man        )
	DEFUNC( rf_pads,       y_pads       )
	DEFUNC( rf_pud,        y_pud        )
	DEFUNC( rf_pull,       y_pull       )
	DEFUNC( rf_pwm,        y_pwm        )
	DEFUNC( rf_spi0,       y_spi0       )
	DEFUNC( rf_timer,      y_timer      )
	DEFUNC( rf_uspi,       y_uspi       )

	DEFUNC( rf_fsel5,      y_fsel5      )
	DEFUNC( rf_rio,        y_rio        )
	DEFUNC( rf_rcon,       y_rcon       )
	DEFUNC( rf_rpad,       y_rpad       )

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

    // Table entry for each sub-command

	class   entry_t {
	  public:
	    int		RpiNum;		// 0= prior to RPi5
	    const char*	CmdName;
	    int		(*Pfunc) ( yOptLong *optx, rgAddrMap *amap );
	};

	static entry_t         functab[] = {
	    { 0, "io",       &rf_io         },
	    { 0, "fsel",     &rf_fsel       },
	    { 0, "header",   &rf_header     },
	    { 0, "clk",      &rf_clk        },
	    { 0, "iic",      &rf_iic        },
	    { 0, "info",     &rf_info       },
	    { 0, "man",      &rf_man        },
	    { 0, "pads",     &rf_pads       },
	    { 0, "pud",      &rf_pud        },
	    { 0, "pull",     &rf_pull       },
	    { 0, "pwm",      &rf_pwm        },
	    { 0, "spi0",     &rf_spi0       },
	    { 0, "timer",    &rf_timer      },
	    { 0, "uspi",     &rf_uspi       },
	    { 5, "fsel5",    &rf_fsel5      },
	    { 5, "rio",      &rf_rio        },
	    { 5, "rcon",     &rf_rcon       },
	    { 5, "rpad",     &rf_rpad       },
	    { -1, NULL,      NULL           }
	};

    // Search table for sub-command

	entry_t			*entry = functab;	// first table entry

	if ( Opx.feature != "" ) {

	    for ( ;  entry->CmdName != NULL;  entry++ )
	    {
		if ( Opx.feature == entry->CmdName ) { break; }
	    }

	    if ( entry->CmdName == NULL ) {
		Error::msg( "unknown feature:  " ) << Opx.feature.c_str() <<endl;
		return  1;
	    }
	} // defer handling null feature until after device file open

    // Configure rgRpiRev default and simulation modes

	if ( entry->RpiNum == 5 ) {
	    rgRpiRev::Global.SocEnum.defaultv( rgRpiRev::soc_BCM2712 );
	}
	// else leave default for RPi3

	if ( Opx.rpi3 ) {
	    rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2837 );
	}
	if ( Opx.rpi4 ) {
	    rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2711 );
	}
	if ( Opx.rpi5 ) {
	    rgRpiRev::Global.simulate_SocEnum( rgRpiRev::soc_BCM2712 );
	}

	if ( Opx.sim ) {
	    rgRpiRev::Global.simulate_RevCode( 0 );
	    // force zero to indicate simulation (same as constructor)

	    rgRpiRev::Global.simulate_SocEnum();
	    // force simulation of current SocEnum, RevCode not affected

	    // BaseAddr derives from SocEnum.
	}

	rgAddrMap		Amx;	// constructor

	Amx.config_FakeNoPi( 1 );	// when not on RPi
	Amx.config_Debug( Opx.debug );

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout <<hex
		<< "+ rgRpiRev::Global.SocEnum  = soc_" <<
		      rgRpiRev::Global.SocEnum.cstr() << endl
		<< "+ rgRpiRev::Global.BaseAddr = 0x" <<setw(8) <<
		      rgRpiRev::Global.BaseAddr.find() <<endl
		;
	    cout.fill(' ');
	    cout <<dec;
	}

	// Debug output for mmap() system call, is OS dependent.
	if ( Opx.debug && ! Opx.TESTMODE ) {
	    cout << "+ getpagesize()     = " << getpagesize()   <<endl;
	    cout << "+ sizeof( char* )   = " << sizeof( char* ) <<endl;
	    cout << "+ sizeof( off_t )   = " << sizeof( off_t ) <<endl;
	    cout << "+ sizeof( off64_t ) = " << sizeof( off64_t ) <<endl;
	}

	if (      *Opx.dev == 'g' ) {
	    Amx.open_dev_gpiomem();
	}
	else if ( *Opx.dev == 'm' ) {
	    Amx.open_dev_mem();
	}
	else if ( *Opx.dev == 'f' ) {
	    Amx.open_fake_mem();
	}

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout << "+ AddrMap.config_BaseAddr() = 0x" <<hex <<setw(8) <<
		    Amx.config_BaseAddr() <<endl;
	    cout.fill(' ');
	    cout <<dec;
	}

	if ( Amx.is_fake_mem() && (Opx.verbose || Opx.debug) ) {
	    cout << "Using Fake memory" <<endl;
	}

	// Note:  Device file cannot be closed until each feature has
	// mapped its memory block.  Fine to leave it open.

	if ( Opx.feature == "" ) {
	    cout << "Do nothing.  Try '" << Opx.ProgName << " --help'" << endl;
	    // Useful to verify device file access.
	    return  0;
	}

	int			retv = 0;	// return value

	retv = (entry->Pfunc) ( &Opx, &Amx );	// call sub-command function

	return  retv;
    }
    catch ( std::domain_error& e ) {
	Error::msg( "RPi platform:  " ) << e.what() <<endl;
    }
    catch ( std::invalid_argument& e ) {
	Error::msg( "invalid argument:  " ) << e.what() <<endl;
	// non-numeric option value error
    }
    catch ( std::exception& e ) {
	Error::msg( "exception caught:  " ) << e.what() <<endl;
    }
    catch (...) {
	Error::msg( "unexpected exception\n" );
    }

    return ( Error::has_err() ? 1 : 0 );
}

