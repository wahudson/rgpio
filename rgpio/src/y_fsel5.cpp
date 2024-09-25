// 2024-05-22  William A. Hudson
//
// rgpio fsel5 -- IO Function Select RPi5 - RaspberryPi GPIO utility program.
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
#include "rgsIoCon.h"
#include "rgsFuncName.h"

#include "yUtil.h"
#include "y_fsel5.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class fsel5_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

    yOpVal		gpio;		// mask Gpio bit select

    bool		Bank0    = 0;
    bool		Bank1    = 0;
    bool		Bank2    = 0;

    bool		tableF   = 1;	// output control
    bool		listF    = 1;

    bool		table    = 0;
    bool		list     = 0;
    bool		show     = 0;
					// IoCntl fields
    yOpVal		InOver_2;
    yOpVal		OutEnOver_2;
    yOpVal		OutOver_2;
    yOpVal		FilterM_7;
    yOpVal		FuncSel_5;

    bool		Md       = 0;	// modify flag
    bool		BinOut   = 1;

    bool		verbose  = 0;
    bool		debug    = 0;
    bool		TESTOP   = 0;

  public:
    fsel5_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		check_f2( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_IoCntl(  rgsIoCon& cx );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    fsel5_yOptLong	Opx  ( opx );
*/
fsel5_yOptLong::fsel5_yOptLong( yOption  *opx )
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
fsel5_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--gpio="          )) { gpio.set(          val() );}
	else if ( is( "--InOver_2="      )) { InOver_2.set(      val() ); Md=1;}
	else if ( is( "--OutEnOver_2="   )) { OutEnOver_2.set(   val() ); Md=1;}
	else if ( is( "--OutOver_2="     )) { OutOver_2.set(     val() ); Md=1;}
	else if ( is( "--FilterM_7="     )) { FilterM_7.set(     val() ); Md=1;}
	else if ( is( "--FuncSel_5="     )) { FuncSel_5.set(     val() ); Md=1;}
	else if ( is( "--func="          )) { FuncSel_5.set(     val() ); Md=1;}

	else if ( is( "--table"      )) { table      = 1; }
	else if ( is( "--list"       )) { list       = 1; }
	else if ( is( "--show"       )) { show       = 1; }

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

    if ( table && list ) {
	tableF = 1;
	listF  = 1;
    }
    else if ( table ) {		// table only
	tableF = 1;
	listF  = 0;
    }
    else if ( list  ) {		// list only
	tableF = 0;
	listF  = 1;
    }
    // default is both table and list

    if ( Md && show ) {
	Error::msg( "modification not valid with --show" ) <<endl;
    }

    check_f2( "InOver_2",      InOver_2.Val      );
    check_f2( "OutEnOver_2",   OutEnOver_2.Val   );
    check_f2( "OutOver_2",     OutOver_2.Val     );

    if (                       FilterM_7.Val > 0x7f  ) {
	Error::msg( "require --FilterM_7 <= 127:  " ) <<
			       FilterM_7.Val <<endl;
    }

    if (                       FuncSel_5.Val > 0x1f  ) {
	Error::msg( "require --FuncSel_5={0..31}:  " ) <<
			       FuncSel_5.Val <<endl;
    }

    if ( gpio.Given && (get_argc() > 0) ) {
	Error::msg( "--gpio= not valid with Gpio bit numbers" ) << endl;
    }

    if ( Md && (get_argc() == 0) && (! gpio.Given) ) {
	Error::msg( "modify requires Gpio bit numbers" ) << endl;
    }

    if ( Bank1 || Bank2 ) {
	Error::msg( "only Bank0 is supported" ) << endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
fsel5_yOptLong::check_f1(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 1 ) {
	Error::msg( "require --" ) << p << "={0,1}:  " << v << endl;
    }
}

/*
* Check option Field_2 value.
* call:
*    check_f1( "Field_2", Field_2.Val );
*/
void
fsel5_yOptLong::check_f2(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 0x3 ) {
	Error::msg( "require --" ) << p << " <= 3:  " << v << endl;
    }
}


