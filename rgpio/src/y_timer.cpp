// 2020-04-25  William A. Hudson
//
// rgpio timer -- System Timer - RaspberryPi GPIO utility program.
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
#include "rgSysTimer.h"

#include "y_timer.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class timer_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

					// registers
    yOpVal		Stat;
    yOpVal		TimeW1;
    yOpVal		TimeW0;
    yOpVal		Cmp3;
    yOpVal		Cmp2;
    yOpVal		Cmp1;
    yOpVal		Cmp0;

					// Stat fields
    yOpVal		Match3_1;
    yOpVal		Match2_1;
    yOpVal		Match1_1;
    yOpVal		Match0_1;

    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    timer_yOptLong( yOption  *opx );	// constructor

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
*    timer_yOptLong	Opx  ( opx );
*/
timer_yOptLong::timer_yOptLong( yOption  *opx )
    : yOption( opx )
{
    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
timer_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--Stat="      )) { Stat.set(     this->val() ); }
	else if ( is( "--TimeW1="    )) { TimeW1.set(   this->val() ); }
	else if ( is( "--TimeW0="    )) { TimeW0.set(   this->val() ); }
	else if ( is( "--Cmp3="      )) { Cmp3.set(     this->val() ); }
	else if ( is( "--Cmp2="      )) { Cmp2.set(     this->val() ); }
	else if ( is( "--Cmp1="      )) { Cmp1.set(     this->val() ); }
	else if ( is( "--Cmp0="      )) { Cmp0.set(     this->val() ); }

	else if ( is( "--Match3_1="  )) { Match3_1.set( this->val() ); }
	else if ( is( "--Match2_1="  )) { Match2_1.set( this->val() ); }
	else if ( is( "--Match1_1="  )) { Match1_1.set( this->val() ); }
	else if ( is( "--Match0_1="  )) { Match0_1.set( this->val() ); }

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

    check_f1( "Match3_1",        Match3_1.Val        );
    check_f1( "Match2_1",        Match2_1.Val        );
    check_f1( "Match1_1",        Match1_1.Val        );
    check_f1( "Match0_1",        Match0_1.Val        );

    if ( get_argc() > 0 ) {
	Error::msg( "extra arguments:  " ) << next_arg() << endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
timer_yOptLong::check_f1(
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
timer_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--Stat        = 0x" <<setw(8) << Stat.Val        << endl;
    cout << "--TimeW1      = 0x" <<setw(8) << TimeW1.Val      << endl;
    cout << "--TimeW0      = 0x" <<setw(8) << TimeW0.Val      << endl;
    cout << "--Cmp3        = 0x" <<setw(8) << Cmp3.Val        << endl;
    cout << "--Cmp2        = 0x" <<setw(8) << Cmp2.Val        << endl;
    cout << "--Cmp1        = 0x" <<setw(8) << Cmp1.Val        << endl;
    cout << "--Cmp0        = 0x" <<setw(8) << Cmp0.Val        << endl;

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
timer_yOptLong::print_usage()
{
    cout <<
    "    System Timer\n"
    "usage:  " << ProgName << " timer [options..]\n"
    "  modify registers:\n"
    "    --Stat=V            Status (read-clear)\n"
    "    --TimeW1=V          Counter word 1, high 32-bits (RO)\n"
    "    --TimeW0=V          Counter word 0, low  32-bits (RO)\n"
    "    --Cmp3=V            Compare to TimeW0\n"
    "    --Cmp2=V            ..\n"
    "    --Cmp1=V            ..\n"
    "    --Cmp0=V            ..\n"
    "  modify Stat bit fields:  (write 1 to clear)\n"
    "    --Match3_1=0        Cmp3 matched\n"
    "    --Match2_1=0        Cmp2 matched\n"
    "    --Match1_1=0        Cmp1 matched\n"
    "    --Match0_1=0        Cmp0 matched\n"
    "  options:\n"
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
timer_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_timer::y_timer(
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
y_timer::doit()
{

    try {
	timer_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // System Timer object

	rgSysTimer		Stx  ( AddrMap );	// constructor

	{
	    bool		md = 0;		// modify flag

	// Heading

	    Opx.trace_msg( "Grab regs" );
	    Stx.grab_regs();
	    Stx.Stat.put( 0 );		// read-clear

#define APPLY( X, Y ) if ( Opx.X.Given ) { Stx.Y( Opx.X.Val );  md = 1; }

	// Registers

	    APPLY( Stat,      Stat.put      )
	    APPLY( TimeW1,    TimeW1.put    )
	    APPLY( TimeW0,    TimeW0.put    )
	    APPLY( Cmp3,      Cmp3.put      )
	    APPLY( Cmp2,      Cmp2.put      )
	    APPLY( Cmp1,      Cmp1.put      )
	    APPLY( Cmp0,      Cmp0.put      )

	// Fields

	    APPLY( Match3_1,  Stat.put_Match3_1  )
	    APPLY( Match2_1,  Stat.put_Match2_1  )
	    APPLY( Match1_1,  Stat.put_Match1_1  )
	    APPLY( Match0_1,  Stat.put_Match0_1  )

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify regs" );
		Stx.Stat.push();
		Stx.TimeW1.push();
		Stx.TimeW0.push();
		Stx.Cmp3.push();
		Stx.Cmp2.push();
		Stx.Cmp1.push();
		Stx.Cmp0.push();
		// Stx.push_regs();

		Opx.trace_msg( "Grab regs" );
		Stx.grab_regs();
	    }

	// Output

	    cout.fill('0');
	    cout <<hex
		<< "   Stat      = 0x" <<setw(8) << Stx.Stat.get()     <<endl
		<< "   TimeW1    = 0x" <<setw(8) << Stx.TimeW1.get()   <<endl
		<< "   TimeW0    = 0x" <<setw(8) << Stx.TimeW0.get()   <<endl
		<< "   Cmp3      = 0x" <<setw(8) << Stx.Cmp3.get()     <<endl
		<< "   Cmp2      = 0x" <<setw(8) << Stx.Cmp2.get()     <<endl
		<< "   Cmp1      = 0x" <<setw(8) << Stx.Cmp1.get()     <<endl
		<< "   Cmp0      = 0x" <<setw(8) << Stx.Cmp0.get()     <<endl
		;

	    cout.fill(' ');
	    cout <<dec
		<< "   TimeDw    = " << Stx.TimeDw.get64()      <<endl
		<< " Stat" <<endl
		<< "   Match3_1  = " << Stx.Stat.get_Match3_1() <<endl
		<< "   Match2_1  = " << Stx.Stat.get_Match2_1() <<endl
		<< "   Match1_1  = " << Stx.Stat.get_Match1_1() <<endl
		<< "   Match0_1  = " << Stx.Stat.get_Match0_1() <<endl
		;
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

