// 2024-10-11  William A. Hudson
//
// rgpio rcon -- IO Control RPi5 - RaspberryPi GPIO utility program.
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
#include "y_rcon.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class rcon_yOptLong : public yOption {

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
    bool		listF    = 0;

    bool		list     = 0;
    bool		all      = 0;

					// Register select
    bool		IoCntl   = 0;
    bool		IoStat   = 0;

					// IoCntl fields
    yOpVal		IrqOver_2;
    yOpVal		EdgeReset_1;
    yOpVal		ImaskFiltHigh_1;
    yOpVal		ImaskFiltLow_1;
    yOpVal		ImaskFiltRise_1;
    yOpVal		ImaskFiltFall_1;
    yOpVal		ImaskHigh_1;
    yOpVal		ImaskLow_1;
    yOpVal		ImaskRise_1;
    yOpVal		ImaskFall_1;
    yOpVal		InOver_2;
    yOpVal		OutEnOver_2;
    yOpVal		OutOver_2;
    yOpVal		FilterM_7;
    yOpVal		FuncSel_5;

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
    rcon_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		check_f2( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_regindex( const char* name, int index, uint32_t vv );
    void		head_reg( const char* title );
    void		out_IoCntl( rgsIoCon& cx );
    void		out_IoStat( rgsIoCon& cx );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    rcon_yOptLong	Opx  ( opx );
*/
rcon_yOptLong::rcon_yOptLong( yOption  *opx )
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
rcon_yOptLong::parse_options()
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

	else if ( is( "--IoCntl"     )) { IoCntl     = 1; }
	else if ( is( "-c"           )) { IoCntl     = 1; }
	else if ( is( "--IoStat"     )) { IoStat     = 1; }
	else if ( is( "-s"           )) { IoStat     = 1; }

	else if ( is( "-0"           )) { Bank0      = 1; }
	else if ( is( "-1"           )) { Bank1      = 1; }
	else if ( is( "-2"           )) { Bank2      = 1; }

	else if ( is( "--IrqOver_2="     )) { IrqOver_2.set( val()     ); Md=1;}
	else if ( is( "--EdgeReset_1="   )) { EdgeReset_1.set( val()   ); Md=1;}
	else if ( is( "--ImaskFiltHigh_1=")) { ImaskFiltHigh_1.set( val()); Md=1;}
	else if ( is( "--ImaskFiltLow_1=" )) { ImaskFiltLow_1.set(  val()); Md=1;}
	else if ( is( "--ImaskFiltRise_1=")) { ImaskFiltRise_1.set( val()); Md=1;}
	else if ( is( "--ImaskFiltFall_1=")) { ImaskFiltFall_1.set( val()); Md=1;}
	else if ( is( "--ImaskHigh_1="   )) { ImaskHigh_1.set( val()   ); Md=1;}
	else if ( is( "--ImaskLow_1="    )) { ImaskLow_1.set( val()    ); Md=1;}
	else if ( is( "--ImaskRise_1="   )) { ImaskRise_1.set( val()   ); Md=1;}
	else if ( is( "--ImaskFall_1="   )) { ImaskFall_1.set( val()   ); Md=1;}
	else if ( is( "--InOver_2="      )) { InOver_2.set( val()      ); Md=1;}
	else if ( is( "--OutEnOver_2="   )) { OutEnOver_2.set( val()   ); Md=1;}
	else if ( is( "--OutOver_2="     )) { OutOver_2.set( val()     ); Md=1;}
	else if ( is( "--FilterM_7="     )) { FilterM_7.set( val()     ); Md=1;}
	else if ( is( "--FuncSel_5="     )) { FuncSel_5.set( val()     ); Md=1;}

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

    check_f2( "IrqOver_2",       IrqOver_2.Val       );
    check_f1( "EdgeReset_1",     EdgeReset_1.Val     );
    check_f1( "ImaskFiltHigh_1", ImaskFiltHigh_1.Val );
    check_f1( "ImaskFiltLow_1",  ImaskFiltLow_1.Val  );
    check_f1( "ImaskFiltRise_1", ImaskFiltRise_1.Val );
    check_f1( "ImaskFiltFall_1", ImaskFiltFall_1.Val );
    check_f1( "ImaskHigh_1",     ImaskHigh_1.Val     );
    check_f1( "ImaskLow_1",      ImaskLow_1.Val      );
    check_f1( "ImaskRise_1",     ImaskRise_1.Val     );
    check_f1( "ImaskFall_1",     ImaskFall_1.Val     );
    check_f2( "InOver_2",        InOver_2.Val        );
    check_f2( "OutEnOver_2",     OutEnOver_2.Val     );
    check_f2( "OutOver_2",       OutOver_2.Val       );

    if (                       FilterM_7.Val > 0x7f   ) {
	Error::msg( "require --FilterM_7={0..127}:  " ) <<
			       FilterM_7.Val <<endl;
    }

    if (                       FuncSel_5.Val > 0x1f   ) {
	Error::msg( "require --FuncSel_5={0..31}:  "  ) <<
			       FuncSel_5.Val <<endl;
    }

    if ( gpio.Val > 0x0fffffff ) {
	Error::msg( "require --gpio=mask <= 0x0fffffff" ) <<endl;
    }

    int		atomic_cnt = norm.Given + flip.Given + set.Given + clr.Given;

    if ( atomic_cnt > 1 ) {
	Error::msg( "require only one write atomic address --norm= .." ) <<endl;
    }

    if ( Mr && ((IoCntl + IoStat) != 1) ) {
	Error::msg( "write atomic requires one register select -c or -s" ) <<endl;
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

    if ( Md ) {		// focus on IoCntl, let user add IoStat
	IoCntl = 1;
    }

    if ( !(IoCntl || IoStat) ) {	// default both
	IoCntl = 1;
	IoStat = 1;
    }

    if ( Bank1 || Bank2 ) {
	Error::msg( "only Bank0 is supported" ) <<endl;
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
rcon_yOptLong::check_f1(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 1 ) {
	Error::msg( "require --" ) << p << "={0,1}:  " << v << endl;
    }
}

void
rcon_yOptLong::check_f2(
    const char*		p,
    uint32_t		v
)
{
    if ( v > 0x3 ) {
	Error::msg( "require --" ) << p << "={0..3}:  " << v << endl;
    }
}


/*
* Show option flags.
*/
void
rcon_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--gpio        = 0x" <<setw(8) << gpio.Val        << endl;
    cout << "--OutOver_2   = 0x" <<setw(8) << OutOver_2.Val   << endl;
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
rcon_yOptLong::print_usage()
{
    cout <<
    "    IO Control interface RPi5\n"
    "usage:  " << ProgName << " rcon [options..]  [gpio..]\n"
    "    gpio                bit numbers {27..0}\n"
    "    --gpio=0x0fffffff   mask to select Gpio[27:0] bits\n"
//  "    -0, -1, -2          bank number, default -0\n"
    "  IoCntl(gpio) field modification:\n"
    "    --IrqOver_2=0       interrupt override ..\n"
    "    --EdgeReset_1=0     edge detector 1=reset\n"
    "    --ImaskFiltHigh_1=0 mask debounced high level\n"
    "    --ImaskFiltLow_1=0  mask debounced low level\n"
    "    --ImaskFiltRise_1=0 mask filtered rising edge\n"
    "    --ImaskFiltFall_1=0 mask filtered falling edge\n"
    "    --ImaskHigh_1=0     mask high level\n"
    "    --ImaskLow_1=0      mask low level\n"
    "    --ImaskRise_1=0     mask rising edge\n"
    "    --ImaskFall_1=0     mask falling edge\n"
    "    --InOver_2=0        input override 0=norm, 1=invert, 2=low, 3=high\n"
    "    --OutEnOver_2=0     output enable override ..\n"
    "    --OutOver_2=0       output override ..\n"
    "    --FilterM_7=4       filter time constant M {0..127}\n"
    "    --FuncSel_5=31      function select {0..31}\n"
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
    "  register display/write selection:\n"
    "    -c, --IoCntl        show IoCntl\n"
    "    -s, --IoStat        show IoStat\n"
    "    --list              list only atomic registers (no field table)\n"
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
rcon_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register index value in hex and binary.
*/
void
rcon_yOptLong::out_regindex( const char* name, int index, uint32_t vv )
{
    cout.fill('0');
    cout << "   0x" <<hex <<right <<setw(8)  << vv;

    cout.fill(' ');
    cout << "  " <<left            << name
	 << "("  <<dec  <<setw(2)  <<right << index << ")"
	 << "  "                   << cstr_bits32( vv ) <<endl;

    cout <<dec <<right;		// restore defaults
}

void
rcon_yOptLong::head_reg( const char* title )
{
    cout <<setw(30) <<left << title
	 << "    28   24   20   16   12    8    4    0" <<endl;
    cout <<right;		// restore defaults
}


/*
* Output IoCntl register table.
*/
void
rcon_yOptLong::out_IoCntl( rgsIoCon& cx )
{
    const int	kMax = 16;
    char	ww[kMax+1][50];
    uint32_t	vv;
    int		jj = 0;		// output char position

    for ( int ii=27;  ii>=0;  ii-- )	// bit [27:0]
    {
	rgsIo_Cntl	cntl  = cx.IoCntl(ii);

	cntl.grab();

	ww[16][jj] = '0' + cntl.get_IrqOver_2();	// binary to ASCII
	ww[15][jj] = '0' + cntl.get_EdgeReset_1();
	ww[14][jj] = '0' + cntl.get_ImaskFiltHigh_1();
	ww[13][jj] = '0' + cntl.get_ImaskFiltLow_1();
	ww[12][jj] = '0' + cntl.get_ImaskFiltRise_1();
	ww[11][jj] = '0' + cntl.get_ImaskFiltFall_1();
	ww[10][jj] = '0' + cntl.get_ImaskHigh_1();
	ww[ 9][jj] = '0' + cntl.get_ImaskLow_1();
	ww[ 8][jj] = '0' + cntl.get_ImaskRise_1();
	ww[ 7][jj] = '0' + cntl.get_ImaskFall_1();
	ww[ 6][jj] = '0' + cntl.get_InOver_2();
	ww[ 5][jj] = '0' + cntl.get_OutEnOver_2();
	ww[ 4][jj] = '0' + cntl.get_OutOver_2();
	vv =               cntl.get_FilterM_7();
	ww[ 3][jj] = hex2char( vv >>4 );
	ww[ 2][jj] = hex2char( vv );
	vv =               cntl.get_FuncSel_5();
	ww[ 1][jj] = hex2char( vv >>4 );
	ww[ 0][jj] = hex2char( vv );

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

    cout << " IoCntl(i).norm     gpio i:  28   24   20   16   12    8    4    0"
	 <<endl;

    cout << "   IrqOver_2       [31:30] ---- " << ww[16] <<endl;
    cout << "   EdgeReset_1     [28]    ---- " << ww[15] <<endl;
    cout << "   ImaskFiltHigh_1 [27]    ---- " << ww[14] <<endl;
    cout << "   ImaskFiltLow_1  [26]    ---- " << ww[13] <<endl;
    cout << "   ImaskFiltRise_1 [25]    ---- " << ww[12] <<endl;
    cout << "   ImaskFiltFall_1 [24]    ---- " << ww[11] <<endl;
    cout << "   ImaskHigh_1     [23]    ---- " << ww[10] <<endl;
    cout << "   ImaskLow_1      [22]    ---- " << ww[ 9] <<endl;
    cout << "   ImaskRise_1     [21]    ---- " << ww[ 8] <<endl;
    cout << "   ImaskFall_1     [20]    ---- " << ww[ 7] <<endl;
    cout << "   InOver_2        [17:16] ---- " << ww[ 6] <<endl;
    cout << "   OutEnOver_2     [15:14] ---- " << ww[ 5] <<endl;
    cout << "   OutOver_2       [13:12] ---- " << ww[ 4] <<endl;
    cout << "   FilterM_7      [11:9]   ---- " << ww[ 3] <<endl;
    cout << "   FilterM_7       [8:5]   ---- " << ww[ 2] <<endl;
    cout << "   FuncSel_5      [4]      ---- " << ww[ 1] <<endl;
    cout << "   FuncSel_5       [3:0]   ---- " << ww[ 0] <<endl;
}


/*
* Output IoStat register table.
*/
void
rcon_yOptLong::out_IoStat( rgsIoCon& cx )
{
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

    cout << " IoStat(i).norm     gpio i:  28   24   20   16   12    8    4    0"
	 <<endl;

    cout << "   IrqToProc_1     [29]    ---- " << ww[17] <<endl;
    cout << "   IrqMasked_1     [28]    ---- " << ww[16] <<endl;
    cout << "   InFiltHigh_1    [27]    ---- " << ww[15] <<endl;
    cout << "   InFiltLow_1     [26]    ---- " << ww[14] <<endl;
    cout << "   InFiltRise_1    [25]    ---- " << ww[13] <<endl;
    cout << "   InFiltFall_1    [24]    ---- " << ww[12] <<endl;
    cout << "   InHigh_1        [23]    ---- " << ww[11] <<endl;
    cout << "   InLow_1         [22]    ---- " << ww[10] <<endl;
    cout << "   InRise_1        [21]    ---- " << ww[ 9] <<endl;
    cout << "   InFall_1        [20]    ---- " << ww[ 8] <<endl;
    cout << "   InToPeri_1      [19]    ---- " << ww[ 7] <<endl;
    cout << "   InFiltered_1    [18]    ---- " << ww[ 6] <<endl;
    cout << "   InOfPad_1       [17]    ---- " << ww[ 5] <<endl;
    cout << "   InIsDirect_1    [16]    ---- " << ww[ 4] <<endl;
    cout << "   OutEnToPad_1    [13]    ---- " << ww[ 3] <<endl;
    cout << "   OutEnOfPeri_1   [12]    ---- " << ww[ 2] <<endl;
    cout << "   OutToPad_1       [9]    ---- " << ww[ 1] <<endl;
    cout << "   OutOfPeri_1      [8]    ---- " << ww[ 0] <<endl;
}


//--------------------------------------------------------------------------
// Constructor
//--------------------------------------------------------------------------

y_rcon::y_rcon(
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
y_rcon::doit()
{

    {
	rcon_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

	rgsIoCon	Cx  ( AddrMap );	// constructor, Bank0

	if ( Opx.debug ) {
	    cout.fill('0');
	    cout << "+ FeatureAddr  = 0x"
		 <<hex <<setw(8) << Cx.get_bcm_address() <<endl;
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

	    if ( Opx.IoCntl ) {
		Opx.trace_msg( "Write atomic registers IoCntl" );

		for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
		{
		    int			gpio   = bitarg[ii];
		    rgsIo_Cntl&		iocntl = Cx.IoCntl(gpio);

		    if ( Opx.norm.Given ) {
			iocntl.write(      Opx.norm.Val ); }

		    if ( Opx.flip.Given ) {
			iocntl.write_flip( Opx.flip.Val ); }

		    if ( Opx.set.Given ) {
			iocntl.write_set(  Opx.set.Val  ); }

		    if ( Opx.clr.Given ) {
			iocntl.write_clr(  Opx.clr.Val  ); }
		}

	    }

	    if ( Opx.IoStat ) {
		Opx.trace_msg( "Write atomic registers IoStat" );

		for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
		{
		    int			gpio   = bitarg[ii];
		    rgsIo_Stat&		iostat = Cx.IoStat(gpio);

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
	}

	if ( Opx.Md ) {
	    Opx.trace_msg( "Modify registers field" );

	    for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
	    {
		int		gpio  = bitarg[ii];
		rgsIo_Cntl&	iocon = Cx.IoCntl(gpio);

		iocon.grab();

		APPLY( IrqOver_2,       iocon.put_IrqOver_2       )
		APPLY( EdgeReset_1,     iocon.put_EdgeReset_1     )
		APPLY( ImaskFiltHigh_1, iocon.put_ImaskFiltHigh_1 )
		APPLY( ImaskFiltLow_1,  iocon.put_ImaskFiltLow_1  )
		APPLY( ImaskFiltRise_1, iocon.put_ImaskFiltRise_1 )
		APPLY( ImaskFiltFall_1, iocon.put_ImaskFiltFall_1 )
		APPLY( ImaskHigh_1,     iocon.put_ImaskHigh_1     )
		APPLY( ImaskLow_1,      iocon.put_ImaskLow_1      )
		APPLY( ImaskRise_1,     iocon.put_ImaskRise_1     )
		APPLY( ImaskFall_1,     iocon.put_ImaskFall_1     )
		APPLY( InOver_2,        iocon.put_InOver_2        )
		APPLY( OutEnOver_2,     iocon.put_OutEnOver_2     )
		APPLY( OutOver_2,       iocon.put_OutOver_2       )
		APPLY( FilterM_7,       iocon.put_FilterM_7       )
		APPLY( FuncSel_5,       iocon.put_FuncSel_5       )

		iocon.push();
	    }
	}

    // Output Table
	if ( Opx.tableF )
	{
	    Opx.trace_msg( "Read registers" );

	    if (    Opx.IoCntl ) {
		Opx.out_IoCntl( Cx );
	    }
	    if (    Opx.IoStat ) {
		Opx.out_IoStat( Cx );
	    }
	}

    // Output List
	if ( Opx.listF )
	{
	    Opx.trace_msg( "Read registers" );

	    if ( Opx.IoCntl ) {
		Opx.head_reg( " Read Atomic register bit:  " );
		for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
		{
		    int			gpio   = bitarg[ii];
		    rgsIo_Cntl&		iocntl = Cx.IoCntl(gpio);

		    if ( Opx.o_norm ) { Opx.out_regindex(
				"norm IoCntl", gpio, iocntl.read()      ); }

		    if ( Opx.o_peek ) { Opx.out_regindex(
				"peek IoCntl", gpio, iocntl.read_peek() ); }

		    if ( Opx.o_set ) { Opx.out_regindex(
				"set  IoCntl", gpio, iocntl.read_set()  ); }

		    if ( Opx.o_clr ) { Opx.out_regindex(
				"clr  IoCntl", gpio, iocntl.read_clr()  ); }
		}
	    }

	    if ( Opx.IoStat ) {
		Opx.head_reg( " Read Atomic register bit:  " );
		for ( int ii=0;  ii<bitcnt;  ii++ )	// each bit
		{
		    int			gpio   = bitarg[ii];
		    rgsIo_Stat&		iostat = Cx.IoStat(gpio);

		    if ( Opx.o_norm ) { Opx.out_regindex(
				"norm IoStat", gpio, iostat.read()      ); }

		    if ( Opx.o_peek ) { Opx.out_regindex(
				"peek IoStat", gpio, iostat.read_peek() ); }

		    if ( Opx.o_set ) { Opx.out_regindex(
				"set  IoStat", gpio, iostat.read_set()  ); }

		    if ( Opx.o_clr ) { Opx.out_regindex(
				"clr  IoStat", gpio, iostat.read_clr()  ); }
		}
	    }

	}

    }

    return ( Error::has_err() ? 1 : 0 );
}

