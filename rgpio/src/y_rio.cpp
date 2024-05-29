// 2024-05-25  William A. Hudson
//
// rgpio rio -- Register Input/Output (RIO) RPi5 - RaspberryPi GPIO utility.
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
#include "rgsRio.h"

#include "yUtil.h"
#include "y_rio.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class rio_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		Bank0    = 0;
    bool		Bank1    = 0;
    bool		Bank2    = 0;

    bool		all      = 0;
					// register select
    bool		RioOut    = 0;
    bool		RioOutEn  = 0;
    bool		RioIn     = 0;
    bool		RioInSync = 0;
					// write value
    yOpVal		norm;
    yOpVal		flip;
    yOpVal		set;
    yOpVal		clr;
					// read display flags
    bool		o_norm    = 0;
    bool		o_peek    = 0;
    bool		o_set     = 0;
    bool		o_clr     = 0;

    bool		Md       = 0;	// modify flag
    bool		BinOut   = 1;	// no binary display

    bool		verbose  = 0;
    bool		debug    = 0;
    bool		TESTOP   = 0;

  public:
    rio_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_reg( const char* name,  uint32_t val );
    void		head_reg( const char* title );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    rio_yOptLong	Opx  ( opx );
*/
rio_yOptLong::rio_yOptLong( yOption  *opx )
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
rio_yOptLong::parse_options()
{

    while ( this->next() )
    {

	     if ( is( "--RioOut"     )) { RioOut     = 1; }
	else if ( is( "--RioOutEn"   )) { RioOutEn   = 1; }
	else if ( is( "--RioIn"      )) { RioIn      = 1; }
	else if ( is( "--RioInSync"  )) { RioInSync  = 1; }

	else if ( is( "--norm="      )) { norm.set(  val() ); Md=1;}
	else if ( is( "--flip="      )) { flip.set(  val() ); Md=1;}
	else if ( is( "--set="       )) { set.set(   val() ); Md=1;}
	else if ( is( "--clr="       )) { clr.set(   val() ); Md=1;}

	else if ( is( "--norm"       )) { o_norm     = 1; }
	else if ( is( "--peek"       )) { o_peek     = 1; }
	else if ( is( "--set"        )) { o_set      = 1; }
	else if ( is( "--clr"        )) { o_clr      = 1; }

	else if ( is( "--all"        )) { all        = 1; }
	else if ( is( "--hex"        )) { BinOut     = 0; }

	else if ( is( "-0"           )) { Bank0      = 1; }
	else if ( is( "-1"           )) { Bank1      = 1; }
	else if ( is( "-2"           )) { Bank2      = 1; }

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

    int		reg_cnt = RioOut + RioOutEn + RioIn + RioInSync;

    if ( !( RioOut || RioOutEn || RioIn || RioInSync ) ) {
	RioOut    = 1;
	RioOutEn  = 1;
	RioIn     = 1;
	RioInSync = 1;
    }

	// focus read on the atomic address written
    if ( norm.Given ) { o_norm = 1; }
    if ( flip.Given ) { o_peek = 1; }
    if ( set.Given  ) { o_set  = 1; }
    if ( clr.Given  ) { o_clr  = 1; }

    if ( !( o_norm || o_peek || o_set || o_clr ) ) {
	o_norm = 1;
    }

    if ( all ) {	// all atomic addresses, mostly debug
	o_norm = 1;
	o_peek = 1;
	o_set  = 1;
	o_clr  = 1;
    }

    if ( Md ) {
	if ( reg_cnt <= 0 ) {
	    Error::msg( "modify requires register option" ) << endl;
	}

	if ( reg_cnt > 1 ) {
	    Error::msg( "modify requires only one register option" ) << endl;
	}
    }

    if ( Bank1 || Bank2 ) {
	Error::msg( "only Bank0 is supported" ) << endl;
    }
}


/*
* Show option flags.
*/
void
rio_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--norm        = 0x" <<setw(8) << norm.Val         << endl;
    cout << "--flip        = 0x" <<setw(8) << flip.Val         << endl;

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
rio_yOptLong::print_usage()
{
    cout <<
    "    Register Input/Output (RIO), RPi5\n"
    "usage:  " << ProgName << " rio [options..]\n"
//  "    -0, -1, -2          bank number, default -0\n"
    "  select register:  (one for write, multiple for display)\n"
    "    --RioOut            output level\n"
    "    --RioOutEn          output enable\n"
    "    --RioIn             input direct from pad\n"
    "    --RioInSync         input synchronized to clk_sys\n"
    "  write atomic register address:\n"
    "    --norm=0x000000ff   write normal               0x0000\n"
    "    --flip=0x000000ff   write atomic bitmask XOR   0x1000\n"
    "    --set=0x000000ff    write atomic bitmask set   0x2000\n"
    "    --clr=0x000000ff    write atomic bitmask clear 0x3000\n"
    "  read atomic register address:\n"
    "    --norm              read normal (default)      0x0000\n"
    "    --peak              read without side-effect   0x1000\n"
    "    --set               read atomic set   address  0x2000\n"
    "    --clr               read atomic clear address  0x3000\n"
    "    --all               all above\n"
    "  options:\n"
    "    --hex               turn-off binary output\n"
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
rio_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register value in hex and binary.
*/
void
rio_yOptLong::out_reg( const char* name,  uint32_t vv )
{
    cout.fill('0');
    cout << "0x" <<std::hex <<setw(8)  << vv;

    cout.fill(' ');
    if ( this->BinOut ) {
	cout << "  " <<left <<setw(16) << name;
	cout << "  "                   << cstr_bits32( vv ) <<endl;
    }
    else {
	cout << "  "                   << name <<endl;
    }

    cout <<std::dec <<right;	// restore defaults
}

void
rio_yOptLong::head_reg( const char* title )
{
    if ( this->BinOut ) {
	cout <<setw(28) <<left << title
	     << "    28   24   20   16   12    8    4    0" <<endl;
    }
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_rio::y_rio(
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
y_rio::doit()
{

    {
	rio_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgsRio		Rx  ( AddrMap );	// constructor, Bank0

#define APPLY( R, M, W ) if ( Opx.R && Opx.M.Given ) { Rx.R.W( Opx.M.Val ); }

    // Modify registers
	if ( Opx.Md ) {
	    Opx.trace_msg( "Write register" );
	}

	APPLY( RioOut,    norm, write      )
	APPLY( RioOut,    flip, write_flip )
	APPLY( RioOut,    set,  write_set  )
	APPLY( RioOut,    clr,  write_clr  )

	APPLY( RioOutEn,  norm, write      )
	APPLY( RioOutEn,  flip, write_flip )
	APPLY( RioOutEn,  set,  write_set  )
	APPLY( RioOutEn,  clr,  write_clr  )

	APPLY( RioIn,     norm, write      )
	APPLY( RioIn,     flip, write_flip )
	APPLY( RioIn,     set,  write_set  )
	APPLY( RioIn,     clr,  write_clr  )

	APPLY( RioInSync, norm, write      )
	APPLY( RioInSync, flip, write_flip )
	APPLY( RioInSync, set,  write_set  )
	APPLY( RioInSync, clr,  write_clr  )

    // Output Table
	#define OUT_NORM( X, Y ) if ( Opx.X && Opx.o_norm ) \
	    { Opx.out_reg( Y, Rx.X.read() ); }

	#define OUT_PEEK( X, Y ) if ( Opx.X && Opx.o_peek ) \
	    { Opx.out_reg( Y, Rx.X.read_peek() ); }

	#define OUT_SET( X, Y )  if ( Opx.X && Opx.o_set ) \
	    { Opx.out_reg( Y, Rx.X.read_set() ); }

	#define OUT_CLR( X, Y )  if ( Opx.X && Opx.o_clr ) \
	    { Opx.out_reg( Y, Rx.X.read_clr() ); }

	Opx.trace_msg( "Read register" );

	Opx.head_reg( "  Register bit:" );

	OUT_NORM( RioOut,    "RioOut     norm" );
	OUT_PEEK( RioOut,    "RioOut     peek" );
	OUT_SET(  RioOut,    "RioOut     set " );
	OUT_CLR(  RioOut,    "RioOut     clr " );

	OUT_NORM( RioOutEn,  "RioOutEn   norm" );
	OUT_PEEK( RioOutEn,  "RioOutEn   peek" );
	OUT_SET(  RioOutEn,  "RioOutEn   set " );
	OUT_CLR(  RioOutEn,  "RioOutEn   clr " );

	OUT_NORM( RioIn,     "RioIn      norm" );
	OUT_PEEK( RioIn,     "RioIn      peek" );
	OUT_SET(  RioIn,     "RioIn      set " );
	OUT_CLR(  RioIn,     "RioIn      clr " );

	OUT_NORM( RioInSync, "RioInSync  norm" );
	OUT_PEEK( RioInSync, "RioInSync  peek" );
	OUT_SET(  RioInSync, "RioInSync  set " );
	OUT_CLR(  RioInSync, "RioInSync  clr " );
    }

    return ( Error::has_err() ? 1 : 0 );
}

