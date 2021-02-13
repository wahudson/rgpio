// 2021-02-09  William A. Hudson
//
// rgpio pull -- IO Pin Pull Up/Down for RPi4
//--------------------------------------------------------------------------

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <stdexcept>	// std::stdexcept

using namespace std;

#include "Error.h"
#include "yOption.h"
#include "yOpVal.h"

#include "rgAddrMap.h"
#include "rgPullPin.h"

#include "yUtil.h"
#include "y_pull.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class pull_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		up   = 0;
    bool		down = 0;
    bool		off  = 0;

    bool		Word0 = 0;
    bool		Word1 = 0;

    yOpVal		w0;
    yOpVal		w1;
					// registers
    yOpVal		PullSel0;
    yOpVal		PullSel1;
    yOpVal		PullSel2;
    yOpVal		PullSel3;

    bool		BinOut = 1;	// binary output
    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    pull_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		do_rw(   const char* name,  rgReg_rw *rp );
    void		out_reg( const char* name,  uint32_t val );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    pull_yOptLong	Opx  ( opx );
*/
pull_yOptLong::pull_yOptLong( yOption  *opx )
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
pull_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--up"         )) { up         = 1; }
	else if ( is( "--down"       )) { down       = 1; }
	else if ( is( "--off"        )) { off        = 1; }

	else if ( is( "-0"           )) { Word0      = 1; }
	else if ( is( "-1"           )) { Word1      = 1; }

	else if ( is( "--w0="             )) { w0.set(             val() ); }
	else if ( is( "--w1="             )) { w1.set(             val() ); }

	else if ( is( "--PullSel0="       )) { PullSel0.set(       val() ); }
	else if ( is( "--PullSel1="       )) { PullSel1.set(       val() ); }
	else if ( is( "--PullSel2="       )) { PullSel2.set(       val() ); }
	else if ( is( "--PullSel3="       )) { PullSel3.set(       val() ); }

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

    if ( (up + down + off) > 1 ) {
	Error::msg( "require only one of:  --up --down --off\n" );
    }

    if ( get_argc() || w0.Given || w1.Given ) {
	if ( ! (up || down || off) ) {
	    Error::msg( "modify requires one of:  --up --down --off\n" );
	}
    }
    else {
	if ( up || down || off ) {
	    Error::msg( "modify requires bit arguments or:  --w0=V --w1=V\n" );
	}
    }

    if ( !(Word0 || Word1) ) {		// default
	Word0 = 1;
	Word1 = 1;
    }
    // Options act to show only the selected word, to provide focus.
}


