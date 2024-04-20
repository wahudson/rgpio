// 2021-03-10  William A. Hudson
//
// rgpio pads -- Pads Control - RaspberryPi GPIO utility program.
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
#include "rgPads.h"

#include "yUtil.h"
#include "y_pads.h"


//--------------------------------------------------------------------------
// Option Handling
//--------------------------------------------------------------------------

class pads_yOptLong : public yOption {

//  public:	// inherited
//    char*		ProgName;
//    int		get_argc();
//    char*		next_arg();
//    bool		next();
//    bool		is( const char* opt );
//    char*		val();
//    char*		current_option();

  public:	// option values

					// fields
    yOpVal		value;
    yOpVal		Passwd_8;
    yOpVal		SlewMax_1;
    yOpVal		HystEn_1;
    yOpVal		DriveStr_3;

    bool		Md          = 0;	// modify flag

					// registers
    bool		PadsA_27s0  = 0;
    bool		PadsB_45s28 = 0;
    bool		PadsC_53s46 = 0;

    bool		BinOut      = 1;
    bool		verbose;
    bool		debug;
    bool		TESTOP;

  public:
    pads_yOptLong( yOption  *opx );	// constructor

    void		parse_options();
    void		check_f1( const char* p,  uint32_t v );
    void		print_option_flags();
    void		print_usage();
    void		trace_msg( const char* text );
    void		out_reg( const char* name,  uint32_t val );
};


