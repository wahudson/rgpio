// 2019-12-15  William A. Hudson
//
// rgpio clk -- Clock control - RaspberryPi GPIO utility program.
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <sstream>	// std::ostringstream
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept
#include <string.h>	// strcmp()

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgClk.h"

#include "y_clk.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class clk_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values
					// clock name options, same as enum
    bool		cm_Clk0   = 0;
    bool		cm_Clk1   = 0;
    bool		cm_Clk2   = 0;
    bool		cm_ClkPcm = 0;
    bool		cm_ClkPwm = 0;
					// registers
    yOpVal		Cntl;
    yOpVal		Divr;

					// Cntl fields
    yOpVal		Mash_2;
    yOpVal		Flip_1;
    yOpVal		Busy_1;
    yOpVal		Kill_1;
    yOpVal		Enable_1;
    yOpVal		Source_4;

					// Divr fields
    yOpVal		DivI_12;
    yOpVal		DivF_12;

    bool		raw;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    clk_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    clk_yOptLong	Opx  ( opx );
*/
clk_yOptLong::clk_yOptLong( yOption  *opx )
    : yOption( opx )
{
    raw         = 0;
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
clk_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--Cntl="      )) { Cntl.set(     this->val() ); }
	else if ( is( "--Divr="      )) { Divr.set(     this->val() ); }

	else if ( is( "--Mash_2="          )) { Mash_2.set(          val() ); }
	else if ( is( "--Flip_1="          )) { Flip_1.set(          val() ); }
	else if ( is( "--Busy_1="          )) { Busy_1.set(          val() ); }
	else if ( is( "--Kill_1="          )) { Kill_1.set(          val() ); }
	else if ( is( "--Enable_1="        )) { Enable_1.set(        val() ); }
	else if ( is( "--Source_4="        )) { Source_4.set(        val() ); }

	else if ( is( "--DivI_12="         )) { DivI_12.set(         val() ); }
	else if ( is( "--DivF_12="         )) { DivF_12.set(         val() ); }

	else if ( is( "-0"           )) { cm_Clk0    = 1; }
	else if ( is( "-1"           )) { cm_Clk1    = 1; }
	else if ( is( "-2"           )) { cm_Clk2    = 1; }
	else if ( is( "--pcm"        )) { cm_ClkPcm  = 1; }
	else if ( is( "--pwm"        )) { cm_ClkPwm  = 1; }

	else if ( is( "--raw"        )) { raw        = 1; }
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

    if ( ! (
	cm_Clk0    ||
	cm_Clk1    ||
	cm_Clk2    ||
	cm_ClkPcm  ||
	cm_ClkPwm  )
    ) {
	cm_Clk0    = 1;	// default
    }

    if (                       Mash_2.Val > 0x3 ) {
	Error::msg( "require --Mash_2={0..3}:  " ) <<
			       Mash_2.Val <<endl;
    }

    check_f1( "Flip_1",          Flip_1.Val          );
    check_f1( "Busy_1",          Busy_1.Val          );
    check_f1( "Kill_1",          Kill_1.Val          );
    check_f1( "Enable_1",        Enable_1.Val        );

    if (                       Source_4.Val > 0xf ) {
	Error::msg( "require --Source_4={0..15}:  " ) <<
			       Source_4.Val <<endl;
    }

    if (                       DivI_12.Val > 0xfff ) {
	Error::msg( "require --DivI_12={0..4095}:  " ) <<
			       DivI_12.Val <<endl;
    }

    if (                       DivF_12.Val > 0xfff ) {
	Error::msg( "require --DivF_12={0..4095}:  " ) <<
			       DivF_12.Val <<endl;
    }

    if ( get_argc() > 0 ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Enable_1", Enable_1.Val );
*/
void
clk_yOptLong::check_f1(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 1 ) {
	Error::msg( "require --" ) << p << "={0,1}:  " << v << endl;
    }
}


/*
* Show option flags.
*/
void
clk_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout << "-0    = " << cm_Clk0    << endl;
    cout << "-1    = " << cm_Clk1    << endl;
    cout << "-2    = " << cm_Clk2    << endl;
    cout << "--pcm = " << cm_ClkPcm  << endl;
    cout << "--pwm = " << cm_ClkPwm  << endl;

    cout <<hex;
    cout << "--Cntl        = 0x" <<setw(8) << Cntl.Val        << endl;
    cout << "--Divr        = 0x" <<setw(8) << Divr.Val        << endl;

    cout <<dec;
    cout << "--Mash_2          = " << Mash_2.Val          << endl;
    cout << "--Flip_1          = " << Flip_1.Val          << endl;
    cout << "--Busy_1          = " << Busy_1.Val          << endl;
    cout << "--Kill_1          = " << Kill_1.Val          << endl;
    cout << "--Enable_1        = " << Enable_1.Val        << endl;
    cout << "--Source_4        = " << Source_4.Val        << endl;

    cout << "--DivI_12         = " << DivI_12.Val         << endl;
    cout << "--DivF_12         = " << DivF_12.Val         << endl;

    cout << "--raw         = " << raw          << endl;
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
*    Register fields ordered similar to BCM doc.
*/
void
clk_yOptLong::print_usage()
{
    cout <<
    "    Clock generator control\n"
    "usage:  " << ProgName << " clk [name..] [options..]\n"
    "  name:  (multiple ok)\n"
    "    -0, -1, -2          Clk0, Clk1, Clk2\n"
    "    --pcm               PCM clock manager\n"
    "    --pwm               PWM clock manager\n"
    "  modify:\n"
    "    --Cntl=0x5a000000   set control register\n"
    "    --Divr=0x5a000000   set divider register\n"
    "    --Mash_2=0          MASH mode {0..3}, 0= integer division\n"
    "    --Flip_1=0          1= invert output, 0= normal\n"
    "    --Busy_1=0          1= clock generator is running (RO)\n"
    "    --Kill_1=0          1= stop and reset, 0= no action\n"
    "    --Enable_1=0        1= enable, 0= stop\n"
    "    --Source_4=0        set clock source {0..15}\n"
    "    --DivI_12=N         set divisor integer,  {0..4095}\n"
    "    --DivF_12=N         set divisor fraction, {0..4095}, for MASH \n"
    "  options:\n"
    "    --raw               no disable before modification\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output\n"
//  "    --debug             debug output\n"
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
clk_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_clk::y_clk(
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
y_clk::doit()
{

    {
	clk_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // Clk objects
	const int	ClkMax = 4;
	rgClk*		Cpx[ClkMax+1] = {NULL, NULL, NULL, NULL, NULL};
			// pointers to Clock objects, NULL if not used.

	if (                                   Opx.cm_Clk0    ) {
	    Cpx[0] = new  rgClk  ( AddrMap, rgClk::cm_Clk0 );	// constructor
	}

	if (                                   Opx.cm_Clk1    ) {
	    Cpx[1] = new  rgClk  ( AddrMap, rgClk::cm_Clk1 );	// constructor
	}

	if (                                   Opx.cm_Clk2    ) {
	    Cpx[2] = new  rgClk  ( AddrMap, rgClk::cm_Clk2 );	// constructor
	}

	if (                                   Opx.cm_ClkPcm  ) {
	    Cpx[3] = new  rgClk  ( AddrMap, rgClk::cm_ClkPcm );	// constructor
	}

	if (                                   Opx.cm_ClkPwm  ) {
	    Cpx[4] = new  rgClk  ( AddrMap, rgClk::cm_ClkPwm );	// constructor
	}

	// Note:  The rgAddrMap cache will provide the same map for each
	// clock object.

    // Process each clock
	for ( int ii=0;  ii<=ClkMax;  ii++ )
	{
	    bool		md = 0;		// modify flag
	    rgClk		*clx;
	    const char*		cs;		// clk name string

	    clx = Cpx[ii];

	    if ( clx == NULL ) {
		continue;
	    }

	// Heading
	    cs = clx->cstr_clk_enum( clx->get_clk_enum() );

	    cout << cs << ":" << endl;		// full clock name

	    Opx.trace_msg( "Grab regs" );
	    clx->grab_regs();

#define APPLY( X, Y ) if ( Opx.X.Given ) { clx->Y( Opx.X.Val );  md = 1; }

	// Registers

	    APPLY( Cntl,      Cntl.put           )
	    APPLY( Divr,      Divr.put           )

	// Fields

	    APPLY( Mash_2,    Cntl.put_Mash_2    )
	    APPLY( Flip_1,    Cntl.put_Flip_1    )
	    APPLY( Busy_1,    Cntl.put_Busy_1    )
	    APPLY( Kill_1,    Cntl.put_Kill_1    )
	    APPLY( Enable_1,  Cntl.put_Enable_1  )
	    APPLY( Source_4,  Cntl.put_Source_4  )

	    APPLY( DivI_12,   Divr.put_DivI_12   )
	    APPLY( DivF_12,   Divr.put_DivF_12   )

	    // Use defaults to help validate library.
	    // clx->wait_time_ns( 0 );
	    // clx->wait_count_n( 10000 );

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify regs" );
		if ( Opx.raw ) {
		    clx->Cntl.apply();
		    clx->Divr.apply();
		}
		else {
		    bool	busy;
		    busy = clx->apply_nicely();
		    if ( busy ) {
			Error::msg( "registers not set, clock busy:  " ) <<
			"BusyCount= " << clx->get_busy_count() <<endl;
		    }
		    if ( Opx.verbose ) {
			cout << "+ BusyCount= " << clx->get_busy_count() <<endl;
		    }
		}

		Opx.trace_msg( "Grab regs" );
		clx->grab_regs();
	    }

	// Output

	    string		cp  ( "   " );	// output prefix

	    // prefix clock number as an integer, looks clean
	    cp += to_string( clx->get_clk_enum() ) + ".";

	    cout.fill('0');
	    cout <<hex
		<< cp << "Cntl    = 0x" <<setw(8) << clx->Cntl.get()     <<endl
		<< cp << "Divr    = 0x" <<setw(8) << clx->Divr.get()     <<endl
		;

	    cout.fill(' ');
	    cout <<dec
		// no Passwd_8, is write only
		<< cp << "Mash_2    = " << clx->Cntl.get_Mash_2()   <<endl
		<< cp << "Flip_1    = " << clx->Cntl.get_Flip_1()   <<endl
		<< cp << "Busy_1    = " << clx->Cntl.get_Busy_1()   <<endl
		<< cp << "Kill_1    = " << clx->Cntl.get_Kill_1()   <<endl
		<< cp << "Enable_1  = " << clx->Cntl.get_Enable_1() <<endl
		<< cp << "Source_4  = " << clx->Cntl.get_Source_4() <<endl
		<< cp << "DivI_12   = " << clx->Divr.get_DivI_12()  <<endl
		<< cp << "DivF_12   = " << clx->Divr.get_DivF_12()  <<endl
		;
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