/*
* Show option flags.
*/
void
pull_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--up          = " << up           << endl;
    cout << "--down        = " << down         << endl;
    cout << "--off         = " << off          << endl;
    cout << "-0            = " << Word0        << endl;
    cout << "-1            = " << Word1        << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    cout.fill('0');
    cout <<hex;
    cout << "--w0              = 0x" <<setw(8) << w0.Val              <<endl;
    cout << "--w1              = 0x" <<setw(8) << w1.Val              <<endl;
    cout << "--PullSel0        = 0x" <<setw(8) << PullSel0.Val        <<endl;
    cout << "--PullSel1        = 0x" <<setw(8) << PullSel1.Val        <<endl;
    cout << "--PullSel2        = 0x" <<setw(8) << PullSel2.Val        <<endl;
    cout << "--PullSel3        = 0x" <<setw(8) << PullSel3.Val        <<endl;
    cout <<std::dec;

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
pull_yOptLong::print_usage()
{
    cout <<
    "    IO pin Pull Up/Down for RPi4\n"
    "usage:  " << ProgName << " pull [options..]  [N..]\n"
    "    N                   modify Gpio bit number 0..57\n"
    "  modify pull direction:\n"
    "    --up                pull-up\n"
    "    --down              pull-down\n"
    "    --off               floating, disable both up and down\n"
    "  modify by word mask:\n"
    "    --w0=0xffffffff     word 0, Gpio[31:0]\n"
    "    --w1=0x03ffffff     word 1, Gpio[57:32]\n"
    "  raw registers:  (V= 32-bit value)\n"
    "    --PullSel0=V        Gpio[15:0]\n"
    "    --PullSel1=V        Gpio[31:16]\n"
    "    --PullSel2=V        Gpio[47:32]\n"
    "    --PullSel3=V        Gpio[57:48]\n"
    "  options:\n"
    "    -0, -1              show word 0, word 1\n"
    "    --help              show this usage\n"
    "    -v, --verbose       verbose output, show all registers\n"
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
pull_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register value in hex and binary.
*/
void
pull_yOptLong::out_reg( const char* name,  uint32_t vv )
{
    cout.fill('0');
    cout << "0x" <<std::hex <<setw(8)  << vv;

    cout.fill(' ');
    if ( this->BinOut ) {
	cout << "  " <<left <<setw(18) << name;
	cout << "  "                   << cstr_bits32( vv ) <<endl;
    }
    else {
	cout << "  "                   << name <<endl;
    }

    cout <<std::dec <<right;	// restore defaults
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_pull::y_pull(
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
y_pull::doit()
{

    try {
	pull_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgPullPin		Gpx  ( AddrMap );	// constructor

	rgPullPin::rgPull_enum	pud_dir;

	const int		BitLimit = 64;
	uint32_t		bitarg[BitLimit];	// bit numbers
	int			bitcnt = 0;
	char			*arg;
	bool			md = 0;			// modify flag

    // Pull direction
	if      ( Opx.up   ) { pud_dir = rgPullPin::pd_Up;   }
	else if ( Opx.down ) { pud_dir = rgPullPin::pd_Down; }
	else if ( Opx.off  ) { pud_dir = rgPullPin::pd_Off;  }

    // Argument bit list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;

	    n = strtol( arg, NULL, 0 );
	    if ( (n < 0) || (n > 57) ) {
		Error::msg( "bit arg out-of-range:  " ) << n <<endl;
		continue;
	    }

	    bitarg[bitcnt] = n;
	    bitcnt++;

	    if ( bitcnt > BitLimit ) {
		Error::msg( "max bit args:  " ) << BitLimit <<endl;
		break;
	    }
	}

	if ( Error::has_err() )  return 1;

    // Registers

#define APPLX( X ) if ( Opx.X.Given ) { Gpx.X.write( Opx.X.Val );  md = 1; }

	APPLX( PullSel0 )
	APPLX( PullSel1 )
	APPLX( PullSel2 )
	APPLX( PullSel3 )

	if ( md ) {		// after the actual modification is not ideal
	    Opx.trace_msg( "Apply registers" );
	}

    // Modify Words
	if ( Opx.w0.Given || Opx.w1.Given ) {
	    Opx.trace_msg( "Modify words" );
	}

	if ( Opx.w0.Given ) {
	    Gpx.modify_Pull_w0( Opx.w0.Val, pud_dir);
	}

	if ( Opx.w1.Given ) {
	    Gpx.modify_Pull_w1( Opx.w1.Val, pud_dir);
	}

    // Modify Bits
	if ( bitcnt ) {
	    Opx.trace_msg( "Modify bits" );
	}

	for ( int ii=0;  ii<bitcnt;  ii++ )
	{
	    Gpx.modify_Pull_bit( bitarg[ii], pud_dir );
	}

    // Output

	Opx.trace_msg( "Read registers" );
	// Note:  Registers are read multiple times, not atomic.

	cout << "  Pull Up/Down RPi4               28   24   20   16   12    8    4    0" << endl;

	// always show real registers
	Opx.out_reg( "PullSel0",    Gpx.PullSel0.read()    );
	Opx.out_reg( "PullSel1",    Gpx.PullSel1.read()    );
	Opx.out_reg( "PullSel2",    Gpx.PullSel2.read()    );
	Opx.out_reg( "PullSel3",    Gpx.PullSel3.read()    );

	if ( Opx.Word0 ) {
	    cout << "            w0_Direction        ";
	    for ( int i=31;  i>=0;  i-- )
	    {
		switch ( Gpx.read_Pull_bit( i ) ) {
		    case rgPullPin::pd_Up      : cout << '1';  break;
		    case rgPullPin::pd_Down    : cout << '0';  break;
		    case rgPullPin::pd_Off     : cout << 'z';  break;
		    case rgPullPin::pd_Unknown : cout << '?';  break;
		}
		if ( (i > 0) && !(i & 0x3) ) { cout << ' '; }
	    }
	    cout <<endl;
	}

	if ( Opx.Word1 ) {
	    cout << "            w1_Direction        ";
	    cout << ".... ..";
	    for ( int i=57;  i>=32;  i-- )
	    {
		switch ( Gpx.read_Pull_bit( i ) ) {
		    case rgPullPin::pd_Up      : cout << '1';  break;
		    case rgPullPin::pd_Down    : cout << '0';  break;
		    case rgPullPin::pd_Off     : cout << 'z';  break;
		    case rgPullPin::pd_Unknown : cout << '?';  break;
		}
		if ( (i > 32) && !(i & 0x3) ) { cout << ' '; }
	    }
	    cout <<endl;
	}

	// reduce clutter of alternate visualization, unless --verbose
	if ( Opx.verbose && Opx.Word0 ) {
	  cout <<endl;
	  Opx.out_reg( "w0_Up",      Gpx.read_Pull_w0( rgPullPin::pd_Up     ));
	  Opx.out_reg( "w0_Down",    Gpx.read_Pull_w0( rgPullPin::pd_Down   ));
	  Opx.out_reg( "w0_Off",     Gpx.read_Pull_w0( rgPullPin::pd_Off    ));
	  Opx.out_reg( "w0_Unknown", Gpx.read_Pull_w0( rgPullPin::pd_Unknown));
	}

	if ( Opx.verbose && Opx.Word1 ) {
	  cout <<endl;
	  Opx.out_reg( "w1_Up",      Gpx.read_Pull_w1( rgPullPin::pd_Up     ));
	  Opx.out_reg( "w1_Down",    Gpx.read_Pull_w1( rgPullPin::pd_Down   ));
	  Opx.out_reg( "w1_Off",     Gpx.read_Pull_w1( rgPullPin::pd_Off    ));
	  Opx.out_reg( "w1_Unknown", Gpx.read_Pull_w1( rgPullPin::pd_Unknown));
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