/*
* Constructor.  Init options with default values.
*    Pass in the main() option object.
* call:
*    pads_yOptLong	Opx  ( opx );
*/
pads_yOptLong::pads_yOptLong( yOption  *opx )
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
pads_yOptLong::parse_options()
{

    while ( this->next() )
    {
	     if ( is( "--value="      )) { value.set(      this->val() ); Md=1;}
	else if ( is( "--Passwd_8="   )) { Passwd_8.set(   this->val() ); Md=1;}
	else if ( is( "--SlewMax_1="  )) { SlewMax_1.set(  this->val() ); Md=1;}
	else if ( is( "--HystEn_1="   )) { HystEn_1.set(   this->val() ); Md=1;}
	else if ( is( "--DriveStr_3=" )) { DriveStr_3.set( this->val() ); Md=1;}

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

    if (                       Passwd_8.Val > 0xff  ) {
	Error::msg( "require --Passwd_8<=0xff:  0x" ) <<hex <<
			       Passwd_8.Val <<endl <<dec;
    }

    check_f1( "SlewMax_1",     SlewMax_1.Val     );
    check_f1( "HystEn_1",      HystEn_1.Val      );

    if (                       DriveStr_3.Val > 0x7   ) {
	Error::msg( "require --DriveStr_3={0..7}:  "  ) <<
			       DriveStr_3.Val <<endl;
    }

    if ( Md && (get_argc() == 0) ) {
	Error::msg( "modify requires register argument" ) << endl;
    }

    if ( Md && ! (Passwd_8.Given || value.Given) ) {	// default
	Passwd_8.set( "0x5a" );
    }

    if ( (get_argc() == 0) && (! Md) ) {	// default, no modify
	PadsA_27s0   = 1;
	PadsB_45s28  = 1;
	PadsC_53s46  = 1;
    }

    while ( get_argc() > 0 )		// Register arguments
    {
	     if ( is( "PadsA_27s0"      )) { PadsA_27s0      = 1; }
	else if ( is( "PadsB_45s28"     )) { PadsB_45s28     = 1; }
	else if ( is( "PadsC_53s46"     )) { PadsC_53s46     = 1; }
	else {
	   Error::msg( "unknown register:  " ) << this->current_option() <<endl;
	}
	next_arg();
    }
}


/*
* Check option Field_1 value.
* call:
*    check_f1( "Field_1", Field_1.Val );
*/
void
pads_yOptLong::check_f1(
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
pads_yOptLong::print_option_flags()
{
    cout.fill('0');

    cout <<hex;
    cout << "--value       = 0x" <<setw(8) << value.Val        << endl;
    cout << "--Passwd_8    = 0x" <<setw(8) << Passwd_8.Val     << endl;
    cout << "--SlewMax_1   = 0x" <<setw(8) << SlewMax_1.Val    << endl;
    cout << "--HystEn_1    = 0x" <<setw(8) << HystEn_1.Val     << endl;
    cout << "--DriveStr_3  = 0x" <<setw(8) << DriveStr_3.Val   << endl;

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
pads_yOptLong::print_usage()
{
    cout <<
    "    Pads Control\n"
    "usage:  " << ProgName << " pads [options..]  [REG..]\n"
    "    REG = { PadsA_27s0, PadsB_45s28, PadsC_53s46 }\n"
    "  modify register:\n"
    "    --value=V           set register value\n"
    "  modify fields:  (of REG..)\n"
    "    --Passwd_8=0x5a     password (default)\n"
    "    --SlewMax_1=0       1= max slew rate, 0= limited\n"
    "    --HystEn_1=0        1= hysteresis enabled, 0= not\n"
    "    --DriveStr_3=0      drive strength (0..7) = (2 mA .. 16 mA)\n"
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
pads_yOptLong::trace_msg( const char* text )
{
    if ( verbose ) {
	cout << "+ " << text <<endl;
    }
}


/*
* Output register value in hex and binary.
*/
void
pads_yOptLong::out_reg( const char* name,  uint32_t vv )
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

y_pads::y_pads(
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
y_pads::doit()
{

    {
	pads_yOptLong		Opx  ( MainOpx );	// constructor

	Opx.parse_options();

	if ( Opx.TESTOP ) {
	    Opx.print_option_flags();
	    return ( Error::has_err() ? 1 : 0 );
	}

	if ( Error::has_err() )  return 1;

    // Pads Control object

	rgPads			Px  ( AddrMap );	// constructor

	if ( Opx.BinOut ) {
	    cout << "Pads Control register:            28   24   20   16   12    8    4    0" << endl;
	}

	for ( int ii=0;  ii<=2;  ii++ )		// sequential registers
	{
	    const char*		regname;
	    rgPads_Cntl*	rgx;		// current register
	    bool		md = 0;		// modify flag

	    switch ( ii ) {
	      case 0:
	        if (    ! Opx.PadsA_27s0  ) { continue; }
	        rgx     = &Px.PadsA_27s0;
	        regname =    "PadsA_27s0";    break;
	      case 1:
	        if (    ! Opx.PadsB_45s28 ) { continue; }
	        rgx     = &Px.PadsB_45s28;
	        regname =    "PadsB_45s28";   break;
	      case 2:
	        if (    ! Opx.PadsC_53s46 ) { continue; }
	        rgx     = &Px.PadsC_53s46;
	        regname =    "PadsC_53s46";   break;
	    }

	    Opx.trace_msg( "Grab register" );
	    rgx->grab();

#define APPLY( X, Y ) if ( Opx.X.Given ) { rgx->Y( Opx.X.Val );  md = 1; }

	// Registers

	    APPLY( value,  put )

	// Fields

	    APPLY( Passwd_8,    put_Passwd_8    )
	    APPLY( SlewMax_1,   put_SlewMax_1   )
	    APPLY( HystEn_1,    put_HystEn_1    )
	    APPLY( DriveStr_3,  put_DriveStr_3  )

	    if ( md ) {			// modified registers
		Opx.trace_msg( "Modify register" );
		rgx->push();

		Opx.trace_msg( "Grab register" );
		rgx->grab();
	    }

	// Output

	    Opx.out_reg( regname, rgx->get() );

	    cout.fill('0');
	    cout <<hex
		<< "   Passwd_8    = 0x" <<setw(2) << rgx->get_Passwd_8() <<endl
		;

	    cout.fill(' ');
	    cout <<dec
		<< "   SlewMax_1   = " << rgx->get_SlewMax_1()   <<endl
		<< "   HystEn_1    = " << rgx->get_HystEn_1()    <<endl
		<< "   DriveStr_3  = " << rgx->get_DriveStr_3()  <<endl
		;
	}

//	do_reg( "PadsA_27s0",  &Px.PadsA_27s0  );

    }

    return ( Error::has_err() ? 1 : 0 );
}

