// 2020-09-10  William A. Hudson
//
// rgpio io -- IO Pin Pull Up/Down - RaspberryPi GPIO utility program.
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
#include "rgIoPins.h"	// object registers
#include "rgPudPin.h"

#include "yUtil.h"
#include "y_pud.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class pud_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    bool		up;
    bool		down;
    bool		off;

    yOpVal		w0;
    yOpVal		w1;

    bool		BinOut     = 1;	// binary output
    bool		verbose;
    bool		debug;
    bool		TESTOP;

    bool		IsProgram;	// derived

  public:	// register args

    yOpVal		PudProgMode;
    yOpVal		PudProgClk_w0;
    yOpVal		PudProgClk_w1;

  public:
    pud_yOptLong( yOption  *opx );	// constructor

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
*    pud_yOptLong	Opx  ( opx );
*/
pud_yOptLong::pud_yOptLong( yOption  *opx )
    : yOption( opx )
{
    up          = 0;
    down        = 0;
    off         = 0;
    IsProgram   = 0;

    verbose     = 0;
    debug       = 0;
    TESTOP      = 0;
}


/*
* Parse options.
*/
void
pud_yOptLong::parse_options()
{
    while ( this->next() )
    {
	     if ( is( "--up"         )) { up         = 1; }
	else if ( is( "--down"       )) { down       = 1; }
	else if ( is( "--off"        )) { off        = 1; }

	else if ( is( "--w0="             )) { w0.set(             val() ); }
	else if ( is( "--w1="             )) { w1.set(             val() ); }

	else if ( is( "--PudProgMode="    )) { PudProgMode.set(    val() ); }
	else if ( is( "--PudProgClk_w0="  )) { PudProgClk_w0.set(  val() ); }
	else if ( is( "--PudProgClk_w1="  )) { PudProgClk_w1.set(  val() ); }

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
	IsProgram = 1;
	if ( ! (up || down || off) ) {
	    Error::msg( "program requires one of:  --up --down --off\n" );
	}
    }
    else {
	if ( up || down || off ) {
	    Error::msg( "program requires bit arguments or:  --w0=V --w1=V\n" );
	}
    }
}


/*
* Show option flags.
*/
void
pud_yOptLong::print_option_flags()
{
    // Beware namespace clash with 'hex'.

    cout << "--up          = " << up           << endl;
    cout << "--down        = " << down         << endl;
    cout << "--off         = " << off          << endl;
    cout << "--verbose     = " << verbose      << endl;
    cout << "--debug       = " << debug        << endl;

    cout.fill('0');
    cout <<hex;
    cout << "--w0              = 0x" <<setw(8) << w0.Val              <<endl;
    cout << "--w1              = 0x" <<setw(8) << w1.Val              <<endl;
    cout << "--PudProgMode     = 0x" <<setw(8) << PudProgMode.Val     <<endl;
    cout << "--PudProgClk_w0   = 0x" <<setw(8) << PudProgClk_w0.Val   <<endl;
    cout << "--PudProgClk_w1   = 0x" <<setw(8) << PudProgClk_w1.Val   <<endl;
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
pud_yOptLong::print_usage()
{
    cout <<
    "    IO pin Pull Up/Down\n"
    "usage:  " << ProgName << " pud [options..]  [N..]\n"
    "    N                   program bit number 0..53\n"
    "  program pull direction:\n"
    "    --up                pull-up\n"
    "    --down              pull-down\n"
    "    --off               floating, disable both up and down\n"
    "  program by IO bit mask:\n"
    "    --w0=0xffffffff     word 0, bits [31..0]\n"
    "    --w1=0x003fffff     word 1, bits [53..32]\n"
    "  modify raw registers:  (V= 32-bit value)\n"
    "    --PudProgMode=2     pull direction {2,1,0}\n"
    "    --PudProgClk_w0=V   clock mask, word 0\n"
    "    --PudProgClk_w1=V   clock mask, word 1\n"
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
pud_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register value in hex and binary.
*/
void
pud_yOptLong::out_reg( const char* name,  uint32_t vv )
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

y_pud::y_pud(
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
y_pud::doit()
{

    {
	pud_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgPudPin		Gpx  ( AddrMap );	// constructor

	rgPudPin::rgPud_enum	pud_dir;
	bool			fail;		// program fail status

	const int		BitLimit = 64;
	uint32_t		bitarg[BitLimit];	// bit numbers
	int			bitcnt = 0;
	char			*arg;

    // Pull direction
	if ( Opx.up   ) { pud_dir = rgPudPin::pd_Up;   }
	if ( Opx.down ) { pud_dir = rgPudPin::pd_Down; }
	if ( Opx.off  ) { pud_dir = rgPudPin::pd_Off;  }

    // Argument bit list
	while ( (arg = Opx.next_arg()) )
	{
	    int				n;

	    n = strtol( arg, NULL, 0 );
	    if ( (n < 0) || (n > 53) ) {
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


    // Program Words
	if ( Opx.w0.Given || Opx.w1.Given ) {
	    cout << " program words:  " << Gpx.pud_enum2cstr( pud_dir ) <<endl;
	}

	if ( Opx.w0.Given ) {
	    fail = Gpx.program_pud_w0( pud_dir, Opx.w0.Val );
	    cout.fill('0');
	    cout <<hex << "   w0= 0x" <<setw(8) << Opx.w0.Val
		 << "  " << ((fail) ? "fail" : "OK") <<endl;
	}

	if ( Opx.w1.Given ) {
	    fail = Gpx.program_pud_w1( pud_dir, Opx.w1.Val );
	    cout.fill('0');
	    cout <<hex << "   w1= 0x" <<setw(8) << Opx.w1.Val
		 << "  " << ((fail) ? "fail" : "OK") <<endl;
	}

    // Program Bits
	if ( bitcnt ) {
	    cout << " program bits:  " << Gpx.pud_enum2cstr( pud_dir ) <<endl;
	}

	for ( int ii=0;  ii<bitcnt;  ii++ )
	{
	    uint32_t		bit;

	    bit = bitarg[ii];

	    fail = Gpx.program_pud_bit( pud_dir, bit );

	    cout.fill(' ');
	    cout <<dec;
	    cout << "   " <<setw(2) << bit
		 << "  "  << ((fail) ? "fail" : "OK")
		 <<endl;
	}

    // Pud Registers
	if ( Opx.PudProgMode.Given   ||
	     Opx.PudProgClk_w0.Given ||
	     Opx.PudProgClk_w1.Given ) {
	    Opx.trace_msg( "Apply register values" );
	}

#define APPLX( X ) if ( Opx.X.Given ) { Gpx.X.write( Opx.X.Val ); }

	APPLX( PudProgMode    )
	APPLX( PudProgClk_w0  )
	APPLX( PudProgClk_w1  )

	if ( ! Opx.IsProgram ) {
	    Opx.trace_msg( "Read registers" );

	    if ( Opx.BinOut ) {
		cout << "Pud Pin Registers:                28   24   20   16   12    8    4    0" << endl;
	    }

	    Opx.out_reg( "PudProgMode",    Gpx.PudProgMode.read()    );
	    Opx.out_reg( "PudProgClk_w0",  Gpx.PudProgClk_w0.read()  );
	    Opx.out_reg( "PudProgClk_w1",  Gpx.PudProgClk_w1.read()  );
	}

    }

    return ( Error::has_err() ? 1 : 0 );
    //#!! return value?
}

