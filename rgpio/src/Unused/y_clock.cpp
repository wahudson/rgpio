// 2018-01-14  William A. Hudson
//
// rgpio clock -- Clock control - RaspberryPi GPIO utility program.
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
#include "rgClock.h"

#include "y_clock.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class clock_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		clk_ch[3];	// Clock channel number requested

    const char*		mash;
    const char*		flip;
    const char*		kill;
    const char*		enable;
    const char*		source;
    const char*		divi;
    const char*		divf;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

    uint32_t		mash_n;
    uint32_t		flip_n;
    uint32_t		kill_n;
    uint32_t		enable_n;
    uint32_t		source_n;
    uint32_t		divi_n;
    uint32_t		divf_n;

  public:
    clock_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    clock_yOptLong	Opx  ( opx );
*/
clock_yOptLong::clock_yOptLong( yOption  *opx )
    : yOption( opx )
{
    clk_ch[0]   = 0;
    clk_ch[1]   = 0;
    clk_ch[2]   = 0;

    mash        = "";
    flip        = "";
    kill        = "";
    enable      = "";
    source      = "";
    divi        = "";
    divf        = "";

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;

    mash_n      = 0;
    flip_n      = 0;
    kill_n      = 0;
    enable_n    = 0;
    source_n    = 0;
    divi_n      = 0;
    divf_n      = 0;
}


/*
* Parse options.
*/
void
clock_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--Mash_2="    )) { mash       = this->val(); }
	else if ( is( "--Flip_1="    )) { flip       = this->val(); }
	else if ( is( "--Kill_1="    )) { kill       = this->val(); }
	else if ( is( "--Enable_1="  )) { enable     = this->val(); }
	else if ( is( "--Source_4="  )) { source     = this->val(); }
	else if ( is( "--DivI_12="   )) { divi       = this->val(); }
	else if ( is( "--DivF_12="   )) { divf       = this->val(); }

	else if ( is( "-0"           )) { clk_ch[0]  = 1; }
	else if ( is( "-1"           )) { clk_ch[1]  = 1; }
	else if ( is( "-2"           )) { clk_ch[2]  = 1; }

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

    if ( !( clk_ch[0] | clk_ch[1] | clk_ch[2] ) ) {	// default
	clk_ch[0] = 1;
    }

    if ( *mash   ) { mash_n   = strtoul( mash,    NULL, 0 ); }
    if ( *flip   ) { flip_n   = strtoul( flip,    NULL, 0 ); }
    if ( *kill   ) { kill_n   = strtoul( kill,    NULL, 0 ); }
    if ( *enable ) { enable_n = strtoul( enable,  NULL, 0 ); }
    if ( *source ) { source_n = strtoul( source,  NULL, 0 ); }
    if ( *divi   ) { divi_n   = strtoul( divi,    NULL, 0 ); }
    if ( *divf   ) { divf_n   = strtoul( divf,    NULL, 0 ); }

    if ( mash_n   > 3    ) {
	Error::msg( "require --Mash_2={0..3}:  "     ) << mash_n   <<endl;
    }
    if ( flip_n   > 1    ) {
	Error::msg( "require --Flip_1={0,1}:  "      ) << flip_n   <<endl;
    }
    if ( kill_n   > 1    ) {
	Error::msg( "require --Kill_1={0,1}:  "      ) << kill_n   <<endl;
    }
    if ( enable_n > 1    ) {
	Error::msg( "require --Enable_1={0,1}:  "    ) << enable_n <<endl;
    }
    if ( source_n > 15   ) {
	Error::msg( "require --Source_4={0..15}:  "  ) << source_n <<endl;
    }
    if ( divi_n   > 4095 ) {
	Error::msg( "require --DivI_12={0..4095}:  " ) << divi_n   <<endl;
    }
    if ( divf_n   > 4095 ) {
	Error::msg( "require --DivF_12={0..4095}:  " ) << divf_n   <<endl;
    }

    if ( this->get_argc() > 0 ) {
	Error::msg( "extra arguments:  " ) << current_option() <<endl;
    }
}


/*
* Show option flags.
*/
void
clock_yOptLong::print_option_flags()
{
    cout << "clk_ch[0]     = " << clk_ch[1]    << endl;
    cout << "clk_ch[1]     = " << clk_ch[1]    << endl;
    cout << "clk_ch[2]     = " << clk_ch[1]    << endl;

    cout << "--Mash_2      = " << mash         << endl;
    cout << "--Flip_1      = " << flip         << endl;
    cout << "--Kill_1      = " << kill         << endl;
    cout << "--Enable_1    = " << enable       << endl;
    cout << "--Source_4    = " << source       << endl;
    cout << "--DivI_12     = " << divi         << endl;
    cout << "--DivF_12     = " << divf         << endl;

    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    char*	arg;
    while ( ( arg = next_arg() ) )
    {
	cout << "arg:          = " << arg          << endl;
    }

    cout << "mash_n        = " << mash_n       << endl;
    cout << "flip_n        = " << flip_n       << endl;
    cout << "kill_n        = " << kill_n       << endl;
    cout << "enable_n      = " << enable_n     << endl;
    cout << "source_n      = " << source_n     << endl;
    cout << "divi_n        = " << divi_n       << endl;
    cout << "divf_n        = " << divf_n       << endl;
}


