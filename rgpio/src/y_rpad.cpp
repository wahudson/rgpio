// 2024-06-12  William A. Hudson
//
// rgpio rpad -- IO Pads RPi5 - RaspberryPi GPIO utility program.
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
#include "rgsIoPads.h"

#include "yUtil.h"
#include "y_rpad.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class rpad_yOptLong : public yOption {

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

					// IoPad fields
    yOpVal		OutDisable_1;
    yOpVal		InEnable_1;
    yOpVal		DriveStr_2;
    yOpVal		PullUpEn_1;
    yOpVal		PullDnEn_1;
    yOpVal		HystEn_1;
    yOpVal		SlewFast_1;

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

    bool		Md       = 0;	// modify field flag
    bool		Mr       = 0;	// modify register flag

    bool		verbose  = 0;
    bool		debug    = 0;
    bool		TESTOP   = 0;

  public:
    rpad_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_regindex(
				const char*	atom,
				const char*	name,
				int		index,
				uint32_t	vv );
    void		head_reg( const char* title );
    void		out_IoPads(  rgsIoPads& px );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    rpad_yOptLong	Opx  ( opx );
*/
rpad_yOptLong::rpad_yOptLong( yOption  *opx )
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
rpad_yOptLong::parse_options()
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

	else if ( is( "--OutDisable_1=" )) { OutDisable_1.set( val() ); Md=1;}
	else if ( is( "--InEnable_1="   )) { InEnable_1.set(   val() ); Md=1;}
	else if ( is( "--DriveStr_2="   )) { DriveStr_2.set(   val() ); Md=1;}
	else if ( is( "--PullUpEn_1="   )) { PullUpEn_1.set(   val() ); Md=1;}
	else if ( is( "--PullDnEn_1="   )) { PullDnEn_1.set(   val() ); Md=1;}
	else if ( is( "--HystEn_1="     )) { HystEn_1.set(     val() ); Md=1;}
	else if ( is( "--SlewFast_1="   )) { SlewFast_1.set(   val() ); Md=1;}

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

    check_f1( "OutDisable_1",  OutDisable_1.Val   );
    check_f1( "InEnable_1",    InEnable_1.Val     );
    check_f1( "PullUpEn_1",    PullUpEn_1.Val     );
    check_f1( "PullDnEn_1",    PullDnEn_1.Val     );
    check_f1( "HystEn_1",      HystEn_1.Val       );
    check_f1( "SlewFast_1",    SlewFast_1.Val     );

    if (                       DriveStr_2.Val > 0x3   ) {
	Error::msg( "require --DriveStr_2={0..3}:  "  ) <<
			       DriveStr_2.Val <<endl;
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

    if ( (Md || Mr) && (get_argc() == 0) && (! gpio.Given) ) {
	Error::msg( "modify requires Gpio bit numbers" ) <<endl;
    }

    if ( Md && (atomic_cnt > 0) ) {
	Error::msg( "field modification not valid with write atomic" ) <<endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
rpad_yOptLong::check_f1(
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
rpad_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--gpio        = 0x" <<setw(8) << gpio.Val        << endl;
    cout << "--HystEn_1    = 0x" <<setw(8) << HystEn_1.Val    << endl;
    cout << "--DriveStr_2  = 0x" <<setw(8) << DriveStr_2.Val  << endl;

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
rpad_yOptLong::print_usage()
{
    cout <<
    "    IO Pad Control (IoPad) - RPi5\n"
    "usage:  " << ProgName << " rpad [options..]  [gpio..]\n"
    "    gpio                bit numbers {27..0}\n"
    "    --gpio=0x0fffffff   mask to select Gpio[27:0] bits\n"
    "    -0, -1, -2          bank number, default -0\n"
    "  IoPad(gpio) field modification:\n"
    "    --OutDisable_1=0    output disable\n"
    "    --InEnable_1=0      input enable\n"
    "    --DriveStr_2=3      drive strength {0..3} = {2, 4, 8, 12 mA}\n"
    "    --PullUpEn_1=0      pull Up enable\n"
    "    --PullDnEn_1=0      pull Down enable\n"
    "    --HystEn_1=0        1= hysteresis enabled, 0= not\n"
    "    --SlewFast_1=0      1= max slew rate, 0= min\n"
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
rpad_yOptLong::trace_msg( const char* text )
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
rpad_yOptLong::out_regindex(
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
rpad_yOptLong::head_reg( const char* title )
{
    cout <<setw(32) <<left << title
	 << "    28   24   20   16   12    8    4    0" <<endl;
    cout <<right;		// restore defaults
}


/*
* Output IoPads register table.
*/
void
rpad_yOptLong::out_IoPads( rgsIoPads& px )
{
    int		bN = px.get_bank_num();		// bank number {0,1,2}

    const int	kMax = 6;
    char	ww[kMax+1][50];
    int		jj = 0;		// output char position

    for ( int ii=27;  ii>=0;  ii-- )	// bit [27:0]
    {
	rgsIo_Pad	preg  = px.IoPad(ii);

	preg.grab();

	ww[6][jj] = '0' + preg.get_OutDisable_1();	// binary to ASCII
	ww[5][jj] = '0' + preg.get_InEnable_1();
	ww[4][jj] = '0' + preg.get_DriveStr_2();
	ww[3][jj] = '0' + preg.get_PullUpEn_1();
	ww[2][jj] = '0' + preg.get_PullDnEn_1();
	ww[1][jj] = '0' + preg.get_HystEn_1();
	ww[0][jj] = '0' + preg.get_SlewFast_1();
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

    cout << " IoPad(i).norm        gpio i:  28   24   20   16   12    8    4    0"
	 <<endl;
    cout << "   " << bN << ".OutDisable_1    [7]     ---- " << ww[6] <<endl;
    cout << "   " << bN << ".InEnable_1      [6]     ---- " << ww[5] <<endl;
    cout << "   " << bN << ".DriveStr_2      [5:4]   ---- " << ww[4] <<endl;
    cout << "   " << bN << ".PullUpEn_1      [3]     ---- " << ww[3] <<endl;
    cout << "   " << bN << ".PullDnEn_1      [2]     ---- " << ww[2] <<endl;
    cout << "   " << bN << ".HystEn_1        [1]     ---- " << ww[1] <<endl;
    cout << "   " << bN << ".SlewFast_1      [0]     ---- " << ww[0] <<endl;
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_rpad::y_rpad(
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
y_rpad::doit()
{

    {
	rpad_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgsIoPads	Px  ( AddrMap, Opx.BankN );	// constructor

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout << "+ " << Px.get_bank_num() << ".FeatureAddr  = 0x"
		 <<hex <<setw(8) << Px.get_doc_address() <<endl;
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
		int		gpio  = bitarg[ii];
		rgsIo_Pad&	iopad = Px.IoPad(gpio);

		if ( Opx.norm.Given ) {
		    iopad.write(      Opx.norm.Val ); }

		if ( Opx.flip.Given ) {
		    iopad.write_flip( Opx.flip.Val ); }

		if ( Opx.set.Given )  {
		    iopad.write_set(  Opx.set.Val  ); }

		if ( Opx.clr.Given )  {
		    iopad.write_clr(  Opx.clr.Val  ); }
	    }
	}

	if ( Opx.Md ) {
	    Opx.trace_msg( "Modify registers field" );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio  = bitarg[ii];
		rgsIo_Pad&	iopad = Px.IoPad(gpio);

		iopad.grab();

		APPLY( OutDisable_1,  iopad.put_OutDisable_1  )
		APPLY( InEnable_1,    iopad.put_InEnable_1    )
		APPLY( DriveStr_2,    iopad.put_DriveStr_2    )
		APPLY( PullUpEn_1,    iopad.put_PullUpEn_1    )
		APPLY( PullDnEn_1,    iopad.put_PullDnEn_1    )
		APPLY( HystEn_1,      iopad.put_HystEn_1      )
		APPLY( SlewFast_1,    iopad.put_SlewFast_1    )

		iopad.push();
	    }
	}

    // Output Table
	if ( Opx.tableF )
	{
	    Opx.trace_msg( "Read registers" );
	    Opx.out_IoPads( Px );
	}

    // Output List
	if ( Opx.listF )
	{
	    int		bN      = Px.get_bank_num();	// bank number {0,1,2}
	    char	rname[] = "0.IoPad";

	    rname[0] = '0' + bN;	// replace bank number char

	    Opx.trace_msg( "Read registers" );
	    Opx.head_reg( " Read Atomic register bit:  " );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio  = bitarg[ii];
		rgsIo_Pad&	iopad = Px.IoPad(gpio);

		if ( Opx.o_norm ) {
		    Opx.out_regindex( "norm", rname, gpio, iopad.read()      );
		}

		if ( Opx.o_peek ) {
		    Opx.out_regindex( "peek", rname, gpio, iopad.read_peek() );
		}

		if ( Opx.o_set )  {
		    Opx.out_regindex( "set ", rname, gpio, iopad.read_set()  );
		}

		if ( Opx.o_clr )  {
		    Opx.out_regindex( "clr ", rname, gpio, iopad.read_clr()  );
		}
	    }
	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