/*
* Show option flags.
*/
void
fsel5_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--gpio        = 0x" <<setw(8) << gpio.Val        << endl;
    cout << "--FuncSel_5   = 0x" <<setw(8) << FuncSel_5.Val   << endl;

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
fsel5_yOptLong::print_usage()
{
    cout <<
    "    IO Function Select, RPi5\n"
    "usage:  " << ProgName << " fsel5 [options..]  [gpio..]\n"
    "    gpio                bit numbers {27..0}\n"
    "    --gpio=0x0fffffff   mask to select Gpio[27:0] bits\n"
//  "    -0, -1, -2          bank number, default -0\n"
    "  output format:\n"
    "    --table             table only, fields for each Gpio\n"
    "    --list              list only, function name for each Gpio\n"
    "  IoCntl(gpio) field modification:\n"
    "    --InOver_2=0        input override 0=norm, 1=invert, 2=low, 3=high\n"
    "    --OutEnOver_2=0     output enable override ..\n"
    "    --OutOver_2=0       output override ..\n"
    "    --FilterM_7=4       filter time constant M {0..127}\n"
    "    --FuncSel_5=31      function select {0..31}\n"
    "    --func=31           alias for --FuncSel_5\n"
    "  options:\n"
    "    --show              show all alternate functions\n"
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
fsel5_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output IoCntl register table.
*/
void
fsel5_yOptLong::out_IoCntl( rgsIoCon& cx )
{
    const int	kMax = 6;
    char	ww[kMax+1][40];
    uint32_t	vv;
    int		jj = 0;		// output char position

    for ( int ii=27;  ii>=0;  ii-- )	// bit [27:0]
    {
	rgsIo_Cntl	creg  = cx.IoCntl(ii);

	creg.grab();

	ww[ 6][jj] = '0' + creg.get_InOver_2();
	ww[ 5][jj] = '0' + creg.get_OutEnOver_2();
	ww[ 4][jj] = '0' + creg.get_OutOver_2();
	vv =               creg.get_FilterM_7();
	ww[ 3][jj] = hex2char( vv >>4 );
	ww[ 2][jj] = hex2char( vv );
	vv =               creg.get_FuncSel_5();
	ww[ 1][jj] = hex2char( vv >>4 );
	ww[ 0][jj] = hex2char( vv );

	jj++;

	if ( (ii & 0x3) == 0 ) {
	    for ( int k=kMax;  k>=0;  k-- ) {
		ww[k][jj] = ' ';
	    }
	    jj++;
	}
    }
    jj--;

    for ( int k=kMax;  k>=0;  k-- ) {
	ww[k][jj] = 0;
    }

    cout << " IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0"
	 <<endl;
    cout << "   InOver_2        [17:16] ---- " << ww[ 6] <<endl;
    cout << "   OutEnOver_2     [15:14] ---- " << ww[ 5] <<endl;
    cout << "   OutOver_2       [13:12] ---- " << ww[ 4] <<endl;
    cout << "   FilterM_7       [11:9]  ---- " << ww[ 3] <<endl;
    cout << "   FilterM_7       [ 8:5]  ---- " << ww[ 2] <<endl;
    cout << "   FuncSel_5       [4]     ---- " << ww[ 1] <<endl;
    cout << "   FuncSel_5       [3:0]   ---- " << ww[ 0] <<endl;
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_fsel5::y_fsel5(
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
y_fsel5::doit()
{

    {
	fsel5_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgsIoCon	Cx  ( AddrMap );	// constructor, Bank0

	const int	BitLimit = 32;
	uint32_t	bitarg[BitLimit];	// bit numbers
	int		bitcnt = 0;
	char		*arg;

    // Argument bit list
	if ( Opx.gpio.Given ) {
	    int		mask = Opx.gpio.Val;
	    bitcnt = 0;
	    for ( int ii=0;  ii<=27;  ii++ )
	    {
		if ( mask & 0x1 ) {
		    bitarg[bitcnt++] = ii;
		}
		mask >>= 1;
	    }
	}
	else if ( Opx.get_argc() <= 0 ) {	// default all bits
	    bitcnt = 0;
	    for ( int ii=0;  ii<=27;  ii++ )
	    {
		bitarg[bitcnt++] = ii;
	    }
	}
	else {				// bits on command line
	    while ( (arg = Opx.next_arg()) )
	    {
		int		n;
		char		*endptr;

		n = strtol( arg, &endptr, 10 );

		if ( *endptr != '\0' ) {
		    Error::msg( "bit arg non-numeric:  " ) << arg <<endl;
		    break;
		}

		if ( (n < 0) || (n > 27) ) {
		    Error::msg( "bit arg out-of-range:  " ) << n <<endl;
		    continue;
		}

		bitarg[bitcnt] = n;
		bitcnt++;

		if ( bitcnt > BitLimit ) {
		    Error::msg( "max Gpio bit args:  " ) << BitLimit <<endl;
		    break;
		}
	    }
	}

    if ( Error::has_err() )  return 1;

    // Show all alternate functions
	if ( Opx.show ) {
	    // Skip FuncSel_5={5,6,7} to make table fit in 80 columns.

	    cout << "     a5= sRIO[], a6= pRIO[], a7= PIO[]" <<endl;

	    cout <<dec << "Gpio";
	    for ( int jj=0;  jj<=8;  jj++ )		// heading
	    {
		if ( (5 <= jj) && (jj <= 7) ) { continue; }
		cout << " a" << setw(10) <<left << jj;
	    }
	    cout <<endl;

	    for ( int ii=0;  ii<bitcnt;  ii++ )		// each bit
	    {
		int		gpio = bitarg[ii];

		cout << " "  <<setw(2) <<right << gpio;
		cout << " ";

		for ( int jj=0;  jj<=8;  jj++ )		// each altnum
		{
		    if ( (5 <= jj) && (jj <= 7) ) { continue; }	// skip 5,6,7
		    cout << " " << setw(11) <<left
			 << rgsFuncName::cstr_altfuncAN( jj, gpio );
		}
		cout <<endl;
	    }
	    return 0;
	}

#define APPLY( X, Y ) if ( Opx.X.Given ) { Y( Opx.X.Val ); }

    // Modify

	if ( Opx.Md ) {
	    Opx.trace_msg( "Modify bits" );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio  = bitarg[ii];
		rgsIo_Cntl&	iocon = Cx.IoCntl(gpio);

		iocon.grab();

		APPLY( InOver_2,     iocon.put_InOver_2    )
		APPLY( OutEnOver_2,  iocon.put_OutEnOver_2 )
		APPLY( OutOver_2,    iocon.put_OutOver_2   )
		APPLY( FilterM_7,    iocon.put_FilterM_7   )
		APPLY( FuncSel_5,    iocon.put_FuncSel_5   )

		iocon.push();
	    }
	}

    // Output Table
	if ( Opx.tableF )
	{
	    Opx.out_IoCntl( Cx );
	}

    // Output List
	if ( Opx.listF )
	{
	    cout << " Gpio  Mode  Function" <<endl;

	    for ( int ii=0;  ii<bitcnt;  ii++ )		// each bit arg
	    {
		int		gpio = bitarg[ii];
		uint32_t	func;
		char		mode_str[5];
		const char*	funcname;
		rgsIo_Cntl&	iocon = Cx.IoCntl(gpio);

		       iocon.grab();
		func = iocon.get_FuncSel_5();

		sprintf( mode_str, "a%d", func );

		funcname = rgsFuncName::cstr_altfuncAN( func, gpio );

		cout.fill(' ');
		cout <<dec;
		cout << "  " <<setw(3) <<right << gpio
		     << "  " <<setw(4) <<right << mode_str
		     << "  "                   << funcname
		     <<endl;
	    }
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