/*
* Show usage.
*/
void
clock_yOptLong::print_usage()
{
    cout <<
    "    Clock generator control\n"
    "usage:  " << ProgName << " clock [-N..] [options..]\n"
    "    -N                  clock number {0,1,2}\n"
    "  modify:\n"
    "    --Mash_2=0          Mash mode {0..3}, 0= integer divison\n"
    "    --Flip_1=0          invert output, 1= invert, 0= normal\n"
    "    --Kill_1=0          1= stop and reset, 0= no action\n"
    "    --Enable_1=0        1= enable, 0= stop, without glitches\n"
    "    --Source_4=0        set clock source {0..15}\n"
    "    --DivI_12=N         set Divisior integer,  {0..4095}\n"
    "    --DivF_12=N         set Divisior fraction, {0..4095}, for MASH \n"
//  " #  --reset             reset registers to power-up value\n"
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

y_clock::y_clock(
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
y_clock::doit()
{

    try {
	clock_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // Clock number Arguments
	const int		ClkMax = 2;
	rgClock*		Cpx[ClkMax+1] = {NULL, NULL, NULL};
				// pointers to Clock objects, NULL if not used.

	for ( int ii=0;  ii<=ClkMax;  ii++ )
	{
	    if ( Opx.clk_ch[ii] ) {
		Cpx[ii] = new  rgClock  ( ii, AddrMap );	// constructor
	    }
	}
	// Note:  The rgAddrMap cache will provide the same map for each
	// clock object.
	//#!! destroy when done?

	if ( Opx.verbose ) {
	    cout << "Clock Controls:" << endl;
	}

	if ( Opx.debug ) {
	    for ( int ii=0;  ii<=ClkMax;  ii++ )
	    {
		rgClock		*clk = Cpx[ii];
		int		n;

		if ( clk == NULL ) {
		    continue;
		}
		n = clk->get_clock_num();

		// Note cout does not know how to show (volatile uint32_t*).

		cout << "    " << n << ".GpioBase=    "
		     <<hex << (uint32_t*) clk->get_base_addr() <<endl;

		cout << "    " << n << ".addr_CtlReg= "
		     <<hex << (uint32_t*) clk->addr_CtlReg() <<endl;

		cout << "    " << n << ".diff_CtlReg= "
		     <<hex
		     << (clk->addr_CtlReg() - clk->get_base_addr())*4 <<endl;

		cout <<dec <<endl;
	    }
	}

    // Process each clock

	for ( int ii=0;  ii<=ClkMax;  ii++ )
	{
	    rgClock		*clk;
	    int			n;
	    bool		md = 0;
	    bool		rv;

	    clk = Cpx[ii];

	    if ( clk == NULL ) {
		continue;
	    }

	    if ( Opx.debug ) {
		cout << "  Grab regs" <<endl;
	    }
	    clk->grab_regs();
	    n = clk->get_clock_num();

	    if ( *Opx.mash   ) { clk->put_Mash_2(   Opx.mash_n   );  md = 1; }
	    if ( *Opx.flip   ) { clk->put_Flip_1(   Opx.flip_n   );  md = 1; }
	    if ( *Opx.kill   ) { clk->put_Kill_1(   Opx.kill_n   );  md = 1; }
	    if ( *Opx.enable ) { clk->put_Enable_1( Opx.enable_n );  md = 1; }
	    if ( *Opx.source ) { clk->put_Source_4( Opx.source_n );  md = 1; }
	    if ( *Opx.divi   ) { clk->put_DivI_12(  Opx.divi_n   );  md = 1; }
	    if ( *Opx.divf   ) { clk->put_DivF_12(  Opx.divf_n   );  md = 1; }

	    if ( md ) {			// modify registers
		if ( Opx.debug ) {
		    cout << "  Modify regs" <<endl;
		}
		rv = clk->apply_regs();

		if ( Opx.debug ) {
		    cout << "    Busy wait count= " << clk->get_BusyCount()
			 <<endl;
		}
		if ( rv ) {
		    Error::msg( "rgClock::apply_regs() still busy\n" );
		}

		if ( Opx.debug ) {
		    cout << "  Grab regs" <<endl;
		}
		clk->grab_regs();
	    }

	    cout.fill('0');
	    cout <<hex
		 << "  " << n << ".CtlReg= 0x" <<setw(8) << clk->get_CtlReg()
		 <<endl
		 << "  " << n << ".DivReg= 0x" <<setw(8) << clk->get_DivReg()
		 <<endl;

	    cout.fill(' ');
	    cout <<dec
		 << "  " << n << ".Mash_2=   " << clk->get_Mash_2()    <<endl
		 << "  " << n << ".Flip_1=   " << clk->get_Flip_1()    <<endl
		 << "  " << n << ".Busy_1=   " << clk->get_Busy_1()    <<endl
		 << "  " << n << ".Kill_1=   " << clk->get_Kill_1()    <<endl
		 << "  " << n << ".Enable_1= " << clk->get_Enable_1()  <<endl
		 << "  " << n << ".Source_4= " << clk->get_Source_4()  <<endl
		 << "  " << n << ".DivI_12=  " << clk->get_DivI_12()   <<endl
		 << "  " << n << ".DivF_12=  " << clk->get_DivF_12()   <<endl
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

