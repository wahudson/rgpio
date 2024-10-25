// 2024-10-17  William A. Hudson
//
// rgpio rstat -- IO Status RPi5 - RaspberryPi GPIO utility program.
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

#include "yUtil.h"
#include "y_rstat.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class rstat_yOptLong : public yOption {

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

    int			BankN    = 0;

    bool		tableF   = 1;	// output control
    bool		listF    = 0;

    bool		list     = 0;
    bool		all      = 0;

					// word write value
    yOpVal		norm;
    yOpVal		flip;
    yOpVal		set;
    yOpVal		clr;
					// read display flags
    bool		o_norm    = 0;
    bool		o_peek    = 0;
    bool		o_set     = 0;
    bool		o_clr     = 0;

    bool		Mr       = 0;	// modify register flag

    bool		verbose  = 0;
    bool		debug    = 0;
    bool		TESTOP   = 0;

  public:
    rstat_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_regindex(
				const char*	atom,
				const char*	name,
				int		index,
				uint32_t	vv );
    void		head_reg( const char* title );
    void		out_IoStat( rgsIoCon& cx );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    rstat_yOptLong	Opx  ( opx );
*/
rstat_yOptLong::rstat_yOptLong( yOption  *opx )
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
rstat_yOptLong::parse_options()
{

    while ( this->next() )
    {
	if      ( is( "--gpio="         )) { gpio.set(         val() ); }

	else if ( is( "--norm="      )) { norm.set(  val() ); Mr=1; }
	else if ( is( "--flip="      )) { flip.set(  val() ); Mr=1; }
	else if ( is( "--set="       )) { set.set(   val() ); Mr=1; }
	else if ( is( "--clr="       )) { clr.set(   val() ); Mr=1; }

	else if ( is( "--norm"       )) { o_norm     = 1; }
	else if ( is( "--peek"       )) { o_peek     = 1; }
	else if ( is( "--set"        )) { o_set      = 1; }
	else if ( is( "--clr"        )) { o_clr      = 1; }

	else if ( is( "--list"       )) { list       = 1; }
	else if ( is( "--all"        )) { all        = 1; }

	else if ( is( "-0"           )) { BankN      = 0; }
	else if ( is( "-1"           )) { BankN      = 1; }
	else if ( is( "-2"           )) { BankN      = 2; }

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

    if ( Mr || o_norm || o_peek || o_set || o_clr ) {
	listF  = 1;
	tableF = 1;
    }

    if ( list ) {	// turn OFF table output
	listF  = 1;
	tableF = 0;
    }
    else {
	tableF = 1;
    }

    if ( gpio.Val > 0x0fffffff ) {
	Error::msg( "require --gpio=mask <= 0x0fffffff" ) <<endl;
    }

    int		atomic_cnt = norm.Given + flip.Given + set.Given + clr.Given;

    if ( atomic_cnt > 1 ) {
	Error::msg( "require only one write atomic address --norm= .." ) <<endl;
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
	listF  = 1;
    }

    if ( gpio.Given && (get_argc() > 0) ) {
	Error::msg( "--gpio= not valid with Gpio bit numbers" ) <<endl;
    }

    if ( Mr && (get_argc() == 0) && (! gpio.Given) ) {
	Error::msg( "modify requires Gpio bit numbers" ) <<endl;
    }
}


/*
* Show option flags.
*/
void
rstat_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--gpio        = 0x" <<setw(8) << gpio.Val        << endl;

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
rstat_yOptLong::print_usage()
{
    cout <<
    "    IO Status interface (IoStat) - RPi5\n"
    "usage:  " << ProgName << " rstat [options..]  [gpio..]\n"
    "    gpio                bit numbers {27..0}\n"
    "    --gpio=0x0fffffff   mask to select Gpio[27:0] bits\n"
    "    -0, -1, -2          bank number, default -0\n"
    "  write atomic register address:  (one of)\n"
    "    --norm=0x000000ff   write normal               0x0000\n"
    "    --flip=0x000000ff   write atomic bitmask XOR   0x1000\n"
    "    --set=0x000000ff    write atomic bitmask set   0x2000\n"
    "    --clr=0x000000ff    write atomic bitmask clear 0x3000\n"
    "  read atomic address:  (show register list)\n"
    "    --norm              read normal (default)      0x0000\n"
    "    --peek              read without side-effect   0x1000\n"
    "    --set               read atomic set   address  0x2000\n"
    "    --clr               read atomic clear address  0x3000\n"
    "    --all               all above\n"
    "  options:\n"
    "    --list              list only atomic registers (no field table)\n"
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
rstat_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register index value in hex and binary.
* output e.g.:
* | Read Atomic register bit:           28   24   20   16   12    8    4    0
* |   0x00000000  norm  2.IoCntl( 0)  0000 0000 0000 0000 0000 0000 0000 0000
*/
void
rstat_yOptLong::out_regindex(
    const char*		atom,
    const char*		name,
    int			index,
    uint32_t		vv )
{
    cout.fill('0');
    cout << "   0x" <<hex <<right <<setw(8)  << vv;

    cout.fill(' ');
    cout << "  " <<left            << atom
	 << "  " <<left            << name
	 << "("  <<dec  <<setw(2)  <<right << index << ")"
	 << "  "                   << cstr_bits32( vv ) <<endl;

    cout <<dec <<right;		// restore defaults
}

void
rstat_yOptLong::head_reg( const char* title )
{
    cout <<setw(33) <<left << title
	 << "    28   24   20   16   12    8    4    0" <<endl;
    cout <<right;		// restore defaults
}


/*
* Output IoStat register table.
*/
void
rstat_yOptLong::out_IoStat( rgsIoCon& cx )
{
    int		bN = cx.get_bank_num();		// bank number {0,1,2}

    const int	kMax = 17;
    char	ww[kMax+1][50];
    int		jj = 0;		// output char position

    for ( int ii=27;  ii>=0;  ii-- )	// bit [27:0]
    {
	rgsIo_Stat	stat  = cx.IoStat(ii);

	stat.grab();

	ww[17][jj] = '0' + stat.get_IrqToProc_1();	// binary to ASCII
	ww[16][jj] = '0' + stat.get_IrqMasked_1();
	ww[15][jj] = '0' + stat.get_InFiltHigh_1();
	ww[14][jj] = '0' + stat.get_InFiltLow_1();
	ww[13][jj] = '0' + stat.get_InFiltRise_1();
	ww[12][jj] = '0' + stat.get_InFiltFall_1();
	ww[11][jj] = '0' + stat.get_InHigh_1();
	ww[10][jj] = '0' + stat.get_InLow_1();
	ww[ 9][jj] = '0' + stat.get_InRise_1();
	ww[ 8][jj] = '0' + stat.get_InFall_1();
	ww[ 7][jj] = '0' + stat.get_InToPeri_1();
	ww[ 6][jj] = '0' + stat.get_InFiltered_1();
	ww[ 5][jj] = '0' + stat.get_InOfPad_1();
	ww[ 4][jj] = '0' + stat.get_InIsDirect_1();
	ww[ 3][jj] = '0' + stat.get_OutEnToPad_1();
	ww[ 2][jj] = '0' + stat.get_OutEnOfPeri_1();
	ww[ 1][jj] = '0' + stat.get_OutToPad_1();
	ww[ 0][jj] = '0' + stat.get_OutOfPeri_1();

	jj++;

	if ( (ii & 0x3) == 0 ) {		// space between nibbles
	    for ( int k=kMax;  k>=0;  k-- ) {
		ww[k][jj] = ' ';
	    }
	    jj++;
	}
    }
    jj--;

    for ( int k=kMax;  k>=0;  k-- ) {		// terminate char strings
	ww[k][jj] = 0;
    }

    cout << " IoStat(i).norm       gpio i:  28   24   20   16   12    8    4    0"
	 <<endl;

    cout << "   " << bN << ".IrqToProc_1     [29]    ---- " << ww[17] <<endl;
    cout << "   " << bN << ".IrqMasked_1     [28]    ---- " << ww[16] <<endl;
    cout << "   " << bN << ".InFiltHigh_1    [27]    ---- " << ww[15] <<endl;
    cout << "   " << bN << ".InFiltLow_1     [26]    ---- " << ww[14] <<endl;
    cout << "   " << bN << ".InFiltRise_1    [25]    ---- " << ww[13] <<endl;
    cout << "   " << bN << ".InFiltFall_1    [24]    ---- " << ww[12] <<endl;
    cout << "   " << bN << ".InHigh_1        [23]    ---- " << ww[11] <<endl;
    cout << "   " << bN << ".InLow_1         [22]    ---- " << ww[10] <<endl;
    cout << "   " << bN << ".InRise_1        [21]    ---- " << ww[ 9] <<endl;
    cout << "   " << bN << ".InFall_1        [20]    ---- " << ww[ 8] <<endl;
    cout << "   " << bN << ".InToPeri_1      [19]    ---- " << ww[ 7] <<endl;
    cout << "   " << bN << ".InFiltered_1    [18]    ---- " << ww[ 6] <<endl;
    cout << "   " << bN << ".InOfPad_1       [17]    ---- " << ww[ 5] <<endl;
    cout << "   " << bN << ".InIsDirect_1    [16]    ---- " << ww[ 4] <<endl;
    cout << "   " << bN << ".OutEnToPad_1    [13]    ---- " << ww[ 3] <<endl;
    cout << "   " << bN << ".OutEnOfPeri_1   [12]    ---- " << ww[ 2] <<endl;
    cout << "   " << bN << ".OutToPad_1       [9]    ---- " << ww[ 1] <<endl;
    cout << "   " << bN << ".OutOfPeri_1      [8]    ---- " << ww[ 0] <<endl;
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_rstat::y_rstat(
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
y_rstat::doit()
{

    {
	rstat_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgsIoCon	Cx  ( AddrMap, Opx.BankN );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout << "+ " << Cx.get_bank_num() << ".FeatureAddr  = 0x"
		 <<hex <<setw(8) << Cx.get_doc_address() <<endl;
	    cout.fill(' ');
	    cout <<dec;
	}

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
	else {					// bits on command line
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

#define APPLY( X, Y ) if ( Opx.X.Given ) { Y( Opx.X.Val ); }

    // Modify
	if ( Opx.Mr ) {

	    Opx.trace_msg( "Write atomic registers" );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio   = bitarg[ii];
		rgsIo_Stat&	iostat = Cx.IoStat(gpio);

		if ( Opx.norm.Given ) {
		    iostat.write(      Opx.norm.Val ); }

		if ( Opx.flip.Given ) {
		    iostat.write_flip( Opx.flip.Val ); }

		if ( Opx.set.Given ) {
		    iostat.write_set(  Opx.set.Val  ); }

		if ( Opx.clr.Given ) {
		    iostat.write_clr(  Opx.clr.Val  ); }
	    }
	}

    // Output Table
	if ( Opx.tableF )
	{
	    Opx.trace_msg( "Read registers" );
	    Opx.out_IoStat( Cx );
	}

    // Output List
	if ( Opx.listF )
	{
	    int		bN      = Cx.get_bank_num();	// bank number {0,1,2}
	    char	rname[] = "0.IoStat";

	    rname[0] = '0' + bN;	// replace bank number char

	    Opx.trace_msg( "Read registers" );
	    Opx.head_reg( " Read Atomic register bit:  " );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio   = bitarg[ii];
		rgsIo_Stat&	iostat = Cx.IoStat(gpio);

		if ( Opx.o_norm ) {
		    Opx.out_regindex( "norm", rname, gpio, iostat.read()      );
		}

		if ( Opx.o_peek ) {
		    Opx.out_regindex( "peek", rname, gpio, iostat.read_peek() );
		}

		if ( Opx.o_set )  {
		    Opx.out_regindex( "set ", rname, gpio, iostat.read_set()  );
		}

		if ( Opx.o_clr )  {
		    Opx.out_regindex( "clr ", rname, gpio, iostat.read_clr()  );
		}
	    }
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

