// 2018-01-26  William A. Hudson
//
// rgpio uspi -- Universal SPI Master - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept
#include <string.h>	// strcmp()

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgUniSpi.h"

#include "y_uspi.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class uspi_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		spi_ch[3];	// SPI module number requested

    yOpVal		cntl0;
    yOpVal		cntl1;
    yOpVal		stat;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    uspi_yOptLong( yOption  *opx );	// constructor

    uint32_t		uint();

    void		parse_options();
    void		print_option_flags();
    void		print_usage();

    int			doit();		//#!! not here
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    uspi_yOptLong	Opx  ( opx );
*/
uspi_yOptLong::uspi_yOptLong( yOption  *opx )
    : yOption( opx )
{
    spi_ch[0]   = 0;
    spi_ch[1]   = 0;
    spi_ch[2]   = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Return arg as an uint32_t.
*/
uint32_t
uspi_yOptLong::uint()
{
    return  strtoul( this->val(), NULL, 0 );
}

/*
* Parse options.
*/
void
uspi_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--cntl0="     )) { cntl0.set( this->val() ); }
	else if ( is( "--cntl1="     )) { cntl1.set( this->val() ); }
	else if ( is( "--stat="      )) { stat.set(  this->val() ); }

	else if ( is( "-1"           )) { spi_ch[1]  = 1; }
	else if ( is( "-2"           )) { spi_ch[2]  = 1; }
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

//    if ( mash_n   > 3    ) {
//	Error::msg( "require --mash={0..3}:  "    ) << mash_n   <<endl;
//    }

}


/*
* Show option flags.
*/
void
uspi_yOptLong::print_option_flags()
{
    cout << "spi_ch[1]     = " << spi_ch[1]    << endl;
    cout << "spi_ch[2]     = " << spi_ch[2]    << endl;

    cout.fill('0');
    cout <<hex;
    cout << "--cntl0       = 0x" <<setw(8) << cntl0.Val       << endl;
    cout << "--cntl1       = 0x" <<setw(8) << cntl1.Val       << endl;
    cout << "--stat        = 0x" <<setw(8) << stat.Val        << endl;

    cout <<dec;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

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
uspi_yOptLong::print_usage()
{
    cout <<
    "    Universal SPI Master control\n"
    "usage:  " << ProgName << " uspi  -N..  [options..]\n"
    "    -N                  SPI number {1,2}\n"
    "  modify full register:\n"
    "    --cntl0=V           Control reg 0\n"
    "    --cntl1=V           Control reg 1\n"
    "    --stat=V            Status reg\n"
    "  modify bit fields:\n"
    "    --AuxEnable_1=B     Aux Enable\n"
    "  options:\n"
    "    --help              show this usage\n"
//  " #  -v, --verbose       verbose output\n"
    "    --debug             debug output\n"
    "  (options with GNU= only)\n"
    ;

// Hidden options:
//       --TESTOP       test mode show all options
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_uspi::y_uspi(
    yOption		*opx,
    rgAddrMap		*rgx
) {
    AddrMap = rgx;
    MainOpx = opx;
}
//#!! Note:  Passing in main program MainOpx as the base class yOption
// does not give access to all main program arguments.  Not quite what we want.


//--------------------------------------------------------------------------
// Main body
//--------------------------------------------------------------------------

int
y_uspi::doit()
{

    try {
	uspi_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // SPI channels
	const int		SpiMax = 2;
	rgUniSpi*		Spx[SpiMax+1] = {NULL, NULL, NULL};
				// pointers to Spi objects, NULL if not used.

	if ( Opx.spi_ch[1] ) {
	    Spx[1] = new  rgUniSpi  ( 1, AddrMap );	// constructor
	}

	if ( Opx.spi_ch[2] ) {
	    Spx[2] = new  rgUniSpi  ( 2, AddrMap );	// constructor
	}

	if ( Opx.debug ) {
	    for ( int ii=1;  ii<=SpiMax;  ii++ )
	    {
		rgUniSpi	*spi = Spx[ii];
		int		n;

		if ( spi == NULL ) {
		    continue;
		}
		n = spi->get_spi_num();

		// Note cout does not know how to show (volatile uint32_t*).

		cout << "    " << n << ".GpioBase=       "
		     <<hex << (uint32_t*) spi->get_base_addr() <<endl;

		cout << "    " << n << ".addr_AuxEnable= "
		     <<hex << (uint32_t*) spi->addr_AuxEnable() <<endl;

		cout << "    " << n << ".addr_Cntl0=     "
		     <<hex << (uint32_t*) spi->addr_Cntl0() <<endl;

		cout << "    " << n << ".diff_Cntl0=     "
		     <<hex
		     << (spi->addr_Cntl0() - spi->get_base_addr())*4 <<endl;

		cout <<dec <<endl;
	    }
	}


    // Process each clock

	for ( int ii=1;  ii<=SpiMax;  ii++ )
	{
	    rgUniSpi		*spi = Spx[ii];
	    int			n;
	    bool		md = 0;		// modify flag

	    spi = Spx[ii];

	    if ( spi == NULL ) {
		continue;
	    }

	    n   = spi->get_spi_num();

	    string		ns = "  " + std::to_string( n );

	    if ( Opx.debug ) {
		cout << "  Grab regs" <<endl;
	    }
	    spi->grab_regs();

	    if ( Opx.cntl0.Given ) { spi->put_Cntl0( Opx.cntl0.Val );  md = 1; }
	    if ( Opx.cntl1.Given ) { spi->put_Cntl1( Opx.cntl1.Val );  md = 1; }
	    if ( Opx.stat.Given  ) { spi->put_Stat(  Opx.stat.Val  );  md = 1; }

	    if ( md ) {			// modify registers
		if ( Opx.debug ) {
		    cout << "  Modify regs" <<endl;
		}
		spi->write_regs();

		if ( Opx.debug ) {
		    cout << "  Grab regs" <<endl;
		}
		spi->grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
		 << ns << ".Cntl0= 0x" <<setw(8) << spi->get_Cntl0() <<endl
		 << ns << ".Cntl1= 0x" <<setw(8) << spi->get_Cntl1() <<endl
		 << ns << ".Stat=  0x" <<setw(8) << spi->get_Stat() <<endl;

	    cout.fill(' ');
	    cout <<dec
		 << ns << ".Spi_Enable_1=  " << spi->read_Spi_Enable_1() <<endl
		 << ns << ".Speed_12=      " << spi->get_Speed_12()      <<endl
		 <<endl;

	}
	cout << "done" <<endl;

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

